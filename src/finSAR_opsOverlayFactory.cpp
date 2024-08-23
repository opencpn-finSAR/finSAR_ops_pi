/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  finSAR_ops Object
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
 *
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include "finSAR_opsUIDialog.h"
#include "finSAR_opsUIDialogBase.h"
#include "finSAR_opsOverlayFactory.h"
#include <vector>

#ifdef __WXOSX__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#endif

#ifdef USE_GLES2
#include "GLES2/gl2.h"
#endif

#ifdef __WXMSW__
#define snprintf _snprintf
#endif  // __WXMSW__

using namespace std;

class finSAR_opsUIDialog;
class PlugIn_ViewPort;
class piDC;
class IndexTarget;

static bool glQueried = false;

static GLboolean QueryExtension(const char *extName) {
  /*
   ** Search for extName in the extensions string. Use of strstr()
   ** is not sufficient because extension names can be prefixes of
   ** other extension names. Could use strtok() but the constant
   ** string returned by glGetString might be in read-only memory.
   */
  char *p;
  char *end;
  int extNameLen;

  extNameLen = strlen(extName);

  p = (char *)glGetString(GL_EXTENSIONS);
  if (NULL == p) {
    return GL_FALSE;
  }

  end = p + strlen(p);

  while (p < end) {
    int n = strcspn(p, " ");
    if ((extNameLen == n) && (strncmp(extName, p, n) == 0)) {
      return GL_TRUE;
    }
    p += (n + 1);
  }
  return GL_FALSE;
}

//----------------------------------------------------------------------------------------------------------
//    finSAR_ops Overlay Factory Implementation
//----------------------------------------------------------------------------------------------------------
finSAR_opsOverlayFactory::finSAR_opsOverlayFactory(finSAR_opsUIDialog &dlg)
    : m_dlg(dlg) {
  m_dFont_map = new wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                           wxFONTWEIGHT_NORMAL);
  m_dFont_war = new wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC,
                           wxFONTWEIGHT_NORMAL);

  m_last_vp_scale = 0.;
  m_dtUseNew = m_dlg.m_dtNow;
}

finSAR_opsOverlayFactory::~finSAR_opsOverlayFactory() {}

void finSAR_opsOverlayFactory::Reset() {}

void finSAR_opsOverlayFactory::setData(double lat1, double lon1) {
  myLat1 = lat1;
  myLon1 = lon1;
}

bool finSAR_opsOverlayFactory::RenderOverlay(piDC &dc, PlugIn_ViewPort &vp) {
  m_dc = &dc;

  if (!dc.GetDC()) {
    if (!glQueried) {
      glQueried = true;
    }
#ifndef USE_GLSL
    glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT);  // Save state

    //      Enable anti-aliased lines, at best quality
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
    glEnable(GL_BLEND);
  }

  wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
              wxFONTWEIGHT_NORMAL);
  m_dc->SetFont(font);

  DrawAllLinesInViewPort(&vp);  // The cross-hairs

  DrawIndexTargets(&vp);

  DrawRangeTargets(&vp);

  DrawDirectionTargets(&vp);

  if (m_dlg.m_bDrawWptDisk) DrawWptDisk(&vp);

  return true;
}

void finSAR_opsOverlayFactory::DrawWptDisk(PlugIn_ViewPort *BBox) {
  wxColour colour = wxColour("YELLOW");
  wxBrush brush(colour);
  c_GLcolour = colour;  // for filling GL arrows
  if (m_dc) {
    wxPen pen(colour, 2);

    m_dc->SetPen(pen);
    brush.SetStyle(wxBRUSHSTYLE_SOLID);
    m_dc->SetBrush(brush);
  }

  wxPoint wpc;
  GetCanvasPixLL(BBox, &wpc, m_dlg.active_wp_lat, m_dlg.active_wp_lon);

  if (m_dc) {
    m_dc->DrawDisk(wpc.x, wpc.y, 5, 10);

    colour = wxColour("RED");
    wxPen pen2(colour, 2);
    wxBrush brush2(colour);

    m_dc->SetPen(pen2);
    brush2.SetStyle(wxBRUSHSTYLE_SOLID);
    m_dc->SetBrush(brush2);
    m_dc->DrawDisk(wpc.x, wpc.y, 10, 15);
  }
}

