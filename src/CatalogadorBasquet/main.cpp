#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/busyinfo.h>
#include <wx/dir.h>
#include <wx/menu.h>
#include <wx/window.h>
#include <wx/display.h>
#include <wx/dnd.h>

#include "CatalogadorBasquet.h"
#include <Guruwx/GuruCapturaPantalla.hpp>
#include <Guruwx/GuruImagePanel.hpp>

#include <jsoncpp/json.h>

#include <sstream>
#include <fstream>

#define ID_SALIR			2000
#define ID_CONFIGURACION	2001
#define ID_TITULOCONF		2002
#define DEFAULT_IMAGE		"basquet00.png"

typedef struct {
	std::string catalogador;

	std::string proxy_usuario;
	std::string proxy_clave;
	std::string proxy_servidor;
	std::string proxy_puerto;
	std::string imagen;
	int ancho;
	int alto;
}Config;

void json_a_config(const string& archivo_config, Config& config_apps) {
	Json::Reader lector;
	Json::Value root;

	ifstream ifs("basquet.json");

	lector.parse(ifs, root);

	ifs.close();

	config_apps.catalogador = root["Catalogador"].asString();

	config_apps.proxy_usuario = root["Proxy"]["Usuario"].asString();
	config_apps.proxy_clave = root["Proxy"]["Clave"].asString();
	config_apps.proxy_servidor = root["Proxy"]["Servidor"].asString();
	config_apps.proxy_puerto = root["Proxy"]["Puerto"].asString();

	config_apps.imagen = root["Imagen"].asString();
	config_apps.ancho = root["Resolucion"]["Ancho"].asInt();
	config_apps.alto = root["Resolucion"]["Alto"].asInt();
}

/*

Ejemplo de mover la ventana con el mouse ( no me funciono pero al usuario si)
void MyClass::OnLeftDown(wxMouseEvent& evt) {
	CaptureMouse();
	wxPoint pos = ::wxGetMousePosition();
	wxPoint origin = this->GetPosition();
	int dx =  pos.x - origin.x;
	int dy = pos.y - origin.y;
	m_delta = wxPoint(dx, dy);
}

void MyClass::OnLeftUp(wxMouseEvent& evt) {
	if (HasCapture())
		ReleaseMouse();
}

void MyClass::OnMouseMove(wxMouseEvent& evt) {
	if (evt.Dragging() && evt.LeftIsDown()) {
		wxPoint pos = ::wxGetMousePosition();
		Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
	}
}
// otro: http://earl-technohow.blogspot.com.ar/2014/03/how-to-implement-mouse-drag-using.html
*/

// A custom drop target that accepts URL objects
class URLDropTarget : public wxDropTarget
{
public:
	URLDropTarget() { SetDataObject(new wxURLDataObject); }
	void OnDropURL(wxCoord x, wxCoord y, const wxString& text) {
		// Of course, a real program would do something more
		// useful here...
		wxMessageBox(text, wxT("URLDropTarget: got URL"), wxICON_INFORMATION | wxOK);
	}
	// URLs can’t be moved, only copied
	virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def) {
		return wxDragLink;
	}

	// translate this to calls to OnDropURL() just for convenience
	virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def) {
		if (!GetData())
			return wxDragNone;
		wxString a = ((wxURLDataObject *)m_dataObject)->GetURL();
		OnDropURL(x, y, ((wxURLDataObject *)m_dataObject)->GetURL());
		return def;
	}
};

class configuracionDialog : public Configuracion
{
	Config data_configuracion;
protected:
	void onGrabar(wxCommandEvent& event) { grabarConf(); }
	void onCancelar(wxCommandEvent& event) { EndModal(wxID_CANCEL); }

