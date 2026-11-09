#ifndef __CATALOGADORWS_HPP__
#define __CATALOGADORWS_HPP__

#include <GuruHttp/GuruhttpResponse.hpp>
#include <GuruHttp/Guruhttp_algo.hpp>
#include <GuruBuffer_algo.hpp>

#include <GuruHttp/GuruhttpREST.hpp>
#include <GuruDirectorio.hpp>

#include <ModeloCatalogo.hpp>

#include <jsoncpp/json.h>
#include <sstream>

// para cambiar el caracter dividor de directorio
// update directorio set nombre=replace (nombre, '\' ,'/')

class CatalogadorWS: public GuruHttpREST {
	modeloCatalogo* catalogo;
public:
	CatalogadorWS():catalogo(NULL) { };
	virtual ~CatalogadorWS() {};

	void procesarGet() {
		catalogo = static_cast<modeloCatalogo*>(config()->extra_ptr());

		if (catalogo == NULL) return throw GuruhttpExcepcion(500);

		init();

		// /catalogadorws/{Accion}/{Etiqueta}/{Recurso}/{Parametro}
		// /catalogadorws/info/{Etiqueta}/{id}
		// /catalogadorws/catalogos/{Etiqueta}
		// /catalogadorws/catalogos/{Etiqueta}/{ruta} --> acceso a la base de datos
		// /catalogadorws/fs/{Etiqueta}/{ruta} --> acceso directo al filesystem
		// /catalogadorws/archivos/{Etiqueta}/{id}
		// /catalogadorws/archivos/{Etiqueta}/{id}/info
		// /catalogadorws/buscar/{Etiqueta}/nombre
		if (equal(0, "catalogos")) {
			switch (totalsubdir()) {
			case 1: // catalogos
				catalogos();
				break;
			default: // catalogos/Juegos
				{
				std::string tmp;
				for (size_t i = 2; i < totalsubdir(); i++)
					tmp += subdir(i) + "/";
				navegar_catalogo_bd(subdir(1), tmp);
				break;
				}
			}
		} else if (equal(0, "fs")) {
			switch (totalsubdir()) {
			case 1: // catalogos
				catalogos();
				break;
			default: // catalogos/Juegos
				{
				std::string tmp;
				for (size_t i = 2; i < totalsubdir(); i++) {
					tmp += subdir(i);
					if (i < totalsubdir()-1) tmp += "/";
				}

				navegar_catalogo_fs(catalogo->consulta_info_etiqueta(subdir(1)).directorio(), tmp);
				break;
				}
			}
		} else if (equal(0, "archivos")) {
			switch (totalsubdir()) {
			case 1: // /archivos
				archivos();
				break;
			case 3: // /archivos/{Etiqueta}/{id}
				archivo(subdir(1), subdir(2));
				break;
			case 4: // /archivos/{Etiqueta}/{id}/info
				if (subdir(3).compare("info") == 0)
					info_archivo(subdir(1), subdir(2));
				else throw GuruhttpExcepcion(404);
				break;
			default:
				throw GuruhttpExcepcion(404);
				break;
			}
		} else if (equal(0, "info")) {
			switch (totalsubdir()) {
			case 3: // /info/{Etiqueta}/{id}
				info_archivo(subdir(1), subdir(2));
				break;
			default:
				throw GuruhttpExcepcion(404);
				break;
			}
		} else if (equal(0, "buscar")) {
			switch (totalsubdir()) {
			case 3: // /buscar/{Etiqueta}/{patron}
				buscar(subdir(1), subdir(2));
				break;
			default:
				throw GuruhttpExcepcion(404);
				break;
			}
		} else throw GuruhttpExcepcion(400);
	}

	void archivos() {
	}

	void archivo(const std::string& etiqueta, const std::string id) {
		try {
			modeloCatalogo::iterator it = catalogo->consulta_info_archivo(atoll(id.c_str()));

			if (it->Archivo.empty()) throw GuruhttpExcepcion(404);

			std::string ruta_archivo(catalogo->consulta_info_etiqueta(it->Etiqueta.nombre()).directorio() + it->Directorio.nombre() + it->Archivo.nombre());

			response()->atributo("Content-Disposition", "inline; filename=" + GuruStringAlgo::comillas(it->Archivo.nombre()));

			enviar_archivo(ruta_archivo);

		} catch (modeloCatalogoException& e) {
			throw GuruhttpExcepcion(500, e.what());
		}
	}

	void info_archivo(const std::string& etiqueta, const std::string id) {
		try {
			Json::Value archivo;

			modeloCatalogo::iterator it = catalogo->consulta_info_archivo(atoll(id.c_str()));

			archivo["id"] = static_cast<Json::Value::UInt64>(it->id);
			archivo["nombre"] = it->Archivo.nombre();
			archivo["bytes"] = static_cast<Json::Value::UInt64>(it->Archivo.bytes());
			archivo["fecha"] = "";

			enviar_buffer(archivo.toStyledString(), "application/json");
		} catch (modeloCatalogoException& e) {
			throw GuruhttpExcepcion(500, e.what());
		}
	}