void finSAR_opsOverlayFactory::DrawIndexTargets(PlugIn_ViewPort *BBox) {
  wxColour colour1 = wxColour("BLACK");
  wxColour colour2 = wxColour("WHITE");

  wxPen pen1(colour1, 2);
  wxPen pen2(colour2, 2);

  pen1.SetStyle(wxPENSTYLE_SHORT_DASH);

  // c_GLcolour = colour;  // for filling GL arrows
  if (m_dc) {
    m_dc->SetPen(pen1);
  }

  for (std::vector<IndexTarget>::iterator it = m_dlg.i_vector.begin();
       it != m_dlg.i_vector.end(); it++) {
    m_dc->SetPen(pen1);
    wxPoint ib;
    GetCanvasPixLL(BBox, &ib, (*it).beginLat, (*it).beginLon);
    wxPoint ie;
    GetCanvasPixLL(BBox, &ie, (*it).endLat, (*it).endLon);
    m_dc->DrawLine(ib.x, ib.y, ie.x, ie.y, true);

    double dlat, dlon;
    dlat = (*it).label_lat;
    dlon = (*it).label_lon;

    wxPoint il;
    GetCanvasPixLL(BBox, &il, dlat, dlon);
    double dist = (*it).distance;
    wxString dist_text = wxString::Format("%3.0f", dist * 100);

    if (dist < 0.10) {
      std::string s = dist_text;
      string r = s.substr(2, 1);
      unsigned int number_of_zeros = 2 - r.length();  // add 1 zero

      r.insert(0, number_of_zeros, '0');
      dist_text = r;
    }
    dist_text = " " + dist_text;

    wxImage image = DrawLabel(dist, 1);
    wxCoord w = image.GetWidth();
    wxCoord h = image.GetHeight();

    wxBitmap bm(image);
    m_dc->DrawBitmap(bm, il.x - w / 4, il.y - h / 4, true);

    wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_dc->SetFont(font);
    m_dc->SetTextForeground("WHITE");
    m_dc->SetPen(pen2);
    m_dc->DrawText(dist_text, il.x - w / 4, il.y - h / 4 + 6);
  }
}

void finSAR_opsOverlayFactory::DrawRangeTargets(PlugIn_ViewPort *BBox) {
  wxColour colour1 = wxColour("BLACK");
  wxColour colour2 = wxColour("WHITE");

  wxPen pen1(colour1, 2);
  wxPen pen2(colour2, 2);

  pen1.SetStyle(wxPENSTYLE_SHORT_DASH);

  // c_GLcolour = colour;  // for filling GL arrows
  if (m_dc) {
    m_dc->SetPen(pen1);
  }

  for (std::vector<RangeTarget>::iterator it = m_dlg.r_vector.begin();
       it != m_dlg.r_vector.end(); it++) {
    m_dc->SetPen(pen1);
    wxPoint ib;
    GetCanvasPixLL(BBox, &ib, (*it).beginLat, (*it).beginLon);
    wxPoint ie;
    GetCanvasPixLL(BBox, &ie, (*it).endLat, (*it).endLon);
    m_dc->DrawLine(ib.x, ib.y, ie.x, ie.y, true);

    double dlat, dlon;
    dlat = (*it).label_lat;
    dlon = (*it).label_lon;

    wxPoint il;
    GetCanvasPixLL(BBox, &il, dlat, dlon);
    double dist = (*it).distance;
    wxString dist_text = wxString::Format("%3.0f", dist * 100);

    if (dist < 0.10) {
      std::string s = dist_text;
      string r = s.substr(2, 1);
      unsigned int number_of_zeros = 2 - r.length();  // add 1 zero

      r.insert(0, number_of_zeros, '0');
      dist_text = r;
    }
    dist_text = " " + dist_text;

    wxImage image = DrawLabel(dist, 1);
    wxCoord w = image.GetWidth();
    wxCoord h = image.GetHeight();

    wxBitmap bm(image);
    m_dc->DrawBitmap(bm, il.x - w / 4, il.y - h / 4, true);

    wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_dc->SetFont(font);
    m_dc->SetTextForeground("WHITE");
    m_dc->SetPen(pen2);
    m_dc->DrawText(dist_text, il.x - w / 4, il.y - h / 4 + 6);
  }
}

