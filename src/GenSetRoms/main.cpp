#include <wx/wx.h>
#include <wx/app.h>

#include "GenSetRomsUI.h"

#include <GuruDirectorio.hpp>
#include <GuruString_algo.hpp>
#include <GuruID.hpp>
#include <GuruError.hpp>

#include <list>
#include <map>

class depuradorJuegos {
public:
	class infoJuego {
		string	_nombre;
		int		_id_directorio;
		static GuruID<std::string>		iddir;

	public:
		infoJuego() {}
		infoJuego(const string& archivo, const string directorio) { agregar(archivo, directorio); }
		virtual ~infoJuego() {}

		void agregar(const string& archivo, const string directorio) {
			_nombre = archivo;
			_id_directorio = iddir.id(directorio);
		}

		inline string  directorio() { return iddir.valor(_id_directorio); }
		inline string  archivo() { return _nombre; }
	};

	typedef std::list<infoJuego>	listaJuegos;
	typedef std::list<string>		filtro;

	std::list<string> directorios;
public:
	depuradorJuegos() {};
	virtual ~depuradorJuegos() {};

	class ordena {
	public:
		ordena() {};
		virtual ~ordena() {};

		bool operator () (infoJuego& a, infoJuego& b) {
			return (a.archivo().compare(b.archivo()) < 0) ? 1 : 0;
		}
	};

	void agregarDirectorioRoms(const string& dir) { directorios.push_back(dir); }
	void borrarDirectorioRoms(const string& dir) { 
		std::list<string>::iterator it = find(directorios.begin(), directorios.end(), dir);

		if (it == directorios.end()) return;
		else directorios.erase(it); 
	}

	void limpiarDirectoriosRoms() { directorios.clear(); }
	std::list<string>& dataDirectorioRoms() { return directorios; }

	// solo saca el nombre del juego sin la region o alguna variante que se encuentre entre parentesis
	string sacar_nombre_juego(const string& nombre) {
		std::string::size_type inicio = nombre.find('(');

		if (inicio != std::string::npos) {
			return GuruStringAlgo::trim_copy(nombre.substr(0, inicio));
		} else return nombre;
	}

	std::list<std::string>& variantes_juego(std::list<std::string>& juego) {
		juego.clear();

		return juego;
	}

	// separa el filtro y lo pone en un contenedor
	int cargarFiltro(const string& str_filtro) {

		filtro_juegos.clear();

		GuruTokenEx t("|,;");

		size_t total = t.extraer(str_filtro);

		for (int i = 0; i < total; i++) filtro_juegos.push_back(t[i]);		

		return filtro_juegos.size();
	}

	uint32_t cargarJuegos(const string& directorio) {
		uint32_t i = 0;
		
		std::string dir(GuruDirectorio::comoDirectorio(directorio));

		for (GuruDirectorio::iterator_recursivo it(dir), itend; it != itend; ++it, i++) {
			infoJuego info;

			info.agregar(it->nombre(), dir);

			juegos.push_back(info);
		}

		juegos.sort(ordena());

		return i;
	}

	listaJuegos& contenedor() { return juegos; }

	list<list<string> > dividir(const string& directorio, const string& filtro) {
		list<list<string> > juegos_separados;

		cargarFiltro(filtro);
		cargarJuegos(directorio);

		string juegoActual, juegoAnterior;
		std::list<string>	juegosFiltrados, tmp_filtrado;

		juegoAnterior = sacar_nombre_juego(juegos.begin()->archivo());

		for (listaJuegos::iterator it = juegos.begin(), itend = juegos.end(); it != itend; ++it) {
			juegoActual = sacar_nombre_juego(it->archivo());

			if (juegoActual.compare(juegoAnterior) == 0) {
				tmp_filtrado.push_back(it->archivo());
			} else {
				juegos_separados.push_back(tmp_filtrado);
				tmp_filtrado.clear();
				tmp_filtrado.push_back(it->archivo());
				juegoAnterior = sacar_nombre_juego(it->archivo());
			}
		}

		juegos_separados.push_back(tmp_filtrado);

		return juegos_separados;
	}

