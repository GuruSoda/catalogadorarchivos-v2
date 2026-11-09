#include <GuruSQLite.hpp>
#include <GuruDirectorio.hpp>
#include <GuruString_algo.hpp>
#include <GuruAlgo.hpp>

#include <argtable/argtable2.h>

#include <string>
#include <locale>

int main(int argc, char* argv[])
{
/*
//	char* nose = setlocale(LC_ALL, "es_AR.UTF-8");
//	cout << "Actual:" << nose << endl;
	std::locale::global(locale("spanish-argentina"));

//	setlocale(LC_ALL, "es_AR.UTF-8");
//	setlocale(LC_ALL, "spanish");
	cout << setlocale(LC_ALL, NULL) << endl;

//	cout.imbue(locale("es_AR.UTF-8"));
//	cout.imbue(locale(""));

	cout << "Buen año en " << __func__ << endl;

//	return 0;
*/
	struct arg_lit* opt_help = arg_lit0("h", "help", "Muestra la ayuda.");
	struct arg_lit* opt_verbose = arg_lit0("v", "verbose", "Muestra informacion por pantalla.");
	struct arg_file* opt_base = arg_file1("c", "catalogo", "<catalogo.sqlite>", "Catalogo Destino.");
	struct arg_str* opt_directorio = arg_str0("d", "directorio", "<DIR>", "Directorio Origen a ser comparado.");
	struct arg_str* opt_etiqueta = arg_str0("e", "etiqueta", "<Etiqueta>", "Etiqueta del catalogo.");
	struct arg_lit* opt_listetiqueta = arg_lit0("l", "listar", "Lista las etiquetas del catalogo.");

	struct arg_lit* opt_catdir = arg_lit0(NULL, "cat-dir", "Compara el catalogo (-c) con el directorio especificado con (-d).");
	struct arg_lit* opt_dircat = arg_lit0(NULL, "dir-cat", "Compara el directorio (-d) Lista las etiquetas del catalogo (-c) .");

	struct arg_end* end = arg_end(10);

	void* argtable[] = { opt_help, opt_verbose, opt_base, opt_directorio, opt_etiqueta, opt_listetiqueta, opt_catdir, opt_dircat, end };
	int nerrors;

	nerrors = arg_parse(argc, argv, argtable);

	if (nerrors > 0) {
		arg_print_errors(stdout, end, "");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		return 1;
	}

	if (opt_help->count > 0) {
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		return 0;
	}

	GuruSQLite::bd catalogo;
	if (opt_base->count > 0) {
		catalogo = GuruSQLite::conectar(*opt_base->filename);

		if (!catalogo.conectado()) {
			std::cout << "Error abriendo el catalogo." << endl;
			return 3;
		}
	}

	std::string directorio;
	if (opt_directorio->count > 0) {
		directorio = GuruDirectorio::comoDirectorio(string(opt_directorio->sval[0]));
	} else if (opt_listetiqueta->count == 0) {
		cout << "Falta especificar el directorio a comparar." << endl;
		return 5;
	}

	if (opt_listetiqueta->count > 0) {

		std::string consulta;

		consulta = "select nombre from etiqueta";

		GuruSQLite::resultado res;

		try {
			res = catalogo.ejecutar(consulta);

//			for (GuruSQLite::resultado::iterator it = res.begin(); it != res.end(); ++it) cout << it->getString(0) << endl;
		
			while (res.fetch()) 
				cout << res->getString(0) << endl;

		} catch (GuruSQLite::GuruSQLiteException& e) {
			cout << "Error:" << e.what() << endl;
		}

		return 4;
	}

	uint64_t id_etiqueta = 0;
	if (opt_etiqueta->count > 0) {

		std::string consulta;

		consulta = "select id_etiqueta from etiqueta where lower(nombre) = lower(" + GuruSQLite::comillas(opt_etiqueta->sval[0]) + ")";

		GuruSQLite::resultado res;

		res = catalogo.ejecutar(consulta);

		if (!res.fetch()) {
			cout << "Etiqueta Inexistente:" << opt_etiqueta->sval[0] << endl;
			return 4;
		} else {
			id_etiqueta = res->getUInt64(0);
		}

		res.finalizar();
	}

	bool catdir = false, dircat = false;
	if (opt_catdir->count > 0 && opt_dircat->count > 0) {
		cout << "Error en parametros, --cat-dir y --dir-cat?" << endl;
		return 6;
	} else if (opt_catdir->count == 0 && opt_dircat->count == 0) {
		cout << "Error en parametros, --cat-dir o --dir-cat?" << endl;
		return 7;
	} else if (opt_catdir->count > 0)
		catdir = true;
	else if (opt_dircat->count > 0)
		dircat = true;
	else {
		cout << "Falta parametro --cat-dir o --dir-cat." << endl;
		return 8;
	}

	bool verbose = false;
	if (opt_verbose->count > 0) verbose = true;

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
	if (dircat) {
		GuruSQLite::resultado res;

		for (GuruDirectorio::iterator_recursivo it(directorio), itend; it != itend; ++it) {
			std::string consulta;

			consulta = "select c.id_catalogo, c.tamanio, strftime('%s', c.fecha, 'utc') from catalogoV2 c, directorio d where d.id_directorio = c.id_directorio ";
			consulta += " and c.archivo=" + GuruSQLite::comillas(it->nombre());
			consulta += " and d.nombre=" + GuruSQLite::comillas(it.dir());

			if (id_etiqueta)
				consulta += " and c.id_etiqueta=" + GuruStringAlgo::itos(id_etiqueta);

			try {
				res = catalogo.ejecutar(consulta);

				if (!res.fetch()) {
					cout << it.dir() << it->nombre() << " --> NO Existe." << endl;
				} else {
					GuruDirectorio::GuruArchivo ar(GuruDirectorio::propiedades(directorio + it.dir() + it->nombre()));

					if (!ar.Existe()) {
						cout << it.dir() << it->nombre() << " NO Existe, Que paso!?!?!?" << endl;
                    } else if (ar.tamanio() != res->getUInt64(1)) {
                        cout << it.dir() << it->nombre() << " Distinto Tamaño." << "(" <<  ar.tamanio() << " <> " <<  res->getUInt64(1) << ")" << endl;
                    } else if (ar.fecha() != res->getUInt64(2))
                        cout << it.dir() << it->nombre() << " Distinta Fecha." << "(" <<  GuruAlgo::armarFecha("%DD%/%MM%/%AA% %H%:%M%:%S%", ar.fecha()) << " <> " <<  GuruAlgo::armarFecha("%DD%/%MM%/%AA% %H%:%M%:%S%", res->getUInt64(2)) << ")" << endl;
				}
			
				res.finalizar();

			} catch (GuruSQLite::GuruSQLiteException& e) {
				cout << "Error:" << e.what() << endl;
			}
		}
	} else if (catdir) {
		std::string consulta;

		consulta = "select d.nombre || c.archivo, c.tamanio, strftime('%s', c.fecha, 'utc'), c.id_catalogo from catalogoV2 c, directorio d where d.id_directorio = c.id_directorio";

		if (id_etiqueta)
			consulta += " and c.id_etiqueta=" + GuruStringAlgo::itos(id_etiqueta);

		try {
			GuruSQLite::resultado res;
			
			res = catalogo.ejecutar(consulta);

			std::string ruta;

			for (GuruSQLite::resultado::iterator itend, it = res.begin(); it != itend; ++it) {
				ruta = directorio + it->getString(0);

				GuruDirectorio::GuruArchivo ar(GuruDirectorio::propiedades(ruta));

				if (!ar.Existe()) {
					cout << ruta << " NO Existe." << endl;
				} else if (ar.tamanio() != it->getUInt64(1)) {
					cout << ruta << " Distinto Tamaño." << "(" << it->getUInt64(1) << " <> " << ar.tamanio() << ")" << endl;
				} else if (ar.fecha() != it->getUInt64(2))
					cout << ruta << " Distinta Fecha." << "(" << GuruAlgo::armarFecha("%DD%/%MM%/%AA% %H%:%M%:%S%", it->getUInt64(2)) << " <> " << GuruAlgo::armarFecha("%DD%/%MM%/%AA% %H%:%M%:%S%", ar.fecha()) << ")" << endl;
			}
		} catch (GuruSQLite::GuruSQLiteException& e) {
			cout << "Error:" << e.what() << endl;
		}
	}

	return 0;
}
