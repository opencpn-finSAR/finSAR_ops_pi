/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  finSAR_ops Plugin Friends
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#ifndef __finSAR_opsUIDIALOG_H__
#define __finSAR_opsUIDIALOG_H__

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/fileconf.h>
#include <wx/glcanvas.h>

#include "finSAR_opsUIDialogBase.h"
#include "routeprop.h"
#include "NavFunc.h"

#include <wx/progdlg.h>
#include <list>
#include <vector>
#include "wx/dateevt.h"
#include "wx/stattext.h"
#include "ocpn_plugin.h"
#include "wx/dialog.h"
#include <wx/calctrl.h>
#include "wx/window.h"
#include <wx/colordlg.h>
#include <wx/event.h>
#include <wx/scrolwin.h>
#include <wx/datetime.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/listctrl.h>
#include "tableroutes.h"
#include <wx/treebase.h>
#include <wx/treectrl.h>
#include <wx/uiaction.h>
#include <wx/app.h>
#include <cmath>
#include <wx/menu.h>
#include <wx/string.h>

#if wxUSE_UIACTIONSIMULATOR
#include "wx/uiaction.h"
#endif

/* XPM */
static const char* eye[] = {"20 20 7 1",
                            ". c none",
                            "# c #000000",
                            "a c #333333",
                            "b c #666666",
                            "c c #999999",
                            "d c #cccccc",
                            "e c #ffffff",
                            "....................",
                            "....................",
                            "....................",
                            "....................",
                            ".......######.......",
                            ".....#aabccb#a#.....",
                            "....#deeeddeebcb#...",
                            "..#aeeeec##aceaec#..",
                            ".#bedaeee####dbcec#.",
                            "#aeedbdabc###bcceea#",
                            ".#bedad######abcec#.",
                            "..#be#d######dadb#..",
                            "...#abac####abba#...",
                            ".....##acbaca##.....",
                            ".......######.......",
                            "....................",
                            "....................",
                            "....................",
                            "....................",
                            "...................."};

using namespace std;

#ifndef PI
#define PI 3.1415926535897931160E0 /* pi */
#endif

#if !defined(NAN)
static const long long lNaN = 0xfff8000000000000;
#define NAN (*(double*)&lNaN)
#endif

#define RT_RCDATA2 MAKEINTRESOURCE(999)

/* Maximum value that can be returned by the rand function. */
#ifndef RAND_MAX
#define RAND_MAX 0x7fff
#endif

#define distance(X, Y) \
  sqrt((X) * (X) +     \
       (Y) * (Y))  // much faster than hypot#define distance(X, Y) sqrt((X)*(X)
                   // + (Y)*(Y)) // much faster than hypot

class finSAR_opsOverlayFactory;
class PlugIn_ViewPort;
class PositionRecordSet;

class wxFileConfig;
class finSAR_ops_pi;
class wxGraphicsContext;
class routeprop;
class TableRoutes;
class ConfigurationDialog;
class NewPositionDialog;
class IndexTarget;
class DirectionTarget;
class RangeTarget;

///////////////////////////////////////////////////////////////////////////////
/// Class ConfigurationDialog
///////////////////////////////////////////////////////////////////////////////
class ConfigurationDialog : public wxDialog {
private:
protected:
  wxButton* m_bDelete;
  wxButton* m_bSelect;
  wxButton* m_bGenerate;
  wxButton* m_bClose;

  // Virtual event handlers, overide them in your derived class
  void OnDelete(wxCommandEvent& event);
  void OnInformation(wxCommandEvent& event);
  void OnGenerate(wxCommandEvent& event);
  void OnClose(wxCommandEvent& event);

public:
  finSAR_ops_pi* pPlugIn;
  wxListBox* m_lRoutes;

  ConfigurationDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
                      const wxString& title = _("Tidal Routes"),
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE);
  ~ConfigurationDialog();
};

class IndexTarget {
public:
  wxString route_name;
  wxString wpId;
  double beginLat, beginLon;
  double endLat, endLon;
};

class DirectionTarget {
public:
  wxString dId;
  double m_lat, m_lon;
  double m_dir;
};

class RangeTarget {
public:
  wxString rId;
  double beginLat, beginLon;
  double endLat, endLon;
};

