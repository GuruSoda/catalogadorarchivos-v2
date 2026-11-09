#ifndef __MODELOCATALOGO_HPP__
#define __MODELOCATALOGO_HPP__

#include <GuruSQLite.hpp>
#include <GuruSQLite_algo.hpp>
#include <GuruCache.hpp>
#include <GuruDirectorio.hpp>
#include <GuruSHA1.hpp>
#include <GuruSHA4.hpp>
#include <GuruMD5.hpp>
#include <GuruString_algo.hpp>

#if __cplusplus <= 199711L
	#include <boost/regex.hpp>
	using namespace boost;
	#define FILTRO boost::regbase::icase | boost::regbase::basic 
	#define REGBOOST
#else
	#include <regex>
	using namespace std;
	#define FILTRO std::regex_constants::icase 
	#define REGSTD
#endif

#include <ddl.h>

#include <iostream>
#include <sstream>

class modeloCatalogoException : public std::exception {
public:
	modeloCatalogoException() {};
	modeloCatalogoException(const string& msg):error(msg) {};
	virtual ~modeloCatalogoException() throw () {};
	const char * what() { return error.c_str(); }
private:
	std::string error;
};

class modeloCatalogoRegex : public modeloCatalogoException {
public:
	modeloCatalogoRegex(const string& msg) :modeloCatalogoException(msg) {}
	virtual ~modeloCatalogoRegex() throw () {};
};

class modeloCatalogo {
public:
	enum estado_archivo { activo = 0 };

	enum opciones_catalogar {
		sha1 = 0x01,		// que catalogo el sha1 de cada archivo
		sha512 = 0x02,		// que catalogo el sha512 de cada archivo
		md5 = 0x04,			// que catalogo el md5 de cada archivo
		nocrear = 0x08,		// no crear la base de datos si no existe
		version2 = 0x10		// nueva forma de catalogar.
	};

	enum opciones {
		contenedor_etiquetas = 0x01,		// carga toda la tabla de etiquetas a memoria
		contenedor_directorios = 0x02,		// carga toda la tabla de directorios a memoria
		contenedor_archivos = 0x04			// carga toda la tabla de archivos a memoria
	};

	class voArchivo {
		int64_t	_id_archivo;
		string	_nombre;
		uint64_t _bytes;
		time_t	_fecha;
		string	_md5;
		string	_sha1;
		string	_sha512;
		uint32_t _estado;

	public:
		voArchivo() {};
		voArchivo(uint64_t id, const string& nombre, uint64_t bytes, time_t fecha, const string& md5="0", const string& sha1="0", const string& sha512="0", uint32_t estado=0):_id_archivo(id), _nombre(nombre), _bytes(bytes), _fecha(fecha), _md5(md5), _sha1(sha1), _sha512(sha512), _estado(estado) {};
		voArchivo(const string& nombre, uint64_t bytes, time_t fecha, const string& md5 = "0", const string& sha1 = "0", const string& sha512 = "0", uint32_t	estado = 0) :_id_archivo(0), _nombre(nombre), _bytes(bytes), _fecha(fecha), _md5(md5), _sha1(sha1), _sha512(sha512), _estado(estado) {};
		virtual ~voArchivo() {};

		inline int64_t	id() { return _id_archivo; };
		inline void		id(int64_t a) { _id_archivo = a; };

		inline string	nombre() { return _nombre; };
		inline void		nombre(const string& a) { _nombre = a; };

		inline uint64_t bytes() { return _bytes; };
		inline void		bytes(uint64_t a) { _bytes = a; };

		inline time_t	fecha() { return _fecha; };
		inline void 	fecha(time_t a) { _fecha = a; };

		inline string	md5() { return _md5; };
		inline void 	md5(const string& a) { _md5 = a; };

		inline string	sha1() { return _sha1; };
		inline void 	sha1(const string& a) { _sha1 = a; };

		inline string	sha512() { return _sha512; };
		inline void 	sha512(const string& a) { _sha512 = a; };

		inline uint32_t estado() { return _estado; };
		inline void		estado(uint32_t a) { _estado = a; };

		inline friend bool operator < (const modeloCatalogo::voArchivo& a, const modeloCatalogo::voArchivo& b){
			return (a._id_archivo < b._id_archivo) ? true : false;
		}

