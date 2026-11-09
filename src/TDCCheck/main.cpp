#include <iostream>
#include <fstream>
#include <locale>

#include <fcntl.h>
#include <io.h>

#include <windows.h>

#include <argtable/argtable2.h>
#include <GuruCompat.h>
#include <GuruDirectorio.hpp>
#include <GuruAlgo.hpp>
#include <GuruString_algo.hpp>
#include <modeloTDC.hpp>

#include <GuruVariable.hpp>

using namespace std;

class GuruTemplate {
public:
	GuruTemplate() {}
	virtual ~GuruTemplate() {}

	string reemplazar(const string& variable) {
		std::string var;

		if (variable.compare("epoc") == 0)
			var = "1";
		else if (variable.compare("fecha822") == 0)
			var = "2";

		return var;
	}

protected:
};

int main(int argc, char* argv[])
{
/*
	GuruTemplate a;

	GuruVariableMap m;

	m.insertar("Hola", "nnnn");
	m.insertar("Chau", "hhhh");

	cout << m.reemplazar("---ddd%Hola%%Chau%");

//	cout << GuruVariable<GuruTemplate>(&a, &GuruTemplate::reemplazar).reemplazar("----%epoc%%fecha822%");
*/
/*
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

//	std::locale::global(std::locale("spanish"));
//	wcout.imbue(std::locale());

//	_setmode(_fileno(stdout), _O_U16TEXT);
*/
/*
	for (GuruDirectorio::iterator_recursivo it("T:\\DOS\\Total DOS Collection\\1992\\", "d*.*"), itend; it != itend; ++it) {
		if (GuruStringAlgo::ci_find_substr(it->nombre(), string("d!2")) != std::string::npos) {
//			wcout << GuruStringAlgo::ansi_unicode(it.dir() + it->nombre()) << endl;
//			cout << GuruStringAlgo::unicode_ansi(string(it.dir() + it->nombre()).c_str()) << endl;
			cout << it.dir() + it->nombre() << endl;
//			cout << it.dir() << it->nombre() << endl;
		}
	}

	return 0;
*/
/*
char* locale = setlocale(LC_ALL, "English"); // Get the CRT's current locale.
std::locale lollocale(locale);
setlocale(LC_ALL, locale); // Restore the CRT.
std::wcout.imbue(lollocale); // Now set the std::wcout to have the locale that we got from the CRT.
std::wcout << L"¡Hola!";
std::cin.get();
return 0;
*/
/*
//	GuruStringAlgo::istring s;
	std::string s;

	s = "Nada Personal";

	s.compare("nada personal");
*/
// Metodo 1
	std::locale::global(std::locale(""));
	wcout.imbue(std::locale());

// Metodo 2
//	std::locale::global(locale("Spanish_Spain.1252"));
//	std::locale::global(locale("Spanish"));

//	cout << GuruStringAlgo::string_upper(string("Como estás???")) << endl;

//	cout << "Como estás???" << endl;

//	GuruStringAlgo::stringequals(string("HOLÁ"), string("holáaaaaaa"));

//	GuruStringAlgo::ci_find_substr(string("000HOLÁAAAAA"), string("holá"));


	struct arg_lit*  ayuda		= arg_lit0("h","help", "Muestra la ayuda.");
	struct arg_file* xml		= arg_file0("i","xml","<xml>", "Archivo .XML a procesar.");
	struct arg_file* dir		= arg_file0("d", "dir", "<dir>", "Directorio a comparar.");
	struct arg_lit*  dump		= arg_lit0(NULL, "dump", "Realiza un vuelco procesado del .XML.");
	struct arg_lit*  dumpheader = arg_lit0(NULL, "dumpheader", "Realiza un vuelco del header procesado del .XML.");
	struct arg_str*  buscar		= arg_str0(NULL, "buscar", "<string>", "Busca coindidencia en nombre de archivos.");
	struct arg_end*  end		= arg_end(10);

	void* argtable[] = {ayuda,xml,dump,dumpheader, buscar, dir, end};

	if (arg_nullcheck(argtable) != 0) {
        cout << "insufficient memory" << endl;
        return 2;
	}

	if (argc == 1) {
		arg_print_glossary(stdout, argtable, "      %-30s %s\n");
		return 4;
	}

	xml->filename[0]="-";

	int nerrors = arg_parse(argc,argv,argtable);

	if (ayuda->count > 0) {
		arg_print_syntax(stdout,argtable,"\n");
		return 1;
	}

    if (nerrors > 0) {
        arg_print_errors(stdout,end,"");
        cout << "Pruebe --help por mas informacion" << endl;
		return 3;
        }

	string entrada;
	if (xml->count > 0) {
		entrada = xml->filename[0];
	}

	if (dump->count > 0) {
		TDC::tdc _tdc;

		TDC::load_xml(_tdc, entrada);

		for (TDC::tdc::iterator it = _tdc.begin(), itend = _tdc.end(); it != itend; ++it) {			
			cout << *it->first << GuruDirectorio::divisor_directorio << it->second->name << endl;
		}

		cout << endl << "Total Archivos: " << _tdc.size() << endl;

		return 10;
	}

	if (dumpheader->count > 0) {
		TDC::tdc _tdc;

		TDC::load_xml(_tdc, entrada);

		for (TDC::tdc::iterator_header it = _tdc.begin_header(), itend = _tdc.end_header(); it != itend; ++it) {
			cout << it->first << ":" << it->second << endl;
		}

		return 11;
	}

	if (buscar->count > 0) {
		TDC::tdc _tdc;

		TDC::load_xml(_tdc, entrada);

		TDC::tdc::resultado res;

		_tdc.buscar(string(buscar->sval[0]), res);

		for (TDC::tdc::iterator_resultado it = res.begin(), itend = res.end(); it != itend; ++it) {
			cout << *it->first << GuruDirectorio::divisor_directorio << it->second->name << endl;
		}

		return 12;
	}

	GuruDirectorio::type_string directorio;
	if (dir->count > 0) {
		directorio = GuruDirectorio::comoDirectorio(GuruDirectorio::type_string(dir->filename[0]));
	} else {
		cout << "Falta especificar el directorio." << endl;
		return 5;
	}

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

// ---------------------------------------------------------------------------------------

	TDC::tdc _tdc;

//	if (1) {
//		load_xml(_tdc, entrada);
//	}

//	Sleep(10000);

	if (1) {
		cout << "Comparacion: FS -> XML" << endl;
		TDC::tdc _tdc;

		GuruAlgo::GuruTiemposEX a;

		a.iniciar(); load_xml(_tdc, entrada); a.detener();

		cout << "Tiempo de procesamiento del XML: " << a.segundos() << " seg." << endl;

		a.iniciar();

		uint32_t existe=0, noexiste = 0;
		for (GuruDirectorio::iterator_recursivo it(directorio), itend; it != itend; ++it) {
			// cout << it.dir() << it->nombre() << endl;
			if (GuruStringAlgo::ci_find_substr(it->nombre(), string("d!2")) != std::string::npos)
				cout << it->nombre() << endl;
				// wcout << GuruStringAlgo::ansi_unicode(it->nombre()) << endl;

			if (!_tdc.exists_file(it->nombre())) {
				// cout << it.dir() << it->nombre() << endl;
				noexiste++;
			} else 
				existe++;
		}

		a.detener();

		cout << "Tiempo de chequeo: " << a.segundos() << endl;

		cout << "Total: " << _tdc.size() << endl << "Existentes: " << existe << endl << "INExistente: " << noexiste << endl;
	}

	if (1) {
		cout << "Comparacion: XML -> FS" << endl;
		TDC::tdc _tdc;

		GuruAlgo::GuruTiempos a;
		TDC::load_xml(_tdc, entrada);
		a.detener();

		cout << "Tiempo de procesamiento del XML: " << a.segundos() << " seg." << endl;

		uint32_t existe = 0, noexiste = 0;

		string archivo;
		a.iniciar();

		for (TDC::tdc::iterator it = _tdc.begin(), itend = _tdc.end(); it != itend; ++it) {
/*
			if (GuruStringAlgo::ci_find_substr(it->second->name, string("Polda")) != std::string::npos) {
				if (!GuruDirectorio::existe(it->second->name)) {
					wcout << GuruStringAlgo::ansi_unicode(it->second->name) << endl;
//					WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), utf16line.c_str(), utf16line.size(), nullptr, nullptr);
				}
			}
*/
			archivo = directorio + *it->first + GuruDirectorio::divisor_directorio + it->second->name;

			if (GuruStringAlgo::ci_find_substr(it->second->name, string("Polda")) != std::string::npos) {

				wstring b(GuruStringAlgo::ansi_unicode(it->second->name));
//				int a = 1;
			}

			if (!GuruDirectorio::existe(archivo)) {
			//	wcout << GuruStringAlgo::ansi_unicode(it->second->name) << endl;
				cout << it->second->name << endl;
				noexiste++;
			} else
				existe++;
		}

		a.detener();

		cout << "Tiempo de chequeo: " << a.segundos() << endl;

		cout << "Total: " << _tdc.size() << endl << "Existentes: " << existe << endl << "INExistente: " << noexiste << endl;
	}

	return 0;
}
