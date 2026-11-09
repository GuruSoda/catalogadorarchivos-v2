#include <argtable/argtable2.h>

#include <GuruHttp/GuruhttpCliente.hpp>
#include <GuruHttp/GuruhttpClienteREST.hpp>
#include <GuruHttp/Guruhttp_algo.hpp>
#include <GuruDirectorio.hpp>
#include <GuruBuffer_algo.hpp>
#include <GuruString_algo.hpp>

#include <iomanip>
#include <iostream>
#include <vector>

/* Ejemplo para hacer un POST en windows con curl de data xml
curl - i - X POST - H "Content-Type: text/xml" - d             ^
"^<?xml version=\"1.0\" encoding=\"UTF-8\" ?^>                ^
^<Transaction^>                                           ^
^<SomeParam1^>Some - Param - 01 ^ < / SomeParam1^>            ^
^<Password^>SomePassW0rd^< / Password^>                 ^
^<Transaction_Type^>00 ^ < / Transaction_Type^>           ^
^<CardHoldersName^>John Smith^< / CardHoldersName^>     ^
^<DollarAmount^>9.97^< / DollarAmount^>                 ^
^<Card_Number^>4111111111111111 ^ < / Card_Number^>       ^
^<Expiry_Date^>1118 ^ < / Expiry_Date^>                   ^
^<VerificationStr2^>123 ^ < / VerificationStr2^>          ^
^<CVD_Presence_Ind^>1 ^ < / CVD_Presence_Ind^>            ^
^<Reference_No^>Some Reference Text^< / Reference_No^>  ^
^<Client_Email^>john@smith.com^< / Client_Email^>       ^
^<Client_IP^>123.4.56.7^< / Client_IP^>                 ^
^<Tax1Amount^>^< / Tax1Amount^>                         ^
^<Tax2Amount^>^< / Tax2Amount^>                         ^
^< / Transaction^>                                          ^
" "http://localhost:8080"
*/

// Ejemplo para Instagram
// https://www.instagram.com/p/BIs7eDwhrCC/a
//

void subir_archivo(const std::string& archivo, const std::string& etiqueta, const std::string& directorio, const std::string& proxy, std::string& url) {
	GuruHttpCliente::response res;
	GuruHttpCliente::opciones_post opt;

	if (!proxy.empty()) opt.proxy_server(proxy);

	GuruDirectorio::GuruArchivo arch;

	if (!GuruDirectorio::propiedades(archivo, arch).Existe()) {
		cout << "No existe el archivo a enviar" << endl;
		return ;
	}

	GuruDirectorio::GuruSplitPath a(archivo);

	url += "subir?nombre=" + GuruhttpAlgo::URLEncode(a.nombre_ext());

	if (!directorio.empty()) url += "&directorio=" + GuruhttpAlgo::URLEncode(directorio);

	url += "&fecha=" + GuruStringAlgo::int64astr(arch.fecha());

	if (!etiqueta.empty()) url += "&etiqueta=" + GuruhttpAlgo::URLEncode(etiqueta);

	GuruBufferChar buf;
	GuruBufferAlgo::archivo_buffer(archivo, buf);

	opt.data(buf.buf());
	opt.largo_data(static_cast<uint32_t>(buf.cantidad_data()));
	opt.atributo("Content-Type:application/octet-stream");

	try {
		res = GuruHttpCliente::post(url, opt);

		if (res.status_code() == 200) {
			Json::Value root;
			Json::Reader	leer;

			leer.parse(res.body(), root, false);

			cout << root["Archivo"].asString() << ":" << root["Status"].asString() << endl;

		} else cout << "Error: Status Code:" << res.status_code() << endl;
	} catch (GuruHttpCliente::GuruHttpException& e) {
		cout << "Error:" << e.what() << endl;
	}
}

void catalogar(const std::vector<std::string>& archivos, const std::string& etiqueta, const std::string& directorio, const std::string& proxy, std::string& url) {

	try {
		GuruHttpCliente::response res;
		GuruHttpCliente::opciones_post opt;

		opt.proxy_server(proxy);

		GuruHttpCliente::opciones_post::tag_input a;
		int i = 0;
		for (std::vector<std::string>::const_iterator it = archivos.begin(), itend = archivos.end(); it != itend; ++it, i++) {
			a.nombre = "archivo-" + GuruStringAlgo::itos(i);
			a.data_file = *it;
			opt.agregar_form(a);
			a.clear();

			a.nombre = "fecha-" + GuruStringAlgo::itos(i);
			a.data_str = GuruStringAlgo::itos(GuruDirectorio::propiedades(*it).fecha());
			opt.agregar_form(a);
			a.clear();

			if (!directorio.empty()) {
				a.nombre = "directorio-" + GuruStringAlgo::itos(i);
				a.data_str = directorio;
				opt.agregar_form(a);
				a.clear();
			}
		}

		if (!etiqueta.empty()) {
			a.nombre = "etiqueta";
			a.data_str = etiqueta;
			opt.agregar_form(a);
			a.clear();
		}

		res = GuruHttpCliente::post(url + "catalogar" , opt);
	} catch (GuruHttpCliente::GuruHttpException& e) {
		cout << "Error:" << e.what() << endl;
	}
}