		inline friend bool operator == (const modeloCatalogo::voArchivo& a, const modeloCatalogo::voArchivo& b) {
			if ((a._nombre.compare(b._nombre) == 0) && (a._bytes == b._bytes) && (a._fecha == b._fecha) && GuruStringAlgo::stringequals(a._md5, b._md5) && GuruStringAlgo::stringequals(a._sha1, b._sha1) && GuruStringAlgo::stringequals(a._sha512, b._sha512))
				return true;
			else 
				return false;
		}

		inline friend bool operator != (const modeloCatalogo::voArchivo& a, const modeloCatalogo::voArchivo& b) {
			return (a._nombre.compare(b._nombre) && a._bytes == b._bytes && a._fecha == b._fecha && GuruStringAlgo::stringequals(a._md5, b._md5) && GuruStringAlgo::stringequals(a._sha1, b._sha1) && GuruStringAlgo::stringequals(a._sha512, b._sha512));
		}

		struct ordenar_nombre {
			bool operator () (const modeloCatalogo::voArchivo& a, const modeloCatalogo::voArchivo& b) {
				return (a._nombre.compare(b._nombre) < 0) ? true : false;
			}
		};

		void clear() {
			_id_archivo=0;
			_nombre.clear();
			_bytes=0;
			_fecha=0;
			_md5.clear();
			_sha1.clear();
			_sha512.clear();
			_estado=0;
		}

		bool empty() { return (_nombre.empty()) ? true : false; }
	};

	class voEtiqueta {
		int64_t	_id_etiqueta;
		string	_nombre;
		string	_directorio;
		string	_filtro;
		string	_excluir;
		int16_t	_opciones;

	public:
		voEtiqueta(int64_t id, const std::string& nombre, const std::string& directorio, const std::string& filtro, const std::string& excluir, int16_t opciones):_id_etiqueta(id), _nombre(nombre), _directorio(directorio), _filtro(filtro), _excluir(excluir), _opciones(opciones) {};
		voEtiqueta():_id_etiqueta(0),_opciones(0) {};
		virtual ~voEtiqueta() {};

		inline int64_t	id() { return _id_etiqueta; };
		inline void		id(int64_t a) { _id_etiqueta = a; };

		inline string	nombre() { return _nombre; };
		inline void		nombre(const string& a) { _nombre = a; };

		inline string	directorio() { return _directorio; };
		inline void 	directorio(const string& a) { _directorio = a; };

		inline string	filtro() { return _filtro; };
		inline void 	filtro(const string& a) { _filtro = a; };

		inline string	excluir() { return _excluir; };
		inline void 	excluir(const string& a) { _excluir = a; };

		inline int16_t	opciones() { return _opciones; };
		inline void 	opciones(int16_t a) { _opciones = a; };

		friend bool operator < (const class voEtiqueta& a, const class voEtiqueta& b) {
			return a._nombre < b._nombre;
		}

		bool opt_sha1() { return (_opciones & modeloCatalogo::sha1) ? true : false; }
		bool opt_sha512() { return (_opciones & modeloCatalogo::sha512) ? true : false; }
		bool opt_md5() { return (_opciones & modeloCatalogo::md5) ? true : false; }

		bool valido() {
			if (_id_etiqueta == 0 && _nombre.empty() && _directorio.empty()) return false;

			return true;
		}

		void clear() {
			_id_etiqueta = 0;
			_nombre.clear();
			_directorio.clear();
			_filtro.clear();
			_excluir.clear();
			_opciones = 0;
		}

		bool empty() {
			return (_nombre.empty()) ? true : false;
		}
	};

	class voDirectorio {
		int64_t	_id_directorio;
		string	_nombre;
		int16_t	_estado;
	public:
		voDirectorio(int64_t id, const string& nombre, int16_t estado=0):_id_directorio(id),_nombre(nombre),_estado(estado) {};
		voDirectorio() {};
		virtual ~voDirectorio() {};

		inline int64_t	id() { return _id_directorio; };
		inline void 	id(int64_t a) { _id_directorio = a; };

		inline string	nombre() { return _nombre; };
		inline void 	nombre(const string& a) { _nombre = a; };