class rtept {
public:
  wxString Name, m_GUID;
  int index;
  wxString lat, lon;
  wxString visible;
  wxString sym;
  wxString planned_speed;
};

class rte {
public:
  wxString Name;
  vector<rtept> m_rteptList;
};

class routeCurrent {
public:
  int m_LegNumber;
  int m_TCRefNumber;
  double lat, lon;
};

class routeLeg {
public:
  wxString LegName;
  vector<rtept> m_rteptList;
};

class Position {
public:
  wxString lat, lon, wpt_num;
  wxString name;
  wxString guid;
  wxString time;
  wxString etd;
  wxString CTS;
  wxString SMG;
  wxString distTo;
  wxString brgTo;
  wxString set;
  wxString rate;
  wxString icon_name;
  bool show_name;
  wxString visible;
  bool is_visible;
  wxString wpSym;
  wxString wpName;
  wxString wpId;
  int routepoint;
  wxString route_name;
  wxString planned_speed;
  int leg_number;
};


struct RouteMapPosition {
  RouteMapPosition(wxString n, double lat0, double lon0)
      : Name(n), lat(lat0), lon(lon0) {}

public:
  wxString Name;
  double lat, lon;
};

class TidalRoute {
public:
  wxString Name, Type, Start, StartTime, End, EndTime, Time, Distance, m_GUID;
  list<Position> m_positionslist;
};

#define pi 3.14159265358979323846

class finSAR_opsUIDialog : public finSAR_opsUIDialogBase {
public:
  finSAR_opsUIDialog(wxWindow* parent, finSAR_ops_pi* ppi);
  ~finSAR_opsUIDialog();

  finSAR_ops_pi* pPlugIn;

  //void SetCursorLatLon(double lat, double lon);

  void SetViewPort(PlugIn_ViewPort* vp);
  PlugIn_ViewPort* vp;

  wxDateTime m_dtNow;
  double m_dInterval;

  bool onNext;
  bool onPrev;

  wxString m_FolderSelected;
  int m_IntervalSelected;

  time_t myCurrentTime;

  wxString MakeDateTimeLabel(wxDateTime myDateTime);
  void OnInformation(wxCommandEvent& event);

  void GetTable(wxString myRoute);
  void AddChartRoute(wxString myRoute);

  virtual void Lock() { routemutex.Lock(); }
  virtual void Unlock() { routemutex.Unlock(); }

  bool OpenXML(wxString filename, bool reportfailure);
  void SaveXML();

  //void OnContextMenu(double m_lat, double m_lon);

  double FindDistanceFromLeg(Position* A, Position* B,
                                                  Position* C);
  int SetActiveWaypoint(double t_lat, double t_lon);
  bool m_bDrawWptDisk;
  Position* FindPreviousWaypoint(wxString ActiveWpt);

  int DeleteChartedRoute();
  wxString FindWaypointGUID(wxString testName);

  PlugIn_Waypoint_Ex* active_wpt, prev_wpt;

  double c_lat, c_lon;

  double deg2rad(double deg);
  double rad2deg(double rad);
  wxString SelectRoute(bool isDR);
  void SelectRoutePoints(wxString routeName);

  int m_tcNum;
  double m_tcLat, m_tcLon;

  vector<RouteMapPosition> Positions;
  wxString m_default_configuration_path;
  wxString m_default_files_path;

  Position my_position;
  vector<Position> my_positions;
  vector<Position> my_points;

  double active_wp_lat, active_wp_lon;
  double prev_wp_lat, prev_wp_lon;

  wxString rte_start;
  wxString rte_end;

  //wxString thisRoute;
  void AddTestItems(wxCommandEvent& event);
  ConfigurationDialog m_ConfigurationDialog;

  PlugIn_ViewPort *m_vp, m_current_vp;
  PlugIn_ViewPort& GetCurrentViewPort() { return m_current_vp; }
  wxPoint myPixArray[5];
  int myPixHeight;
  double chartScale;
  double centreLat;
  double centreLon;
  void OnButtonEBL(wxCommandEvent& event);
  void OnButtonEBL_off(wxCommandEvent& event);
  void MakeBoxPoints();
  bool m_bBearingLine;
  bool m_bMoveUpDownLeftRight;
  double m_ShipLat1, m_ShipLon1;
  double m_ShipLat2, m_ShipLon2;
  double ebl_lat, ebl_lon;
  void MakeEBLEvent();
  void key_shortcut(wxKeyEvent& event);
  void OnCursor(void);
  void OnTimer(wxTimerEvent& event);
  void SetNMEAMessage(wxString sentence);
  IndexTarget* i_target;
  DirectionTarget* d_target;
  RangeTarget* r_target;