void finSAR_opsOverlayFactory::DrawDirectionTargets(PlugIn_ViewPort *BBox) {
  if (BBox->chart_scale > 1000000) {
    return;
  }

  // Set up the scaler
  double mmx = PlugInGetDisplaySizeMM();

  int sx, sy;
  wxDisplaySize(&sx, &sy);

  double m_pix_per_mm = ((double)sx) / (mmx);

  float draw_scaler = m_pix_per_mm;

  double scale = draw_scaler / 3.5;

  // End setting up scaler

  double dlat, dlon, direction_brg, rot_angle;

  for (std::vector<DirectionTarget>::iterator it = m_dlg.d_vector.begin();
       it != m_dlg.d_vector.end(); it++) {
    dlat = (*it).m_lat;
    dlon = (*it).m_lon;
    direction_brg = (*it).m_dir;

    rot_angle = m_dlg.deg2rad((*it).m_dir) + 3 * m_dlg.deg2rad(90);

    wxPoint dl;
    GetCanvasPixLL(BBox, &dl, dlat, dlon);

    wxImage image = DrawDirectionArrows(0, 0, scale);
    wxCoord w = image.GetWidth();
    wxCoord h = image.GetHeight();

    wxPoint rot_point(w / 2, h / 2);

    wxImage rot_image = image.Rotate(-rot_angle, rot_point, true, 0);
    wxImage rot_90 = image.Rotate90(true);

    wxBitmap bm(rot_image);

    m_dc->DrawBitmap(bm, dl.x - 75, dl.y - 75, true);

    // Now draw the direction labels
    bool reverse_label = false;
    if (direction_brg > 180) reverse_label = true;

    wxImage label_image =
        DrawDirectionLabels(direction_brg, 0, 0, 1, reverse_label);
    wxImage rot_label_image =
        label_image.Rotate(-rot_angle, rot_point, true, 0);
    // wxImage rot_90 = image.Rotate90(true);

    wxBitmap bml(rot_label_image);

    m_dc->DrawBitmap(bml, dl.x - 75, dl.y - 75, true);
  }
}

void finSAR_opsOverlayFactory::DrawAllLinesInViewPort(PlugIn_ViewPort *BBox) {
  wxColour colour = wxColour("RED");
  wxBrush brush(colour);
  c_GLcolour = colour;  // for filling GL arrows
  if (m_dc) {
    wxPen pen(colour, 2);

    m_dc->SetPen(pen);
    brush.SetStyle(wxBRUSHSTYLE_SOLID);
    m_dc->SetBrush(brush);
  }

  wxPoint r;
  GetCanvasPixLL(BBox, &r, m_dlg.centreLat, m_dlg.centreLon);

  wxString latlon = wxString::Format("%i", r.x);
  // wxMessageBox(latlon);

  latlon = wxString::Format("%i", r.y);
  //  wxMessageBox(latlon);

  // setData(m_dlg.centreLat, m_dlg.centreLon);
  // m_dlg.chartScale = vp->chart_scale;

  // wxMessageBox("in draw lines");

  // Move to the first point
  int x = r.x;
  int y = r.y;

  float xt = m_dlg.myPixArray[0].x;
  float yt = m_dlg.myPixArray[0].y;

  int x1 = xt;
  int y1 = yt;

  p[0].x = x + xt;
  p[0].y = y + yt;

  // Walk thru the point list
  for (int ip = 1; ip < 5; ip++) {
    xt = m_dlg.myPixArray[ip].x;
    yt = m_dlg.myPixArray[ip].y;

    int x2 = xt;
    int y2 = yt;

    if (m_dc) {
      m_dc->DrawLine(x1 + x, y1 + y, x2 + x, y2 + y, false);
    }

    p[ip].x = x1 + x;
    p[ip].y = y1 + y;

    x1 = x2;
    y1 = y2;
  }
}

