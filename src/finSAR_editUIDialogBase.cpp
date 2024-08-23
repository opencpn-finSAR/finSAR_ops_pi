///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "finSAR_editUIDialogBase.h"

///////////////////////////////////////////////////////////////////////////

finSAR_editUIDialogBase::finSAR_editUIDialogBase(wxWindow* parent,
                                                 wxWindowID id,
                                                 const wxString& title,
                                                 const wxPoint& pos,
                                                 const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxFlexGridSizer* fgSizer1;
  fgSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer1->SetFlexibleDirection(wxBOTH);
  fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxBoxSizer* bSizer3;
  bSizer3 = new wxBoxSizer(wxVERTICAL);

  m_notebook1 =
      new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
  m_notebook1->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_NORMAL, false, wxT("Arial")));
  m_notebook1->SetBackgroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

  m_panelRoutes = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
                              wxDefaultSize, wxTAB_TRAVERSAL);
  m_panelRoutes->SetBackgroundColour(wxColour(128, 255, 255));

  wxBoxSizer* bSizer9;
  bSizer9 = new wxBoxSizer(wxVERTICAL);

  m_textCtrl9 = new wxTextCtrl(m_panelRoutes, wxID_ANY, _("Routes"),
                               wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
  m_textCtrl9->SetBackgroundColour(wxColour(255, 255, 0));

  bSizer9->Add(m_textCtrl9, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer* bSizer101;
  bSizer101 = new wxBoxSizer(wxHORIZONTAL);

  m_staticText2 = new wxStaticText(m_panelRoutes, wxID_ANY, _("Ops Area    "),
                                   wxDefaultPosition, wxDefaultSize, 0);
  m_staticText2->Wrap(-1);
  bSizer101->Add(m_staticText2, 0, wxALL, 5);

  wxString m_choice2Choices[] = {_("Area 1"), _("Area 2"), _("Area 3")};
  int m_choice2NChoices = sizeof(m_choice2Choices) / sizeof(wxString);
  m_choice2 =
      new wxChoice(m_panelRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                   m_choice2NChoices, m_choice2Choices, 0);
  m_choice2->SetSelection(0);
  bSizer101->Add(m_choice2, 0, wxALL | wxEXPAND, 5);

  bSizer9->Add(bSizer101, 0, wxEXPAND, 5);

  wxBoxSizer* bSizer1011;
  bSizer1011 = new wxBoxSizer(wxHORIZONTAL);

  m_staticText21 = new wxStaticText(m_panelRoutes, wxID_ANY, _("VesselType"),
                                    wxDefaultPosition, wxDefaultSize, 0);
  m_staticText21->Wrap(-1);
  bSizer1011->Add(m_staticText21, 0, wxALL, 5);

  wxString m_choice21Choices[] = {_("Vessel 1"), _("Vessel 2"), _("Vessel 3")};
  int m_choice21NChoices = sizeof(m_choice21Choices) / sizeof(wxString);
  m_choice21 =
      new wxChoice(m_panelRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                   m_choice21NChoices, m_choice21Choices, 0);
  m_choice21->SetSelection(0);
  bSizer1011->Add(m_choice21, 0, wxALL | wxEXPAND, 5);

  bSizer9->Add(bSizer1011, 0, wxEXPAND, 5);

  m_button11 = new wxButton(m_panelRoutes, wxID_ANY, _("New"),
                            wxDefaultPosition, wxDefaultSize, 0);
  m_button11->SetBackgroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION));

  bSizer9->Add(m_button11, 0, wxALL | wxEXPAND, 5);

  m_button112 = new wxButton(m_panelRoutes, wxID_ANY, _("Save"),
                             wxDefaultPosition, wxDefaultSize, 0);
  m_button112->SetBackgroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION));

  bSizer9->Add(m_button112, 0, wxALL | wxEXPAND, 5);

  m_button1122 = new wxButton(m_panelRoutes, wxID_ANY, _("Save As..."),
                              wxDefaultPosition, wxDefaultSize, 0);
  m_button1122->SetBackgroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION));

  bSizer9->Add(m_button1122, 0, wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND,
               5);

  m_button1121 = new wxButton(m_panelRoutes, wxID_ANY, _("Import"),
                              wxDefaultPosition, wxDefaultSize, 0);
  m_button1121->SetBackgroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION));

  bSizer9->Add(m_button1121, 0, wxALL | wxEXPAND, 5);

  m_button11211 = new wxButton(m_panelRoutes, wxID_ANY, _("Export"),
                               wxDefaultPosition, wxDefaultSize, 0);
  m_button11211->SetBackgroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION));

  bSizer9->Add(m_button11211, 0, wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND,
               5);

  wxStaticBoxSizer* sbSizer2;
  sbSizer2 = new wxStaticBoxSizer(
      new wxStaticBox(m_panelRoutes, wxID_ANY, _("Route Selection")),
      wxVERTICAL);

  bSizer9->Add(sbSizer2, 0, wxEXPAND, 5);

  wxBoxSizer* bSizer10;
  bSizer10 = new wxBoxSizer(wxVERTICAL);

  wxArrayString m_choiceRoutesChoices;
  m_choiceRoutes = new wxChoice(m_panelRoutes, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, m_choiceRoutesChoices, 0);
  m_choiceRoutes->SetSelection(0);
  bSizer10->Add(m_choiceRoutes, 0, wxALL | wxEXPAND, 5);

  m_bLoadRoute = new wxButton(m_panelRoutes, wxID_ANY, _("Load"),
                              wxDefaultPosition, wxDefaultSize, 0);
  m_bLoadRoute->SetBackgroundColour(wxColour(0, 255, 64));

  bSizer10->Add(m_bLoadRoute, 0, wxALL | wxEXPAND, 5);

  wxStaticBoxSizer* sbSizer3;
  sbSizer3 = new wxStaticBoxSizer(
      new wxStaticBox(m_panelRoutes, wxID_ANY, _("Route in use")), wxVERTICAL);

  m_textCtrlRouteInUse =
      new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString,
                     wxDefaultPosition, wxDefaultSize, 0);
  sbSizer3->Add(m_textCtrlRouteInUse, 0, wxALL | wxEXPAND, 5);

  bSizer10->Add(sbSizer3, 1, wxEXPAND, 5);

  bSizer9->Add(bSizer10, 1, wxEXPAND, 5);

  wxBoxSizer* bSizer11;
  bSizer11 = new wxBoxSizer(wxVERTICAL);

  m_bDeleteRoute = new wxButton(m_panelRoutes, wxID_ANY, _("Delete"),
                                wxDefaultPosition, wxDefaultSize, 0);
  m_bDeleteRoute->SetBackgroundColour(wxColour(255, 43, 43));

  bSizer11->Add(m_bDeleteRoute, 0, wxALL | wxEXPAND, 5);

  bSizer9->Add(bSizer11, 0, wxEXPAND, 5);

  m_panelRoutes->SetSizer(bSizer9);
  m_panelRoutes->Layout();
  bSizer9->Fit(m_panelRoutes);
  m_notebook1->AddPage(m_panelRoutes, _("         Routes       "), true);
  m_panelExtensions = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
                                  wxDefaultSize, wxTAB_TRAVERSAL);
  m_panelExtensions->SetBackgroundColour(wxColour(139, 197, 197));

  wxBoxSizer* bSizer71;
  bSizer71 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* bSizer25;
  bSizer25 = new wxBoxSizer(wxVERTICAL);

  m_textCtrl8 = new wxTextCtrl(m_panelExtensions, wxID_ANY, _("Extensions"),
                               wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
  m_textCtrl8->SetForegroundColour(wxColour(255, 255, 255));
  m_textCtrl8->SetBackgroundColour(wxColour(75, 126, 211));

  bSizer25->Add(m_textCtrl8, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer* bSizer8;
  bSizer8 = new wxBoxSizer(wxVERTICAL);

  m_staticText1 =
      new wxStaticText(m_panelExtensions, wxID_ANY, _("Extensions File"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText1->Wrap(-1);
  bSizer8->Add(m_staticText1, 0, wxALL, 5);

  m_textExtName = new wxTextCtrl(m_panelExtensions, wxID_ANY, wxEmptyString,
                                 wxDefaultPosition, wxDefaultSize, 0);
  bSizer8->Add(m_textExtName, 0, wxALL | wxEXPAND, 5);

  bSizer25->Add(bSizer8, 0, wxEXPAND, 5);

  wxBoxSizer* bSizer81;
  bSizer81 = new wxBoxSizer(wxVERTICAL);

  bSizer25->Add(bSizer81, 1, wxEXPAND, 5);

  bSizer71->Add(bSizer25, 0, wxEXPAND, 5);

  wxStaticBoxSizer* sbSizer4;
  sbSizer4 = new wxStaticBoxSizer(
      new wxStaticBox(m_panelExtensions, wxID_ANY, _("Objects Editor")),
      wxHORIZONTAL);

  wxBoxSizer* bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* bSizer5;
  bSizer5 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer* fgSizer2;
  fgSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
  fgSizer2->SetFlexibleDirection(wxBOTH);
  fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_staticText4 =
      new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, _("Index"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText4->Wrap(-1);
  fgSizer2->Add(m_staticText4, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_staticText41 =
      new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, _("Range"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText41->Wrap(-1);
  fgSizer2->Add(m_staticText41, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_staticText411 =
      new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, _("Direction"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText411->Wrap(-1);
  fgSizer2->Add(m_staticText411, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_buttonAttach = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, _("New"),
                                wxDefaultPosition, wxDefaultSize, 0);
  m_buttonAttach->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_NORMAL, false, wxT("Arial")));

  fgSizer2->Add(m_buttonAttach, 0, wxALL, 5);

  m_buttonAttach1 = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, _("New"),
                                 wxDefaultPosition, wxDefaultSize, 0);
  m_buttonAttach1->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_NORMAL, false, wxT("Arial")));

  fgSizer2->Add(m_buttonAttach1, 0, wxALL, 5);

  m_buttonAttach2 = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, _("New"),
                                 wxDefaultPosition, wxDefaultSize, 0);
  m_buttonAttach2->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_NORMAL, false, wxT("Arial")));

  fgSizer2->Add(m_buttonAttach2, 0, wxALL, 5);

  m_buttonAttach21 = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, _("Del"),
                                  wxDefaultPosition, wxDefaultSize, 0);
  m_buttonAttach21->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                   wxFONTWEIGHT_NORMAL, false, wxT("Arial")));

  fgSizer2->Add(m_buttonAttach21, 0, wxALL, 5);

  m_buttonAttach22 = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, _("Del"),
                                  wxDefaultPosition, wxDefaultSize, 0);
  m_buttonAttach22->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                   wxFONTWEIGHT_NORMAL, false, wxT("Arial")));

  fgSizer2->Add(m_buttonAttach22, 0, wxALL, 5);

  m_buttonAttach23 = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, _("Del"),
                                  wxDefaultPosition, wxDefaultSize, 0);
  m_buttonAttach23->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                   wxFONTWEIGHT_NORMAL, false, wxT("Arial")));

  fgSizer2->Add(m_buttonAttach23, 0, wxALL, 5);

  bSizer5->Add(fgSizer2, 0, wxEXPAND, 5);

  bSizer1->Add(bSizer5, 0, wxEXPAND, 5);

  sbSizer4->Add(bSizer1, 0, wxEXPAND, 5);

  bSizer71->Add(sbSizer4, 0, wxEXPAND, 5);

  m_panelExtensions->SetSizer(bSizer71);
  m_panelExtensions->Layout();
  bSizer71->Fit(m_panelExtensions);
  m_notebook1->AddPage(m_panelExtensions, _("     Extensions     "), false);

  bSizer3->Add(m_notebook1, 0, wxEXPAND | wxALL, 5);

  fgSizer1->Add(bSizer3, 1, wxEXPAND, 5);

  this->SetSizer(fgSizer1);
  this->Layout();
  m_menubar3 = new wxMenuBar(0);
  m_mHelp = new wxMenu();
  wxMenuItem* m_mInformation;
  m_mInformation = new wxMenuItem(m_mHelp, wxID_ANY, wxString(_("Guide")),
                                  wxEmptyString, wxITEM_NORMAL);
  m_mHelp->Append(m_mInformation);

  wxMenuItem* m_mAbout;
  m_mAbout = new wxMenuItem(m_mHelp, wxID_ANY, wxString(_("About")),
                            wxEmptyString, wxITEM_NORMAL);
  m_mHelp->Append(m_mAbout);

  m_menubar3->Append(m_mHelp, _("Help"));

  m_mRoutes = new wxMenu();
  wxMenuItem* m_mNewRoute;
  m_mNewRoute = new wxMenuItem(m_mRoutes, wxID_ANY, wxString(_("New Route")),
                               wxEmptyString, wxITEM_NORMAL);
  m_mRoutes->Append(m_mNewRoute);

  wxMenuItem* m_mSaveRoute;
  m_mSaveRoute = new wxMenuItem(m_mRoutes, wxID_ANY, wxString(_("Save Route")),
                                wxEmptyString, wxITEM_NORMAL);
  m_mRoutes->Append(m_mSaveRoute);

  wxMenuItem* m_mLoadRoute;
  m_mLoadRoute = new wxMenuItem(m_mRoutes, wxID_ANY, wxString(_("Load Route")),
                                wxEmptyString, wxITEM_NORMAL);
  m_mRoutes->Append(m_mLoadRoute);

  wxMenuItem* m_mImportRoute;
  m_mImportRoute =
      new wxMenuItem(m_mRoutes, wxID_ANY, wxString(_("Load Route")),
                     wxEmptyString, wxITEM_NORMAL);
  m_mRoutes->Append(m_mImportRoute);

  wxMenuItem* m_mDeleteRoute;
  m_mDeleteRoute =
      new wxMenuItem(m_mRoutes, wxID_ANY, wxString(_("Load Route")),
                     wxEmptyString, wxITEM_NORMAL);
  m_mRoutes->Append(m_mDeleteRoute);

  m_menubar3->Append(m_mRoutes, _("Routes"));

  m_mExtensions = new wxMenu();
  wxMenuItem* m_mSaveExtensionsFile;
  m_mSaveExtensionsFile = new wxMenuItem(m_mExtensions, wxID_ANY,
                                         wxString(_("Save Extensions File")),
                                         wxEmptyString, wxITEM_NORMAL);
  m_mExtensions->Append(m_mSaveExtensionsFile);

  m_menubar3->Append(m_mExtensions, _("Extensions"));

  this->SetMenuBar(m_menubar3);

  this->Centre(wxBOTH);

  // Connect Events
  this->Connect(wxEVT_CLOSE_WINDOW,
                wxCloseEventHandler(finSAR_editUIDialogBase::OnClose));
  this->Connect(wxEVT_KEY_DOWN,
                wxKeyEventHandler(finSAR_editUIDialogBase::key_shortcut));
  this->Connect(wxEVT_SIZE,
                wxSizeEventHandler(finSAR_editUIDialogBase::OnSize));
  m_button11->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnNewRoute), NULL, this);
  m_button112->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnSaveRoute), NULL, this);
  m_button1122->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnSaveRouteAs), NULL,
      this);
  m_button1121->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnImportRoute), NULL,
      this);
  m_button11211->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnExportRoute), NULL,
      this);
  m_bLoadRoute->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnLoadRoute), NULL, this);
  m_bDeleteRoute->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnDeleteRoute), NULL,
      this);
  m_panelExtensions->Connect(
      wxEVT_KEY_DOWN, wxKeyEventHandler(finSAR_editUIDialogBase::key_shortcut),
      NULL, this);
  m_buttonAttach->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnIndex), NULL, this);
  m_buttonAttach1->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnRange), NULL, this);
  m_buttonAttach2->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnDirection), NULL, this);
  m_buttonAttach21->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnIndexDelete), NULL,
      this);
  m_buttonAttach22->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnRangeDelete), NULL,
      this);
  m_buttonAttach23->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnDirectionDelete), NULL,
      this);
  m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(finSAR_editUIDialogBase::OnInformation),
                this, m_mInformation->GetId());
  m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(finSAR_editUIDialogBase::OnAbout), this,
                m_mAbout->GetId());
  m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(finSAR_editUIDialogBase::OnNewRoute),
                  this, m_mNewRoute->GetId());
  m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(finSAR_editUIDialogBase::OnSaveRoute),
                  this, m_mSaveRoute->GetId());
  m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(finSAR_editUIDialogBase::OnLoadRoute),
                  this, m_mLoadRoute->GetId());
  m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(finSAR_editUIDialogBase::OnImportRoute),
                  this, m_mImportRoute->GetId());
  m_mRoutes->Bind(wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(finSAR_editUIDialogBase::OnDeleteRoute),
                  this, m_mDeleteRoute->GetId());
  m_mExtensions->Bind(
      wxEVT_COMMAND_MENU_SELECTED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnSaveExtensions), this,
      m_mSaveExtensionsFile->GetId());
}

