///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CatalogadorBasquet.h"

///////////////////////////////////////////////////////////////////////////

Configuracion::Configuracion( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sizer_catalogador;
	sizer_catalogador = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("[Catalogador]") ), wxHORIZONTAL );
	
	Text_Catalogador = new wxStaticText( sizer_catalogador->GetStaticBox(), wxID_ANY, wxT("Catalogador:"), wxDefaultPosition, wxDefaultSize, 0 );
	Text_Catalogador->Wrap( -1 );
	sizer_catalogador->Add( Text_Catalogador, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_Catalogador = new wxComboBox( sizer_catalogador->GetStaticBox(), wxID_ANY, wxT("http://www.planetaguru.com.ar/catalogadorws/catalogar"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sizer_catalogador->Add( m_Catalogador, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( sizer_catalogador, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sizer_proxy;
	sizer_proxy = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("[Proxy]") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3 = new wxStaticText( sizer_proxy->GetStaticBox(), wxID_ANY, wxT("Usuario:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_Usuario = new wxTextCtrl( sizer_proxy->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_Usuario, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText4 = new wxStaticText( sizer_proxy->GetStaticBox(), wxID_ANY, wxT("Clave:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer2->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_Clave = new wxTextCtrl( sizer_proxy->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer2->Add( m_Clave, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText6 = new wxStaticText( sizer_proxy->GetStaticBox(), wxID_ANY, wxT("Servidor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer2->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_Servidor = new wxTextCtrl( sizer_proxy->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_Servidor, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText7 = new wxStaticText( sizer_proxy->GetStaticBox(), wxID_ANY, wxT("Puerto:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer2->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_Puerto = new wxTextCtrl( sizer_proxy->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_Puerto, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sizer_proxy->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer4->Add( sizer_proxy, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sizer_personalizacion;
	sizer_personalizacion = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("[Personalizacion]") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( sizer_personalizacion->GetStaticBox(), wxID_ANY, wxT("Imagen:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer41->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_imagen = new wxFilePickerCtrl( sizer_personalizacion->GetStaticBox(), wxID_ANY, wxT("basquet.png"), wxT("Select a file"), wxT("All Images files | *.bmp;*.gif;*.png;*.jpg;*.jpeg;*.pcx;*.xpm;*.xbm|\nBMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|\nPNG files (*.png)|*.png|\nJPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|\nPCX files (*.pcx) | *.pcx|\nXPM files | *.xpm|\nXBM files | *.xbm\n"), wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizer41->Add( m_imagen, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	bSizer6->Add( bSizer41, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText9 = new wxStaticText( sizer_personalizacion->GetStaticBox(), wxID_ANY, wxT("Ancho:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer7->Add( m_staticText9, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ancho = new wxSpinCtrl( sizer_personalizacion->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1920, 100 );
	bSizer7->Add( m_ancho, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( sizer_personalizacion->GetStaticBox(), wxID_ANY, wxT("Alto:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer7->Add( m_staticText10, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_alto = new wxSpinCtrl( sizer_personalizacion->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1080, 100 );
	bSizer7->Add( m_alto, 0, wxALL, 5 );
	
	
	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );
	
	
	sizer_personalizacion->Add( bSizer6, 1, wxEXPAND, 5 );
	
	
	bSizer4->Add( sizer_personalizacion, 0, wxEXPAND, 5 );
	
	wxBoxSizer* sizer_botones;
	sizer_botones = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_aplicar = new wxButton( m_panel1, wxID_ANY, wxT("Aplicar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_aplicar, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sizer_botones->Add( bSizer71, 1, 0, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_grabar = new wxButton( m_panel1, wxID_ANY, wxT("Grabar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_grabar, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cancelar = new wxButton( m_panel1, wxID_ANY, wxT("Cancelar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_cancelar, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sizer_botones->Add( bSizer8, 1, 0, 5 );
	
	
	bSizer4->Add( sizer_botones, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	m_panel1->SetSizer( bSizer4 );
	m_panel1->Layout();
	bSizer4->Fit( m_panel1 );
	bSizer3->Add( m_panel1, 1, wxEXPAND|wxALL, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_grabar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Configuracion::onGrabar ), NULL, this );
	m_cancelar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Configuracion::onCancelar ), NULL, this );
}

Configuracion::~Configuracion()
{
	// Disconnect Events
	m_grabar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Configuracion::onGrabar ), NULL, this );
	m_cancelar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Configuracion::onCancelar ), NULL, this );
	
}