wxImage &finSAR_opsOverlayFactory::DrawLabel(double value, int precision) {
  wxString labels;

  int p = precision;

  value *= 100;

  labels = wxString::Format("%3.0f", value);
  labels = " " + labels + " ";

  if (value < 0.01) {
    labels = " " + labels;
  }
  // labels.Printf("%.*f", p, value);

  wxMemoryDC mdc(wxNullBitmap);

  wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  mdc.SetFont(font);

  wxCoord w, h;
  mdc.GetTextExtent(labels, &w, &h);

  wxBitmap bm(w * 2, w * 2);

  mdc.SelectObject(bm);
  mdc.Clear();

  wxColour disk_color = wxColour("BLACK");
  wxColour text_color = wxColour("WHITE");

  mdc.SetBackground(*wxTRANSPARENT_BRUSH);
  mdc.SetBrush(disk_color);

  wxCoord r = w / 2 - w / 200 - 1;

  mdc.DrawCircle(w / 2, w / 2, r);

  //
  // Now drawing in DrawIndexTargets to avoid transparency of text
  //

  // mdc.SetTextForeground(text_color);
  // mdc.SetPen(text_color);

  int xd = 0;
  int yd = w / 2;

  // mdc.DrawText(labels, xd, yd - 12);
  mdc.SelectObject(wxNullBitmap);

  m_labelCache[value] = bm.ConvertToImage();

  // Setup the alpha channel.
  unsigned char *alphaData = new unsigned char[bm.GetWidth() * bm.GetHeight()];
  memset(alphaData, wxIMAGE_ALPHA_TRANSPARENT, bm.GetWidth() * bm.GetHeight());

  // Create an image with alpha.
  m_labelCache[value].SetAlpha(alphaData);

  wxImage &image = m_labelCache[value];

  unsigned char *d = image.GetData();
  unsigned char *a = image.GetAlpha();

  w = image.GetWidth(), h = image.GetHeight();
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      int r, g, b;
      int ioff = (y * w + x);
      r = d[ioff * 3 + 0];
      g = d[ioff * 3 + 1];
      b = d[ioff * 3 + 2];

      a[ioff] = 255 - (r + g + b) / 3;
    }

  return image;
}

