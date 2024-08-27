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

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_notebook1->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );
	m_notebook1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	m_panelRoutes = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelRoutes->SetBackgroundColour( wxColour( 128, 255, 255 ) );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl9 = new wxTextCtrl( m_panelRoutes, wxID_ANY, _("Routes"), wxDefaultPosition, wxDefaultSize, wxTE_CENTER );
	m_textCtrl9->SetBackgroundColour( wxColour( 255, 255, 0 ) );

	bSizer9->Add( m_textCtrl9, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText2 = new wxStaticText( m_panelRoutes, wxID_ANY, _("Ops Area    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer101->Add( m_staticText2, 0, wxALL, 5 );

	wxString m_choice2Choices[] = { _("Area 1"), _("Area 2"), _("Area 3") };
	int m_choice2NChoices = sizeof( m_choice2Choices ) / sizeof( wxString );
	m_choice2 = new wxChoice( m_panelRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2NChoices, m_choice2Choices, 0 );
	m_choice2->SetSelection( 0 );
	bSizer101->Add( m_choice2, 0, wxALL|wxEXPAND, 5 );


	bSizer9->Add( bSizer101, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer1011;
	bSizer1011 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText21 = new wxStaticText( m_panelRoutes, wxID_ANY, _("VesselType"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer1011->Add( m_staticText21, 0, wxALL, 5 );

	wxString m_choice21Choices[] = { _("Vessel 1"), _("Vessel 2"), _("Vessel 3") };
	int m_choice21NChoices = sizeof( m_choice21Choices ) / sizeof( wxString );
	m_choice21 = new wxChoice( m_panelRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice21NChoices, m_choice21Choices, 0 );
	m_choice21->SetSelection( 0 );
	bSizer1011->Add( m_choice21, 0, wxALL|wxEXPAND, 5 );


	bSizer9->Add( bSizer1011, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panelRoutes, wxID_ANY, _("Route Selection") ), wxVERTICAL );


	bSizer9->Add( sbSizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxArrayString m_choiceRoutesChoices;
	m_choiceRoutes = new wxChoice( m_panelRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRoutesChoices, 0 );
	m_choiceRoutes->SetSelection( 0 );
	bSizer10->Add( m_choiceRoutes, 0, wxALL|wxEXPAND, 5 );

	m_bLoadRoute = new wxButton( m_panelRoutes, wxID_ANY, _("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bLoadRoute->SetBackgroundColour( wxColour( 0, 255, 64 ) );

	bSizer10->Add( m_bLoadRoute, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panelRoutes, wxID_ANY, _("Route in use") ), wxVERTICAL );

	m_textCtrlRouteInUse = new wxTextCtrl( sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_textCtrlRouteInUse, 0, wxALL|wxEXPAND, 5 );


	bSizer10->Add( sbSizer3, 1, wxEXPAND, 5 );


	bSizer9->Add( bSizer10, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_bButtonBearing1 = new wxButton( m_panelRoutes, wxID_ANY, _("EBL"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_bButtonBearing1, 0, wxALL, 5 );

	m_button8 = new wxButton( m_panelRoutes, wxID_ANY, _("EBL Off"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button8, 0, wxALL, 5 );


	bSizer9->Add( bSizer11, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6 = new wxStaticText( m_panelRoutes, wxID_ANY, _("EBL Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALL, 5 );

	m_Lat1 = new wxTextCtrl( m_panelRoutes, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_Lat1, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( m_panelRoutes, wxID_ANY, _("EBL Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALL, 5 );

	m_Lon1 = new wxTextCtrl( m_panelRoutes, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_Lon1, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( m_panelRoutes, wxID_ANY, _("Bearing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer3->Add( m_staticText8, 0, wxALL, 5 );

	m_EBLbearing = new wxTextCtrl( m_panelRoutes, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_EBLbearing, 0, wxALL, 5 );


	bSizer9->Add( fgSizer3, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxHORIZONTAL );

	m_bButtonIconize = new wxButton( m_panelRoutes, wxID_ANY, _("Remote ON"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer111->Add( m_bButtonIconize, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );


	bSizer9->Add( bSizer111, 0, wxEXPAND, 5 );


	m_panelRoutes->SetSizer( bSizer9 );
	m_panelRoutes->Layout();
	bSizer9->Fit( m_panelRoutes );
	m_notebook1->AddPage( m_panelRoutes, _("         Routes       "), false );

	bSizer3->Add( m_notebook1, 0, wxEXPAND | wxALL, 5 );


	fgSizer1->Add( bSizer3, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer1 );
	this->Layout();
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
	wxMenuItem* m_mLoadRoute;
	m_mLoadRoute = new wxMenuItem( m_mRoutes, wxID_ANY, wxString( _("Load Route") ) , wxEmptyString, wxITEM_NORMAL );
	m_mRoutes->Append( m_mLoadRoute );

	m_menubar3->Append( m_mRoutes, _("Routes") );

	this->SetMenuBar( m_menubar3 );

	m_timer1.SetOwner( this, m_timer1.GetId() );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( finSAR_opsUIDialogBase::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( finSAR_opsUIDialogBase::key_shortcut ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( finSAR_opsUIDialogBase::OnSize ) );
	m_bLoadRoute->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnLoadRoute ), NULL, this );
	m_bButtonBearing1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnButtonEBL ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnButtonEBL_off ), NULL, this );
	m_bButtonIconize->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnButtonRemoteOn ), NULL, this );
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnInformation ), this, m_mInformation->GetId());
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnAbout ), this, m_mAbout->GetId());
	m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnLoadRoute ), this, m_mLoadRoute->GetId());
	this->Connect( m_timer1.GetId(), wxEVT_TIMER, wxTimerEventHandler( finSAR_opsUIDialogBase::OnTimer ) );
}

finSAR_opsUIDialogBase::~finSAR_opsUIDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( finSAR_opsUIDialogBase::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( finSAR_opsUIDialogBase::key_shortcut ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( finSAR_opsUIDialogBase::OnSize ) );
	m_bLoadRoute->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnLoadRoute ), NULL, this );
	m_bButtonBearing1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnButtonEBL ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnButtonEBL_off ), NULL, this );
	m_bButtonIconize->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( finSAR_opsUIDialogBase::OnButtonRemoteOn ), NULL, this );
	this->Disconnect( m_timer1.GetId(), wxEVT_TIMER, wxTimerEventHandler( finSAR_opsUIDialogBase::OnTimer ) );

}

finSAR_opsPreferences::finSAR_opsPreferences( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel2 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText12 = new wxStaticText( m_panel2, wxID_ANY, _("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer21->Add( m_staticText12, 0, wxALL, 5 );

	HDTPredictorMiles = new wxTextCtrl( m_panel2, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( HDTPredictorMiles, 0, wxALL, 5 );

	wxString m_choice5Choices[] = { _("Nautical Miles"), _("Meters") };
	int m_choice5NChoices = sizeof( m_choice5Choices ) / sizeof( wxString );
	m_choice5 = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice5NChoices, m_choice5Choices, 0 );
	m_choice5->SetSelection( 0 );
	bSizer21->Add( m_choice5, 0, wxALL, 5 );


	bSizer14->Add( bSizer21, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText17 = new wxStaticText( m_panel2, wxID_ANY, _("Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer22->Add( m_staticText17, 0, wxALL, 5 );

	HDTPredictorWidth = new wxTextCtrl( m_panel2, wxID_ANY, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( HDTPredictorWidth, 0, wxALL, 5 );

	m_staticText18 = new wxStaticText( m_panel2, wxID_ANY, _("pixels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer22->Add( m_staticText18, 0, wxALL, 5 );


	bSizer14->Add( bSizer22, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText19 = new wxStaticText( m_panel2, wxID_ANY, _("Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer23->Add( m_staticText19, 0, wxALL, 5 );

	myColourPicker = new wxColourPickerCtrl( m_panel2, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer23->Add( myColourPicker, 0, wxALL, 5 );


	bSizer14->Add( bSizer23, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText32 = new wxStaticText( m_panel2, wxID_ANY, _("Style       "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	bSizer30->Add( m_staticText32, 0, wxALL, 5 );

	wxString m_choice10Choices[] = { _("Solid"), _("Dashed") };
	int m_choice10NChoices = sizeof( m_choice10Choices ) / sizeof( wxString );
	m_choice10 = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice10NChoices, m_choice10Choices, 0 );
	m_choice10->SetSelection( 0 );
	bSizer30->Add( m_choice10, 0, wxALL, 5 );


	bSizer14->Add( bSizer30, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer14 );
	m_panel2->Layout();
	bSizer14->Fit( m_panel2 );
	m_notebook3->AddPage( m_panel2, _("HeadingLine"), true );
	m_panel3 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );


	m_panel3->SetSizer( bSizer19 );
	m_panel3->Layout();
	bSizer19->Fit( m_panel3 );
	m_notebook3->AddPage( m_panel3, _("COG Prediction"), false );

	bSizer13->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();

	bSizer13->Add( m_sdbSizerButtons, 0, wxEXPAND, 5 );


	fgSizer5->Add( bSizer13, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer5 );
	this->Layout();
	fgSizer5->Fit( this );

	this->Centre( wxBOTH );
}

finSAR_opsPreferences::~finSAR_opsPreferences()
{
}
