///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 14 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GenSetRomsUI.h"

///////////////////////////////////////////////////////////////////////////

GenSetRoms::GenSetRoms( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sizeDirectorio;
	sizeDirectorio = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Directorios de Roms") ), wxHORIZONTAL );
	
	m_directorios = new wxListBox( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SINGLE ); 
	sizeDirectorio->Add( m_directorios, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_botonAgregarDir = new wxBitmapButton( m_panel1, wxID_ANY, wxArtProvider::GetBitmap( wxART_NEW, wxART_BUTTON ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer15->Add( m_botonAgregarDir, 0, wxALL, 5 );
	
	m_botonBorrarDir = new wxBitmapButton( m_panel1, wxID_ANY, wxArtProvider::GetBitmap( wxART_DELETE, wxART_BUTTON ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer15->Add( m_botonBorrarDir, 0, wxALL, 5 );
	
	
	sizeDirectorio->Add( bSizer15, 0, wxEXPAND, 5 );
	
	
	bSizer4->Add( sizeDirectorio, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sizerRegiones;
	sizerRegiones = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Regiones") ), wxHORIZONTAL );
	
	m_filtro = new wxTextCtrl( m_panel1, wxID_ANY, wxT("(Europe),(Spain),(USA),(Japan)"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerRegiones->Add( m_filtro, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( sizerRegiones, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sizerPatron;
	sizerPatron = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Patron") ), wxHORIZONTAL );
	
	m_patron = new wxTextCtrl( m_panel1, wxID_ANY, wxT("%Directorio%%Archivo%"), wxDefaultPosition, wxDefaultSize, wxTE_LEFT );
	sizerPatron->Add( m_patron, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer4->Add( sizerPatron, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sizerEjecutar;
	sizerEjecutar = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Ejecutar") ), wxHORIZONTAL );
	
	m_button6 = new wxButton( m_panel1, wxID_ANY, wxT("Ejecutar"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEjecutar->Add( m_button6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button2 = new wxButton( m_panel1, wxID_ANY, wxT("Copiar"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEjecutar->Add( m_button2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button3 = new wxButton( m_panel1, wxID_ANY, wxT("Mover"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEjecutar->Add( m_button3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( sizerEjecutar, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sizerResultado;
	sizerResultado = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Resultado") ), wxVERTICAL );
	
	m_notebook1 = new wxNotebook( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel4 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	salidaCtrl = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_LEFT|wxTE_MULTILINE|wxTE_READONLY );
	bSizer7->Add( salidaCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer7 );
	m_panel4->Layout();
	bSizer7->Fit( m_panel4 );
	m_notebook1->AddPage( m_panel4, wxT("Salida"), true );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	logDebug = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE|wxTE_READONLY );
	bSizer5->Add( logDebug, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( bSizer5 );
	m_panel3->Layout();
	bSizer5->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Debug"), false );
	
	sizerResultado->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer4->Add( sizerResultado, 1, wxEXPAND|wxALL, 5 );
	
	
	m_panel1->SetSizer( bSizer4 );
	m_panel1->Layout();
	bSizer4->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar2 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Salir") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItem1->SetBitmaps( wxArtProvider::GetBitmap( wxART_QUIT, wxART_MENU ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItem1->SetBitmap( wxArtProvider::GetBitmap( wxART_QUIT, wxART_MENU ) );
	#endif
	m_menu1->Append( m_menuItem1 );
	
	m_menubar2->Append( m_menu1, wxT("Archivo") ); 
	
	this->SetMenuBar( m_menubar2 );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_botonAgregarDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onAgregarDirectorio ), NULL, this );
	m_botonBorrarDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onBorrarDirectorio ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onEjecutar ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onCopiar ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onMover ), NULL, this );
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenSetRoms::onSalir ) );
}

GenSetRoms::~GenSetRoms()
{
	// Disconnect Events
	m_botonAgregarDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onAgregarDirectorio ), NULL, this );
	m_botonBorrarDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onBorrarDirectorio ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onEjecutar ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onCopiar ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GenSetRoms::onMover ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenSetRoms::onSalir ) );
	
}