	// realiza el filtrado de los archivos en el directorio,
	list<infoJuego>& filtrar(list<infoJuego>& juegosFiltrados, const string& filtro, const string& directorio="") {

		juegosFiltrados.clear();

		cargarFiltro(filtro);

		juegos.clear();

		if (!directorio.empty()) {
			limpiarDirectoriosRoms();
			cargarJuegos(directorio);
		} else {
			dataDirectorioRoms().sort();
			dataDirectorioRoms().unique();

			for (list<string>::iterator it = dataDirectorioRoms().begin(), itend = dataDirectorioRoms().end(); it != itend; ++it) {
				cargarJuegos(*it);
			}

		}

		if (juegos.empty()) return juegosFiltrados;

		string juegoActual, juegoAnterior, archivoAnterior, archivoActual;
		std::list<listaJuegos::iterator>	juego_encontrado;

		listaJuegos::iterator it = juegos.begin(), itend = juegos.end(); it != itend;

		juegoAnterior = juegoActual = sacar_nombre_juego(it->archivo());
		archivoAnterior = archivoActual = it->archivo();

		do {
			if (juegoActual.compare(juegoAnterior) == 0) {
				juego_encontrado.push_back(it);
			} else {
				for (list<listaJuegos::iterator>::iterator it2 = juego_encontrado.begin(), itend2 = juego_encontrado.end(); it2 != itend2; ++it2) {
					for (filtro::iterator itj = filtro_juegos.begin(), itendj = filtro_juegos.end(); itj != itendj; ++itj) {

						if (GuruStringAlgo::ci_find_substr((*it2)->archivo(), *itj) != string::npos) {
							juegosFiltrados.push_back(infoJuego((*it2)->archivo(), (*it2)->directorio()));

							++it2;

							while (it2 != itend2) {
								if (GuruStringAlgo::ci_find_substr((*it2)->archivo(), *itj) != string::npos) {
									juegosFiltrados.push_back(infoJuego((*it2)->archivo(), (*it2)->directorio()));
								}

								++it2;
							}

							if (it2 == itend2) --it2;

							itj = itendj;
							--itj;
						}
					}
				}
				juego_encontrado.clear();

				juego_encontrado.push_back(it);
			}

			juegoAnterior = juegoActual;

			if (it != itend) ++it;

			if (it == itend) 
				juegoActual.clear();
			else {
				juegoActual = sacar_nombre_juego(it->archivo());
				archivoActual = it->archivo();
			}

		} while (!juegoAnterior.empty());

		return juegosFiltrados;
	}

private:
	listaJuegos juegos;
	filtro		filtro_juegos;
};

// esto va aca por hay en la estructura infoJuego una clase estatica de este mismo tipo y definicio ( no se que es )
GuruID<std::string>		depuradorJuegos::infoJuego::iddir;

class GuruTemplate {
public:
	GuruTemplate() {}
	virtual ~GuruTemplate() {}

	string reemplazar(const string& variable) {
		std::string var;

		if (GuruStringAlgo::ci_find_substr(variable, std::string("archivo")) == 0)
			var = _info.archivo();
		else if (GuruStringAlgo::ci_find_substr(variable, std::string("directorio")) == 0)
			var = _info.directorio();

		return var;
	}

	void cargarInfoJuego(const depuradorJuegos::infoJuego& info) {
		_info = info;
	}

protected:
	depuradorJuegos::infoJuego _info;
};

class ui : public GenSetRoms {
	depuradorJuegos depura;
protected:

