/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  finSAR_edit Object
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Mike Rossiter  *
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
 *
 */
#include <wx/intl.h>
#include "wx/wx.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/debug.h>
#include <wx/graphics.h>
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "finSAR_edit_pi.h"
#include "icons.h"
#include <wx/arrimpl.cpp>

#ifdef __WXMSW__
#include <windows.h>
#endif
#include <memory.h>

#include <wx/colordlg.h>
#include <wx/event.h>
#include "pugixml.hpp"

class Position;

using namespace std;
using namespace pugi;

#define FAIL(X)  \
  do {           \
    error = X;   \
    goto failed; \
  } while (0)

static double deg2rad(double degrees) { return M_PI * degrees / 180.0; }
static double rad2deg(double radians) { return 180.0 * radians / M_PI; }

GetRouteDialog::GetRouteDialog(wxWindow* parent, wxWindowID id,
                               const wxString& title, const wxPoint& position,
                               const wxSize& size, long style)
    : wxDialog(parent, id, title, position, size, style) {
  wxString dimensions = wxT(""), s;
  wxPoint p;
  wxSize sz;

  sz.SetWidth(size.GetWidth() - 20);
  sz.SetHeight(size.GetHeight() - 70);

  p.x = 6;
  p.y = 2;

  wxBoxSizer* bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  dialogText =
      new wxListView(this, wxID_ANY, p, sz, wxLC_NO_HEADER | wxLC_REPORT,
                     wxDefaultValidator, wxT(""));
  wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

  dialogText->SetFont(font);

  bSizer1->Add(dialogText, 0, wxALL, 5);

  m_sdbSizer1 = new wxStdDialogButtonSizer();
  m_sdbSizer1OK = new wxButton(this, wxID_OK);
  m_sdbSizer1->AddButton(m_sdbSizer1OK);
  m_sdbSizer1Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer1->AddButton(m_sdbSizer1Cancel);
  m_sdbSizer1->Realize();

  bSizer1->Add(m_sdbSizer1, 1, wxALIGN_CENTER, 5);

  m_sdbSizer1OK->SetDefault();

  this->SetSizer(bSizer1);
  this->Layout();
  bSizer1->Fit(this);
}

GetRouteDialog::~GetRouteDialog() {}

finSAR_editUIDialog::finSAR_editUIDialog(wxWindow* parent, finSAR_edit_pi* ppi)
    : finSAR_editUIDialogBase(parent) {
  pParent = parent;
  pPlugIn = ppi;
  m_bBearingLine = false;

  wxFileConfig* pConf = GetOCPNConfigObject();

  if (pConf) {
    pConf->SetPath(_T ( "/PlugIns/finSAR_edit" ));

    pConf->Read(_T("finSAR_editFolder"), &m_FolderSelected);
  }

  // m_dirPicker1->GetTextCtrlValue();

  // wxMessageBox(m_FolderSelected);

  this->Connect(wxEVT_MOVE, wxMoveEventHandler(finSAR_editUIDialog::OnMove));

  m_dtNow = wxDateTime::Now();
  m_dtNow.MakeUTC(false);
  wxString initStartDate = m_dtNow.Format(_T("%Y-%m-%d %H:%M:%S"));

  Fit();
  SetMinSize(GetBestSize());

  for (int z = 0; z < 10; z++) {
    for (int fz = 0; fz < 10; fz++) {
      my_files[z][fz].Add("0");
    }
  }

  id_wpt = wxEmptyString;
}

finSAR_editUIDialog::~finSAR_editUIDialog() {
  wxFileConfig* pConf = GetOCPNConfigObject();
  ;

  if (pConf) {
    pConf->SetPath(_T ( "/PlugIns/finSAR_edit" ));
  }

  i_vector.clear();
  r_vector.clear();
  d_vector.clear();
}

void finSAR_editUIDialog::SetViewPort(PlugIn_ViewPort* vp) {
  if (m_vp == vp) return;

  m_vp = new PlugIn_ViewPort(*vp);
  m_bMoveUpDownLeftRight = false;
  centreLat = m_vp->clat;
  centreLon = m_vp->clon;
}

void finSAR_editUIDialog::OnClose(wxCloseEvent& event) {
  pPlugIn->OnfinSAR_editDialogClose();

  i_vector.clear();
  r_vector.clear();
  d_vector.clear();

  DeleteChartedRoute();
}

void finSAR_editUIDialog::OnMove(wxMoveEvent& event) {
  //    Record the dialog position
  wxPoint p = GetPosition();
  pPlugIn->SetfinSAR_editDialogX(p.x);
  pPlugIn->SetfinSAR_editDialogY(p.y);

  event.Skip();
}

void finSAR_editUIDialog::OnSize(wxSizeEvent& event) {
  //    Record the dialog size
  wxSize p = event.GetSize();
  pPlugIn->SetfinSAR_editDialogSizeX(p.x);
  pPlugIn->SetfinSAR_editDialogSizeY(p.y);

  event.Skip();
}

wxString finSAR_editUIDialog::MakeDateTimeLabel(wxDateTime myDateTime) {
  wxDateTime dt = myDateTime;

  wxString s2 = dt.Format("%Y-%m-%d");
  wxString s = dt.Format("%H:%M:%S");
  wxString dateLabel = s2 + " " + s;

  return dateLabel;
}

void finSAR_editUIDialog::OnInformation(wxCommandEvent& event) {}

void finSAR_editUIDialog::AddChartRoute(wxString myRoute) {}

int finSAR_editUIDialog::GetRandomNumber(int range_min, int range_max) {
  long u = (long)wxRound(
      ((double)rand() / ((double)(RAND_MAX) + 1) * (range_max - range_min)) +
      range_min);
  return (int)u;
}