		inline int16_t	estado() { return _estado; };
		inline void 	estado(int16_t a) { _estado = a; };

		friend bool operator < (const class voDirectorio& a, const class voDirectorio& b){
			return (a._nombre.compare(b._nombre) < 0) ? true : false;
		}

		bool valido() {
			if (_nombre.empty()) return false;

			return true;
		}

		void clear() {
			_id_directorio = 0;
			_nombre.clear();
			_estado = 0;
		}

		bool empty() { return (_nombre.empty()) ? true : false; }
	};

	struct voItemCatalogo {
		voEtiqueta		Etiqueta;
		voDirectorio	Directorio;
		voArchivo		Archivo;

		uint64_t		id;			// id del catalogo, unico en el todo el catalogo.

		void clear() {
			Etiqueta.clear();
			Directorio.clear();
			Archivo.clear();

			id = 0;
		}
	};

public:
	modeloCatalogo(const std::string& archivo):archivo_bdd(archivo), cacheEtiqueta(5),cacheDirectorio(10), cacheArchivo(10) {};
	virtual ~modeloCatalogo() {
		if (bdd.conectado()) bdd.desconectar();
	};

	voEtiqueta& agregar_etiqueta(voEtiqueta& data) {
		try {
			data.id(cacheEtiqueta(data.nombre()));
		} catch (GuruCacheException&) {
			string stm;

			stm = "insert or fail into etiqueta (nombre, directorio, filtro, excluir, opciones) values ("
				+ GuruSQLite::comillas(data.nombre()) + ","
				+ GuruSQLite::comillas(GuruDirectorio::comoDirectorio(data.directorio())) + ","
				+ (data.filtro().empty() ? GuruSQLite::comillas(".") : GuruSQLite::comillas(data.filtro())) + ","
				+ (data.excluir().empty() ? GuruSQLite::comillas(".") : GuruSQLite::comillas(data.excluir())) + ","
				+ GuruStringAlgo::itos(data.opciones()) + ")";

			try {
				
				if (bdd.exec(stm)) {
					data.id(bdd.int_sqlite3_last_insert_rowid());

					cacheEtiqueta.insertar(data.nombre(), data.id());
				}
			} catch (GuruSQLite::GuruSQLiteException&) {
				data.id(GuruSQLiteAlgo::getInt(bdd, string("etiqueta"), string("nombre"), string("id_etiqueta"), data.nombre()));

				if (data.id() != 0)
					cacheEtiqueta.insertar(data.nombre(), data.id());
			}
		}

		return data;
	};

	voEtiqueta& etiqueta(voEtiqueta& data) {
		ostringstream oss;

		oss << "select directorio, filtro, excluir, opciones";
		if (data.id() == 0) oss << ", id_etiqueta";
		if (data.nombre().empty()) oss << ", nombre";

		oss << " from etiqueta where ";
		if (!data.nombre().empty()) oss << " nombre=" << GuruSQLite::comillas(data.nombre());
		
		if (!data.nombre().empty() && data.id() != 0) oss << " and id_etiqueta=" << data.id();
		else if (data.id() != 0) oss << " id_etiqueta=" << data.id();

		try {
			GuruSQLite::resultado res(bdd.ejecutar(oss.str()));

			if (!res.fetch()) {
				data.clear();
			} else {
				data.directorio(res->getString(0));
				data.filtro(res->getString(1));
				data.excluir(res->getString(2));
				data.opciones(res->getInt16(3));

				if (data.id() == 0) data.id(res->getInt64(4));
				if (!data.nombre().empty() && data.id() == 0) data.nombre(res->getString(5));
				else if (!data.nombre().empty() && data.id() != 0) data.nombre(res->getString(4));

				cacheEtiqueta.insertar(data.nombre(), data.id());
			}
		} catch (GuruSQLite::GuruSQLiteException&) {
		}

		return data;
	}

