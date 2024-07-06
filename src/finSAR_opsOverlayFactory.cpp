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

#define NUM_DIRECTION_ARROW_POINTS 9
static wxPoint CurrrentArrowArray[NUM_DIRECTION_ARROW_POINTS] = {
    wxPoint(0, 0),    wxPoint(0, -10), wxPoint(55, -10),
    wxPoint(55, -25), wxPoint(100, 0), wxPoint(55, 25),
    wxPoint(55, 10),  wxPoint(0, 10),  wxPoint(0, 0)};
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

  DrawAllLinesInViewPort(&vp);

  DrawAllDirectionsInViewPort(&vp);

  DrawIndexTargets(&vp);

  if (m_dlg.m_bBearingLine) {
    DrawEBLLineInViewPort(&vp);
  } else
    return false;

  return true;
}

void finSAR_opsOverlayFactory::DrawEBLLineInViewPort(PlugIn_ViewPort *BBox) {
  wxColour colour = wxColour("RED");
  wxBrush brush(colour);
  c_GLcolour = colour;  // for filling GL arrows
  if (m_dc) {
    wxPen pen(colour, 2);

    m_dc->SetPen(pen);
    brush.SetStyle(wxBRUSHSTYLE_SOLID);
    m_dc->SetBrush(brush);
  }

  wxPoint ebl;
  GetCanvasPixLL(BBox, &ebl, m_dlg.ebl_lat, m_dlg.ebl_lon);
  wxPoint s;
  GetCanvasPixLL(BBox, &s, m_dlg.m_ShipLat2, m_dlg.m_ShipLon2);

  if (m_dc) {
    m_dc->DrawLine(s.x, s.y, ebl.x, ebl.y, false);
  }
}

void finSAR_opsOverlayFactory::DrawIndexTargets(PlugIn_ViewPort *BBox) {
  wxColour colour = wxColour("RED");
  wxBrush brush(colour);
  wxPen pen2(colour, 2);

  c_GLcolour = colour;  // for filling GL arrows
  if (m_dc) {
    m_dc->SetPen(pen2);
    // brush.SetStyle(wxBRUSHSTYLE_SOLID);
    // m_dc->SetBrush(brush);
  }

  for (std::vector<IndexTarget>::iterator it = m_dlg.i_vector.begin();
       it != m_dlg.i_vector.end(); it++) {
    wxPoint ib;
    GetCanvasPixLL(BBox, &ib, (*it).beginLat, (*it).beginLon);
    wxPoint ie;
    GetCanvasPixLL(BBox, &ie, (*it).endLat, (*it).endLon);
    DrawGLLine(ib.x, ib.y, ie.x, ie.y, 2, colour);
  }
}