  vector<IndexTarget> i_vector;
  vector<DirectionTarget> d_vector;
  vector<RangeTarget> r_vector;

  void OnSaveObjects(wxCommandEvent& event);

protected:
private:
  double myDist;

  double initLat;
  double initLon;
  double nextLat;
  double nextLon;

  int nextRoutePointIndex;
  double nextRoutePoint;
  double followDir;
  int countRoutePoints;
  wxMutex routemutex;

  void OnClose(wxCloseEvent& event);
  void OnMove(wxMoveEvent& event);
  void OnSize(wxSizeEvent& event);

  vector<rte> my_routes;
  vector<rtept> routePoints;
  vector<routeLeg> routeLegs;


  void OnReadRTZ(wxCommandEvent& event);

  wxString mySelectedRoute;

  int GetRandomNumber(int range_min, int range_max);

  //    Data
  wxWindow* pParent;

  double m_cursor_lat, m_cursor_lon;
  wxString g_SData_Locn;

  bool error_found;
  bool dbg;

  wxString waypointName[2000];
  wxString waypointVisible[2000];

  int myZoom;
  int GetScale(double myChartScale);

  wxString rtz_version;
  void SetBearingWaypoint();
  wxArrayString my_areas[10], my_files[10][10];
  int ca, cf;
  wxString id_wpt;
  void ReadRTZ(wxString file_name);
  void ChartTheRoute(wxString myRoute);

  Position* active_waypoint;
  Position* prev_waypoint;
  void OnNewRoute(wxCommandEvent& event);
  void OnSaveRoute(wxCommandEvent& event);
  unique_ptr<PlugIn_Route_Ex> thisRoute;
  vector<PlugIn_Waypoint_Ex*> theWaypoints;
  Plugin_WaypointExList* myList;
  void WriteRTZ(wxString route_name);
  void OnLoadRoute(wxCommandEvent& event);
  //void OnDeleteRoute(wxCommandEvent& event);
  //void ChartTheRoute(wxString myRoute);
  void OnIndex(wxCommandEvent& event);
  void FindIndex(Position* A, Position* B);
  void OnRange(wxCommandEvent& event);
  void FindRange(Position* A, Position* B);
  void OnDirection(wxCommandEvent& event);
  void FindDirection(Position* A, Position* B);

 
};

class GetRouteDialog : public wxDialog {
public:
  GetRouteDialog(wxWindow* parent, wxWindowID id,
                 const wxString& title = "Routes available",
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_DIALOG_STYLE);

  ~GetRouteDialog();

  wxStdDialogButtonSizer* m_sdbSizer1;
  wxButton* m_sdbSizer1OK;
  wxButton* m_sdbSizer1Cancel;
  wxListView* dialogText;

  void OnCancel();

protected:
private:
};

class MyTreeItemData : public wxTreeItemData {
public:
  MyTreeItemData(const wxString& desc) : m_desc(desc) {}

  void ShowInfo(wxTreeCtrl* tree);
  wxString const& GetDesc() const { return m_desc; }

private:
  wxString m_desc;
};

class treeCtrlRoutes : public wxTreeCtrl {
public:
  enum {
    TreeCtrlIcon_File,
    TreeCtrlIcon_FileSelected,
    TreeCtrlIcon_Folder,
    TreeCtrlIcon_FolderSelected,
    TreeCtrlIcon_FolderOpened
  };

  treeCtrlRoutes() {
    m_alternateImages = false;
    m_alternateStates = false;
  }
  treeCtrlRoutes(wxWindow* parent, const wxWindowID id, const wxPoint& pos,
                 const wxSize& size, long style);
  virtual ~treeCtrlRoutes() {}