finSAR_editUIDialogBase::~finSAR_editUIDialogBase() {
  // Disconnect Events
  this->Disconnect(wxEVT_CLOSE_WINDOW,
                   wxCloseEventHandler(finSAR_editUIDialogBase::OnClose));
  this->Disconnect(wxEVT_KEY_DOWN,
                   wxKeyEventHandler(finSAR_editUIDialogBase::key_shortcut));
  this->Disconnect(wxEVT_SIZE,
                   wxSizeEventHandler(finSAR_editUIDialogBase::OnSize));
  m_button11->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnNewRoute), NULL, this);
  m_button112->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnSaveRoute), NULL, this);
  m_button1122->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnSaveRouteAs), NULL,
      this);
  m_button1121->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnImportRoute), NULL,
      this);
  m_button11211->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnExportRoute), NULL,
      this);
  m_bLoadRoute->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnLoadRoute), NULL, this);
  m_bDeleteRoute->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnDeleteRoute), NULL,
      this);
  m_panelExtensions->Disconnect(
      wxEVT_KEY_DOWN, wxKeyEventHandler(finSAR_editUIDialogBase::key_shortcut),
      NULL, this);
  m_buttonAttach->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnIndex), NULL, this);
  m_buttonAttach1->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnRange), NULL, this);
  m_buttonAttach2->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnDirection), NULL, this);
  m_buttonAttach21->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnIndexDelete), NULL,
      this);
  m_buttonAttach22->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnRangeDelete), NULL,
      this);
  m_buttonAttach23->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(finSAR_editUIDialogBase::OnDirectionDelete), NULL,
      this);
}