void finSAR_opsOverlayFactory::DrawBearingLineInViewPort(PlugIn_ViewPort *BBox) {
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
  wxPoint s;
  GetCanvasPixLL(BBox, &s, m_dlg.m_ShipLat1, m_dlg.m_ShipLon1);

  if (m_dc) {
    m_dc->DrawLine(r.x, r.y, s.x, s.y, false);
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

void finSAR_opsOverlayFactory::DrawGLLine(double x1, double y1, double x2,
                                          double y2, double width,
                                          wxColour myColour) {
  {
    wxColour isoLineColor = myColour;
    glColor4ub(isoLineColor.Red(), isoLineColor.Green(), isoLineColor.Blue(),
               255 /*isoLineColor.Alpha()*/);

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_ENABLE_BIT |
                 GL_POLYGON_BIT | GL_HINT_BIT);  // Save state
    {
      //      Enable anti-aliased lines, at best quality
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glLineWidth(width);

      glLineStipple(3, 0xAAAA);
      glEnable(GL_LINE_STIPPLE);

      glBegin(GL_LINES);
      glVertex2d(x1, y1);
      glVertex2d(x2, y2);
      glEnd();
    }

    glPopAttrib();
  }
}


void finSAR_opsOverlayFactory::DrawMessageWindow(wxString msg, int x, int y,
                                               wxFont *mfont) {
  if (msg.empty()) return;

  wxMemoryDC mdc;
  wxBitmap bm(1000, 1000);
  mdc.SelectObject(bm);
  mdc.Clear();

  mdc.SetFont(*mfont);
  mdc.SetPen(*wxTRANSPARENT_PEN);
  mdc.SetBrush(wxColour(243, 229, 47));
  int w, h;
  mdc.GetMultiLineTextExtent(msg, &w, &h);
  h += 2;
  int label_offset = 10;
  int wdraw = w + (label_offset * 2);
  mdc.DrawRectangle(0, 0, wdraw, h);

  mdc.DrawLabel(msg, wxRect(label_offset, 0, wdraw, h),
                wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  mdc.SelectObject(wxNullBitmap);

  wxBitmap sbm = bm.GetSubBitmap(wxRect(0, 0, wdraw, h));

  DrawOLBitmap(sbm, 0, y - (GetChartbarHeight() + h), false);
}

wxImage &finSAR_opsOverlayFactory::DrawGLText(double value, int precision) {
  wxString labels;

  int p = precision;

  labels.Printf(_T("%.*f"), p, value);

  wxMemoryDC mdc(wxNullBitmap);

  wxFont *pTCFont;
  pTCFont =
      wxTheFontList->FindOrCreateFont(12, wxDEFAULT, wxNORMAL, wxBOLD, FALSE,
                                      wxString(_T ( "Eurostile Extended" )));
  mdc.SetFont(*pTCFont);

  int w, h;
  mdc.GetTextExtent(labels, &w, &h);

  int label_offset = 10;  // 5

  wxBitmap bm(w + label_offset * 2, h + 1);
  mdc.SelectObject(bm);
  mdc.Clear();

  wxColour text_color;

  GetGlobalColor(_T ("UINFD" ), &text_color);
  wxPen penText(text_color);
  mdc.SetPen(penText);

  mdc.SetBrush(*wxTRANSPARENT_BRUSH);
  mdc.SetTextForeground(text_color);
  mdc.SetTextBackground(wxTRANSPARENT);

  int xd = 0;
  int yd = 0;

  mdc.DrawText(labels, label_offset + xd, yd + 1);

  mdc.SelectObject(wxNullBitmap);

  m_labelCache[value] = bm.ConvertToImage();

  m_labelCache[value].InitAlpha();

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

  return m_labelCache[value];
}

wxImage &finSAR_opsOverlayFactory::DrawGLTextDir(double value, int precision) {
  wxString labels;

  int p = precision;

  labels.Printf(_T("%03.*f"), p, value);

  wxMemoryDC mdc(wxNullBitmap);

  wxFont *pTCFont;
  pTCFont =
      wxTheFontList->FindOrCreateFont(12, wxDEFAULT, wxNORMAL, wxBOLD, FALSE,
                                      wxString(_T ( "Eurostile Extended" )));

  mdc.SetFont(*pTCFont);

  int w, h;
  mdc.GetTextExtent(labels, &w, &h);

  int label_offset = 10;  // 5

  wxBitmap bm(w + label_offset * 2, h + 1);
  mdc.SelectObject(bm);
  mdc.Clear();

  wxColour text_color;

  GetGlobalColor(_T ("UINFD" ), &text_color);
  wxPen penText(text_color);
  mdc.SetPen(penText);

  mdc.SetBrush(*wxTRANSPARENT_BRUSH);
  mdc.SetTextForeground(text_color);
  mdc.SetTextBackground(wxTRANSPARENT);

  int xd = 0;
  int yd = 0;

  mdc.DrawText(labels, label_offset + xd, yd + 1);

  mdc.SelectObject(wxNullBitmap);

  m_labelCache[value] = bm.ConvertToImage();

  m_labelCache[value].InitAlpha();

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

  return m_labelCache[value];
}

wxImage &finSAR_opsOverlayFactory::DrawGLTextString(wxString myText) {
  wxString labels;
  labels = myText;

  wxMemoryDC mdc(wxNullBitmap);

  wxFont *pTCFont;
  pTCFont =
      wxTheFontList->FindOrCreateFont(12, wxDEFAULT, wxNORMAL, wxBOLD, FALSE,
                                      wxString(_T ( "Eurostile Extended" )));
  mdc.SetFont(*pTCFont);

  int w, h;
  mdc.GetTextExtent(labels, &w, &h);

  int label_offset = 10;  // 5

  wxBitmap bm(w + label_offset * 2, h + 1);
  mdc.SelectObject(bm);
  mdc.Clear();

  wxColour text_color;

  GetGlobalColor(_T ("UINFD" ), &text_color);
  wxPen penText(text_color);
  mdc.SetPen(penText);

  mdc.SetBrush(*wxTRANSPARENT_BRUSH);
  mdc.SetTextForeground(text_color);
  mdc.SetTextBackground(wxTRANSPARENT);

  int xd = 0;
  int yd = 0;

  mdc.DrawText(labels, label_offset + xd, yd + 1);
  mdc.SelectObject(wxNullBitmap);

  m_labelCacheText[myText] = bm.ConvertToImage();

  m_labelCacheText[myText].InitAlpha();

  wxImage &image = m_labelCacheText[myText];

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

  return m_labelCacheText[myText];
}

void finSAR_opsOverlayFactory::DrawOLBitmap(const wxBitmap &bitmap, wxCoord x,
                                          wxCoord y, bool usemask) {
  wxBitmap bmp;
  if (x < 0 || y < 0) {
    int dx = (x < 0 ? -x : 0);
    int dy = (y < 0 ? -y : 0);
    int w = bitmap.GetWidth() - dx;
    int h = bitmap.GetHeight() - dy;
    /* picture is out of viewport */
    if (w <= 0 || h <= 0) return;
    wxBitmap newBitmap = bitmap.GetSubBitmap(wxRect(dx, dy, w, h));
    x += dx;
    y += dy;
    bmp = newBitmap;
  } else {
    bmp = bitmap;
  }
  if (m_pdc)
    m_pdc->DrawBitmap(bmp, x, y, usemask);
  else {
    wxImage image = bmp.ConvertToImage();
    int w = image.GetWidth(), h = image.GetHeight();

    if (usemask) {
      unsigned char *d = image.GetData();
      unsigned char *a = image.GetAlpha();

      unsigned char mr, mg, mb;
      if (!image.GetOrFindMaskColour(&mr, &mg, &mb) && !a)
        printf("trying to use mask to draw a bitmap without alpha or mask\n");

      unsigned char *e = new unsigned char[4 * w * h];
      {
        for (int y = 0; y < h; y++)
          for (int x = 0; x < w; x++) {
            unsigned char r, g, b;
            int off = (y * image.GetWidth() + x);
            r = d[off * 3 + 0];
            g = d[off * 3 + 1];
            b = d[off * 3 + 2];

            e[off * 4 + 0] = r;
            e[off * 4 + 1] = g;
            e[off * 4 + 2] = b;

            e[off * 4 + 3] =
                a ? a[off] : ((r == mr) && (g == mg) && (b == mb) ? 0 : 255);
          }
      }

      glColor4f(1, 1, 1, 1);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glRasterPos2i(x, y);
      glPixelZoom(1, -1);
      glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, e);
      glPixelZoom(1, 1);
      glDisable(GL_BLEND);

      delete[] (e);
    } else {
      glRasterPos2i(x, y);
      glPixelZoom(1, -1); /* draw data from top to bottom */
      glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
      glPixelZoom(1, 1);
    }
  }
}

void finSAR_opsOverlayFactory::DrawGLLabels(finSAR_opsOverlayFactory *pof, wxDC *dc,
                                          PlugIn_ViewPort *vp,
                                          wxImage &imageLabel, double myLat,
                                          double myLon, int offset) {
  //---------------------------------------------------------
  // Ecrit les labels
  //---------------------------------------------------------

  wxPoint ab;
  GetCanvasPixLL(vp, &ab, myLat, myLon);

  wxPoint cd;
  GetCanvasPixLL(vp, &cd, myLat, myLon);

  int w = imageLabel.GetWidth();
  int h = imageLabel.GetHeight();

  int label_offset = 0;
  int xd = (ab.x + cd.x - (w + label_offset * 2)) / 2;
  int yd = (ab.y + cd.y - h) / 2 + offset;

  if (dc) {
    /* don't use alpha for isobars, for some reason draw bitmap ignores
       the 4th argument (true or false has same result) */
    wxImage img(w, h, imageLabel.GetData(), true);
    dc->DrawBitmap(img, xd, yd, false);
  } else { /* opengl */

    int w = imageLabel.GetWidth(), h = imageLabel.GetHeight();

    unsigned char *d = imageLabel.GetData();
    unsigned char *a = imageLabel.GetAlpha();

    unsigned char mr, mg, mb;
    if (!imageLabel.GetOrFindMaskColour(&mr, &mg, &mb) && !a)
      wxMessageBox(
          _T(
                    "trying to use mask to draw a bitmap without alpha or mask\n" ));

    unsigned char *e = new unsigned char[4 * w * h];
    {
      for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
          unsigned char r, g, b;
          int off = (y * imageLabel.GetWidth() + x);
          r = d[off * 3 + 0];
          g = d[off * 3 + 1];
          b = d[off * 3 + 2];

          e[off * 4 + 0] = r;
          e[off * 4 + 1] = g;
          e[off * 4 + 2] = b;

          e[off * 4 + 3] =
              a ? a[off] : ((r == mr) && (g == mg) && (b == mb) ? 0 : 255);
        }
    }

    glColor4f(1, 1, 1, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(xd, yd);
    glPixelZoom(1, -1);
    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, e);
    glPixelZoom(1, 1);
    glDisable(GL_BLEND);

    delete[] (e);
  }
}