	// Version 1
	voArchivo archivo(uint64_t id) {
		string oss;

		oss = "select nombre, tamanio, fecha, md5, sha1, sha512 from archivo where id_archivo=" + GuruStringAlgo::int64astr(id);

		GuruSQLite::resultado res;

		try {
			res = bdd.ejecutar(oss);

			if (!res.fetch()) return voArchivo();
		} catch (GuruSQLite::GuruSQLiteException&) {
			return voArchivo();
		}

		return voArchivo(id, res->getString(0), res->getUInt64(1), res->getUInt64(2), res->getString(3), res->getString(4), res->getString(5), res->getUInt32(6));
	};

	// version 1
	voArchivo& buscar_archivo(voArchivo& data) {
		ostringstream oss;

		oss << "select id_archivo, nombre, tamanio, fecha, md5, sha1, sha512 from archivo where id_archivo <> 0 ";
		if (!data.nombre().empty()) oss << " and nombre=" << GuruSQLite::comillas(data.nombre());
		if (data.bytes()) oss << " and tamanio=" << data.bytes();
		if (data.fecha()) oss << " and fecha=" << "datetime(" << data.fecha() << ", 'unixepoch', 'localtime')";
		if (!data.md5().empty()) oss << " and md5=" << GuruSQLite::comillas(data.md5());
		if (!data.sha1().empty()) oss << " and sha1=" << GuruSQLite::comillas(data.sha1());
		if (!data.sha512().empty()) oss << " and sha512=" << GuruSQLite::comillas(data.sha512());

		try {
			GuruSQLite::resultado res;
			
			res = bdd.ejecutar(oss.str());

			if (!res.fetch()) 
				data.clear();
			else {
				data.id(res->getUInt64(0));
				data.nombre(res->getString(1));
				data.bytes(res->getUInt64(2));
				data.fecha(res->getUInt64(3));
				data.md5(res->getString(4));
				data.sha1(res->getString(5));
				data.sha512(res->getString(6));
			}
		} catch (GuruSQLite::GuruSQLiteException&) {
			//			std::cout << e.what() << endl;
		}

		return data;
	}

	// Version 1
	voArchivo& archivo(voArchivo& data) {
		ostringstream oss;

		data.id(0);

		oss << "select id_archivo from archivo where nombre=" << GuruSQLite::comillas(data.nombre()) << " and tamanio=" << data.bytes() << " and fecha=" << "datetime(" << data.fecha() << ", 'unixepoch', 'localtime') "
			<< " and md5=" << (data.md5().empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(data.md5())) << " and sha1=" << (data.sha1().empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(data.sha1())) << " and sha512=" << (data.sha512().empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(data.sha512()));

		try {
//			data.id_archivo = bdd.ejecutarEX(oss.str())->getUInt64(0);

			GuruSQLite::resultado res(bdd.ejecutar(oss.str()));

			if (res.fetch()) data.id(res->getUInt64(0));

		} catch (GuruSQLite::GuruSQLiteException&) {
//			std::cout << e.what() << endl;
		}

		return data;
	};

	voDirectorio& agregar_directorio(voDirectorio& data) {

		try {
			data.id(cacheDirectorio(data.nombre()));
		} catch (GuruCacheException&) {
			string consulta;

			consulta = "insert or fail into directorio (nombre, estado) values (" + GuruSQLite::comillas(data.nombre()) + "," + GuruStringAlgo::itos(data.estado()) + ")";

			try {
				bdd.exec(consulta);

				data.id(bdd.int_sqlite3_last_insert_rowid());
				cacheDirectorio.insertar(data.nombre(), data.id());
			} catch (GuruSQLite::GuruSQLiteException&) {
//				std::cout << e.what() << endl;

				data.id(GuruSQLiteAlgo::getInt(bdd, string("directorio"), string("nombre"), string("id_directorio"), data.nombre()));

				if (data.id() != 0)
					cacheDirectorio.insertar(data.nombre(), data.id());
			}
		}

		return data;
	};

	// Version 1
	voArchivo& agregar_archivo(voArchivo& data) {
		ostringstream oss;

		oss << "insert or fail into archivo (nombre, tamanio, fecha, md5, sha1, sha512, estado) values ("
			<< GuruSQLite::comillas(data.nombre()) << ","
			<< data.bytes() << ","
			<< "datetime(" << data.fecha() << ", 'unixepoch', 'localtime'),"
			<< (data.md5().empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(data.md5())) << ","
			<< (data.sha1().empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(data.sha1())) << ","
			<< (data.sha512().empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(data.sha512())) << ","
			<< data.estado() << ")";

		try {
			bdd.exec(oss.str());

			data.id(bdd.int_sqlite3_last_insert_rowid());
		} catch (GuruSQLite::GuruSQLiteException&) {
//			std::cout << e.what() << endl;
			archivo(data);
		}

		return data;
	};

