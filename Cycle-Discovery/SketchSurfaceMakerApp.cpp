/***************************************************************
 * Name:      SketchSurfaceMakerApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Yixin Zhuang (yixin.zhuang@gmail.com)
 * Created:   2012-12-04
 * Copyright: Yixin Zhuang (none)
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "SketchSurfaceMakerApp.h"

//(*AppHeaders
#include "SketchSurfaceMakerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(SketchSurfaceMakerApp);

bool SketchSurfaceMakerApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
//	_CrtDumpMemoryLeaks();
   if ( wxsOK )
    {
   		SketchSurfaceMakerFrame* Frame = new SketchSurfaceMakerFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
/*
		wxFrame* FrameText = new wxFrame;
		FrameText->Show();
		wxTextCtrl *control = new wxTextCtrl;
		control->Create(FrameText,0);
		*control << 123.456 << " some text\n";
*/

    }
    //*)
    return wxsOK;

}