wxImage &finSAR_opsOverlayFactory::DrawGLPolygon() {
  wxString labels;
  labels = _T("");  // dummy label for drawing with

  wxColour c_orange = c_GLcolour;

  wxPen penText(c_orange);
  wxBrush backBrush(c_orange);

  wxMemoryDC mdc(wxNullBitmap);

  wxFont mfont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
               wxFONTWEIGHT_NORMAL);
  mdc.SetFont(mfont);

  int w, h;
  mdc.GetTextExtent(labels, &w, &h);

  w = 200;
  h = 200;

  wxBitmap bm(w, h);
  mdc.SelectObject(bm);
  mdc.Clear();

  mdc.SetPen(penText);
  mdc.SetBrush(backBrush);
  mdc.SetTextForeground(c_orange);
  mdc.SetTextBackground(c_orange);

  int xd = 0;
  int yd = 0;
  // mdc.DrawRoundedRectangle(xd, yd, w+(label_offset * 2), h+2, -.25);
  /*

                          wxPoint p2;
                                          p2.x = 100;
                                          p2.y = 100 ;

                          wxPoint z[9];
                                          z[0].x = p2.x;
                      z[0].y = p2.y;
                      z[1].x = p2.x;
                      z[1].y = p2.y -10 ;
                      z[2].x = p2.x + 55;
                      z[2].y = p2.y - 10;
                      z[3].x = p2.x + 55;
                      z[3].y = p2.y -25;
                                          z[4].x = p2.x + 100;
                      z[4].y = p2.y;
                      z[5].x = p2.x +55;
                      z[5].y = p2.y + 25 ;
                      z[6].x = p2.x + 55;
                      z[6].y = p2.y + 10;
                      z[7].x = p2.x;
                      z[7].y = p2.y +10;
                                          z[8].x = p2.x;
                      z[8].y = p2.y;

          */

  mdc.DrawPolygon(9, p_basic, 0);

  mdc.SelectObject(wxNullBitmap);

  m_labelCacheText[labels] = bm.ConvertToImage();

  m_labelCacheText[labels].InitAlpha();

  wxImage &image = m_labelCacheText[labels];

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

  return m_labelCacheText[labels];
  /*
                  wxPoint p;
                                  p.x = 200;
                                  p.y = 200;

                  wxPoint z[9];
                                  z[0].x = p.x;
              z[0].y = p.y;
              z[1].x = p.x;
              z[1].y = p.y -10 ;
              z[2].x = p.x + 55;
              z[2].y = p.y - 10;
              z[3].x = p.x + 55;
              z[3].y = p.y -25;
                                  z[4].x = p.x + 100;
              z[4].y = p.y;
              z[5].x = p.x +55;
              z[5].y = p.y + 25 ;
              z[6].x = p.x + 55;
              z[6].y = p.y + 10;
              z[7].x = p.x;
              z[7].y = p.y +10;
                                  z[8].x = p.x;
              z[8].y = p.y;



  */
}
void finSAR_opsOverlayFactory::drawGLPolygons(finSAR_opsOverlayFactory *pof,
                                            wxDC *dc, PlugIn_ViewPort *vp,
                                            wxImage &imageLabel, double myLat,
                                            double myLon, int offset) {
  //---------------------------------------------------------
  // Ecrit les labels
  //---------------------------------------------------------

  wxPoint ab;
  GetCanvasPixLL(vp, &ab, myLat, myLon);

  wxPoint cd;
  GetCanvasPixLL(vp, &cd, myLat, myLon);

  int w = imageLabel.GetWidth();
  int h = imageLabel.GetHeight();

  int label_offset = 0;
  int xd = (ab.x + cd.x - (w + label_offset * 2)) / 2;
  int yd = (ab.y + cd.y - h) / 2 + offset;

  if (dc) {
    /* don't use alpha for isobars, for some reason draw bitmap ignores
       the 4th argument (true or false has same result) */
    wxImage img(w, h, imageLabel.GetData(), true);
    dc->DrawBitmap(img, xd, yd, false);
  } else { /* opengl */

    int w = imageLabel.GetWidth(), h = imageLabel.GetHeight();

    unsigned char *d = imageLabel.GetData();
    unsigned char *a = imageLabel.GetAlpha();

    unsigned char mr, mg, mb;
    if (!imageLabel.GetOrFindMaskColour(&mr, &mg, &mb) && !a)
      wxMessageBox(
          _T(
                    "trying to use mask to draw a bitmap without alpha or mask\n" ));

    unsigned char *e = new unsigned char[4 * w * h];
    {
      for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
          unsigned char r, g, b;
          int off = (y * imageLabel.GetWidth() + x);
          r = d[off * 3 + 0];
          g = d[off * 3 + 1];
          b = d[off * 3 + 2];

          e[off * 4 + 0] = r;
          e[off * 4 + 1] = g;
          e[off * 4 + 2] = b;

          e[off * 4 + 3] =
              a ? a[off] : ((r == mr) && (g == mg) && (b == mb) ? 0 : 255);
        }
    }

    glColor4f(1, 1, 1, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(xd, yd);
    glPixelZoom(1, -1);
    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, e);
    glPixelZoom(1, 1);
    glDisable(GL_BLEND);

    delete[] (e);
  }
}