	void catalogos() {

		try {
			modeloCatalogo::iterator it = catalogo->consulta_info_catalogos();
			modeloCatalogo::iterator itend;

			Json::Value root;

			for (; it != itend; ++it) {
				Json::Value archivo;

				archivo["Nombre"] = it->Etiqueta.nombre();
				archivo["Catalogados"] = static_cast<Json::Value::UInt64>(it->Etiqueta.id());
				archivo["Catalogo"] = "http://" + atributo("Host") + recurso().directorio() + recurso().archivo() + "/" + it->Etiqueta.nombre();

				root["Catalogos"].append(archivo);
			}

			if (root.empty()) throw GuruhttpExcepcion(404);

			enviar_buffer(root.toStyledString(), "application/json");
		} catch (modeloCatalogoException& e) {
			throw GuruhttpExcepcion(500, e.what());
		}
	}

	void navegar_catalogo_bd(const std::string& etiqueta, const std::string& directorio) {

		try {
			Json::Value root;
			modeloCatalogo::iterator it = catalogo->consulta_listar_archivos_en(directorio, etiqueta);
			modeloCatalogo::iterator itend;

			for (; it != itend; ++it) {
				Json::Value archivo;

				archivo["id"] = static_cast<Json::Value::UInt64>(it->id);
				archivo["Nombre"] = it->Archivo.nombre();
				archivo["Bytes"] = static_cast<Json::Value::UInt64>(it->Archivo.bytes());
				archivo["Fecha"] = "";
				archivo["archivo"] = "http://" + atributo("Host") + recurso().directorio() + "archivos" + "/" + etiqueta + "/" + GuruStringAlgo::int64astr(static_cast<Json::Value::UInt64>(it->id));

				root["Archivos"].append(archivo);
			}

			it = catalogo->consulta_listar_directorios_en(directorio, etiqueta);

			for (; it != itend; ++it) {
				std::string tmpdir;

				if (it->Directorio.nombre().empty()) 
					continue;

				Json::Value value_directorio;

				tmpdir = it->Directorio.nombre();
				tmpdir.erase(tmpdir.length()-1, 1);
				value_directorio["Nombre"] = tmpdir;
				value_directorio["directorio"] = "http://" + atributo("Host") + recurso().directorio() + recurso().subdirectorio() + recurso().archivo() + "/" + tmpdir;

				root["Directorios"].append(value_directorio);
			}

			//			Json::StyledWriter writer;
			//			Json::FastWriter writer;
			//			oss << writer.write(vector);

			if (root.empty()) throw GuruhttpExcepcion(404);

			enviar_buffer(root.toStyledString(), "application/json");
		} catch (GuruSQLite::GuruSQLiteException& e) {
			throw GuruhttpExcepcion(500, e.what());
		}
	}

	void navegar_catalogo_fs(const std::string& raiz, const std::string& directorio) {

		try {
			Json::Value root;

			Json::Value archivos;
			Json::Value directorios;

			std::string tmpdir(raiz + directorio);

			GuruDirectorio::GuruArchivo ar;

			ar = GuruDirectorio::propiedades(tmpdir);

			// pregunto si navego el directorio o descargo el archivo
			if (GuruDirectorio::esDirectorio(tmpdir)) {

				tmpdir = GuruhttpAlgo::comoDirectorioHTTP(tmpdir);

				for (GuruDirectorio::iterator it(tmpdir), end; it != end; ++it) {
					Json::Value item;

					if (it->EsDirectorio()) {
						item["Nombre"] = it->nombre();
						item["directorio"] = "http://" + atributo("Host") + recurso().directorio() + recurso().subdirectorio() + recurso().archivo() + "/" + it->nombre();

						root["Directorios"].append(item);
					} else if (it->EsArchivo()) {
						item["Nombre"] = it->nombre();
						item["Bytes"] = static_cast<Json::Value::UInt64>(it->tamanio());
						item["Fecha"] = GuruAlgo::fechaHoraRFC822(it->fecha());
						item["archivo"] = "http://" + atributo("Host") + recurso().directorio() + recurso().subdirectorio() + recurso().archivo() + "/" + it->nombre();

						root["Archivos"].append(item);
					}

					item.clear();
				}
			} else if (ar.EsArchivo()) {
					response()->content_type(GuruhttpAlgo::mime(ar.nombre()));
					request()->recurso().info(ar);
					response()->enviar_archivo();

					return;
			} else throw GuruhttpExcepcion(404);

		//			Json::StyledWriter writer;
		//			Json::FastWriter writer;
		//			oss << writer.write(vector);

			if (root.empty()) throw GuruhttpExcepcion(404);

			enviar_buffer(root.toStyledString(), "application/json");
		} catch (GuruSQLite::GuruSQLiteException& e) {
			throw GuruhttpExcepcion(500, e.what());
		}
	}

