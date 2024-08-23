/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  finSAR_edit Plugin
 * Author:   David Register, Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _finSAR_editPI_H_
#define _finSAR_editPI_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include <wx/glcanvas.h>
#endif  // precompiled headers

#include <sqlite3.h>
#include "ocpn_plugin.h"
#include "finSAR_editOverlayFactory.h"
#include "finSAR_editUIDialog.h"
#include <wx/datetime.h>
#include "pidc.h"
#include <wx/tokenzr.h>
#include <wx/datetime.h>

#define DATABASE_NAME "finSAR.db"

class piDC;
class finSAR_editUIDialog;

#define ABOUT_AUTHOR_URL "http://mikerossiter.co.uk"

#include "config.h"  // made by the build process

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define finSAR_edit_TOOL_POSITION \
  -1  // Request default positioning of toolbar tool

class finSAR_edit_pi : public opencpn_plugin_118 {
public:
  finSAR_edit_pi(void *ppimgr);
  ~finSAR_edit_pi(void);

  //    The required PlugIn Methods
  int Init(void);
  bool DeInit(void);

  int GetAPIVersionMajor();
  int GetAPIVersionMinor();
  int GetPlugInVersionMajor();
  int GetPlugInVersionMinor();
  wxBitmap *GetPlugInBitmap();
  wxString GetCommonName();
  wxString GetShortDescription();
  wxString GetLongDescription();

  //    The override PlugIn Methods
  bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
  bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
  // void SetCursorLatLon(double lat, double lon);
  void SetPositionFix(PlugIn_Position_Fix &pfix);
  void SetDefaults(void);
  int GetToolbarToolCount(void);
  void OnToolbarToolCallback(int id);

  double GetCursorLon(void) { return m_cursor_lon; }
  double GetCursorLat(void) { return m_cursor_lat; }

  // Other public methods
  void SetfinSAR_editDialogX(int x) { m_finSAR_edit_dialog_x = x; };
  void SetfinSAR_editDialogY(int x) { m_finSAR_edit_dialog_y = x; }
  void SetfinSAR_editDialogSizeX(int x) { m_finSAR_edit_dialog_sx = x; }
  void SetfinSAR_editDialogSizeY(int x) { m_finSAR_edit_dialog_sy = x; }
  void SetColorScheme(PI_ColorScheme cs);
  double GetShipLon(void) { return m_ship_lon; }
  double GetShipLat(void) { return m_ship_lat; }

  void OnContextMenuItemCallback(int id);
  void SetCursorLatLon(double lat, double lon);

  // bool MouseEventHook(wxMouseEvent &event);
  void OnfinSAR_editDialogClose();

  wxString GetFolderSelected() { return m_CopyFolderSelected; }
  int GetIntervalSelected() { return m_CopyIntervalSelected; }

  finSAR_editOverlayFactory *GetfinSAR_editOverlayFactory() {
    return m_pfinSAR_editOverlayFactory;
  }

  double m_boat_lat, m_boat_lon;

  double m_tr_spd;
  double m_tr_dir;
  finSAR_editOverlayFactory *m_pfinSAR_editOverlayFactory;

  wxString StandardPath();
  wxString StandardPathRTZ();
  wxString StandardPathEXT();

  finSAR_editUIDialog *m_pfinSAR_editDialog;

  // ******** Database stuff ******************************************

  sqlite3 *m_database;
  int ret;
  char *err_msg;
  bool b_dbUsable;

  int Add_RTZ_db(wxString route_name);
  int Add_EXT_db(wxString extensions_file, wxString route_name,
                 wxString rtz_date_stamp);

  int GetActiveFileDBId() { return m_activeFileDB; }
  void SetActiveFileDBId(int id) { m_activeFileDB = id; }

  int m_activeFileDB;

  void DeleteRTZ_Id(int id);
  void DeleteRTZ_Name(wxString route_name);
  void DeleteEXT_Name(wxString route_name);
  wxString m_activefiledbname;
  int dbGetIntNotNullValue(wxString sql);
  void dbGetTable(wxString sql, char ***results, int &n_rows, int &n_columns);
  void dbFreeResults(char **results);
  int GetRoute_Id(wxString route_name);
  wxString GetRTZDateStamp(wxString route_name);
  void FillRouteNamesDropdown();
  wxArrayString GetRouteList();

private:
  double m_cursor_lat, m_cursor_lon;
  int m_position_menu_id;

  bool LoadConfig(void);
  bool SaveConfig(void);

  wxFileConfig *m_pconfig;
  wxWindow *m_parent_window;

  int m_display_width, m_display_height;
  int m_leftclick_tool_id;

  int m_finSAR_edit_dialog_x, m_finSAR_edit_dialog_y;
  int m_finSAR_edit_dialog_sx, m_finSAR_edit_dialog_sy;

  wxString m_CopyFolderSelected;
  int m_CopyIntervalSelected;

  int m_bTimeZone;

  int m_bStartOptions;
  wxString m_RequestConfig;
  wxString *pTC_Dir;

  bool m_bfinSAR_editShowIcon;

  int m_height;

  bool m_bShowfinSAR_edit;
  int m_table_menu_id;

  wxBitmap m_panelBitmap;
  double m_ship_lon, m_ship_lat;

  bool dbQuery(wxString sql);
};

#endif