void finSAR_opsOverlayFactory::DrawAllDirectionsInViewPort(
    PlugIn_ViewPort *BBox) {
  // if (BBox->chart_scale > 1000000){
  // return;
  //}

  double rot_vp = BBox->rotation * 180 / M_PI;

  // Set up the scaler
  int mmx, mmy;
  wxDisplaySizeMM(&mmx, &mmy);

  int sx, sy;
  wxDisplaySize(&sx, &sy);

  double m_pix_per_mm = ((double)sx) / ((double)mmx);

  int mm_per_knot = 10;
  float current_draw_scaler = mm_per_knot * m_pix_per_mm * 100 / 100.0;

  // End setting up scaler

  double tcvalue, dir, rdir;
  bool bnew_val = true;
  double lon_last = 0.;
  tcvalue = 1;

  double myLat;
  double myLon;

  for (std::vector<DirectionTarget>::iterator it = m_dlg.d_vector.begin();
       it != m_dlg.d_vector.end(); it++) {
    myLat = (*it).m_lat;
    myLon = (*it).m_lon;
    dir = (*it).m_dir;
    rdir = dir - 180.0;

    int pixxc, pixyc;
    wxPoint cpoint;
    GetCanvasPixLL(BBox, &cpoint, myLat, myLon);
    pixxc = cpoint.x;
    pixyc = cpoint.y;

    // wxMessageBox(wxString::Format(_T("%5.2f"), myLat));
    //     Adjust drawing size using logarithmic scale
    double a1 = tcvalue * 5;
    // a1 = wxMax(5.0, a1);      // Current values less than 0.1 knot
    // will be displayed as 0
    double a2 = log10(a1);
    double scale = current_draw_scaler * a2;

    DrawDirectionArrow(pixxc, pixyc, dir - 90 + rot_vp, scale / 30, tcvalue,
                       wxColour("GREEN"));

    char sbuf[20];

    if (m_dc) {
      wxFont myFont(30, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD);

      m_dc->SetFont(myFont);
      snprintf(sbuf, 19, "%03.0f", dir);
      m_dc->DrawRotatedText(wxString(sbuf, wxConvUTF8), pixxc, pixyc, dir);
    }

    // move pix for second arrow
    double new_lat, new_lon;

    double move_dir = 360 - (((int)dir + 360) % 360);
    PositionBearingDistanceMercator_Plugin(myLat, myLon, move_dir, 0.06,
                                           &new_lat, &new_lon);

    GetCanvasPixLL(BBox, &cpoint, new_lat, new_lon);

    pixxc = cpoint.x;
    pixyc = cpoint.y;

    DrawDirectionArrow(pixxc, pixyc, rdir - 90 + rot_vp, scale / 30, tcvalue,
                       wxColour("RED"));

    double text_dir = (int)rdir % 360;

    double print_dir = (((int)text_dir + 360) % 360);

    if (m_dc) {
      wxFont myFont(30, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD);

      m_dc->SetFont(myFont);

      m_dc->SetTextForeground("RED");

      snprintf(sbuf, 19, "%03.0f", print_dir);
      m_dc->DrawRotatedText(wxString(sbuf, wxConvUTF8), pixxc, pixyc, print_dir);
    }
  }
}