	void catalogarV2(const string& etiqueta, const string& directorio) {
		GuruDirectorio::iterator_recursivo itfin;

		voEtiqueta voe;

		voe.nombre(etiqueta);

		this->etiqueta(voe);

		try {
			uint64_t registros = 0;
			int64_t id_directorio;

			regex e, f;
			e.assign((voe.excluir().empty() ? "." : voe.excluir()), FILTRO);
			f.assign((voe.filtro().empty() ? "." : voe.filtro()), FILTRO);

			bdd.exec("BEGIN TRANSACTION");

			string sha1, sha512, md5, directorio_actual;

			for (GuruDirectorio::iterator_recursivo it(directorio); it != itfin; ++it) {

				// agregado el 11/01/2014, algunos archivo pueden NO tener nombre!
				if (it->nombre().empty()) continue;

				bool excluir = false;
				if (voe.filtro().compare(".") != 0)
					if (!regex_search(it.dir() + it->nombre(), f)) continue;

				if (voe.excluir().compare(".") != 0)
					excluir = regex_search(it.dir() + it->nombre(), e);

				if (!excluir) {
					// para que muestre por pantalla el directorio que se esta procesando.
					if (directorio_actual.compare(it.dir()) != 0) {
						if (!it.dir().empty()) 	cout << it.dir() << endl;
						directorio_actual = it.dir();
					}

					sha1.clear(); sha512.clear(); md5.clear();
					if (voe.opt_sha1())
						sha1 = GuruSHA1::archivo(voe.directorio() + it.dir() + it->nombre());
					if (voe.opt_sha512())
						sha512 = GuruSHA4::archivo_sha512(voe.directorio() + it.dir() + it->nombre());
					if (voe.opt_md5())
						md5 = GuruMD5::archivo(voe.directorio() + it.dir() + it->nombre());

					string consulta;

//					if (it.dir().empty()) int a = 0;
					string directorio(it.dir());

				#ifdef __GURUWIN32__
					std::replace_if(directorio.begin(), directorio.end(), bind2nd(std::equal_to<char>(), '\\'), '/');
				#endif

					id_directorio = GuruSQLiteAlgo::id(bdd, cacheDirectorio, "directorio", "nombre", "id_directorio", directorio);

					consulta = "insert into catalogoV2 (id_etiqueta, id_directorio, archivo, tamanio, fecha, md5, sha1, sha512) values \
												  (" + GuruStringAlgo::itos(voe.id()) + "," +
													GuruStringAlgo::itos(id_directorio) + ", " + 
													GuruSQLite::comillas(it->nombre()) + "," + 
													GuruStringAlgo::itos(it->tamanio()) + ", " + 
													"datetime(" + GuruStringAlgo::itos(it->fecha()) + ", 'unixepoch', 'localtime')" + "," + 
													(md5.empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(md5)) + "," +
													(sha1.empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(sha1)) + "," +
													(sha512.empty() ? GuruSQLite::comillas("0") : GuruSQLite::comillas(sha512)) + ")";
					try {
						bdd.exec(consulta);
						registros++;
					} catch (GuruSQLite::GuruSQLiteException&) {
					}
				}
			}

			bdd.exec("END TRANSACTION");
		} catch (GuruSQLite::GuruSQLiteException& e) {
			cout << "Error:" << e.what() << endl;
		}
	}