int main(int argc, char* argv[])
{
/*
	CURL* curl = curl_easy_init();
	string ruta;
//	ruta = curl_easy_unescape(curl, "http://localhost/catalogadorws/fs/Musica/Cerati/1999%20-%20Gran%20Rex%20%2022-10-99/Disco%201/01.%20R%C3%ADo%20Babel%20(Gran%20Rex\'99).mp3", 0, NULL);
	ruta = curl_easy_escape(curl, "c:\\Musica\\Cerati\\1999 - Gran Rex  22-10-99\\Disco 1\\03. Raíz (Gran Rex\'99).mp3", 0);

	ruta = curl_easy_unescape(curl, ruta.c_str(), 0, NULL);

	GuruHttpCliente::response res;
	GuruHttpCliente::opciones_post opt;

	GuruHttpCliente::opciones_post::tag_input a;

	a.campo = "Nombre_Apellido";
	a.data_str = "De las Mercedez San Martin.";
	
	opt.agregar_form(a);
	a.clear();

	a.campo = "Imagen";
	a.data_file = "c:\\Cosas\\05.10.jpg";
	opt.agregar_form(a);
	a.clear();

	a.campo = "Imagen2";
	a.data_file = "c:\\Cosas\\034.jpg";
	opt.agregar_form(a);
	a.clear();

	a.campo = "Imagen3";
	a.data_file = "c:\\Cosas\\0529-Lilly.jpg";
	opt.agregar_form(a);
	a.clear();

	GuruBufferChar buffer;

	GuruBufferAlgo::archivo_buffer("c:\\Cosas\\475_1000.jpg", buffer);

	a.campo = "Imagen4";
	a.data_buffer = buffer.buf();
	a.data_buffer_length = buffer.cantidad_data();
	a.data_buffer_nombre = "475-1000.jpg";
	opt.agregar_form(a);

	res = GuruHttpCliente::post("http://localhost/catalogadorws/subir", opt);

	buffer.liberar();
*/
//--------------------------------------------------------------------------------------------------------------------------
	struct arg_lit* param_help = arg_lit0("H", "help", "Ayuda detallada.");
	struct arg_lit* param_help_mini = arg_lit0("h", NULL, "Resumen de ayuda.");
	struct arg_str* param_busqueda = arg_str0("b", "busqueda", "archivo", "Archivo/Directorio a buscar en el catalogador.");
	struct arg_str* param_proxy = arg_str0("p", "proxy", "proxy", "Proxy a utilizar (http://usuario:clave@server:puerto).");
	struct arg_str* param_etiqueta = arg_str0("e", "etiqueta", "Etiqueta", "Etiqueta dentro del catalogo.");
	struct arg_str* param_directorio = arg_str0("d", "directorio", "Directorio", "Directorio a descargar/subir archivo.");
	struct arg_file* param_archivo = arg_file0("a", "archivo", "Archivo", "Archivo a enviar al catalogador.");
	struct arg_str* param_url = arg_str0("u", "url", "url", "URL del catalogador, default http://www.planetaguru.com.ar/catalogadorws/.");
	struct arg_file* param_archivos = arg_filen(NULL, NULL, NULL, 0, argc + 2, "input file(s)");
	struct arg_end* end = arg_end(20);
	void* argtable[] = { param_help, param_help_mini, param_busqueda, param_proxy, param_etiqueta, param_directorio, param_archivo, param_url, param_archivos, end };
	int nerrors;

	if (arg_nullcheck(argtable) != 0) {
		std::cout << "insufficient memory" << std::endl;
		return 2;
	}

	nerrors = arg_parse(argc, argv, argtable);

	if (nerrors > 0) {
		arg_print_errors(stdout, end, "");
		arg_print_syntax(stdout, argtable, "\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		return 1;
	}

	if (param_help->count > 0) {
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		return 3;
	}

	if (param_help_mini->count > 0) {
		arg_print_syntax(stdout, argtable, NULL);
		return 4;
	}

	std::string busqueda;
	if (param_busqueda->count > 0) busqueda = param_busqueda->sval[0];

	std::string url = "http://www.planetaguru.com.ar/catalogadorws/";
	if (param_url->count > 0) url = param_url->sval[0];

	std::string proxy;
	if (param_proxy->count > 0) proxy = param_proxy->sval[0];

	std::string etiqueta;
	if (param_etiqueta->count > 0) etiqueta = param_etiqueta->sval[0];

	std::string directorio;
	if (param_directorio->count > 0) directorio = param_directorio->sval[0];

	vector<std::string> archivos;
	if (param_archivos->count > 0) {
		for (int i = 0; i < param_archivos->count; i++)
			archivos.push_back(param_archivos->filename[i]);
	}

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

// ---------[Fin Procesamiento de Parametros]-------------------------

	if (!busqueda.empty()) {
		GuruHttpCliente::rest::response res;
		GuruHttpCliente::opciones_get opt;

		if (!proxy.empty()) opt.proxy_server(proxy);

		url += "?b=" + busqueda;

		if (!etiqueta.empty()) url += "&e=" + etiqueta;

		try {
			res = GuruHttpCliente::rest::get(url, opt);

			for (Json::ValueIterator it = res.json().begin(); it != res.json().end(); ++it) {
				//			std::cout << std::setw(8) << std::setfill(' ') << (*it)["id"].asString() << " - " << (*it)["Ruta"].asString() << std::endl;
				std::cout << (*it)["Ruta"].asString() << std::endl;
			}
		} catch (GuruHttpCliente::GuruHttpException& e) {
			cout << "Error:" << e.what() << endl;
		}
	} else if (param_archivo->count > 0) {
		subir_archivo(param_archivo->filename[0], etiqueta, directorio, proxy, url);
	} else if (!archivos.empty()) {
		catalogar(archivos, etiqueta, directorio, proxy, url);
	} else {
		cout << "Que desea hacer?" << endl;
	}

	return 0;
}