	void grabarConf() {
		Json::Value root;

		root["Catalogador"] = m_Catalogador->GetValue().ToStdString();

		root["Proxy"]["Usuario"] = m_Usuario->GetValue().ToStdString();
		root["Proxy"]["Clave"] = m_Clave->GetValue().ToStdString();
		root["Proxy"]["Servidor"] = m_Servidor->GetValue().ToStdString();
		root["Proxy"]["Puerto"] = m_Puerto->GetValue().ToStdString();

		root["Imagen"] = m_imagen->GetPath().ToStdString();
		root["Resolucion"]["Ancho"] = m_ancho->GetValue();
		root["Resolucion"]["Alto"] = m_alto->GetValue();

		ofstream ofs("basquet.json");

		ofs << root.toStyledString();

		ofs.close();

		data_configuracion.catalogador = m_Catalogador->GetValue().ToStdString();

		data_configuracion.proxy_usuario = m_Usuario->GetValue().ToStdString();
		data_configuracion.proxy_clave = m_Clave->GetValue().ToStdString();
		data_configuracion.proxy_servidor = m_Servidor->GetValue().ToStdString();
		data_configuracion.proxy_puerto = m_Puerto->GetValue().ToStdString();

		data_configuracion.imagen = m_imagen->GetPath().ToStdString();
		data_configuracion.ancho = m_ancho->GetValue();
		data_configuracion.alto = m_alto->GetValue();

		EndModal(wxID_OK);
	}

	void cargarConf() {
		Json::Reader lector;
		Json::Value root;

		ifstream ifs("basquet.json");

		lector.parse(ifs, root);

		ifs.close();

		m_Catalogador->SetValue(root["Catalogador"].asString());

		m_Usuario->SetValue(root["Proxy"]["Usuario"].asString());
		m_Clave->SetValue(root["Proxy"]["Clave"].asString());
		m_Servidor->SetValue(root["Proxy"]["Servidor"].asString());
		m_Puerto->SetValue(root["Proxy"]["Puerto"].asString());

		m_imagen->SetPath(root["Imagen"].asString());
		m_ancho->SetValue(root["Resolucion"]["Ancho"].asString());
		m_alto->SetValue(root["Resolucion"]["Alto"].asString());
	}

	void Init(wxCommandEvent &evt) { }

public:
	configuracionDialog(wxWindow* parent) :Configuracion(parent) { 
		Connect(wxEVT_INIT_DIALOG, wxCommandEventHandler(configuracionDialog::Init), NULL, this);
		cargarConf(); 
	}

	Config& data() {
		return data_configuracion;
	}

};

class wxWidgetsApp : public wxApp
{
	wxFrame*	frame;
	wxImagePanel* drawPanel;

	wxPoint		m_delta;
	bool		dragging;

	Config		configuracion;

public:
	wxWidgetsApp() { dragging=false; };
    virtual ~wxWidgetsApp() {};

	void OnDropFiles(wxDropFilesEvent& event) {
		if (event.GetNumberOfFiles() > 0) {

			wxString* dropped = event.GetFiles();
			wxASSERT(dropped);

			wxBusyCursor busyCursor;
			wxWindowDisabler disabler;
			wxBusyInfo busyInfo(_("Catalogando, Por Favor Espere..."));

			wxString name;
			wxArrayString files;

			for (int i = 0; i < event.GetNumberOfFiles(); i++) {
				name = dropped[i];
				if (wxFileExists(name))
					files.push_back(name);
				else if (wxDirExists(name))
					wxDir::GetAllFiles(name, &files);
			}
		}
	}

