#ifndef __MODELOTDC_HPP__
#define __MODELOTDC_HPP__

#include <string>
#include <stack>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>

#include <expat.h>

#include <GuruString_algo.hpp>

using namespace std;

namespace TDC {

	typedef struct {
		string name;
		string description;
		string rootdir;
		string category;
		string version;
		string date;
		string author;
		string email;
		string homepage;
		string url;
		string comment;
		string clrmamepro;
	} header;

	typedef struct {
		string name;
		string size;
		string crc;
		string sha1;
	} rom;

	typedef struct {
		string name;
		string description;

		vector<rom> roms;
	}game;

	typedef struct {
		header			_header;
		vector<game>	_games;
	}xml;

/////////////// prueba de modelado de datos ///////////////////
	class tdc {
	public:
		tdc() {}
		virtual ~tdc() {}

		typedef string dir;
		typedef rom file;

		typedef set<file> files;
		typedef list<dir> dirs;

		typedef typename dirs::iterator	iterator_dirs;
		typedef typename files::iterator iterator_files;

		typedef vector<pair<string, string> > header_tdc;
		typedef list<pair<iterator_dirs, iterator_files> > cont_tdc;

		typedef typename cont_tdc::iterator iterator;
		typedef typename header_tdc::iterator iterator_header;

		typedef cont_tdc resultado;
		typedef typename cont_tdc::iterator iterator_resultado;

		void add_file(const dir& y, const file& g) {

			if (GuruStringAlgo::ci_find_substr(g.name, string("En tierra b")) != std::string::npos) {

				GuruStringAlgo::ansi_unicode(g.name);
				int a = 1;
			}

			iterator_dirs itd = std::find(_dirs.begin(), _dirs.end(), y);
//			typename iterator_files itf = std::find(_files.begin(), _files.end(), g);

			if (itd == _dirs.end()) itd = _dirs.insert(_dirs.end(), y);
//			if (itf == _files.end()) itf = _files.insert(_files.end(), g);

			iterator_files itf = _files.insert(_files.end(), g);

			content.push_back(make_pair(itd, itf));
		}

		void add_header(const string& key, const string& value) {
			header.push_back(make_pair(key, value));
		}

		template <typename T>
		resultado& buscar(const T& str, resultado& res) {
			res.clear();

			for (TDC::tdc::iterator it = content.begin(), itend = content.end(); it != itend; ++it) {
				if (GuruStringAlgo::ci_find_substr(it->second->name, str) != T::npos || GuruStringAlgo::ci_find_substr(*it->first, str) != T::npos)
					res.push_back(make_pair(it->first, it->second));
			}

			return res;
		}

		iterator begin() { return content.begin(); }
		iterator end() { return content.end(); }
		void	 clear() { content.clear(); header.clear(); }
		size_t	 size() { return content.size(); }

		iterator_header begin_header() { return header.begin(); }
		iterator_header end_header() { return header.end(); }

//		iterator find(const dir& key) { return content.find(key); };

		template <typename T>
		bool exists_file(const T& n) {
			file tmpfile;

			tmpfile.name = n;

			iterator_files itf = _files.find(tmpfile);

			return (itf == _files.end()) ? false : true;

//			for (TDC::tdc::iterator_files it = _files.begin(), itend = _files.end(); it != itend; ++it) {
//				if (GuruStringAlgo::stringequals(it->name, n)) return true;
//			}

			return false;
		}
	private:
		cont_tdc		content;
		header_tdc		header;
		files			_files;
		dirs			_dirs;
	};

	bool operator == (const tdc::file& r1, const tdc::file& r2) {
		return (r1.name.compare(r2.name) == 0) ? true : false;
	}

	bool operator < (const tdc::file& r1, const tdc::file& r2) {
		return (r1.name.compare(r2.name) < 0) ? true : false;
	}

	bool operator > (const tdc::file& r1, const tdc::file& r2) {
		return (r1.name.compare(r2.name) > 0) ? true : false;
	}

/////////////// fin prueba de modelado de datos //////////////

	struct data {
		data(tdc& a):_tdc(a) {}

		bool			cargar_data;
		string			buffer;

		stack<string>	nivel;

		tdc&			_tdc;

		string			tmp;
	};

