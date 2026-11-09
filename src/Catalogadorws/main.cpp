#include <iostream>
#include <argtable/argtable2.h>

#include <GuruHttp/Guruhttp.hpp>
#include <GuruHttp/GuruhttpServidor.hpp>
#include <GuruHttp/GuruhttpInterface.hpp>
#include <GuruHttp/GuruhttpSitioStats.hpp>
#include <GuruSocket/GuruServidorTCP.hpp>
#include <GuruDirectorio.hpp>

#include <ModeloCatalogo.hpp>
#include <CatalogadorWS.hpp>

#include <jsoncpp/json.h>

#ifdef __GURUWIN32__
#include <Winsvc.h>
#elif defined __GURULINUX__
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#endif

#ifdef __GURUWIN32__
BOOL	Control_Consola(DWORD requerimiento);
HANDLE	Corriendo;
#elif defined __GURULINUX__
void 	Control_Consola(int);
sem_t	Corriendo;
#endif

void mostrar_conf_default() {
	Json::Value root;

	root["Puerto"] = "28275";
	root["Interfaz"] = "0.0.0.0";
	root["bd"] = "mamrarcade.bd";
	root["VirtualDir"] = "/armamews/";

	Json::StyledWriter writer;

	cout << writer.write(root);
}

int main(int argc, char* argv[])
{
/*
	try {
//		modeloCatalogo	a("c:\\Desarrollo\\Catalogador de Archivos V2\\src\\build\\vs2015\\Catalogador\\Debug\\x64\\desarrollo.sqlite3");
		modeloCatalogo	a("d:\\Desarrollo\\CatalogadorArchivosV2\\src\\build\\vs2015\\Catalogador\\Debug\\x64\\desarrollo.sqlite3");
	
		a.abrir()
		
		modeloCatalogo::iterator it = a.consulta_buscar("GURU", "desarrollo");
		modeloCatalogo::iterator itend;

		for (; it != itend; ++it) {
			cout << it->Directorio.nombre() << " - " << it->Archivo.nombre() << endl;
		}
	} catch (modeloCatalogoException& e) {
		cerr << e.what() << endl;
	}
*/
	struct arg_lit* opt_help = arg_lit0("h", "help", "Muestra la ayuda.");
	struct arg_lit* opt_verbose = arg_lit0("v", "verbose", "Muestra informacion por pantalla.");
	struct arg_str* opt_interfaz = arg_str0("i", "interfaz", "<IP>", "Interfaz a escuchar (default:0.0.0.0)");
	struct arg_int* opt_puerto = arg_int0("p", "puerto", "<n>", "Puerto que escucha el servidor (default:80)");
	struct arg_int* opt_velocidad = arg_int0("v", "velocidad", "<n>", "Ancho de banda a utilizar en Kbytes (default:0)");
	struct arg_str* opt_dirvirtual = arg_str0("d", "virtualdir", "<dir virtual>", "Directorio virtual para ingresar (default:/catalogadorws/)");
	struct arg_lit* opt_create_config = arg_lit0("r", "create-config", "Crea una configuracion por default.");
	struct arg_str* opt_config = arg_str0("c", "config", "<archivo>", "Archivo de configuracion.");
	struct arg_str* opt_host = arg_str0(NULL, "host", "<nombre>", "Nombre del host (default: none)");
	struct arg_file* opt_base = arg_file1("b", "bd", "<base.bd>", "Base de Datos con el Catalogo.");

	struct arg_end* end = arg_end(20);

	void* argtable[] = { opt_help, opt_verbose, opt_interfaz, opt_puerto, opt_velocidad, opt_dirvirtual, opt_create_config, opt_config, opt_host, opt_base, end };
	int nerrors;

	opt_interfaz->sval[0] = "0.0.0.0";
	opt_puerto->ival[0] = 80;
	opt_velocidad->ival[0] = 0;

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

	if (opt_create_config->count > 0) {
		mostrar_conf_default();
		return 2;
	}

	string bdd;

	if (opt_base->count > 0) { bdd = *opt_base->filename; }

	modeloCatalogo catalogo(bdd);

	try {
		catalogo.abrir();
	} catch(modeloCatalogoException& e) {
		std::cerr << "Error abriendo el catalogo " << bdd << " - " << e.what() << endl;
		return 3;
	}

	GuruhttpDirVirtual	catalogadorws;
	catalogadorws.sitio("/catalogadorws/");
	catalogadorws.agregarHost("*");
	catalogadorws.interfaz(new GuruhttpSitio<CatalogadorWS>());
	catalogadorws.velocidad(opt_velocidad->ival[0] * 1024);
	catalogadorws.extra_ptr(&catalogo);

	GuruhttpDirVirtual	estadistica;
	estadistica.sitio("/stats/");
	estadistica.agregarHost("*");
	estadistica.interfaz(new GuruhttpSitio<stats>());

	if (opt_dirvirtual->count > 0) catalogadorws.sitio(opt_dirvirtual->sval[0]);

	if (opt_verbose->count > 0) catalogadorws.agregarLog("+", "%fecha822% | %status% | %ip_request% | %request% | %request.User-Agent%");

	if (opt_host->count > 0) {
		GuruToken tk(" ,;|");
		vector<string> tokens;

		if (!tk.extraer(opt_host->sval[0], tokens).empty())
			catalogadorws.agregarHost(tokens);
	}

	GuruDirectorio::GuruSplitPath ruta(argv[0]);
	GuruDirectorio::cambiarDirectorio(ruta.drive() + ruta.dir());

#ifdef __GURUWIN32__
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)Control_Consola, true);
	Corriendo = CreateSemaphore(NULL, 0, 2, NULL);
#elif defined __GURULINUX__
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, Control_Consola);
	signal(SIGQUIT, Control_Consola);
	signal(SIGINT, Control_Consola);

	sem_init(&Corriendo, 0, 0);
#endif

	try {
		inicializar_GuruSocket();

		GuruServidorTCP<Guruhttp> w(opt_puerto->ival[0], opt_interfaz->sval[0]);

		estadistica.extra_ptr(static_cast<void*>(&w));

		GuruSingleton<GuruhttpConfig>::instancia()->nuevo_sitio(catalogadorws);
		GuruSingleton<GuruhttpConfig>::instancia()->nuevo_sitio(estadistica);

		w.iniciar();

		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

#ifdef __GURUWIN32__
		WaitForSingleObject(Corriendo, INFINITE);
#elif defined __GURULINUX__
		sem_wait(&Corriendo);
#endif

		w.detener();

		GuruSingleton<GuruhttpLog>::instancia()->terminar();

#ifdef __GURUWIN32__
		CloseHandle(Corriendo);
#elif defined __GURULINUX__
		sem_destroy(&Corriendo);
#endif
	}
	catch (GuruSocketException& e) {
		cout << "ERROR:" << e.what() << endl;
	}
	catch (...) {
		cout << "Hubo una excepcion no atrapada!!!" << endl;
	}

	limpiar_GuruSocket();

	cout << "Finalizando ..." << endl;
	return 0;
}

#ifdef __GURUWIN32__
BOOL Control_Consola(DWORD requerimiento)
#elif defined __GURULINUX__
void Control_Consola(int requerimiento)
#endif
{
#ifdef __GURUWIN32__
	switch (requerimiento) {
	case CTRL_C_EVENT: cout << "Cancelado por CTRL-C ..." << endl;
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		break;
	}

	ReleaseSemaphore(Corriendo, 1, NULL);
	return TRUE;
#elif defined __GURULINUX__
	sem_post(&Corriendo);
#endif
}
