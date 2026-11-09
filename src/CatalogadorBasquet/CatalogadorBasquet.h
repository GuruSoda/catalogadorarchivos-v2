///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CATALOGADORBASQUET_H__
#define __CATALOGADORBASQUET_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class Configuracion
///////////////////////////////////////////////////////////////////////////////
class Configuracion : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxStaticText* Text_Catalogador;
		wxComboBox* m_Catalogador;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_Usuario;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_Clave;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_Servidor;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_Puerto;
		wxStaticText* m_staticText2;
		wxFilePickerCtrl* m_imagen;
		wxStaticText* m_staticText9;
		wxSpinCtrl* m_ancho;
		wxStaticText* m_staticText10;
		wxSpinCtrl* m_alto;
		wxButton* m_aplicar;
		wxButton* m_grabar;
		wxButton* m_cancelar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onGrabar( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCancelar( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		Configuracion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Configuracion"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~Configuracion();
	
};

#endif //__CATALOGADORBASQUET_H__