	void catalogar(const string& nombre_etiqueta) {

		voEtiqueta voe;

		voe.nombre(nombre_etiqueta);

		etiqueta(voe);

		regex e, f;

#if defined REGSTD 
		try {
			e.assign((voe.excluir().empty() ? "." : voe.excluir()), FILTRO);
			f.assign((voe.filtro().empty() ? "." : voe.filtro()), FILTRO);
		} catch (std::regex_error& e) {
			ostringstream oss;
			oss << "Error en creacion de regex, Code:" << e.code() << ", msg:" << e.what();

			throw modeloCatalogoRegex(oss.str());
		}

#elif defined REGBOOST
		e.assign((voe.excluir().empty() ? "." : voe.excluir()), FILTRO);
		f.assign((voe.filtro().empty() ? "." : voe.filtro()), FILTRO);
#endif
		cout << "directorio:" << voe.directorio() << endl;
		cout << "filtro:" << voe.filtro() << endl;
		cout << "exclusion:" << voe.excluir() << endl;

		GuruDirectorio::iterator_recursivo itfin;
		string sha1, sha512, md5, directorio_actual;

		bdd.exec("BEGIN TRANSACTION");
		for (GuruDirectorio::iterator_recursivo it(voe.directorio()); it != itfin; ++it) {
			//			cout << it.dir() << it->sacar_nombre() << endl;

			bool excluir = false;
#if defined REGSTD
			try {
#endif
				if (voe.filtro().compare(".") != 0)
					if (!regex_search(it.dir() + it->nombre(), f)) continue;

				if (voe.excluir().compare(".") != 0)
					excluir = regex_search(it.dir() + it->nombre(), e);
#if defined REGSTD
			} catch (std::regex_error& e) {
				cout << "Error en busqueda de regex, Code:" << e.code() << " - " << e.what() << endl;
			}
#endif
			if (!excluir) {
				// para que muestre por pantalla el directorio que se esta procesando.
				if (directorio_actual.compare(it.dir()) != 0) {
					if (!it.dir().empty()) 	cout << it.dir() << endl;
					directorio_actual = it.dir();
				}

				sha1.clear(); sha512.clear(); md5.clear();
				if (voe.opt_sha1())
					sha1 = GuruSHA1::archivo(voe.directorio() + it.dir() + it->nombre());
				if (voe.opt_sha512())
					sha512 = GuruSHA4::archivo_sha512(voe.directorio() + it.dir() + it->nombre());
				if (voe.opt_md5())
					md5 = GuruMD5::archivo(voe.directorio() + it.dir() + it->nombre());

				catalogar(nombre_etiqueta, it.dir(), it->nombre(), it->tamanio(), it->fecha(), md5, sha1, sha512);
			}
		}

		bdd.exec("END TRANSACTION");
	}

	void catalogar(const string& etiqueta, const string& directorio, const string& archivo, uint64_t tamanio = 0, time_t fecha = 0, const string& md5 = "0", const string& sha1 = "0", const string& sha512 = "0") {

		voArchivo		voa(0, archivo, tamanio, fecha, md5, sha1, sha512);
		voDirectorio	vod(0, directorio);

		agregar_directorio(vod);
		agregar_archivo(voa);
		
		ostringstream oss;

		oss << "insert into catalogo(id_etiqueta, id_directorio, id_archivo) values ("
			<< cacheEtiqueta(etiqueta) << ","
			<< vod.id() << ","
			<< voa.id() << ")";

		try {
			bdd.exec(oss.str());
		} catch (GuruSQLite::GuruSQLiteException& e) {
			std::cout << e.what() << endl;
		}
	}

	uint64_t total_catalogado(const string& etiqueta) {

		string consulta;

		consulta = "select count(id_catalogo) from catalogo a, etiqueta b where a.id_etiqueta = b.id_etiqueta and b.nombre=" + GuruSQLite::comillas(etiqueta);

		try {
			return bdd.ejecutar(consulta)->getUInt64(0, -1);
		} catch (GuruSQLite::GuruSQLiteException& e) {
			std::cout << e.what() << endl;
		}

		return 0;
	}

	void abrir() {
		try {
			bdd = GuruSQLite::conectar(archivo_bdd, ddl);
		} catch (GuruSQLite::GuruSQLiteException& e) {
			throw modeloCatalogoException(e.what());
		}
	};

	typedef voItemCatalogo&(modeloCatalogo::*Metodo)(GuruSQLite::registro& reg, voItemCatalogo& vo);

