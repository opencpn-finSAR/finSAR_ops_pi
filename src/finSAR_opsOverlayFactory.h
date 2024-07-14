/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  finSAR_ops Plugin Freinds
 * Author:   David Register
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

#include <map>
#include <wx/string.h>
#include "pidc.h"
#include <wx/brush.h>
#include <wx/gdicmn.h>
#include "ocpn_plugin.h"
#include <wx/object.h>
#include <wx/font.h>

class piDC;

using namespace std;

#define NUM_COURSE_ARROW_POINTS 9
static wxPoint DirectionArrowArray[NUM_COURSE_ARROW_POINTS] = {
    wxPoint(0, 0),    wxPoint(0, -10), wxPoint(55, -10),
    wxPoint(55, -25), wxPoint(100, 0), wxPoint(55, 25),
    wxPoint(55, 10),  wxPoint(0, 10),  wxPoint(0, 0)};

//----------------------------------------------------------------------------------------------------------
//    finSAR_ops Overlay Specification
//----------------------------------------------------------------------------------------------------------

class finSAR_opsOverlay {
public:
  finSAR_opsOverlay(void) {
    m_iTexture = 0;
    m_pDCBitmap = NULL, m_pRGBA = NULL;
  }

  ~finSAR_opsOverlay(void) {
    if (m_iTexture)
      // glDeleteTextures( 1, &m_iTexture );
      delete m_pDCBitmap, delete[] m_pRGBA;
  }

  unsigned int m_iTexture; /* opengl mode */

  wxBitmap *m_pDCBitmap; /* dc mode */
  unsigned char *m_pRGBA;

  int m_width;
  int m_height;
};

//----------------------------------------------------------------------------------------------------------
//    finSAR_ops Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------

class finSAR_opsUIDialog;

class finSAR_opsOverlayFactory {
public:
  finSAR_opsOverlayFactory(finSAR_opsUIDialog &dlg);
  ~finSAR_opsOverlayFactory();

  void SetSettings(bool hiDefGraphics, bool GradualColors) {
    m_hiDefGraphics = hiDefGraphics;
    m_bGradualColors = GradualColors;
  }

  void SetMessage(wxString message) { m_Message = message; }
  void SetTimeZone(int TimeZone) { m_TimeZone = TimeZone; }
  void SetParentSize(int w, int h) {
    m_ParentSize.SetWidth(w);
    m_ParentSize.SetHeight(h);
  }
  void setData(double lat1, double lon1);
  bool RenderOverlay(piDC &dc, PlugIn_ViewPort &vp);
  /*
  void DrawCurrentIndicators(PlugIn_ViewPort *BBox, bool bRebuildSelList,
                             bool bforce_redraw_currents,
                             bool bdraw_mono_for_mask, wxDateTime myTime);
 */
  void Reset();
  wxImage &DrawGLText(double value, int precision);
  wxImage &DrawGLTextDir(double value, int precision);
  wxImage &DrawGLTextString(wxString myText);
  wxImage &DrawGLPolygon();

  void DrawGLLabels(finSAR_opsOverlayFactory *pof, wxDC *dc, PlugIn_ViewPort *vp,
                    wxImage &imageLabel, double myLat, double myLon,
                    int offset);

  void drawGLPolygons(finSAR_opsOverlayFactory *pof, wxDC *dc,
                      PlugIn_ViewPort *vp, wxImage &imageLabel, double myLat,
                      double myLon, int offset);

  void DrawOLBitmap(const wxBitmap &bitmap, wxCoord x, wxCoord y, bool usemask);
  PlugIn_ViewPort *vp;
  bool m_bShowRate;
  bool m_bShowDirection;
  bool m_bShowFillColour;
  wxDateTime m_dtUseNew;

  piDC *m_dc;
  /*
  bool drawCourseArrows(int x, int y, double rot_angle, double scale,
                        double rate);
  */
  wxPoint polyPoints[7];
  wxPoint rectPoints[7];

private:
  double myLat1;
  double myLon1;
  void DrawAllLinesInViewPort(PlugIn_ViewPort *BBox);
  void DrawWptDisk(PlugIn_ViewPort *BBox);
  void DrawEBLLineInViewPort(PlugIn_ViewPort *BBox);
  void DrawBearingLineInViewPort(PlugIn_ViewPort *BBox);
  void DrawIndexTargets(PlugIn_ViewPort *BBox);

  void DrawAllDirectionsInViewPort(PlugIn_ViewPort *BBox);

  bool DrawDirectionArrow(int x, int y, double rot_angle, double scale,
                          double direction, wxColour arrow_color);
  wxPoint p[12];

  // bool DoRenderfinSAR_opsOverlay(PlugIn_ViewPort *vp);
  void DrawGLLine(double x1, double y1, double x2, double y2, double width,
                  wxColour myColour);
  void DrawMessageWindow(wxString msg, int x, int y, wxFont *mfont);

  double m_last_vp_scale;

  //  for GL
  wxColour c_GLcolour;
  wxPoint p_basic[9];
  //
  wxString m_Message;
  wxString m_Message_Hiden;
  int m_TimeZone;
  wxSize m_ParentSize;

  wxDC *m_pdc;
  wxGraphicsContext *m_gdc;

  wxFont *m_dFont_map;
  wxFont *m_dFont_war;

  bool m_hiDefGraphics;
  bool m_bGradualColors;

  std::map<double, wxImage> m_labelCache;
  std::map<wxString, wxImage> m_labelCacheText;

  finSAR_opsUIDialog &m_dlg;
};