	void onAgregarDirectorio(wxCommandEvent& event) { 
		wxDirDialog* dlg = new wxDirDialog(this, "Agregar Directorio de Roms", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

		if (dlg->ShowModal() == wxID_OK) {
			depura.agregarDirectorioRoms(dlg->GetPath().ToStdString());

			m_directorios->Append(dlg->GetPath());
		}
	}

	void onBorrarDirectorio(wxCommandEvent& event) {
		int nro = m_directorios->GetSelection();

		if (nro == wxNOT_FOUND) return;

		depura.borrarDirectorioRoms(m_directorios->GetString(nro).ToStdString());

		m_directorios->Delete(nro);
	}

	void onEjecutar(wxCommandEvent& event) {

		*logDebug << wxT("Directorios a inspeccionar:") << "\n";

		for (list<string>::iterator it = depura.dataDirectorioRoms().begin(), itend = depura.dataDirectorioRoms().end(); it != itend; ++it) {
			*logDebug << wxString(*it) << "\n";
		}

		salidaCtrl->Clear();
		m_statusBar->SetStatusText(wxT(""));

		list<depuradorJuegos::infoJuego> juegosFiltrados;

		depura.filtrar(juegosFiltrados, m_filtro->GetValue().ToStdString());

		if (juegosFiltrados.size() == 0) return;

		m_statusBar->SetStatusText(wxT("Archivos Filtrados:") + GuruStringAlgo::itos(juegosFiltrados.size()));

		GuruTemplate t;
		GuruVariable<GuruTemplate> r(&t, &GuruTemplate::reemplazar);

		for (list<depuradorJuegos::infoJuego>::iterator it = juegosFiltrados.begin(), itend = juegosFiltrados.end(); it != itend; ++it) {
			t.cargarInfoJuego(*it);

			*salidaCtrl << r.reemplazar(m_patron->GetValue().ToStdString())  << "\n";
		}
	}

	void onCopiar( wxCommandEvent& event ) { 
		wxDirDialog* dlg = new wxDirDialog(this, "Directorio a Copiar las roms", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

		if (dlg->ShowModal() == wxID_OK) {
			if (wxMessageBox("Esta Seguro que Desea COPIAR las roms Filtradas a " + dlg->GetPath().ToStdString(), "Confirme", wxYES_NO | wxICON_EXCLAMATION) == wxYES) {

				salidaCtrl->Clear();
				m_statusBar->SetStatusText(wxT(""));

				list<depuradorJuegos::infoJuego> juegosFiltrados;

				depura.filtrar(juegosFiltrados, m_filtro->GetValue().ToStdString());

				if (juegosFiltrados.size() == 0) return;

				m_statusBar->SetStatusText(wxT("Archivos Filtrados:") + GuruStringAlgo::itos(juegosFiltrados.size()));

				for (list<depuradorJuegos::infoJuego>::iterator it = juegosFiltrados.begin(), itend = juegosFiltrados.end(); it != itend; ++it) {

					if (GuruDirectorio::copiarArchivo(it->directorio() + it->archivo(), GuruDirectorio::comoDirectorio(dlg->GetPath().ToStdString()) + it->archivo())) {
						*salidaCtrl << it->directorio() + it->archivo() << "... OK!" << "\n";
					} else
						*logDebug << GuruError().descripcion() << "\n";
				}
			}
		}

	}

	void onMover( wxCommandEvent& event ) { 
		wxDirDialog* dlg = new wxDirDialog(this, "Directorio a Mover las roms", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

		if (dlg->ShowModal() == wxID_OK) {
			if (wxMessageBox("Esta Seguro que Desea MOVER las roms Filtradas a " + dlg->GetPath().ToStdString(), "Confirme", wxYES_NO | wxICON_EXCLAMATION) == wxYES) {
				salidaCtrl->Clear();
				m_statusBar->SetStatusText(wxT(""));

				list<depuradorJuegos::infoJuego> juegosFiltrados;

				depura.filtrar(juegosFiltrados, m_filtro->GetValue().ToStdString());

				if (juegosFiltrados.size() == 0) return;

				m_statusBar->SetStatusText(wxT("Archivos Filtrados:") + GuruStringAlgo::itos(juegosFiltrados.size()));

				for (list<depuradorJuegos::infoJuego>::iterator it = juegosFiltrados.begin(), itend = juegosFiltrados.end(); it != itend; ++it) {

					if (GuruDirectorio::moverArchivo(it->directorio() + it->archivo(), GuruDirectorio::comoDirectorio(dlg->GetPath().ToStdString()) + it->archivo())) {
						*salidaCtrl << it->directorio() + it->archivo() << "... OK!" << "\n";
					} else
						*logDebug << GuruError().descripcion() << "\n";
				}

			}
		}

	}

	void onSalir(wxCommandEvent& event) { 
		cout << "onSalir" << endl;

		if (onNada() == true) Close(); 
		else event.Skip();
 
	}

public:
	ui(wxWindow* parent): GenSetRoms(parent) { }
	virtual ~ui() { cout << "Destruyendome" << endl; }

	bool onNada() {
		cout << "Nada" << endl;

		int answer = wxMessageBox("Seguro que desea salir?", "Confirme", wxYES_NO | wxICON_QUESTION);

		return (answer == wxYES) ? true : false;
	}
};

class wxWidgetsApp : public wxApp
{
	ui* dialog;
public:
	wxWidgetsApp() {};
	virtual ~wxWidgetsApp() {};

	virtual bool OnInit() {
		if (!wxApp::OnInit()) return false;

		dialog = new ui((wxWindow*)NULL);

		dialog->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(wxWidgetsApp::OnClose), NULL, this);

		dialog->Show();
		SetTopWindow(dialog);

		return true;
	}

	void OnClose(wxCloseEvent& event) {
		cout << "OnClose" << endl;

//		if (dialog->onNada() == false) { 
//			event.Skip();
//			return ;
//		}

		ExitMainLoop();
	}
};

DECLARE_APP(wxWidgetsApp)

IMPLEMENT_APP(wxWidgetsApp)

