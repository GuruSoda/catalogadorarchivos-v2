///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 14 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GENSETROMSUI_H__
#define __GENSETROMSUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GenSetRoms
///////////////////////////////////////////////////////////////////////////////
class GenSetRoms : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxListBox* m_directorios;
		wxBitmapButton* m_botonAgregarDir;
		wxBitmapButton* m_botonBorrarDir;
		wxTextCtrl* m_filtro;
		wxTextCtrl* m_patron;
		wxButton* m_button6;
		wxButton* m_button2;
		wxButton* m_button3;
		wxNotebook* m_notebook1;
		wxPanel* m_panel4;
		wxTextCtrl* salidaCtrl;
		wxPanel* m_panel3;
		wxTextCtrl* logDebug;
		wxMenuBar* m_menubar2;
		wxMenu* m_menu1;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onAgregarDirectorio( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBorrarDirectorio( wxCommandEvent& event ) { event.Skip(); }
		virtual void onEjecutar( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCopiar( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMover( wxCommandEvent& event ) { event.Skip(); }
		virtual void onSalir( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GenSetRoms( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Generacion de Roms"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,650 ), long style = wxDEFAULT_FRAME_STYLE|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		
		~GenSetRoms();
	
};

#endif //__GENSETROMSUI_H__