wxImage finSAR_opsOverlayFactory::DrawDirectionArrows(int x, int y,
                                                       double scale) {
  wxMemoryDC mdc(wxNullBitmap);

  wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  mdc.SetFont(font);

  wxCoord w, h;
  w = 150, h = 150;

  wxBitmap bm(w, h);

  mdc.SelectObject(bm);
  mdc.Clear();

  wxColour green_color = wxColour("GREEN");
  wxColour text_color = wxColour("BLACK");

  wxPen penGreen(green_color);
  mdc.SetPen(penGreen);

  mdc.SetBackground(*wxTRANSPARENT_BRUSH);
  mdc.Clear();
  mdc.SetBrush(green_color);

  // Move to the first point

  float xt = DirectionArrowArray[0].x;
  float yt = DirectionArrowArray[0].y;

  int x1 = (int)(xt * scale);
  int y1 = (int)(yt * scale);

  p[0].x = x1 + 80;
  p[0].y = y1 + 85;

  x = 45;
  y = 85;

  // Walk thru the point list
  for (int ip = 1; ip < NUM_DIRECTION_ARROW_POINTS; ip++) {
    xt = DirectionArrowArray[ip].x;
    yt = DirectionArrowArray[ip].y;

    int x2 = (int)(xt * scale);
    int y2 = (int)(yt * scale);

    p[ip].x = x2 + x;
    p[ip].y = y2 + y;

    x1 = x2;
    y1 = y2;
  }

  /*
   *           4
   *          /\
   *         /  \
   *        /    \
   *     3 /      \ 5
   *      /_ 2   6_\
   *        |    |
   *        |    |
   *        |    |
   *        |____|
   *       1   0  7
   */

  polyPoints[0] = p[3];
  polyPoints[1] = p[4];
  polyPoints[2] = p[5];

  rectPoints[0] = p[1];
  rectPoints[1] = p[2];
  rectPoints[2] = p[6];
  rectPoints[3] = p[7];

  // polyPoints[4] = p[8];

  mdc.DrawPolygon(3, polyPoints);
  mdc.DrawPolygon(4, rectPoints);

  mdc.SetTextForeground(text_color);
  mdc.SetTextBackground(*wxWHITE);
  wxPoint green_direction(50, 95);
  // mdc.DrawText("123", green_direction);

  // End of green arrow
  // Start of red arrow
  wxColour red_color = wxColour("RED");

  wxPen penRed(red_color);
  mdc.SetPen(penRed);

  mdc.SetBackground(*wxTRANSPARENT_BRUSH);
  mdc.SetBrush(red_color);
  // Move to the first point

  float xtr = ReverseArrowArray[0].x;
  float ytr = ReverseArrowArray[0].y;

  x1 = (int)(xtr * scale);
  y1 = (int)(ytr * scale);

  r[0].x = x1 + 0;
  r[0].y = y1 + 15;

  // Walk thru the point list
  for (int ip = 1; ip < NUM_REVERSE_ARROW_POINTS; ip++) {
    xt = ReverseArrowArray[ip].x;
    yt = ReverseArrowArray[ip].y;

    int x2 = (int)(xt * scale);
    int y2 = (int)(yt * scale);

    x = -2;
    y = 160;

    r[ip].x = x2 + x;
    r[ip].y = y2 + y;

    x1 = x2;
    y1 = y2;
  }

  /*
   *           4
   *          /\
   *         /  \
   *        /    \
   *     3 /      \ 5
   *      /_ 2   6_\
   *        |    |
   *        |    |
   *        |    |
   *        |____|
   *       1   0  7
   */

  polyReversePoints[0] = r[3];
  polyReversePoints[1] = r[4];
  polyReversePoints[2] = r[5];

  rectReversePoints[0] = r[1];
  rectReversePoints[1] = r[2];
  rectReversePoints[2] = r[6];
  rectReversePoints[3] = r[7];

  // polyPoints[4] = p[8];

  mdc.DrawPolygon(3, polyReversePoints);
  mdc.DrawPolygon(4, rectReversePoints);

  wxPoint red_direction(50, 52);
  // mdc.DrawText("123", red_direction);

  wxImage arrow = bm.ConvertToImage();

  // Setup the alpha channel.
  unsigned char *alphaData = new unsigned char[bm.GetWidth() * bm.GetHeight()];
  memset(alphaData, wxIMAGE_ALPHA_TRANSPARENT, bm.GetWidth() * bm.GetHeight());

  // Create an image with alpha.
  arrow.SetAlpha(alphaData);

  wxImage &image = arrow;

  unsigned char *d = image.GetData();
  unsigned char *a = image.GetAlpha();

  w = image.GetWidth(), h = image.GetHeight();
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      int r, g, b;
      int ioff = (y * w + x);
      r = d[ioff * 3 + 0];
      g = d[ioff * 3 + 1];
      b = d[ioff * 3 + 2];

      a[ioff] = 255 - (r + g + b) / 3;
    }

  return image;
}

