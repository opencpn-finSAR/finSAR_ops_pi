///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/clrpicker.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class finSAR_opsUIDialogBase
///////////////////////////////////////////////////////////////////////////////
class finSAR_opsUIDialogBase : public wxFrame
{
	private:

	protected:
		wxNotebook* m_notebook1;
		wxPanel* m_panelRoutes;
		wxTextCtrl* m_textCtrl9;
		wxStaticText* m_staticText2;
		wxChoice* m_choice2;
		wxStaticText* m_staticText21;
		wxChoice* m_choice21;
		wxButton* m_bLoadRoute;
		wxTextCtrl* m_textCtrlRouteInUse;
		wxButton* m_bButtonBearing1;
		wxButton* m_button8;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxButton* m_bButtonIconize;
		wxMenu* m_mHelp;
		wxMenu* m_mRoutes;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void key_shortcut( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnLoadRoute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonEBL( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonEBL_off( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonRemoteOn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimer( wxTimerEvent& event ) { event.Skip(); }


	public:
		wxChoice* m_choiceRoutes;
		wxTextCtrl* m_Lat1;
		wxTextCtrl* m_Lon1;
		wxTextCtrl* m_EBLbearing;
		wxMenuBar* m_menubar3;
		wxTimer m_timer1;

		finSAR_opsUIDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("finSAR_ops"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 377,839 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~finSAR_opsUIDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class finSAR_opsPreferences
///////////////////////////////////////////////////////////////////////////////
class finSAR_opsPreferences : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook3;
		wxPanel* m_panel2;
		wxStaticText* m_staticText12;
		wxChoice* m_choice5;
		wxStaticText* m_staticText17;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText32;
		wxChoice* m_choice10;
		wxPanel* m_panel3;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;
		wxButton* m_sdbSizerButtonsCancel;

	public:
		wxTextCtrl* HDTPredictorMiles;
		wxTextCtrl* HDTPredictorWidth;
		wxColourPickerCtrl* myColourPicker;

		finSAR_opsPreferences( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("finSAR Operations Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxRESIZE_BORDER );

		~finSAR_opsPreferences();

};