  void OnBeginDrag(wxTreeEvent& event);
  void OnBeginRDrag(wxTreeEvent& event);
  void OnEndDrag(wxTreeEvent& event);
  void OnBeginLabelEdit(wxTreeEvent& event);
  void OnEndLabelEdit(wxTreeEvent& event);
  void OnDeleteItem(wxTreeEvent& event);
  void OnMenu(wxContextMenuEvent& event);
  void OnItemMenu(wxTreeEvent& event);
  void OnGetInfo(wxTreeEvent& event);
  void OnSetInfo(wxTreeEvent& event);
  void OnItemExpanded(wxTreeEvent& event);
  void OnItemExpanding(wxTreeEvent& event);
  void OnItemCollapsed(wxTreeEvent& event);
  void OnItemCollapsing(wxTreeEvent& event);
  void OnSelChanged(wxTreeEvent& event);
  void OnSelChanging(wxTreeEvent& event);
  void OnTreeKeyDown(wxTreeEvent& event);
  void OnItemActivated(wxTreeEvent& event);
  void OnItemStateClick(wxTreeEvent& event);
  void OnItemRClick(wxTreeEvent& event);

  //void OnRMouseDown(wxMouseEvent& event);
  //void OnRMouseUp(wxMouseEvent& event);
  //void OnRMouseDClick(wxMouseEvent& event);

  wxTreeItemId GetLastTreeITem() const;
  void GetItemsRecursively(const wxTreeItemId& idParent,
                           wxTreeItemIdValue cookie = 0);

  // This function behaves differently depending on the value of size:
  //  - If it's -1, it turns off the use of images entirely.
  //  - If it's 0, it reuses the last used size.
  //  - If it's strictly positive, it creates icons in this size.
  void CreateImages(int size);

  void CreateButtonsImageList(int size = 11);
  void CreateStateImageList(bool del = false);

  void AddTestItemsToTree(size_t numChildren, size_t depth);

  void DoSortChildren(const wxTreeItemId& item, bool reverse = false) {
    m_reverseSort = reverse;
    wxTreeCtrl::SortChildren(item);
  }

  void DoToggleIcon(const wxTreeItemId& item);
  void DoToggleState(const wxTreeItemId& item);

  void ShowMenu(wxTreeItemId id, const wxPoint& pt);

  int ImageSize(void) const { return m_imageSize; }

  void SetAlternateImages(bool show) { m_alternateImages = show; }
  bool AlternateImages() const { return m_alternateImages; }

  void SetAlternateStates(bool show) { m_alternateStates = show; }
  bool AlternateStates() const { return m_alternateStates; }

  void ResetBrokenStateImages() {
    const size_t count = GetStateImageList()->GetImageCount();
    int state = count > 0 ? count - 1 : wxTREE_ITEMSTATE_NONE;
    DoResetBrokenStateImages(GetRootItem(), 0, state);
  }

protected:
  virtual int OnCompareItems(const wxTreeItemId& i1,
                             const wxTreeItemId& i2) wxOVERRIDE;

  // is this the test item which we use in several event handlers?
  bool IsTestItem(const wxTreeItemId& item) {
    // the test item is the first child folder
    return GetItemParent(item) == GetRootItem() && !GetPrevSibling(item);
  }

private:
  // Find the very last item in the tree.
  void AddItemsRecursively(const wxTreeItemId& idParent, size_t nChildren,
                           size_t depth, size_t folder);

  void DoResetBrokenStateImages(const wxTreeItemId& idParent,
                                wxTreeItemIdValue cookie, int state);

  void LogEvent(const wxString& name, const wxTreeEvent& event);

  int m_imageSize;             // current size of images
  bool m_reverseSort;          // flag for OnCompareItems
  wxTreeItemId m_draggedItem;  // item being dragged right now
  bool m_alternateImages;
  bool m_alternateStates;

  // NB: due to an ugly wxMSW hack you _must_ use wxDECLARE_DYNAMIC_CLASS();
  //     if you want your overloaded OnCompareItems() to be called.
  //     OTOH, if you don't want it you may omit the next line - this will
  //     make default (alphabetical) sorting much faster under wxMSW.
  wxDECLARE_DYNAMIC_CLASS(MyTreeCtrl);
  wxDECLARE_EVENT_TABLE();
};

#endif