	static void start_element(void* userData, const XML_Char* char_name, const XML_Char** char_atts) {
		struct data* data = (struct data*) userData;
		data->cargar_data = false;

		// XML_UNICODE_WCHAR_T
	#if defined XML_UNICODE

//		if (GuruStringAlgo::ci_find_substr(wstring(char_name), wstring(L"d!2 v1.13")) != std::string::npos) {

//			wstring b(GuruStringAlgo::ansi_unicode((char*)char_name));
//			int a = 1;
//		}

		std::string name(GuruStringAlgo::unicode_ansi(std::wstring(char_name)));
		std::vector<std::string> atts;

		for (int i = 0; char_atts[i]; i++) atts.push_back(GuruStringAlgo::unicode_ansi(std::wstring(char_atts[i])));
	#else
		std::string name(char_name);
		std::vector<std::string> atts;

		for (int i = 0; char_atts[i]; i++) atts.push_back(char_atts[i]);
	#endif

		if (data->nivel.empty()) {
			data->nivel.push(name);
		} else if (data->nivel.top().compare("datafile") == 0) {
			if (name.compare("game") == 0) {
				data->nivel.push(name);
				data->tmp = atts[1];
			} else if (name.compare("header") == 0) {
				data->nivel.push(name);
			}
		} else if (data->nivel.top().compare("game") == 0) {
			if (name.compare("rom") == 0) {
				rom tmprom;
				for (vector<string>::iterator it=atts.begin(),itend=atts.end();it!=itend;++it,++it) {
					if (it->compare("name") == 0) {
						tmprom.name = *(it+1);
					} else if (it->compare("size") == 0) {
						tmprom.size = *(it + 1);
					} else if (it->compare("crc") == 0) {
						tmprom.crc = *(it + 1);
					} else if (it->compare("sha1") == 0) {
						tmprom.sha1 = *(it + 1);
					}
				}

				data->_tdc.add_file(data->tmp, tmprom);
			} else if (name.compare("description") == 0) {
				data->cargar_data = true;
			}
		} else if (data->nivel.top().compare("header") == 0) {
			data->cargar_data = true;
		} else
			data->nivel.push(name);
	}

	static void end_element(void* userData, const XML_Char* char_name) {
		struct data* data = (struct data*) userData;

	#if defined XML_UNICODE
		std::string name(GuruStringAlgo::unicode_ansi(std::wstring((wchar_t*)char_name)));
	#else
		std::string name(char_name);
	#endif

		if (data->nivel.top().compare("game") == 0) {
			if (name.compare("description") == 0) {
				data->tmp = data->buffer;
			} else if (name.compare("game") == 0) {
				data->nivel.pop();
			}
		} else if (data->nivel.top().compare("header") == 0) {
			if (name.compare("header") == 0) {
				data->nivel.pop();
			} else
				data->_tdc.add_header(name, data->buffer);
		} else 
			data->nivel.pop();

		data->cargar_data = false;
		data->buffer.clear();
	}

	static void char_data(void* userData, const XML_Char* s, int len) {
		struct data* data = (struct data*) userData;

		if (data->cargar_data) {
			for (int a = 0; a<len; a++)
				data->buffer += *s++;
		}
	}

	static TDC::tdc& load_xml(TDC::tdc& _tdc, const string& filexml) {
		std::ifstream	in;
		char			buf[16384];
		int				depth = 0;
		TDC::data		user_data(_tdc);

		if (filexml.empty()) {
			in.copyfmt(std::cin);
			in.clear(std::cin.rdstate());
			in.basic_ios<char>::rdbuf(cin.rdbuf());
		} else {
			in.open(filexml.c_str());
			if (!in.is_open()) {
				cout << "Error abriendo archivo de entrada" << endl;
				return _tdc;
			}
		}

//		XML_Parser parser = XML_ParserCreate(L"UTF-8");
		XML_Parser parser = XML_ParserCreate(NULL);

		XML_SetElementHandler(parser, TDC::start_element, TDC::end_element);
		XML_SetUserData(parser, &user_data);
		XML_SetCharacterDataHandler(parser, TDC::char_data);

		time_t inicio = time(NULL);

		while (8) {
			in.read(buf, sizeof(buf));
			if (in.bad()) break;

			std::streamsize len;
			bool done;

			len = in.gcount();
			done = in.eof();

			if (XML_Parse(parser, buf, static_cast<int>(len), done) == XML_STATUS_ERROR)
				printf("Error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));

			if (done)
				break;
		}

		XML_ParserFree(parser);

//		cout << "Tiempo transcurrido: " << time(NULL) - inicio << endl;

		return _tdc;
	}
};

#endif
