#pragma once
#include "wx/wxprec.h"
#include "wx/app.h"
#include "wx/defs.h"
#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/splitter.h"
#include "wx/glcanvas.h"
#include "wx/msw/glcanvas.h"
#include <GL/glu.h>
#include <gl/glut.h>
#include <vector>
#include "colormap.h"
#include "amlVec.h"
#include <time.h>

class cycleUtils;//in charge of curve data and processing;

class SketchGLCanvas : public wxGLCanvas
{
public:

	SketchGLCanvas(	wxWindow *parent, wxWindowID id,
				const wxPoint& pos,
				const wxSize& size, long style,
				const wxString& name, int *attribList);

	~SketchGLCanvas ();

	void Initialize();

	void Render();

	void OnPaint ( wxPaintEvent &event );
	void OnMouse ( wxMouseEvent &event );
	void OnIdle ( wxIdleEvent &event );
	void OnSize ( wxSizeEvent &event );
//	void OnKeyDown(wxKeyEvent &event);
/*
	void OnKeyDown ( wxKeyEvent &event );
*/
	cycleUtils** getCycleUtils(){return &m_cycleUtils;}
	void screenShot();
	void setRotate(){ m_isRotate = !m_isRotate;}
	void setViewPort();
	void setRotateTimes(int val);

private:

	/************************************************************************/
	/*                         data structure                               */
	/************************************************************************/

	/*
		for data accessing
	*/
	cycleUtils* m_cycleUtils;

	/*
		for opengl
	*/
	bool m_initialized;

	wxCoord m_lastx;
	wxCoord m_lasty;

	int m_width;
	int m_height;

	int m_frameno;
	unsigned int m_rotationCount;
	unsigned int m_rotationTimes;

	//matrix4 m_RotationMatrix; // NO_MATRIX
	double m_RotationMatrix[16];

	AML::double3 m_Eye;

	AML::Scalar m_PerspectiveAngleDegrees;
	AML::Scalar m_NearPlane;
	AML::Scalar m_FarPlaneOffset;

	bool m_isRotate;
	clock_t m_frameRate;
	clock_t m_timePrev;
	clock_t m_timeCurr;
	int m_frames;
	int m_shotNum;
	wxStatusBar *m_statusbar;

	std::vector<AML::double3> computeRay(int mouseX,int mouseY);
	void pickArc(int mouseX, int mouseY,enum OperationType oT);
	void pickCycle(int mouseX, int mouseY,enum OperationType oT);
	void rotateModel();

	DECLARE_EVENT_TABLE()
};
