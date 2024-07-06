///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "finSAR_opsUIDialogBase.h"

///////////////////////////////////////////////////////////////////////////

finSAR_opsUIDialogBase::finSAR_opsUIDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Route Selection") ), wxVERTICAL );

	m_staticText2 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, _("Ops Area"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	sbSizer2->Add( m_staticText2, 0, wxALL, 5 );

	wxString m_choice2Choices[] = { _("Area 1"), _("Area 2"), _("Area 3") };
	int m_choice2NChoices = sizeof( m_choice2Choices ) / sizeof( wxString );
	m_choice2 = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2NChoices, m_choice2Choices, 0 );
	m_choice2->SetSelection( 0 );
	sbSizer2->Add( m_choice2, 0, wxALL, 5 );


	bSizer9->Add( sbSizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	m_buttonAddTestItems = new wxButton( this, wxID_ANY, _("Add Routes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAddTestItems->SetFont( wxFont( 9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );
	m_buttonAddTestItems->SetForegroundColour( wxColour( 255, 255, 255 ) );
	m_buttonAddTestItems->SetBackgroundColour( wxColour( 255, 0, 0 ) );

	bSizer10->Add( m_buttonAddTestItems, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_listBox1 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_NEEDED_SB );
	bSizer10->Add( m_listBox1, 0, wxALL|wxEXPAND, 5 );

	m_bLoadRTZ = new wxButton( this, wxID_ANY, _("Load RTZ"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_bLoadRTZ, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Route in use") ), wxVERTICAL );

	m_textCtrl5 = new wxTextCtrl( sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_textCtrl5, 0, wxALL|wxEXPAND, 5 );

	m_staticText3 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, _("Extensions File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	sbSizer3->Add( m_staticText3, 0, wxALL, 5 );

	m_textCtrl6 = new wxTextCtrl( sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_textCtrl6, 0, wxALL|wxEXPAND, 5 );


	bSizer10->Add( sbSizer3, 1, wxEXPAND, 5 );


	bSizer9->Add( bSizer10, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer9 );
	this->Layout();
	m_timer1.SetOwner( this, m_timer1.GetId() );
	m_menubar3 = new wxMenuBar( 0 );
	m_mHelp = new wxMenu();
	wxMenuItem* m_mInformation;
	m_mInformation = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("Guide") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mInformation );

	wxMenuItem* m_mAbout;
	m_mAbout = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mAbout );

	m_menubar3->Append( m_mHelp, _("Help") );

	m_mRoutes = new wxMenu();
	wxMenuItem* m_mLoadRTZ;
	m_mLoadRTZ = new wxMenuItem( m_mRoutes, wxID_ANY, wxString( _("Load RTZ") ) , wxEmptyString, wxITEM_NORMAL );
	m_mRoutes->Append( m_mLoadRTZ );

	wxMenuItem* m_mAbout1;
	m_mAbout1 = new wxMenuItem( m_mRoutes, wxID_ANY, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_mRoutes->Append( m_mAbout1 );

	m_menubar3->Append( m_mRoutes, _("Routes") );

	this->SetMenuBar( m_menubar3 );


	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( finSAR_opsUIDialogBase::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( finSAR_opsUIDialogBase::key_shortcut ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( finSAR_opsUIDialogBase::OnSize ) );
	m_buttonAddTestItems->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::AddTestItems ), NULL, this );
	m_bLoadRTZ->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnLoadRTZ ), NULL, this );
	this->Connect( m_timer1.GetId(), wxEVT_TIMER, wxTimerEventHandler( finSAR_opsUIDialogBase::OnTimer ) );
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnInformation ), this, m_mInformation->GetId());
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnAbout ), this, m_mAbout->GetId());
	m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnInformation ), this, m_mLoadRTZ->GetId());
	m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnAbout ), this, m_mAbout1->GetId());
}

finSAR_opsUIDialogBase::~finSAR_opsUIDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( finSAR_opsUIDialogBase::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( finSAR_opsUIDialogBase::key_shortcut ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( finSAR_opsUIDialogBase::OnSize ) );
	m_buttonAddTestItems->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::AddTestItems ), NULL, this );
	m_bLoadRTZ->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnLoadRTZ ), NULL, this );
	this->Disconnect( m_timer1.GetId(), wxEVT_TIMER, wxTimerEventHandler( finSAR_opsUIDialogBase::OnTimer ) );

}
