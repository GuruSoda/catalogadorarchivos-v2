///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CATALOGADORWSCLUI_H__
#define __CATALOGADORWSCLUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class UI_MainFrame
///////////////////////////////////////////////////////////////////////////////
class UI_MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxStaticText* m_staticText1;
		wxComboBox* m_comboBusquedas;
		wxButton* m_buttonSalir;
		wxTreeCtrl* m_treeCatalogos;
		wxListCtrl* m_listCtrlContenido;
		wxButton* m_buttonDescargar;
		wxButton* m_buttonSubir;
		wxStaticLine* m_staticline1;
		wxButton* BotonSalir;
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBuscarEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSalirClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		UI_MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Catalogador de Archivos UI"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~UI_MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Conexion
///////////////////////////////////////////////////////////////////////////////
class Conexion : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel2;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrl3;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textCtrl6;
		wxButton* m_button5;
		wxButton* m_button6;
	
	public:
		
		Conexion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Datos de la Conexion"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 453,245 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE ); 
		~Conexion();
	
};

#endif //__CATALOGADORWSCLUI_H__