	void OnPopupClick(wxCommandEvent &evt) {
		switch (evt.GetId()) {
		case ID_TITULOCONF: {
						if (frame->GetWindowStyle() & wxDEFAULT_FRAME_STYLE) 
							frame->SetWindowStyleFlag(wxSTAY_ON_TOP | wxTAB_TRAVERSAL);
						else
							frame->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE);
						break;
		case ID_CONFIGURACION:
			configuracionDialog dlg(NULL);

			if (dlg.ShowModal() == wxID_OK) {
				configuracion = dlg.data();
				if (!drawPanel->cargarImagen(wxString(configuracion.imagen.c_str()))) {					
					drawPanel->cargarImagen(wxT(DEFAULT_IMAGE));
				}

				frame->Move(wxGetClientDisplayRect().GetWidth() - configuracion.ancho, wxGetClientDisplayRect().GetHeight() - configuracion.alto);
				frame->SetSize(configuracion.ancho, configuracion.alto);
				drawPanel->paintNow();
			}

			break;
		}
		case ID_SALIR:
			frame->Destroy();
			break;
		}
	}

	void OnClickDerecho(wxMouseEvent& event) {
		// Show popupmenu at position
		wxMenu menu(wxT("Catalogador Basquet"));
		menu.Append(ID_CONFIGURACION, wxT("&Configuracion"));
		menu.Append(ID_TITULOCONF, wxT("&Mostrar Titulo"));
		menu.Append(ID_SALIR, wxT("&Salir"));

		menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxWidgetsApp::OnPopupClick), NULL, this);

		frame->PopupMenu(&menu);
	}

	void OnClickIzquierdo(wxMouseEvent& event) {

		std::ostringstream oss;

		oss << "X:" << event.GetPosition().x << " Y:" << event.GetPosition().y << "\n";

		OutputDebugStringA(oss.str().c_str());

//		wxPoint mouseOnScreen = wxGetMousePosition();
	}

	void OnLeftDown(wxMouseEvent& evt) {
		OutputDebugStringA("Click!");
	//	frame->CaptureMouse();
		m_delta = wxPoint(evt.GetX(), evt.GetY());
		dragging = true;
	}

	void OnLeftUp(wxMouseEvent& evt) {
		OutputDebugStringA("Up!");
	//	if (frame->HasCapture())
	//		frame->ReleaseMouse();

		dragging = false;
	}

	void OnMouseMove(wxMouseEvent& evt) {
		if (dragging) {
			OutputDebugStringA("Dragging!");
			wxPoint pos = wxGetMousePosition();
			frame->Move(pos);
		}
	}

	// el main ...
    virtual bool OnInit() {

		if (wxFileExists("basquet.json"))
			json_a_config("basquet.json", configuracion);
		else {
			configuracionDialog dlg(NULL);

			if (dlg.ShowModal() == wxID_OK) {
				configuracion = dlg.data();
			}
		} 

		wxInitAllImageHandlers();

		frame = new wxFrame(NULL, wxID_ANY, wxT("El Basquet"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP | wxTAB_TRAVERSAL);
		frame->SetSize(configuracion.ancho, configuracion.alto);

		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		drawPanel = new wxImagePanel(frame);
		if (!drawPanel->cargarImagen(configuracion.imagen))
			drawPanel->cargarImagen(wxT(DEFAULT_IMAGE));

		drawPanel->DragAcceptFiles(true);
		drawPanel->SetDropTarget(new URLDropTarget);
		drawPanel->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(wxWidgetsApp::OnDropFiles), NULL, this);
		drawPanel->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxWidgetsApp::OnClickDerecho), NULL, this);

//		drawPanel->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxWidgetsApp::OnLeftDown), NULL, this);
//		drawPanel->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(wxWidgetsApp::OnLeftUp), NULL, this);
//		drawPanel->Connect(wxEVT_MOTION, wxMouseEventHandler(wxWidgetsApp::OnMouseMove), NULL, this);
		
		sizer->Add(drawPanel, 1, wxEXPAND);
//		drawPanel = new wxImagePanel(frame, wxT("d:\\Desarrollo\\CatalogadorArchivosV2\\src\\CatalogadorBasquet\\Backet_basket_shopping_ecommerce.png"));
//		sizer->Add(drawPanel, 1, wxEXPAND);

		frame->SetSizer(sizer);
		frame->Layout();

		// estos dos metodos retornan la misma informacion (no se cual elegir)
		wxRect r = wxGetClientDisplayRect();
//		wxRect r2 = wxDisplay().GetClientArea();

		frame->Move(r.GetWidth() - configuracion.ancho, r.GetHeight() - configuracion.alto);

		frame->Show();
		SetTopWindow(frame);

		return true;
	}
};

DECLARE_APP(wxWidgetsApp)

IMPLEMENT_APP(wxWidgetsApp)