	class iterator {
	public:
		iterator():modelo(NULL), miembro_callback(NULL),finalizado(true) {};
		iterator(modeloCatalogo* esto, GuruSQLite::bd& base, const string& consulta, Metodo llamada):modelo(esto), miembro_callback(llamada),finalizado(false) {

			res = base.ejecutarEX(consulta);

			if (!res.empty()) {
				actual.clear();
				(modelo->*miembro_callback)(res.registro_actual(), actual);
			} else
				finalizado = true;
		};

		virtual ~iterator() {};

		void operator++() { 
			if (res.fetch()) {
				actual.clear();
				(modelo->*miembro_callback)(res.registro_actual(), actual);
			} else
				finalizado = true;
		};

		voItemCatalogo* operator->() { return static_cast<voItemCatalogo*>(&actual); }

		bool operator!=(const iterator& it2) {
			return (this->finalizado != it2.finalizado) ? true : false;
		}

		bool operator==(const iterator& it2) {
			return (this->finalizado != it2.finalizado) ? true : false;
		}

		void callback(Metodo llamada) {
			miembro_callback = llamada;
		}

	protected:
		voItemCatalogo			actual;
		Metodo					miembro_callback;
		GuruSQLite::resultado	res;
		modeloCatalogo*			modelo;
		bool					finalizado; // true=llego a su fin el iterador, false= todavia no llego a su fin.
	};

	voItemCatalogo& callback_buscar(GuruSQLite::registro& reg, voItemCatalogo& vo) {

		vo.Directorio.nombre(reg.getString(1));
		vo.Archivo.nombre(reg.getString(2));
		vo.Archivo.bytes(reg.getUInt64(3));
		vo.Archivo.md5(reg.getString(5));
		vo.Archivo.sha1(reg.getString(6));
		vo.Archivo.sha512(reg.getString(7));

		return vo;
	}

	// la instruccion like con % es case insensitive
	modeloCatalogo::iterator consulta_buscar(const string& busqueda, const string& etiqueta="") {

		std::string consulta;

		consulta = "select c.id_catalogo, d.nombre, c.archivo, c.tamanio, c.fecha, c.md5, c.sha1, c.sha512 \
					from catalogoV2 c, directorio d, etiqueta e \
					where c.id_directorio = d.id_directorio and e.id_etiqueta = c.id_etiqueta " +
					(!etiqueta.empty() ? string("and e.nombre = '" + etiqueta + "'") : string()) +
					"and (d.nombre like '%" + busqueda + "%' \
						or c.archivo like '%" + busqueda + "%'" +
						(GuruStringAlgo::numerico(busqueda) ? string("or c.tamanio like '%" + busqueda + "%'") : string()) +
						"or c.md5 like '%" + busqueda + "%' \
						or c.sha1 like '%" + busqueda + "%' \
						or c.sha512 like '%" + busqueda + "%')";

		return iterator(this, bdd, consulta, &modeloCatalogo::callback_buscar);
	}

	voItemCatalogo& callback_info_etiqueta(GuruSQLite::registro& reg, voItemCatalogo& vo) {

		vo.Etiqueta.id(reg.getUInt64(0));
		vo.Etiqueta.nombre(reg.getString(1));
		vo.Etiqueta.directorio(reg.getString(2));
		vo.Etiqueta.filtro(reg.getString(3));
		vo.Etiqueta.excluir(reg.getString(4));
		vo.Etiqueta.opciones(reg.getUInt16(5));

		return vo;
	}

	modeloCatalogo::voEtiqueta consulta_info_etiqueta(const string& etiqueta = "") {

		std::string consulta;

		consulta = "select id_etiqueta, nombre, directorio, filtro, excluir, opciones from etiqueta" +
			(!etiqueta.empty() ? string(" where nombre = '" + etiqueta + "'") : string());

		return iterator(this, bdd, consulta, &modeloCatalogo::callback_info_etiqueta)->Etiqueta;
	}

	voItemCatalogo& callback_info_archivo(GuruSQLite::registro& reg, voItemCatalogo& vo) {

		vo.id = reg.getUInt64(0);
		vo.Directorio.nombre(reg.getString(1));
		vo.Archivo.nombre(reg.getString(2));
		vo.Archivo.bytes(reg.getUInt64(3));
//		vo.Archivo.fecha(reg.getString(4));
		vo.Archivo.md5(reg.getString(5));
		vo.Archivo.sha1(reg.getString(6));
		vo.Archivo.sha512(reg.getString(7));
		vo.Etiqueta.nombre(reg.getString(8));

		return vo;
	}

