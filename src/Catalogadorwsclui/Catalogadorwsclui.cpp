///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Catalogadorwsclui.h"

///////////////////////////////////////////////////////////////////////////

UI_MainFrame::UI_MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panel1, wxID_ANY, wxT("Buscar"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer6->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_comboBusquedas = new wxComboBox( m_panel1, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER ); 
	bSizer6->Add( m_comboBusquedas, 1, wxALL|wxEXPAND, 5 );
	
	m_buttonSalir = new wxButton( m_panel1, wxID_ANY, wxT("Bucar!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_buttonSalir, 0, wxALL, 5 );
	
	
	bSizer3->Add( bSizer6, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_treeCatalogos = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer5->Add( m_treeCatalogos, 0, wxALL|wxEXPAND, 5 );
	
	m_listCtrlContenido = new wxListCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ALIGN_LEFT|wxLC_LIST|wxLC_SINGLE_SEL );
	bSizer5->Add( m_listCtrlContenido, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonDescargar = new wxButton( m_panel1, wxID_ANY, wxT("Descargar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_buttonDescargar, 0, wxALL, 5 );
	
	m_buttonSubir = new wxButton( m_panel1, wxID_ANY, wxT("Subir"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_buttonSubir, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer61, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline1, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	BotonSalir = new wxButton( m_panel1, wxID_ANY, wxT("Salir"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( BotonSalir, 0, wxALL, 5 );
	
	
	bSizer3->Add( bSizer2, 0, wxALIGN_RIGHT, 5 );
	
	
	m_panel1->SetSizer( bSizer3 );
	m_panel1->Layout();
	bSizer3->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Salir") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	m_menubar1->Append( m_menu1, wxT("Menu") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_comboBusquedas->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( UI_MainFrame::OnBuscarEnter ), NULL, this );
	BotonSalir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UI_MainFrame::OnSalirClick ), NULL, this );
}

UI_MainFrame::~UI_MainFrame()
{
	// Disconnect Events
	m_comboBusquedas->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( UI_MainFrame::OnBuscarEnter ), NULL, this );
	BotonSalir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UI_MainFrame::OnSalirClick ), NULL, this );
	
}

Conexion::Conexion( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("[Sitio]") ), wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("URL Catalogador:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	sbSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_textCtrl2, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer9->Add( sbSizer1, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("[Proxy]") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Usuario:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer12->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );
	
	m_textCtrl3 = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_textCtrl3, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText5 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Clave:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer12->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl4 = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_textCtrl4, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer15->Add( bSizer12, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText13 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Servidor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer16->Add( m_staticText13, 0, wxALL, 5 );
	
	m_textCtrl6 = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_textCtrl6, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer15->Add( bSizer16, 1, wxEXPAND, 5 );
	
	
	sbSizer2->Add( bSizer15, 1, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer9->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button5 = new wxButton( m_panel2, wxID_ANY, wxT("Aceptar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( m_panel2, wxID_ANY, wxT("Cancelar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_button6, 0, wxALL, 5 );
	
	
	bSizer9->Add( bSizer8, 0, wxALIGN_RIGHT, 5 );
	
	
	m_panel2->SetSizer( bSizer9 );
	m_panel2->Layout();
	bSizer9->Fit( m_panel2 );
	bSizer7->Add( m_panel2, 1, wxEXPAND|wxALL, 5 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

Conexion::~Conexion()
{
}
