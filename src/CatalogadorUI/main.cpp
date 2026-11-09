#include <wx/wx.h>

#include <string>

#include <Windows.h>

#include <GuruError.hpp>
#include "iconoTareas.hpp"


class wxWidgetsApp : public wxApp {
	iconoTareas*		m_taskBarIcon;
public:
	wxWidgetsApp() {};
	virtual ~wxWidgetsApp() {};
	virtual bool OnInit() {

		try {
			if (!wxApp::OnInit())
				return false;

			VerificarEjecucion();

			wxInitAllImageHandlers();

			// Aqui la barra de tareas!
			m_taskBarIcon = new iconoTareas();

		} catch (std::string& e) {
			wxMessageBox(e);
			return false;
		}

		return true;
	}

	void wxWidgetsApp::VerificarEjecucion() {
		HANDLE hmut;

		hmut = CreateMutex(NULL, FALSE, L"Catalogador UI");

		if (hmut == NULL || GetLastError() == ERROR_ALREADY_EXISTS) { // ya otro proceso creo el mutex
			std::string msg_error;
			msg_error = GuruError().descripcion();

			if (hmut != NULL) CloseHandle(hmut);
			throw msg_error;
		}
	}

};

DECLARE_APP(wxWidgetsApp)

IMPLEMENT_APP(wxWidgetsApp)