	modeloCatalogo::iterator consulta_info_archivo(uint64_t id) {

		std::string consulta;

		consulta = "select c.id_catalogo, d.nombre, c.archivo, c.tamanio, c.fecha, c.md5, c.sha1, c.sha512, e.nombre \
					from catalogoV2 c, etiqueta e, directorio d \
					where c.id_etiqueta = e.id_etiqueta and c.id_directorio = d.id_directorio \
					and c.id_catalogo = " + GuruStringAlgo::itos(id);

		return iterator(this, bdd, consulta, &modeloCatalogo::callback_info_archivo);
	}

	voItemCatalogo& callback_info_catalogos(GuruSQLite::registro& reg, voItemCatalogo& vo) {

		vo.Etiqueta.id(reg.getUInt64(1));
		vo.Etiqueta.nombre(reg.getString(0));

		return vo;
	}

	modeloCatalogo::iterator consulta_info_catalogos() {

		std::string consulta;

		consulta = "select e.nombre, count(c.id_etiqueta) from etiqueta e, catalogoV2 c where e.id_etiqueta = c.id_etiqueta group by e.nombre order by 2";

		return iterator(this, bdd, consulta, &modeloCatalogo::callback_info_catalogos);
	}

	voItemCatalogo& callback_listar_archivos_desde(GuruSQLite::registro& reg, voItemCatalogo& vo) {

		vo.id = reg.getUInt64(0);
		vo.Archivo.nombre(reg.getString(1));
		vo.Archivo.bytes(reg.getUInt64(2));
//		vo.Archivo.fecha(3);

		return vo;
	}

	modeloCatalogo::iterator consulta_listar_archivos_en(const string& directorio, const string& etiqueta="") {

		std::string consulta;

		consulta = "select c.id_catalogo, c.archivo, c.tamanio, c.fecha \
					from catalogoV2 c, etiqueta e, directorio d \
					where c.id_etiqueta = e.id_etiqueta and c.id_directorio = d.id_directorio \
					and e.nombre =" + GuruSQLite::comillas(etiqueta) + " and d.nombre =" + GuruSQLite::comillas(directorio) +
					" order by c.archivo";

		return iterator(this, bdd, consulta, &modeloCatalogo::callback_listar_archivos_desde);
	}

	voItemCatalogo& callback_listar_directorios_desde(GuruSQLite::registro& reg, voItemCatalogo& vo) {

		vo.Directorio.nombre(reg.getString(0));

		return vo;
	}

	modeloCatalogo::iterator consulta_listar_directorios_en(const string& directorio, const string& etiqueta = "") {

		std::string consulta;
// En caso que sea el directorio raiz siempre es la misma consulta
		if (directorio.empty()) {
			consulta = "select distinct(substr(d.nombre, 1, instr(d.nombre, '/'))) as dir ";
		} else { // hay un directorio la consulta varia.
			consulta = "select distinct(substr(substr(d.nombre, length(" + GuruSQLite::comillas(directorio) + ") + 1), 1, instr(substr(d.nombre, length(" + GuruSQLite::comillas(directorio) + ") + 1), '/'))) as dir ";
		}
		
		consulta += "from catalogoV2 c, directorio d, etiqueta e \
					where d.id_directorio = c.id_directorio and c.id_etiqueta = e.id_etiqueta and e.nombre = " + GuruSQLite::comillas(etiqueta) + " and d.nombre != ''";

		if (!directorio.empty()) consulta += " and d.nombre like " + GuruSQLite::comillas(directorio + "%");
		
		consulta += " order by dir";
		return iterator(this, bdd, consulta, &modeloCatalogo::callback_listar_directorios_desde);
	}

private:
	GuruSQLite::bd	bdd;
	std::string		archivo_bdd;

	GuruCacheLFUEX<std::string, uint64_t> cacheEtiqueta;
	GuruCacheLFUEX<std::string, uint64_t> cacheDirectorio;
	GuruCacheLFUEX<std::string, uint64_t> cacheArchivo;
};

#endif
