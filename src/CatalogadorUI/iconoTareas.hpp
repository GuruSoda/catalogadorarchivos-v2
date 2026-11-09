#ifndef __ICONOTAREAS_HPP__
#define __ICONOTAREAS_HPP__

#include <wx/taskbar.h>
#include <wx/menu.h>

enum {
	PU_EXIT = 10001,
	PU_CATALOGAR,
	PU_BUSCAR,
	PU_BALOON,
	PU_VER
};

class iconoTareas : public wxTaskBarIcon {
public:
	iconoTareas() {

		if (!SetIcon(wxIcon("programa"), wxT("Catalogador de Archivos")))
			wxMessageBox(wxT("El icono de la aplicacion no pudo ser instalado."));

	}
	virtual ~iconoTareas() {}

	void OnMenuExit(wxCommandEvent&) {
		Destroy();
	}

	void OnMenuVer(wxCommandEvent& e) {
	#ifdef __WXMSW__
			ShellExecute(NULL, wxT("open"), wxT("http://localhost:9780/agenteimpresiones/"), NULL, NULL, SW_SHOWDEFAULT);
	#endif
	}

	void OnLeftButtonDClick(wxTaskBarIconEvent&) {
	#ifdef __WXMSW__
			ShellExecute(NULL, wxT("open"), wxT("http://localhost:9780/agenteimpresiones/"), NULL, NULL, SW_SHOWDEFAULT);
	#endif
	}

	wxMenu* CreatePopupMenu() {
		wxMenu *menu = new wxMenu;

		//	menu->Append(PU_CAPTURAR, _T("&Capurar"));
		//	menu->Append(PU_VER_BALOON, _T("&Ver Baloon"));
		menu->SetTitle("Catalogador de Archivos");
		menu->AppendSeparator();
		menu->Append(PU_CATALOGAR, _T("&Catalogar"));
		menu->Append(PU_BUSCAR, _T("&Buscar"));
		menu->Append(PU_BALOON, _T("&Baloon"));
		menu->Append(PU_EXIT, _T("&Terminar"));

		return menu;
	}

	void verBaloon(wxCommandEvent&) {
		ShowBalloon("El Titulo", "Aca el Texto");
	}

	DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(iconoTareas, wxTaskBarIcon)
	EVT_MENU(PU_EXIT, iconoTareas::OnMenuExit)
	EVT_MENU(PU_VER, iconoTareas::OnMenuVer)
	EVT_MENU(PU_BALOON, iconoTareas::verBaloon)
	EVT_TASKBAR_LEFT_DCLICK(iconoTareas::OnLeftButtonDClick)
END_EVENT_TABLE()

#endif