	void buscar(const string& etiqueta, const string& busqueda) {

		try {
			modeloCatalogo::iterator it = catalogo->consulta_buscar(busqueda, etiqueta);
			modeloCatalogo::iterator itend;
			Json::Value vector;

			for (; it != itend; ++it) {
				cout << it->Directorio.nombre() << " - " << it->Archivo.nombre() << endl;

				Json::Value root;

				root["id"] = static_cast<Json::Value::UInt64>(it->id);
				root["Ruta"] = it->Directorio.nombre() + it->Archivo.nombre();
				root["Bytes"] = static_cast<Json::Value::UInt64>(it->Archivo.bytes());
				root["Fecha"] = "";
				root["archivo"] = "http://" + atributo("Host") + recurso().directorio() + "archivos" + "/" + etiqueta + "/" + GuruStringAlgo::int64astr(static_cast<Json::Value::UInt64>(it->id));

				vector.append(root);
			}

			if (vector.empty()) throw GuruhttpExcepcion(404);

			enviar_buffer(vector.toStyledString(), "application/json");
		} catch (modeloCatalogoException& e) {
			throw GuruhttpExcepcion(500, e.what());
		}
	}

	void procesarPost() {
		catalogo = static_cast<modeloCatalogo*>(config()->extra_ptr());

		if (recurso().archivo().compare("atri-param") == 0) {
			ostringstream oss;

			for (map<string, string>::iterator it = request()->atributos().begin(); it != request()->atributos().end(); ++it) {
				oss << it->first << ":" << it->second << endl;
			}

			oss << "--" << endl;
			for (map<string, string>::iterator it = request()->parametro().begin(); it != request()->parametro().end(); ++it) {
				oss << it->first << ":" << it->second << endl;
			}

			oss << "--" << endl;
			for (std::map<std::string, std::string>::const_iterator it = request()->config()->env().begin(), end = request()->config()->env().end(); it != end; ++it) {
				oss << it->first << ":" << it->second << endl;
			}

			enviar_buffer(oss.str(), "text/plain");
		} else if (recurso().archivo().compare("subir") == 0) {
			if (!parametro("nombre").empty() && post().configData().cantidad_data() != 0 && atributo("Content-Type").compare("application/octet-stream") == 0 && !atributo("Content-Length").empty()) {

				GuruBufferAlgo::buffer_archivo(post().configData(), parametro("nombre"));

				if (!parametro("fecha").empty())
					GuruDirectorio::fechaArchivo(parametro("nombre"), atol(parametro("fecha").c_str()));

				Json::Value root;

				root["Status"] = "OK";
				root["Archivo"] = parametro("nombre");
				root["Bytes"] = static_cast<Json::Value::UInt64>(post().configData().cantidad_data());

				Json::StyledWriter writer;
				//	Json::FastWriter writer;

				enviar_buffer(root.toStyledString(), "application/json");
			} else throw GuruhttpExcepcion(400);
		} else if (recurso().archivo().compare("catalogar") == 0) {
			int indice = 0;

			std::string etiqueta(post().campo("etiqueta"));
			std::string directorio_etiqueta;

			if (etiqueta.empty()) directorio_etiqueta = GuruDirectorio::comoDirectorio(config()->env("no-etiqueta"));
			else directorio_etiqueta = catalogo->consulta_info_etiqueta(etiqueta).directorio();

			do {
				std::string destino(directorio_etiqueta);

				std::string campo_archivo("archivo-" + GuruStringAlgo::itos(indice));
				std::string campo_fecha("fecha-" + GuruStringAlgo::itos(indice));
				std::string campo_directorio("directorio-" + GuruStringAlgo::itos(indice));

				if (post().campo(campo_archivo).empty()) break;

				std::string archivo(post().campo(campo_archivo));
				std::string fecha(post().campo(campo_fecha));
				std::string directorio(GuruDirectorio::comoDirectorio(post().campo(campo_directorio)));

				destino += directorio;

				if (!GuruDirectorio::existe(destino)) 
					GuruDirectorio::crearDirectorioRecursivo(destino);

				GuruDirectorio::GuruSplitPath a(archivo);

				destino += a.nombre_ext();

				if (GuruDirectorio::moverArchivo(archivo, destino)) {
					GuruDirectorio::fechaArchivo(destino, atol(fecha.c_str()));
				}

				indice++;

			} while (true);

			Json::Value root;

			root["Status"] = "OK";
	//		root["Archivo"] = request()->parametro("nombre");
//			root["Bytes"] = static_cast<Json::Value::UInt64>(request()->post().configData().cantidad_data());

			enviar_buffer(root.toStyledString(), "application/json");
		} else throw GuruhttpExcepcion(400);
	};
};

#endif