void finSAR_editUIDialog::SaveIndexRangeDirection(wxString route_name,
                                                  wxString date_stamp) {
  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  // Create XML root node called extensions
  xml_node pRoot = xmlDoc.append_child("extensions");

  int idn = 0;
  wxString route;

  xml_node m_rootNode = pRoot.append_child("route");
  m_rootNode.append_attribute("route_name").set_value(route_name.mb_str());
  m_rootNode.append_attribute("date_stamp").set_value(date_stamp.mb_str());

  xml_node m_targetNode = m_rootNode.append_child("targets");

  for (std::vector<IndexTarget>::iterator itOut = i_vector.begin();
       itOut != i_vector.end(); itOut++) {
    xml_node m_targetpoint = m_targetNode.append_child("index_target");

    // wxString myIdn = wxString::Format("%i", idn);
    m_targetpoint.append_attribute("wp_id").set_value((*itOut).wpId.mb_str());
    wxString wpid = (*itOut).wpId.mb_str();
    // wxMessageBox(wpid);

    xml_node b_position = m_targetpoint.append_child("begin");

    wxString bLat = wxString::Format("%f", (*itOut).beginLat);
    wxString bLon = wxString::Format("%f", (*itOut).beginLon);

    b_position.append_attribute("lat").set_value(bLat);
    b_position.append_attribute("lon").set_value(bLon);

    xml_node e_position = m_targetpoint.append_child("end");

    wxString eLat = wxString::Format("%f", (*itOut).endLat);
    wxString eLon = wxString::Format("%f", (*itOut).endLon);

    e_position.append_attribute("lat").set_value(eLat);
    e_position.append_attribute("lon").set_value(eLon);

    xml_node idistance = m_targetpoint.append_child("index_distance");
    wxString dist = wxString::Format("%f", (*itOut).distance);
    idistance.append_attribute("distance").set_value(dist);

    xml_node label = m_targetpoint.append_child("label");

    wxString llat = wxString::Format("%f", (*itOut).label_lat);
    wxString llon = wxString::Format("%f", (*itOut).label_lon);

    label.append_attribute("label_lat").set_value(llat);
    label.append_attribute("label_lon").set_value(llon);

    idn++;
  }

  for (std::vector<RangeTarget>::iterator itOutRange = r_vector.begin();
       itOutRange != r_vector.end(); itOutRange++) {
    xml_node r_targetpoint = m_targetNode.append_child("range_target");

    // wxString myIdn = wxString::Format("%i", idn);
    r_targetpoint.append_attribute("wp_id").set_value(
        (*itOutRange).wpId.mb_str());
    wxString wpid = (*itOutRange).wpId.mb_str();
    // wxMessageBox(wpid);

    xml_node b_position = r_targetpoint.append_child("begin");

    wxString bLat = wxString::Format("%f", (*itOutRange).beginLat);
    wxString bLon = wxString::Format("%f", (*itOutRange).beginLon);

    b_position.append_attribute("lat").set_value(bLat);
    b_position.append_attribute("lon").set_value(bLon);

    xml_node e_position = r_targetpoint.append_child("end");

    wxString eLat = wxString::Format("%f", (*itOutRange).endLat);
    wxString eLon = wxString::Format("%f", (*itOutRange).endLon);

    e_position.append_attribute("lat").set_value(eLat);
    e_position.append_attribute("lon").set_value(eLon);

    xml_node idistance = r_targetpoint.append_child("range_distance");
    wxString dist = wxString::Format("%f", (*itOutRange).distance);
    idistance.append_attribute("distance").set_value(dist);

    xml_node label = r_targetpoint.append_child("label");

    wxString ldist = wxString::Format("%f", (*itOutRange).label_lat);
    wxString ldir = wxString::Format("%f", (*itOutRange).label_lon);

    label.append_attribute("label_lat").set_value(ldist);
    label.append_attribute("label_lon").set_value(ldir);

    idn++;
  }

  for (std::vector<DirectionTarget>::iterator itOutDirection = d_vector.begin();
       itOutDirection != d_vector.end(); itOutDirection++) {
    xml_node d_directionpoint = m_targetNode.append_child("direction_target");

    wxString dLat = wxString::Format("%f", (*itOutDirection).m_lat);
    d_directionpoint.append_attribute("direction_lat").set_value(dLat);
    ;

    wxString dlon = wxString::Format("%f", (*itOutDirection).m_lon);
    d_directionpoint.append_attribute("direction_lon").set_value(dlon);

    wxString dir = wxString::Format("%f", (*itOutDirection).m_dir);
    d_directionpoint.append_attribute("direction").set_value(dir);

    idn++;
  }

  // done adding index, range and direction targets
  // Write xmlDoc into a file
  wxString file_name = route_name + ".xml";
  wxString file_path = pPlugIn->StandardPathEXT() + file_name;
  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void finSAR_editUIDialog::SelectRoutePoints(wxString routeName) {}

wxString finSAR_editUIDialog::SelectRoute(bool isDR) { return ""; }

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts decimal degrees to radians :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double finSAR_editUIDialog::deg2rad(double deg) { return (deg * pi / 180); }
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts radians to decimal degrees :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double finSAR_editUIDialog::rad2deg(double rad) { return (rad * 180 / pi); };

struct xml_string_writer : pugi::xml_writer {
  std::string result;

  virtual void write(const void* data, size_t size) {
    result.append(static_cast<const char*>(data), size);
  }
};

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

std::string InnerXML(pugi::xml_node target) {
  xml_string_writer writer;
  for (pugi::xml_node child = target.first_child(); child;
       child = child.next_sibling())
    child.print(writer, "");
  return writer.result;
}

void finSAR_editUIDialog::AddTestItems(wxCommandEvent& event) {
  wxString filename;
  wxFileDialog dlg(this, "Select file", wxEmptyString, wxEmptyString,
                   "RTZ files(*.rtz) | *.*;*.*", wxFD_OPEN);
  if (dlg.ShowModal() == wxID_OK) {
    if (dlg.GetPath() != wxEmptyString) {
      filename = dlg.GetPath();
      // wxMessageBox(filename);
    }

  } else
    wxMessageBox(_("No file entered"));

  ca = 0;
  cf = 0;

  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result = xmlDoc.load_file(filename.mb_str());

  // pugi::xml_node pRoot = xmlDoc.child("Areas");
  // if (pRoot == nullptr) return;

  // pugi::xml_node pAreasElement = xmlDoc.child("Areas");
  string area_name;
  pugi::xml_node nodes = xmlDoc.child("Areas");
  for (pugi::xml_node node = nodes.first_child(); node;
       node = node.next_sibling()) {
    if (!strcmp(node.name(), "Area")) {
      pugi::xml_node name = node.first_child();
      // pugi::xml_node name = node.child("Name");

      if (!strcmp(name.name(), "Name")) {
        string area_name = InnerXML(node.child("Name"));
        // wxMessageBox(area_name);
        my_areas[ca].Add(area_name);
        ca++;
      }
      pugi::xml_node routes = node.first_child().next_sibling();

      if (!strcmp(routes.name(), "Routes")) {
        // wxMessageBox("here");

        pugi::xml_node rnode = routes.first_child();
        if (!strcmp(rnode.name(), "file")) {
          cf = 0;
          for (pugi::xml_node child = routes.first_child(); child;
               child = child.next_sibling()) {
            string f = InnerXML(child);
            my_files[ca][cf].Add(f, 1);
            // wxMessageBox(my_files[ca][cf].Item(1));

            cf++;
          }
        }
        /*
          pugi::xml_node files;

          for (pugi::xml_node files = routes.first_child(); routes;
               files = routes.next_sibling()) {
            string file_name = InnerXML(routes.child("file");
            wxMessageBox(file_name);
          }*/
      }
    }
  }
  wxString testca = wxString::Format("%i", ca);
  wxString testcf = wxString::Format("%i", cf);
}

int finSAR_editUIDialog::GetScale(double myChartScale) {
  // If myChartScale is not exactly as shown in OpenCPN get the best scale to
  // use.

  if (myChartScale < 1200) {
    myChartScale = 800;
  }

  else if (myChartScale >= 1200 && myChartScale < 2400) {
    myChartScale = 1600;
  }

  else if (myChartScale >= 2400 && myChartScale < 4800) {
    myChartScale = 3200;
  }

  else if (myChartScale >= 4800 && myChartScale < 9600) {
    myChartScale = 6400;
  }

  else if (myChartScale >= 9600 && myChartScale < 19200) {
    myChartScale = 12800;
  }

  else if (myChartScale >= 19200 && myChartScale < 38400) {
    myChartScale = 25600;
  }

  else if (myChartScale >= 38400 && myChartScale < 76800) {
    myChartScale = 51200;
  }

  else if (myChartScale >= 76800 && myChartScale < 153550) {
    myChartScale = 102400;
  }

  else if (myChartScale >= 153550 && myChartScale < 306850) {
    myChartScale = 204700;
  }

  else if (myChartScale >= 306850 && myChartScale < 613700) {
    myChartScale = 409000;
  }

  else if (myChartScale >= 613700 && myChartScale < 1227400) {
    myChartScale = 818000;
  }

  else if (myChartScale >= 1227400 && myChartScale < 2454800) {
    myChartScale = 1636000;
  }

  else if (myChartScale >= 2454800 && myChartScale < 4909600) {
    myChartScale = 3272000;
  }

  else
    myChartScale = 9999000;

  // Use the modified scale to find the correct zoom level
  // Would switch case be quicker???

  if (myChartScale == 800) {
    return 19;
  }

  else if (myChartScale == 1600) {
    return 18;
  }

  else if (myChartScale == 3200) {
    return 17;
  }

  else if (myChartScale == 6400) {
    return 16;
  }

  else if (myChartScale == 12800) {
    return 15;
  }

  else if (myChartScale == 25600) {
    return 14;
  }

  else if (myChartScale == 51200) {
    return 13;
  }

  else if (myChartScale == 102400) {
    return 12;
  }

  else if (myChartScale == 204700) {
    return 11;
  } else if (myChartScale == 409000) {
    return 10;
  }

  else if (myChartScale == 818000) {
    return 9;
  }

  else if (myChartScale == 1636000) {
    return 8;
  }

  else if (myChartScale == 3272000) {
    return 7;
  }

  else
    return 6;
}

void finSAR_editUIDialog::key_shortcut(wxKeyEvent& event) {
  // wxMessageBox("here");
  //  of course, it doesn't have to be the control key. You can use others:
  //  http://docs.wxwidgets.org/stable/wx_wxkeyevent.html
  if (event.GetModifiers() == wxMOD_CONTROL) {
    switch (event.GetKeyCode()) {
      case 'S':  // can return the upper ASCII value of a key
        // do whatever you like for a Ctrl+G event here!
        // wxMessageBox(_("CTRL+G") );
        // OnCursor();
        break;  /*
      case WXK_LEFT: // we also have special keycodes for non-ascii values.
          // get a full list of special keycodes here:
          // http://docs.wxwidgets.org/stable/wx_keycodes.html
          wxMessageBox(_("CTRL+Left") );
          break;*/
      default:  // do nothing
        break;
    }
  }
  event.Skip();
}

void finSAR_editUIDialog::MakeBoxPoints() {
  myZoom = GetScale(m_vp->chart_scale);

  double boxlat = m_vp->clat;
  double boxlon = m_vp->clon;

  double mpp;
  mpp = 156543.03392 * cos(boxlat * PI / 180) / pow(2, myZoom);

  mpp *= 640;   // Reference box is 640x640
  mpp /= 1852;  // metres to NM

  double halfbox = mpp / 60 / 2;  // decimal degree

  double centreC = boxlat;
  double dlat1, dlat2;

  dlat1 = centreC + halfbox;
  dlat2 = centreC - halfbox;

  int pixheight;

  wxPoint p1;
  GetCanvasPixLL(m_vp, &p1, dlat1, boxlon);

  wxPoint p2;
  GetCanvasPixLL(m_vp, &p2, dlat2, boxlon);

  pixheight = (p2.y) - (p1.y);

  pixheight /= 2;

  myPixArray[0].x = 0;
  myPixArray[0].y = pixheight;
  myPixArray[0].y *= -1;

  myPixArray[1].x = 0;
  myPixArray[1].y = pixheight;

  myPixArray[2].x = 0;
  myPixArray[2].y = 0;

  myPixArray[3].x = pixheight;
  myPixArray[3].x *= -1;
  myPixArray[3].y = 0;

  myPixArray[4].x = pixheight;
  myPixArray[4].y = 0;

  myPixHeight = pixheight * 2;
}

void finSAR_editUIDialog::OnNewRoute(wxCommandEvent& event) {
  // This sleep is needed to give the time for the currently pressed modifier
  // keys, if any, to be released. Notice that Control modifier could well be
  // pressed if this command was activated from the menu using accelerator
  // and keeping it pressed would totally derail the test below, e.g. "A" key
  // press would actually become "Ctrl+A" selecting the entire text and so on.

  // Remove any indexes from previous route
  i_vector.clear();
  r_vector.clear();
  d_vector.clear();

  auto uids = GetRouteGUIDArray();
  for (size_t i = 0; i < uids.size(); i++) {
    DeletePlugInRoute(uids[i]);
  }

  RequestRefresh(pParent);

  wxMessageBox(
      "Right-Click \"End Route\" after drawing the route.\nRight-Click on the "
      "route\nPress CTRL+R and enter route name in route properties");

  pParent->SetFocus();
  wxUIActionSimulator sim;
  sim.KeyDown(82, wxMOD_CONTROL);

  // sim.KeyUp(82, wxMOD_CONTROL);
}

void finSAR_editUIDialog::OnSaveRoute(wxCommandEvent& event) {
  // This sleep is needed to give the time for the currently pressed modifier
  // keys, if any, to be released. Notice that Control modifier could well be
  // pressed if this command was activated from the menu using accelerator
  // and keeping it pressed would totally derail the test below, e.g. "A" key
  // press would actually become "Ctrl+A" selecting the entire text and so on.

  pPlugIn->m_pfinSAR_editDialog->SetFocus();
  wxUIActionSimulator sim;
  sim.KeyUp(82, wxMOD_CONTROL);
  //
  //
  std::vector<std::unique_ptr<PlugIn_Route_Ex>> routes;
  auto uids = GetRouteGUIDArray();
  for (size_t i = 0; i < uids.size(); i++) {
    routes.push_back(std::move(GetRouteEx_Plugin(uids[i])));
  }

  GetRouteDialog RouteDialog(this, -1, _("Select the route to save"),
                             wxPoint(200, 200), wxSize(300, 200),
                             wxCAPTION | wxRESIZE_BORDER);

  RouteDialog.dialogText->InsertColumn(0, "", 0, wxLIST_AUTOSIZE);
  RouteDialog.dialogText->SetColumnWidth(0, 290);
  RouteDialog.dialogText->InsertColumn(1, "", 0, wxLIST_AUTOSIZE);
  RouteDialog.dialogText->SetColumnWidth(1, 0);
  RouteDialog.dialogText->DeleteAllItems();

  int in = 0;
  std::vector<std::string> names;
  for (const auto& r : routes) names.push_back(r->m_NameString.ToStdString());

  for (size_t n = 0; n < names.size(); n++) {
    wxString routeName = names[in];
    if (routeName == "") {
      wxMessageBox(
          "Please add name of route in \"Route->Properties\"\nand Press \"Save "
          "Route\" again",
          "Missing Name");
      return;
    }
    RouteDialog.dialogText->InsertItem(in, "", -1);
    RouteDialog.dialogText->SetItem(in, 0, routeName);
    in++;
  }

  // ReadNavobj();
  long si = -1;
  long itemIndex = -1;
  // int f = 0;

  wxListItem row_info;
  wxString cell_contents_string = wxEmptyString;
  bool foundRoute = false;

  if (RouteDialog.ShowModal() != wxID_OK) {
    wxMessageBox("Cancelled");
  } else {
    for (;;) {
      itemIndex = RouteDialog.dialogText->GetNextItem(
          itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

      if (itemIndex == -1) break;

      // Got the selected item index
      if (RouteDialog.dialogText->IsSelected(itemIndex)) {
        si = itemIndex;
        foundRoute = true;
        break;
      }
    }

    if (foundRoute) {
      // Set what row it is (m_itemId is a member of the regular
      // wxListCtrl class)
      row_info.m_itemId = si;
      // Set what column of that row we want to query for information.
      row_info.m_col = 0;
      // Set text mask
      row_info.m_mask = wxLIST_MASK_TEXT;

      // Get the info and store it in row_info variable.
      RouteDialog.dialogText->GetItem(row_info);
      // Extract the text out that cell
      cell_contents_string = row_info.m_text;
      rtept initPoint;
      nextRoutePointIndex = 0;
      bool foundRoute = false;
      wxString myGuid;

      for (size_t i = 0; i < uids.size(); i++) {
        thisRoute = GetRouteEx_Plugin(uids[i]);
        if (thisRoute->m_NameString == cell_contents_string) {
          myGuid = uids[i];
          foundRoute = true;
          break;
        }
      }

      if (foundRoute) {
        countRoutePoints = thisRoute->pWaypointList->size();
        myList = thisRoute->pWaypointList;

        PlugIn_Waypoint_Ex* myWaypoint;
        theWaypoints.clear();
        // Trying to remove the route by deleting the waypoints
        // Plugin_WaypointExList* temp_list;

        wxPlugin_WaypointExListNode* pwpnode = myList->GetFirst();
        while (pwpnode) {
          myWaypoint = pwpnode->GetData();
          theWaypoints.push_back(myWaypoint);
          // temp_list->Append(myWaypoint);
          pwpnode = pwpnode->GetNext();
        }

        WriteRTZ(thisRoute->m_NameString);

        // wxString rtz_file = thisRoute->m_NameString;
        wxString extensions_file = thisRoute->m_NameString + ".xml";
        // wxMessageBox(rtz_file);

        int id = pPlugIn->GetRoute_Id(thisRoute->m_NameString);
        // wxString sid = wxString::Format("%i", id);
        // wxMessageBox(sid);
        if (id != 0) pPlugIn->DeleteRTZ_Id(id);
        // Now add the modified route
        pPlugIn->Add_RTZ_db(thisRoute->m_NameString);

        WriteEXT(thisRoute->m_NameString);
        m_dateStamp = pPlugIn->GetRTZDateStamp(thisRoute->m_NameString);
        pPlugIn->Add_EXT_db(extensions_file, thisRoute->m_NameString,
                            m_dateStamp);

        /*
        temp_list->DeleteContents(true);
        temp_list->Clear();

        thisRoute->pWaypointList->Clear();  // no-longer-managed object
        thisRoute->pWaypointList->delete()(thisRoute->pWaypointList);
        */
        DeletePlugInRoute(myGuid);
        pPlugIn->FillRouteNamesDropdown();
        //
      } else
        wxMessageBox("Route not found");
    }
  }
  RequestRefresh(pParent);
}

void finSAR_editUIDialog::OnImportRoute(wxCommandEvent& event) {
  rtz_version = "";
  Position my_position;
  my_positions.clear();

  wxString filename;
  wxFileDialog dlg(this, "Select file", wxEmptyString, wxEmptyString,
                   "RTZ files(*.rtz) | *.rtz;*.RTZ", wxFD_OPEN);
  if (dlg.ShowModal() == wxID_OK) {
    if (dlg.GetPath() != wxEmptyString) {
      filename = dlg.GetPath();
      // wxMessageBox(filename);
    }

  } else
    wxMessageBox(_("No file entered"));

  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result =
      xmlDoc.load_file(filename.mb_str(), parse_default | parse_declaration);

  string rtz_version = xmlDoc.child("route").attribute("version").value();

  if (rtz_version == "1.0") {
    wxMessageBox("RTZ Version must be 1.2\nImport stopped", "RTZ Version");
    return;
  } else if (rtz_version == "1.1") {
    wxMessageBox("RTZ Version must be 1.2\nImport Stopped", "RTZ Version");
    return;
  }

  pugi::xml_node pRoot = xmlDoc.child("route").child("routeInfo");
  if (pRoot == nullptr) return;

  wxString error;
  wxProgressDialog* progressdialog = NULL;

  my_position.wpSym = "diamond";

  xml_node pRouteNameElement = xmlDoc.child("route").child("routeInfo");

  if (pRouteNameElement == nullptr) return;

  string route_name = pRouteNameElement.attribute("routeName").value();
  my_position.route_name = route_name;
  bool exists = false;
  // wxMessageBox(s);

  xml_node pWaypointsElement = xmlDoc.child("route").child("waypoints");
  if (pWaypointsElement == nullptr) return;

  xml_node pListWaypointsElement = pWaypointsElement.child("waypoint");
  if (pListWaypointsElement == nullptr) return;

  while (pListWaypointsElement != nullptr) {
    string value = "nullptr";
    value = pListWaypointsElement.attribute("id").value();
    if (value == "nullptr") return;  // must have id
    my_position.wpId = value;
    // wxMessageBox(sti);

    value = pListWaypointsElement.attribute("name").value();
    if (value != "nullptr") {
      my_position.wpName = value;
    } else {
      my_position.wpName = my_position.wpId;  // make name same as id if missing
    }

    xml_node pListPositionElement = pListWaypointsElement.child("position");
    if (pListPositionElement == nullptr) return;
    while (pListPositionElement != nullptr) {
      string stp = pListPositionElement.attribute("lat").value();
      my_position.lat = stp;
      // wxMessageBox(stp);

      string stpl = pListPositionElement.attribute("lon").value();
      my_position.lon = stpl;
      // wxMessageBox(stpl);

      pListPositionElement = pListPositionElement.next_sibling(
          "position");  // stop the loop when position empty
    }

    pListWaypointsElement = pListWaypointsElement.next_sibling(
        "waypoint");  // stop the loop when waypoints empty

    my_positions.push_back(my_position);
  }

  ChartTheRoute(route_name);

  wxMessageBox("Press \"Save\" to complete the import");
}

void finSAR_editUIDialog::OnExportRoute(wxCommandEvent& event) {
  int c = pPlugIn->m_pfinSAR_editDialog->m_choiceRoutes->GetSelection();
  if (c == 0) {
    wxMessageBox("No route has been selected\nAborting");
    return;
  }
  wxString rt = pPlugIn->m_pfinSAR_editDialog->m_choiceRoutes->GetString(c);
  ExportRoute(rt);
}

void finSAR_editUIDialog::ExportRoute(wxString route) {
  wxString route_name = route;

  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  // Create XML root node called animals
  xml_node pRoot = xmlDoc.append_child("route");

  const char* xmlns_value = "";

  xmlns_value = "http://www.cirm.org/RTZ/1/2";

  pRoot.append_attribute("xmlns").set_value(xmlns_value);

  pRoot.append_attribute("xmlns:xsi")
      .set_value("http://www.w3.org/2001/XMLSchema-instance");

  pRoot.append_attribute("version").set_value("1.2");

  // ************* Add routeInfo to root node *******

  xml_node routeInfo = pRoot.append_child("routeInfo");
  routeInfo.append_attribute("routeName").set_value(route_name.mb_str());

  // Insert cat's name as first child of animal

  // ************* Add waypoints *******
  xml_node waypoints = pRoot.append_child("waypoints");

  int idn = 0;

  for (std::vector<Position>::iterator itOut = my_positions.begin();
       itOut != my_positions.end(); itOut++) {
    xml_node m_waypoint = waypoints.append_child("waypoint");
    wxString myIdn = wxString::Format(wxT("%i"), idn);
    m_waypoint.append_attribute("id").set_value(myIdn.mb_str());
    m_waypoint.append_attribute("name").set_value((*itOut).wpName.mb_str());
    m_waypoint.append_attribute("revision").set_value("0");

    xml_node position = m_waypoint.append_child("position");

    position.append_attribute("lat").set_value((*itOut).lat.mb_str());
    position.append_attribute("lon").set_value((*itOut).lon.mb_str());

    idn++;
  }
  // done adding waypoints
  // Write xmlDoc into a file

  wxFileDialog dlg(this, _("Save in RTZ format"), wxEmptyString, route_name,
                   " RTZ files(*.rtz) | *.rtz", wxFD_SAVE);

  if (dlg.ShowModal() == wxID_CANCEL) {
    return;
  }

  wxString file_name = dlg.GetFilename();
  wxString file_path = dlg.GetPath();

  wxString rtz_path = pPlugIn->StandardPathRTZ() + route_name + ".rtz";

  if (rtz_path == file_path) {
    wxMessageBox(_("RTZ file cannot overwrite the program data\nAborting"),
                 "Error");
    return;
  }

  // Route name must be the same as the file name, without file extension

  int fl = file_name.length();
  wxString rtz_name = file_name.SubString(0, (fl - 5));

  if (route_name != rtz_name) {
    wxMessageBox(_("RTZ file name must be the same as route name"), "Error");
    return;
  }

  xmlDoc.save_file(file_path.mb_str());
}

void finSAR_editUIDialog::WriteRTZ(wxString route_name) {
  // Select the route from the route table
  //
  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  const char* value = "http://www.cirm.org/RTZ/1/2";

  // Create XML root node called route
  xml_node pRoot = xmlDoc.append_child("route");

  pRoot.append_attribute("xmlns").set_value(value);
  pRoot.append_attribute("xmlns:xsi")
      .set_value("http://www.w3.org/2001/XMLSchema-instance");

  pRoot.append_attribute("version").set_value("1.2");

  // ************* Add routeInfo to root node *******

  xml_node routeInfo = pRoot.append_child("routeInfo");
  routeInfo.append_attribute("routeName").set_value(route_name.mb_str());

  // Insert cat's name as first child of animal

  // ************* Add waypoints *******
  xml_node waypoints = pRoot.append_child("waypoints");

  for (std::vector<PlugIn_Waypoint_Ex*>::iterator itOut = theWaypoints.begin();
       itOut != theWaypoints.end(); itOut++) {
    xml_node m_waypoint = waypoints.append_child("waypoint");

    m_waypoint.append_attribute("id").set_value((*itOut)->m_MarkName.mb_str());
    m_waypoint.append_attribute("name").set_value(
        (*itOut)->m_MarkName.mb_str());
    m_waypoint.append_attribute("revision").set_value("0");

    xml_node position = m_waypoint.append_child("position");
    double dLat = (*itOut)->m_lat;
    double dLon = (*itOut)->m_lon;
    wxString sLat = wxString::Format("%f", dLat);
    wxString sLon = wxString::Format("%f", dLon);

    position.append_attribute("lat").set_value(sLat);
    position.append_attribute("lon").set_value(sLon);
  }

  wxString file_name = route_name + ".rtz";
  wxString file_path = pPlugIn->StandardPathRTZ() + file_name;

  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void finSAR_editUIDialog::WriteEXT(wxString route_name) {
  // Select the route from the route table
  //
  // Create Main level XML container
  xml_document xmlDoc;

  // Create XML root node called extensions

  xml_node pRoot = xmlDoc.append_child("extensions");

  // ************* Add extensionsInfo to root node *******

  xml_node extensionsInfo = pRoot.append_child("route");

  m_dateStamp = pPlugIn->GetRTZDateStamp(route_name);
  extensionsInfo.append_attribute("route_name").set_value(route_name.mb_str());
  extensionsInfo.append_attribute("date_stamp").set_value(m_dateStamp.mb_str());

  // done adding data
  // Write xmlDoc into an extensions file

  wxString file_name = route_name + ".xml";
  wxString file_path = pPlugIn->StandardPathEXT() + file_name;

  // wxMessageBox(file_path);

  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void finSAR_editUIDialog::OnLoadRoute(wxCommandEvent& event) {
  int c = pPlugIn->m_pfinSAR_editDialog->m_choiceRoutes->GetSelection();
  wxString rt = pPlugIn->m_pfinSAR_editDialog->m_choiceRoutes->GetString(c);

  auto uids = GetRouteGUIDArray();
  for (size_t i = 0; i < uids.size(); i++) {
    DeletePlugInRoute(uids[i]);
  }

  mySelectedRoute = rt;
  if (rt == "-----") {
    wxMessageBox("No route selected", "No Selection");
    return;
  }

  wxString file_folder = pPlugIn->StandardPathRTZ();
  wxString file_name = file_folder + rt + ".rtz";
  m_textCtrlRouteInUse->SetValue(rt);
  m_textExtName->SetValue(rt + ".xml");
  // wxMessageBox(file_name);
  ReadRTZ(file_name);
  ChartTheRoute(file_name);
  i_vector.clear();
  r_vector.clear();
  d_vector.clear();
  m_bDrawWptDisk = false;
  m_bDrawDirectionArrow = false;

  RequestRefresh(pParent);

  wxString file_folder_ext = pPlugIn->StandardPathEXT();
  wxString file_name_ext = file_folder_ext + rt + ".xml";

  ReadEXT(file_name_ext);
  mySelectedLeg = 999;
}

void finSAR_editUIDialog::ChartTheRoute(wxString myRoute) {
  PlugIn_Route_Ex* newRoute =
      new PlugIn_Route_Ex;  // for adding a route on OpenCPN chart display

  newRoute->m_NameString = myRoute;
  newRoute->m_isVisible = true;

  double lati, loni, value, value1;
  bool m_bNameVisible = true;

  for (std::vector<Position>::iterator itp = my_positions.begin();
       itp != my_positions.end(); itp++) {
    PlugIn_Waypoint_Ex* wayPoint = new PlugIn_Waypoint_Ex;

    wayPoint->m_MarkName = (*itp).wpName;

    if (!(*itp).lat.ToDouble(&value)) { /* error! */
    }
    lati = value;
    if (!(*itp).lon.ToDouble(&value1)) { /* error! */
    }
    loni = value1;

    m_bNameVisible = true;
    //(*itp).name_visible;

    wayPoint->m_lat = lati;
    wayPoint->m_lon = loni;
    wayPoint->IsVisible = true;
    wayPoint->IsNameVisible = m_bNameVisible;

    wayPoint->IconName = "diamond";

    newRoute->pWaypointList->Append(wayPoint);
  }

  AddPlugInRouteEx(newRoute, true);

  GetParent()->Refresh();
}

void finSAR_editUIDialog::OnDeleteRoute(wxCommandEvent& event) {
  int c = m_choiceRoutes->GetSelection();
  wxString rt = m_choiceRoutes->GetString(c);
  if (rt == "-----") {
    wxMessageBox("No route selected", "No Selection");
    return;
  }

  pPlugIn->DeleteRTZ_Name(rt);  // remove from db
  pPlugIn->DeleteEXT_Name(rt);  // remove from db

  DeleteRTZFile(rt);
  DeleteEXTFile(rt);

  i_vector.clear();
  r_vector.clear();
  d_vector.clear();
  m_bDrawWptDisk = false;

  m_choiceRoutes->Delete(c);
  m_choiceRoutes->SetSelection(0);
  m_textCtrlRouteInUse->SetValue("");
  m_textExtName->SetValue("");
  DeleteChartedRoute();

  RequestRefresh(pParent);
}

void finSAR_editUIDialog::DeleteRTZFile(wxString route_name) {
  wxString rt = route_name;
  wxString file_folder = pPlugIn->StandardPathRTZ();
  wxString file_name = file_folder + rt + ".rtz";
  wxFileName fn;
  fn.SetFullName(file_name);
  // wxMessageBox(file_name);
  if (!wxFileExists(fn.GetFullName())) {
    bool success = wxRemove(file_name);
  } else {
    wxMessageBox("File not found");
  }
}

void finSAR_editUIDialog::DeleteEXTFile(wxString route_name) {
  wxString rt = route_name;
  wxString file_folder = pPlugIn->StandardPathEXT();
  wxString file_name = file_folder + rt + ".xml";
  wxFileName fn;
  fn.SetFullName(file_name);
  // wxMessageBox(file_name);
  if (!wxFileExists(fn.GetFullName())) {
    bool success = wxRemove(file_name);
  } else {
    wxMessageBox("File not found");
  }
}

void finSAR_editUIDialog::OnIndex(wxCommandEvent& event) {
  if (mySelectedLeg == 999) {
    wxMessageBox("Please activate the waypoint for the leg");
    return;
  }
  GetIndex(active_waypoint, prev_waypoint);
  RequestRefresh(pParent);
}

void finSAR_editUIDialog::GetIndex(Position* A, Position* B) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;

  double lat3 = centreLat;
  double lon3 = centreLon;

  // Bearing A -> C
  double dist1, dist2, brg1, brg2;
  DistanceBearingMercator_Plugin(lat3, lon3, lat1, lon1, &brg1, &dist1);
  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat2, lon2, lat1, lon1, &brg2, &dist2);

  wxString sbrg1 = wxString::Format("%f", brg1);
  // wxMessageBox(sbrg1);

  wxString sbrg2 = wxString::Format("%f", brg2);
  // wxMessageBox(sbrg2);
  /*
  Location a;
  Location b;
  Location x;
*/
  double ax = dist1;
  double alfa = (abs(brg2 - brg1)) / 180 * PI;
  double distance = sin(alfa) * ax;

  double beta = cos(alfa) * ax;

  double dlat, dlon;
  PositionBearingDistanceMercator_Plugin(lat1, lon1, brg2, beta, &dlat, &dlon);

  wxString sdlat = wxString::Format("%f", dlat);
  // wxMessageBox(sdlat);

  wxString sdlon = wxString::Format("%f", dlon);
  // wxMessageBox(sdlon);

  i_target = new IndexTarget;
  i_target->route_name = A->route_name;
  // wxMessageBox(i_target->route_name);
  i_target->wpId = A->wpId;
  // wxMessageBox(i_target->wpId);
  i_target->beginLat = dlat;
  i_target->beginLon = dlon;
  i_target->endLat = lat3;
  i_target->endLon = lon3;

  DistanceBearingMercator_Plugin(lat3, lon3, dlat, dlon, &brg2, &dist2);
  i_target->distance = dist2;
  double label_distance = dist2 / 2;

  double llat, llon;
  PositionBearingDistanceMercator_Plugin(dlat, dlon, brg2, label_distance,
                                         &llat, &llon);
  i_target->label_lat = llat;
  i_target->label_lon = llon;

  i_vector.push_back(*i_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
}
void finSAR_editUIDialog::OnIndexDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  if (i_vector.size() == 0) {
    wxMessageBox("No RTZ file has been read");
  }

  // while (!foundWP) {
  for (vector<IndexTarget>::iterator it = i_vector.begin();
       it != i_vector.end(); it++) {
    label_lat = (*it).label_lat;
    label_lon = (*it).label_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  i_vector.erase(i_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void finSAR_editUIDialog::OnRange(wxCommandEvent& event) {
  if (mySelectedLeg == 999) {
    wxMessageBox("Please activate the waypoint for the leg");
    return;
  }
  Position* range_object = new Position;
  range_object->lat = wxString::Format("%f", centreLat);
  range_object->lon = wxString::Format("%f", centreLon);
  GetRange(active_waypoint, range_object);
}

void finSAR_editUIDialog::GetRange(Position* A, Position* B) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;

  double dist, brg;
  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat2, lon2, lat1, lon1, &brg, &dist);

  r_target = new RangeTarget;
  r_target->route_name = A->route_name;
  // wxMessageBox(r_target->route_name);
  r_target->wpId = A->wpId;
  // wxMessageBox(r_target->wpId);
  r_target->beginLat = lat1;
  r_target->beginLon = lon1;
  r_target->endLat = lat2;
  r_target->endLon = lon2;

  r_target->distance = dist;

  double label_distance = dist / 2;
  double dlat, dlon;
  PositionBearingDistanceMercator_Plugin(lat1, lon1, brg, label_distance, &dlat,
                                         &dlon);

  r_target->label_lat = dlat;
  r_target->label_lon = dlon;

  r_vector.push_back(*r_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
}

void finSAR_editUIDialog::OnRangeDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  if (r_vector.size() == 0) {
    wxMessageBox("No RTZ file has been read");
  }

  for (vector<RangeTarget>::iterator it = r_vector.begin();
       it != r_vector.end(); it++) {
    label_lat = (*it).label_lat;
    label_lon = (*it).label_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  r_vector.erase(r_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void finSAR_editUIDialog::OnDirection(wxCommandEvent& event) {
  if (mySelectedLeg == 999) {
    wxMessageBox("Please activate the waypoint for the leg");
    return;
  }
  if (!m_bDrawDirectionArrow) {
    wxMessageBox(
        "No previous waypoint\nPlease activate the waypoint for the leg");
    return;
  }
  GetDirection(active_waypoint, prev_waypoint);
}

void finSAR_editUIDialog::GetDirection(Position* A, Position* B) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;

  double brg, dist;

  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat1, lon1, lat2, lon2, &brg, &dist);

  d_target = new DirectionTarget;
  d_target->m_lat = centreLat;
  d_target->m_lon = centreLon;
  d_target->m_dir = brg;

  d_vector.push_back(*d_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
}

void finSAR_editUIDialog::OnDirectionDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  if (d_vector.size() == 0) {
    wxMessageBox("No RTZ file has been read");
  }

  for (vector<DirectionTarget>::iterator it = d_vector.begin();
       it != d_vector.end(); it++) {
    label_lat = (*it).m_lat;
    label_lon = (*it).m_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  d_vector.erase(d_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void finSAR_editUIDialog::OnSaveExtensions(wxCommandEvent& event) {
  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
  pPlugIn->DeleteEXT_Name(mySelectedRoute);
  pPlugIn->Add_EXT_db(extensions_file, mySelectedRoute, date_stamp);
  m_bDrawWptDisk = false;
  m_bDrawDirectionArrow = false;
}

void finSAR_editUIDialog::ReadRTZ(wxString file_name) {
  my_positions.clear();  // Set up a new vector

  wxString file = file_name;
  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result =
      xmlDoc.load_file(file.mb_str(), parse_default | parse_declaration);

  string rtz_version = xmlDoc.child("route").attribute("version").value();

  pugi::xml_node pRoot = xmlDoc.child("route").child("routeInfo");
  if (pRoot == nullptr) return;

  wxString error;
  wxProgressDialog* progressdialog = NULL;

  my_position.visible = "1";  // dummy value to make sure there is an entry
  my_position.planned_speed =
      "5";  // dummy value to make sure there is an entry
  my_position.route_name = "na";
  my_position.wpSym = "diamond";

  xml_node pRouteNameElement = xmlDoc.child("route").child("routeInfo");

  if (pRouteNameElement == nullptr) return;

  string route_name = pRouteNameElement.attribute("routeName").value();
  my_position.route_name = route_name;
  mySelectedRoute = route_name;

  bool exists = false;
  // wxMessageBox(route_name);

  xml_node pWaypointsElement = xmlDoc.child("route").child("waypoints");
  if (pWaypointsElement == nullptr) return;

  xml_node pListWaypointsElement = pWaypointsElement.child("waypoint");
  if (pListWaypointsElement == nullptr) return;

  int legnum = 0;

  while (pListWaypointsElement != nullptr) {
    string value = "nullptr";
    value = pListWaypointsElement.attribute("id").value();
    if (value == "nullptr") return;  // must have id

    my_position.leg_number =
        legnum;  // Route leg defined by the waypoint at the end of each leg

    my_position.wpId = value;
    // wxMessageBox(value);

    value = pListWaypointsElement.attribute("name").value();
    if (value != "nullptr") {
      my_position.wpName = value;
    } else {
      my_position.wpName = my_position.wpId;  // make name same as id if missing
    }

    xml_node pListPositionElement = pListWaypointsElement.child("position");
    if (pListPositionElement == nullptr) return;
    while (pListPositionElement != nullptr) {
      string stp = pListPositionElement.attribute("lat").value();
      my_position.lat = stp;
      // wxMessageBox(stp);

      string stpl = pListPositionElement.attribute("lon").value();
      my_position.lon = stpl;
      // wxMessageBox(stpl);

      pListPositionElement = pListPositionElement.next_sibling(
          "position");  // stop the loop when position empty
    }

    pListWaypointsElement = pListWaypointsElement.next_sibling(
        "waypoint");  // stop the loop when waypoints empty

    my_positions.push_back(my_position);

    legnum++;
  }

  int count = my_positions.size();
  wxString mycount = wxString::Format("%i", count);
}

void finSAR_editUIDialog::ReadEXT(wxString file_name) {
  i_vector.clear();  // Set up a new vector
  i_target = new IndexTarget;
  r_vector.clear();  // Set up a new vector
  r_target = new RangeTarget;
  d_vector.clear();  // Set up a new vector
  d_target = new DirectionTarget;

  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result =
      xmlDoc.load_file(file_name.mb_str(), parse_default | parse_declaration);
  bool exists = false;

  pugi::xml_node root = xmlDoc.child("extensions");
  pugi::xml_node route = root.child("route");
  i_target->route_name = route.attribute("route_name").value();
  i_target->date_stamp = route.attribute("date_stamp").value();

  //
  pugi::xml_node targetNode = route.child("targets");
  if (targetNode == NULL) return;

  for (pugi::xml_node indexNode = targetNode.child("index_target"); indexNode;
       indexNode = indexNode.next_sibling("index_target")) {
    i_target->wpId = indexNode.attribute("wp_id").value();
    xml_node pBeginElement = indexNode.child("begin");

    double dvalue = 0.0;
    wxString stp = pBeginElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    i_target->beginLat = dvalue;

    wxString stpl = pBeginElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    i_target->beginLon = dvalue;

    xml_node pEndElement = indexNode.child("end");
    stp = pEndElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    i_target->endLat = dvalue;

    stpl = pEndElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    i_target->endLon = dvalue;

    xml_node pDistance = indexNode.child("index_distance");
    stp = pDistance.attribute("distance").value();
    stp.ToDouble(&dvalue);
    i_target->distance = dvalue;

    xml_node pLabel = indexNode.child("label");
    stp = pLabel.attribute("label_lat").value();
    stp.ToDouble(&dvalue);
    i_target->label_lat = dvalue;

    stp = pLabel.attribute("label_lon").value();
    stp.ToDouble(&dvalue);
    i_target->label_lon = dvalue;

    i_vector.push_back(*i_target);
  }

  for (pugi::xml_node rangeNode = targetNode.child("range_target"); rangeNode;
       rangeNode = rangeNode.next_sibling("range_target")) {
    r_target->wpId = rangeNode.attribute("wp_id").value();
    xml_node pBeginElement = rangeNode.child("begin");

    double dvalue = 0.0;
    wxString stp = pBeginElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    r_target->beginLat = dvalue;

    wxString stpl = pBeginElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    r_target->beginLon = dvalue;

    xml_node pEndElement = rangeNode.child("end");
    stp = pEndElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    r_target->endLat = dvalue;

    stpl = pEndElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    r_target->endLon = dvalue;

    xml_node pDistance = rangeNode.child("range_distance");
    stp = pDistance.attribute("distance").value();
    // wxMessageBox(stp);
    stp.ToDouble(&dvalue);
    r_target->distance = dvalue;

    xml_node pLabel = rangeNode.child("label");
    stp = pLabel.attribute("label_distance").value();
    stp.ToDouble(&dvalue);
    r_target->distance = dvalue;

    stp = pLabel.attribute("label_lat").value();
    stp.ToDouble(&dvalue);
    r_target->label_lat = dvalue;

    stp = pLabel.attribute("label_lon").value();
    stp.ToDouble(&dvalue);
    r_target->label_lon = dvalue;

    r_vector.push_back(*r_target);
  }

  for (pugi::xml_node directionNode = targetNode.child("direction_target");
       directionNode;
       directionNode = directionNode.next_sibling("direction_target")) {
    double dvalue = 0.0;
    wxString stp = directionNode.attribute("direction_lat").value();
    stp.ToDouble(&dvalue);
    d_target->m_lat = dvalue;

    wxString stpl = directionNode.attribute("direction_lon").value();
    stpl.ToDouble(&dvalue);
    d_target->m_lon = dvalue;

    stp = directionNode.attribute("direction").value();
    stp.ToDouble(&dvalue);
    d_target->m_dir = dvalue;

    d_vector.push_back(*d_target);
  }

  RequestRefresh(pParent);
}

void finSAR_editUIDialog::OnContextMenu(double m_lat, double m_lon) {
  c_lat = m_lat;
  c_lon = m_lon;

  m_bDrawWptDisk = false;
  m_bDrawDirectionArrow = false;

  int leg_number = SetActiveWaypoint(c_lat, c_lon);
  mySelectedLeg = leg_number;
}

int finSAR_editUIDialog::SetActiveWaypoint(double t_lat, double t_lon) {
  double wpDistance = 1000;
  double rLat, rLon;
  double minDist = 500.0;
  int wpRefNum = 0;
  int it_num = 0;
  bool foundWP = false;
  wxString wp_name;
  wxString it_name;
  double it_lat, it_lon;

  if (my_positions.size() == 0) {
    wxMessageBox("No RTZ file has been read");
    return 999;
  }

  // while (!foundWP) {
  for (vector<Position>::iterator it = my_positions.begin();
       it != my_positions.end(); it++) {
    wpRefNum = (*it).leg_number;

    wp_name = (*it).wpName;

    double value = 0.0;
    wxString slat = (*it).lat;
    slat.ToDouble(&value);
    rLat = value;

    double value2 = 0.0;
    wxString slon = (*it).lon;
    slon.ToDouble(&value2);
    rLon = value2;

    double brg;
    // Bearing/Distance A -> B
    DistanceBearingMercator_Plugin(rLat, rLon, t_lat, t_lon, &brg, &wpDistance);

    if (wpDistance < minDist) {
      minDist = wpDistance;
      it_num = wpRefNum;
      it_name = wp_name;
      it_lat = rLat;
      it_lon = rLon;
    }
  }

  if (it_num != 0)
    FindPreviousWaypoint(it_name);
  else {
    active_waypoint = new Position;
    active_waypoint->wpName = it_name;

    wxString slat = wxString::Format("%f", it_lat);
    active_wp_lat = it_lat;
    active_waypoint->lat = slat;
    wxString slon = wxString::Format("%f", it_lon);
    active_wp_lon = it_lon;
    active_waypoint->lon = slon;
    active_waypoint->route_name = mySelectedRoute;
    m_bDrawWptDisk = true;
    m_bDrawDirectionArrow = false;
  }

  return it_num;
}

Position* finSAR_editUIDialog::FindPreviousWaypoint(wxString ActiveWpt) {
  active_waypoint = new Position;
  prev_waypoint = new Position;
  std::vector<Position>::iterator prev;
  for (std::vector<Position>::iterator itp = my_positions.begin(),
                                       prev = my_positions.end();
       itp != my_positions.end(); prev = itp, itp++) {
    wxString wpn = (*itp).wpName;

    if (wpn == ActiveWpt) {
      active_waypoint->wpName = wpn;
      active_waypoint->lat = (*itp).lat;
      active_waypoint->lon = (*itp).lon;
      active_waypoint->route_name = (*itp).route_name;
      active_waypoint->wpId = (*itp).wpId;

      double value = 0.0;
      wxString slat = active_waypoint->lat;
      slat.ToDouble(&value);
      active_wp_lat = value;

      double value2 = 0.0;
      wxString slon = active_waypoint->lon;
      slon.ToDouble(&value2);
      active_wp_lon = value2;

      prev_waypoint->wpId = (*prev).wpId;
      prev_waypoint->wpName = (*prev).wpName;

      prev_waypoint->lat = (*prev).lat;
      prev_waypoint->lon = (*prev).lon;

      // wxMessageBox(prev_waypoint->wpId);

      m_bDrawWptDisk = true;
      m_bDrawDirectionArrow = true;

      RequestRefresh(pParent);

      return active_waypoint;
    }
  }

  active_waypoint->wpName = wxEmptyString;
  return active_waypoint;
}

int finSAR_editUIDialog::DeleteChartedRoute() {
  std::vector<std::unique_ptr<PlugIn_Route_Ex>> routes;
  auto uids = GetRouteGUIDArray();
  if (uids.size() > 1) {
    wxMessageBox("Clear the other routes!");
    return 0;
  }

  DeletePlugInRoute(uids[0]);
  return 1;
}

double finSAR_editUIDialog::FindDistanceFromLeg(Position* A, Position* B,
                                                Position* C) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;
  C->lat.ToDouble(&value);
  double lat3 = value;
  C->lon.ToDouble(&value);
  double lon3 = value;

  // Bearing A -> C
  double dist1, dist2, brg1, brg2;
  DistanceBearingMercator_Plugin(lat3, lon3, lat1, lon1, &brg1, &dist1);
  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat2, lon2, lat1, lon1, &brg2, &dist2);

  wxString sbrg1 = wxString::Format("%f", brg1);
  // wxMessageBox(sbrg1);

  wxString sbrg2 = wxString::Format("%f", brg2);
  // wxMessageBox(sbrg2);
  /*
  Location a;
  Location b;
  Location x;
*/
  double ax = dist1;
  double alfa = (abs(brg2 - brg1)) / 180 * PI;
  double distance = sin(alfa) * ax;

  return distance;
}
