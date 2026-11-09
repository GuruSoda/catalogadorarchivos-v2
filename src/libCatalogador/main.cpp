#include <GuruHttp/GuruhttpInterface.hpp>
#include <CatalogadorWS.hpp>
#include <GuruLog.hpp>

#include <string>
#include <map>

#ifdef __cplusplus
        extern "C" {
#endif

#if (defined __WIN32__) || (defined WIN32)
#define LIBRARY_EXPORT __declspec(dllexport)
#else
#define LIBRARY_EXPORT
#endif

LIBRARY_EXPORT
bool init(GuruhttpDirVirtual& dir) {
	static modeloCatalogo catalogo(dir.env("base"));
	
	try {
		catalogo.abrir();

		dir.extra_ptr(&catalogo);
	} catch (modeloCatalogoException& e) {
		cout << e.what() << endl;
		return false;
	}

	return true;
}

LIBRARY_EXPORT 
GuruhttpInterface* catalogadorws(const std::map<std::string, std::string>& env) {
	return new GuruhttpSitio<CatalogadorWS>();
}

#ifdef __cplusplus
        }
#endif
