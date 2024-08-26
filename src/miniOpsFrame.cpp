/**************************************************************************
*
* Project:  OpenCPN
* Purpose:  RouteProperties Support
* Author:   David Register
*
***************************************************************************
*   Copyright (C) 2010 by David S. Register                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
**************************************************************************/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/datetime.h>
#include <wx/clipbrd.h>
#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/stattext.h>
#include <wx/clrpicker.h>
#include <wx/event.h>
#include "miniOpsFrame.h"
#include "finSAR_ops_pi.h"



miniOpsFrame::miniOpsFrame(wxFrame* parent, wxWindowID id, const wxString& title,
                     const wxPoint& position, const wxSize& size, long style)
    : wxFrame(parent, id, title, position, size, style) {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxFlexGridSizer* fgSizer3;
  fgSizer3 = new wxFlexGridSizer(1, 3, 0, 0);
  fgSizer3->SetFlexibleDirection(wxBOTH);
  fgSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_button14 = new wxButton(this, wxID_ANY, _("EBL On"), wxDefaultPosition,
                            wxDefaultSize, 0);
  fgSizer3->Add(m_button14, 0, wxALL, 5);

  m_button15 = new wxButton(this, wxID_ANY, _("EBL Off"), wxDefaultPosition,
                            wxDefaultSize, 0);
  fgSizer3->Add(m_button15, 0, wxALL, 5);

  m_button16 = new wxButton(this, wxID_ANY, _("Remote OFF"), wxDefaultPosition,
                            wxDefaultSize, 0);
  fgSizer3->Add(m_button16, 0, wxALL, 5);

  this->SetSizer(fgSizer3);
  this->Layout();
  fgSizer3->Fit(this);

  this->Centre(wxBOTH);

  // Connect Events
  m_button14->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler(miniOpsFrame::OnShowEBL), NULL, this);
  m_button15->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler(miniOpsFrame::OnEBLOff), NULL,
                      this);
  m_button16->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler(miniOpsFrame::OnRemoteOff), NULL, this);
}

miniOpsFrame::~miniOpsFrame() {
  // Disconnect Events
  m_button14->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                         wxCommandEventHandler(miniOpsFrame::OnShowEBL), NULL,
                         this);
  m_button15->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler(miniOpsFrame::OnEBLOff), NULL,
                      this);
  m_button16->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler(miniOpsFrame::OnRemoteOff), NULL, this);
}

void miniOpsFrame::OnShowEBL(wxCommandEvent& event) {
  //wxMessageBox("In Cancel");

  parentFrame->m_bBearingLine = true;

  parentFrame->m_timer1.Start(200);

}

void miniOpsFrame::OnEBLOff(wxCommandEvent& event) {
  //wxMessageBox("In Hide");

  parentFrame->m_bBearingLine = false;

  parentFrame->m_timer1.Stop();
 
}

void miniOpsFrame::OnRemoteOff(wxCommandEvent& event) {
  //wxMessageBox("In Hide");  

  
  parentFrame->OnRemoteOff();
  
  this->Hide();

}