wxImage finSAR_opsOverlayFactory::DrawDirectionLabels(double value, int x,
                                                       int y, double scale,
                                                       bool reverse) {
  wxMemoryDC mdc(wxNullBitmap);

  wxString direction_brg = wxString::Format("%3.0f", value);

  if (value < 10) {
    std::string s = direction_brg;
    string r = s.substr(2, 1);
    unsigned int number_of_zeros = 3 - r.length();  // add 2 zero

    r.insert(0, number_of_zeros, '0');
    direction_brg = r;

  } else if (value > 9 && value < 100) {
    std::string s = direction_brg;
    string r = s.substr(1, 2);
    unsigned int number_of_zeros = 3 - r.length();  // add 1 zero

    r.insert(0, number_of_zeros, '0');
    direction_brg = r;
  }

  double rev = value + 180.0;
  double reverse_direction = 0.0;
  if (rev > 360)
    reverse_direction = rev - 360;
  else
    reverse_direction = rev;

  wxString reverse_direction_brg = wxString::Format("%3.0f", reverse_direction);

  if (reverse_direction < 10) {
    std::string s = reverse_direction_brg;
    string r = s.substr(2, 1);
    unsigned int number_of_zeros = 3 - r.length();  // add 2 zero

    r.insert(0, number_of_zeros, '0');
    reverse_direction_brg = r;

  } else if (reverse_direction > 9 && reverse_direction < 100) {
    std::string s = reverse_direction_brg;
    string r = s.substr(1, 2);
    unsigned int number_of_zeros = 3 - r.length();  // add 1 zero

    r.insert(0, number_of_zeros, '0');
    reverse_direction_brg = r;
  }

  // wxMessageBox(direction_brg);

  wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  mdc.SetFont(font);

  wxCoord w, h;
  w = 150, h = 150;

  wxBitmap bm(w, h);

  mdc.SelectObject(bm);
  mdc.Clear();

  wxColour green_color = wxColour("GREEN");
  wxColour text_color = wxColour("BLACK");

  wxPen penGreen(green_color);
  mdc.SetPen(penGreen);

  mdc.SetBackground(*wxTRANSPARENT_BRUSH);
  mdc.Clear();
  mdc.SetBrush(green_color);

  // Write green direction

  mdc.SetTextForeground(text_color);
  mdc.SetTextBackground(*wxWHITE);

  int gx, gy;
  if (value > 180) {
    gx = 50, gy = 52;
  } else {
    gx = 50, gy = 95;
  }

  wxPoint green_direction(gx, gy);

  mdc.DrawText(direction_brg, green_direction);

  // End of green arrow
  // Start of red arrow
  wxColour red_color = wxColour("RED");

  wxPen penRed(red_color);
  mdc.SetPen(penRed);

  mdc.SetBackground(*wxTRANSPARENT_BRUSH);
  mdc.SetBrush(red_color);

  int rx, ry;
  if (value > 180) {
    rx = 50, ry = 95;
  } else {
    rx = 50, ry = 52;
  }

  wxPoint red_direction(rx, ry);

  // Write red direction

  mdc.DrawText(reverse_direction_brg, red_direction);

  wxImage label_image = bm.ConvertToImage();
  wxImage label;

  if (reverse) {
    label = label_image.Rotate180();

  } else
    label = label_image;

  // Setup the alpha channel.
  unsigned char *alphaData = new unsigned char[bm.GetWidth() * bm.GetHeight()];
  memset(alphaData, wxIMAGE_ALPHA_TRANSPARENT, bm.GetWidth() * bm.GetHeight());

  // Create an image with alpha.
  label.SetAlpha(alphaData);

  wxImage &image = label;

  unsigned char *d = image.GetData();
  unsigned char *a = image.GetAlpha();

  w = image.GetWidth(), h = image.GetHeight();
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      int r, g, b;
      int ioff = (y * w + x);
      r = d[ioff * 3 + 0];
      g = d[ioff * 3 + 1];
      b = d[ioff * 3 + 2];

      a[ioff] = 255 - (r + g + b) / 3;
    }

  return image;
}