bool finSAR_opsOverlayFactory::DrawDirectionArrow(int x, int y, double rot_angle,
                                                double scale, double direction,
                                                wxColour arrow_color) {
  // double m_rate = fabs(rate);

  wxColour colour;
  colour = arrow_color;

  c_GLcolour = arrow_color;  // for filling GL arrows
  if (scale <= 1e-2) return false;

  wxBrush brush(colour);

  if (m_dc) {
    wxPen pen(colour, 2);

    m_dc->SetPen(pen);
    m_dc->SetBrush(brush);
  }

  float sin_rot = sin(rot_angle * PI / 180.);
  float cos_rot = cos(rot_angle * PI / 180.);

  // Move to the first point

  float xt = DirectionArrowArray[0].x;
  float yt = DirectionArrowArray[0].y;

  float xp = (xt * cos_rot) - (yt * sin_rot);
  float yp = (xt * sin_rot) + (yt * cos_rot);
  int x1 = (int)(xp * scale);
  int y1 = (int)(yp * scale);

  p[0].x = x1 + x;
  p[0].y = y1 + y;

  p_basic[0].x = 100;
  p_basic[0].y = 100;

  // Walk thru the point list
  for (int ip = 1; ip < NUM_DIRECTION_ARROW_POINTS; ip++) {
    xt = DirectionArrowArray[ip].x;
    yt = DirectionArrowArray[ip].y;

    float xp = (xt * cos_rot) - (yt * sin_rot);
    float yp = (xt * sin_rot) + (yt * cos_rot);
    int x2 = (int)(xp * scale);
    int y2 = (int)(yp * scale);

    p_basic[ip].x = 100 + x2;
    p_basic[ip].y = 100 + y2;

    // if (m_dc) {
    //   m_dc->DrawLine(x1 + x, y1 + y, x2 + x, y2 + y);
    // }

    p[ip].x = x2 + x;
    p[ip].y = y2 + y;

    x1 = x2;
    y1 = y2;
  }

  if (m_dc) {
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

    brush.SetStyle(wxBRUSHSTYLE_SOLID);
    m_dc->SetBrush(brush);

    m_dc->DrawPolygonTessellated(3, polyPoints);
    m_dc->DrawPolygonTessellated(4, rectPoints);
  }
  return true;
}
