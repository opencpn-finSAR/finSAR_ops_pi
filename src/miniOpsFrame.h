/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  RouteProerties Support
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

#ifndef _MINI_H_
#define _MINI_H_

/*!
 * Includes
 */
#include "wx/listctrl.h"   // for ColorScheme
#include "wx/hyperlink.h"  // toh, 2009.02.08
#include <wx/choice.h>
#include <wx/tglbtn.h>
#include <wx/bmpcbox.h>
#include <wx/notebook.h>
#include <wx/filesys.h>
#include <wx/clrpicker.h>


#if wxCHECK_VERSION(2, 9, 0)
#include <wx/dialog.h>
#else
#include "scrollingdialog.h"
#endif

/*!
 * Forward declarations
 */

class finSAR_opsUIDialog;

class miniOpsFrame : public wxFrame {
public:
  miniOpsFrame(wxFrame* parent, wxWindowID id,
            const wxString& title = "Routes available",
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE);

  ~miniOpsFrame();

  wxStdDialogButtonSizer* m_sdbSizer1;
  wxButton* m_sdbSizer1OK;
  wxButton* m_sdbSizer1Cancel;
  wxListView* dialogText;

  wxDialog* ddd;

  finSAR_opsUIDialog* parentFrame;

  void OnShowEBL(wxCommandEvent& event);
  void OnEBLOff(wxCommandEvent& event);
  void OnRemoteOff(wxCommandEvent& event);

protected:
  wxButton* m_button14;
  wxButton* m_button15;
  wxButton* m_button16;

private:
};

#endif