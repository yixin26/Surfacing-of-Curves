/***************************************************************
 * Name:      SketchSurfaceMakerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Yixin Zhuang (yixin.zhuang@gmail.com)
 * Created:   2012-12-04
 * Copyright: Yixin Zhuang (none)
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "SketchSurfaceMakerMain.h"
#include <wx/msgdlg.h>
#include <wx/colordlg.h>

//(*InternalHeaders(SketchSurfaceMakerFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <cstring>
#include "SketchGLCanvas.h"
#include "fileIOHelper.h"
#include "cycleUtils.h"
#include "HyperGraph.h"
#include "time.h"

using namespace std;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(SketchSurfaceMakerFrame)
const long SketchSurfaceMakerFrame::ID_BUTTONLOADFILE = wxNewId();
const long SketchSurfaceMakerFrame::ID_BUTTONSAVEFILE = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICLINE2 = wxNewId();
const long SketchSurfaceMakerFrame::ID_CYCLECONSTRAINT = wxNewId();
const long SketchSurfaceMakerFrame::ID_BUTTONCAPACITY = wxNewId();
const long SketchSurfaceMakerFrame::ID_BUTTONPARAMETERS = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICLINE1 = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWCURVENETWORK = wxNewId();
const long SketchSurfaceMakerFrame::ID_RADIOBOXCURVEMODE = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICTEXT2 = wxNewId();
const long SketchSurfaceMakerFrame::ID_CURVELINESIZE = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWCAPACITY = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWCONSTRAINT = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWCYCLES = wxNewId();
const long SketchSurfaceMakerFrame::ID_RADIOBOXCYCLEMODE = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICTEXTCYCLESIZE = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWCYCLESIZE = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICTEXT1 = wxNewId();
const long SketchSurfaceMakerFrame::ID_LINESIZE = wxNewId();
const long SketchSurfaceMakerFrame::ID_CYCLERANDOMCOLOR = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWSINGLECYCLE = wxNewId();
const long SketchSurfaceMakerFrame::ID_CHECKBOX1 = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICTEXT3 = wxNewId();
const long SketchSurfaceMakerFrame::ID_TEXTCTRL1 = wxNewId();
const long SketchSurfaceMakerFrame::ID_BUTTON1 = wxNewId();
const long SketchSurfaceMakerFrame::ID_SHOWSURFACE = wxNewId();
const long SketchSurfaceMakerFrame::ID_SURFACEMODE = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATICLINE3 = wxNewId();
const long SketchSurfaceMakerFrame::ID_GLCANVAS = wxNewId();
const long SketchSurfaceMakerFrame::idMenuCurveFile = wxNewId();
const long SketchSurfaceMakerFrame::ID_MENUITEM2 = wxNewId();
const long SketchSurfaceMakerFrame::idMenuSaveModifiedFile = wxNewId();
const long SketchSurfaceMakerFrame::ID_MENUITEM6 = wxNewId();
const long SketchSurfaceMakerFrame::idMenuSaveParameters = wxNewId();
const long SketchSurfaceMakerFrame::ID_MENUITEM3 = wxNewId();
const long SketchSurfaceMakerFrame::idMenuQuit = wxNewId();
const long SketchSurfaceMakerFrame::idMenuResetVisualization = wxNewId();
const long SketchSurfaceMakerFrame::idMenuResetAll = wxNewId();
const long SketchSurfaceMakerFrame::idMenuRandomRotationGraph = wxNewId();
const long SketchSurfaceMakerFrame::idMenuRotationGraph = wxNewId();
const long SketchSurfaceMakerFrame::idMenuRotationGraphExtended = wxNewId();
const long SketchSurfaceMakerFrame::idMenuRotationGraphSuperGraph = wxNewId();
const long SketchSurfaceMakerFrame::idMenuRotationGraphSuperGraphEx = wxNewId();
const long SketchSurfaceMakerFrame::ID_MENUITEM1 = wxNewId();
const long SketchSurfaceMakerFrame::idMenuCycleDiscovery = wxNewId();
const long SketchSurfaceMakerFrame::idMenuCycleBreaking = wxNewId();
const long SketchSurfaceMakerFrame::idMenuTriangulation = wxNewId();
const long SketchSurfaceMakerFrame::idMenuAbout = wxNewId();
const long SketchSurfaceMakerFrame::ID_STATUSBARMAIN = wxNewId();
//*)
const long SketchSurfaceMakerFrame::ID_CAPACITY_ONE = wxNewId();
const long SketchSurfaceMakerFrame::ID_CAPACITY_TWO = wxNewId();
const long SketchSurfaceMakerFrame::ID_CAPACITY_THREE = wxNewId();
const long SketchSurfaceMakerFrame::ID_CAPACITY_FOUR = wxNewId();
const long SketchSurfaceMakerFrame::ID_CAPACITY_FIVE = wxNewId();
const long SketchSurfaceMakerFrame::ID_CAPACITY_SIX = wxNewId();

BEGIN_EVENT_TABLE(SketchSurfaceMakerFrame,wxFrame)
    //(*EventTable(SketchSurfaceMakerFrame)
	EVT_IDLE( SketchSurfaceMakerFrame::OnIdle )
    //*)
END_EVENT_TABLE()

SketchSurfaceMakerFrame::SketchSurfaceMakerFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(SketchSurfaceMakerFrame)
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer16;
    wxFlexGridSizer* FlexGridSizer19;
    wxFlexGridSizer* FlexGridSizer23;
    wxMenuItem* MenuItem2;
    wxFlexGridSizer* FlexGridSizer10;
    wxFlexGridSizer* FlexGridSizer3;
    wxMenuItem* MenuItem1;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer22;
    wxFlexGridSizer* FlexGridSizer9;
    wxFlexGridSizer* FlexGridSizer2;
    wxMenu* Menu1;
    wxFlexGridSizer* FlexGridSizer7;
    wxFlexGridSizer* FlexGridSizer15;
    wxFlexGridSizer* FlexGridSizer18;
    wxFlexGridSizer* FlexGridSizer8;
    wxMenuItem* MenuItem6;
    wxFlexGridSizer* FlexGridSizer14;
    wxFlexGridSizer* FlexGridSizer20;
    wxFlexGridSizer* FlexGridSizer13;
    wxFlexGridSizer* FlexGridSizer12;
    wxFlexGridSizer* FlexGridSizer6;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer11;
    wxFlexGridSizer* FlexGridSizer17;
    wxMenu* Menu2;
    
    Create(parent, wxID_ANY, _("SketchSurfaceMaker"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxWANTS_CHARS, _T("wxID_ANY"));
    SetClientSize(wxSize(1200,1100));
    Move(wxPoint(50,0));
    SetForegroundColour(wxColour(0,0,0));
    SetBackgroundColour(wxColour(255,255,255));
    FlexGridSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(1);
    FlexGridSizer1->AddGrowableRow(2);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer7->Add(39,33,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer2->Add(FlexGridSizer7, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    FlexGridSizer6 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2->Add(FlexGridSizer6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
    FlexGridSizer3 = new wxFlexGridSizer(1, 4, 0, 0);
    FlexGridSizer3->AddGrowableCol(2);
    FlexGridSizer3->AddGrowableRow(1);
    FlexGridSizer14 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer14->Add(20,20,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(FlexGridSizer14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer8 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer11 = new wxFlexGridSizer(0, 3, 0, 0);
    loadFile = new wxButton(this, ID_BUTTONLOADFILE, _("&Load"), wxDefaultPosition, wxSize(55,24), wxNO_BORDER, wxDefaultValidator, _T("ID_BUTTONLOADFILE"));
    loadFile->SetToolTip(_("press Ctrl + L"));
    loadFile->SetHelpText(_("press Ctrl + L"));
    FlexGridSizer11->Add(loadFile, 1, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer11->Add(7,20,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    saveFile = new wxButton(this, ID_BUTTONSAVEFILE, _("&Save"), wxDefaultPosition, wxSize(55,24), 0, wxDefaultValidator, _T("ID_BUTTONSAVEFILE"));
    saveFile->Disable();
    saveFile->SetToolTip(_("press Ctrl + S"));
    saveFile->SetHelpText(_("press Ctrl + S"));
    FlexGridSizer11->Add(saveFile, 1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer8->Add(FlexGridSizer11, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(130,2), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
    FlexGridSizer8->Add(StaticLine2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer10 = new wxFlexGridSizer(0, 1, 0, 0);
    cycleConstraint = new wxButton(this, ID_CYCLECONSTRAINT, _("&Constraint"), wxDefaultPosition, wxSize(100,24), 0, wxDefaultValidator, _T("ID_CYCLECONSTRAINT"));
    cycleConstraint->Disable();
    cycleConstraint->SetToolTip(_("press Ctrl + C"));
    cycleConstraint->SetHelpText(_("press Ctrl + C"));
    FlexGridSizer10->Add(cycleConstraint, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, wxDLG_UNIT(this,wxSize(12,0)).GetWidth());
    capacityBtn = new wxButton(this, ID_BUTTONCAPACITY, _("C&Apacity"), wxDefaultPosition, wxSize(100,24), 0, wxDefaultValidator, _T("ID_BUTTONCAPACITY"));
    capacityBtn->Disable();
    capacityBtn->SetToolTip(_("press Ctrl + A"));
    capacityBtn->SetHelpText(_("press Ctrl + A"));
    FlexGridSizer10->Add(capacityBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer10->Add(0,4,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    parameters = new wxButton(this, ID_BUTTONPARAMETERS, _("&Parameters"), wxDefaultPosition, wxSize(100,24), 0, wxDefaultValidator, _T("ID_BUTTONPARAMETERS"));
    parameters->Disable();
    parameters->SetToolTip(_("press Ctrl + P"));
    parameters->SetHelpText(_("press Ctrl + P"));
    FlexGridSizer10->Add(parameters, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, wxDLG_UNIT(this,wxSize(4,0)).GetWidth());
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(130,2), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    FlexGridSizer10->Add(StaticLine1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer8->Add(FlexGridSizer10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer15 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer17 = new wxFlexGridSizer(0, 1, 0, 0);
    showCurveNetwork = new wxCheckBox(this, ID_SHOWCURVENETWORK, _("&Arcs"), wxDefaultPosition, wxSize(90,14), 0, wxDefaultValidator, _T("ID_SHOWCURVENETWORK"));
    showCurveNetwork->SetValue(true);
    showCurveNetwork->Disable();
    showCurveNetwork->SetToolTip(_("press A"));
    showCurveNetwork->SetHelpText(_("press A"));
    FlexGridSizer17->Add(showCurveNetwork, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    wxString __wxRadioBoxChoices_1[2] = 
    {
    	_("Solid"),
    	_("Dashed")
    };
    showCurveMode = new wxRadioBox(this, ID_RADIOBOXCURVEMODE, wxEmptyString, wxDefaultPosition, wxSize(120,64), 2, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOXCURVEMODE"));
    showCurveMode->Disable();
    showCurveMode->SetToolTip(_("press shift+A"));
    showCurveMode->SetHelpText(_("press shift+A"));
    FlexGridSizer17->Add(showCurveMode, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer12 = new wxFlexGridSizer(0, 2, 0, 0);
    curveLineSizeTex = new wxStaticText(this, ID_STATICTEXT2, _("Line Size"), wxDefaultPosition, wxSize(60,14), 0, _T("ID_STATICTEXT2"));
    curveLineSizeTex->Disable();
    FlexGridSizer12->Add(curveLineSizeTex, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    curveLineSize = new wxSpinButton(this, ID_CURVELINESIZE, wxDefaultPosition, wxSize(57,16), wxSP_HORIZONTAL|wxSP_ARROW_KEYS, _T("ID_CURVELINESIZE"));
    curveLineSize->SetValue(1);
    curveLineSize->SetRange(1, 10);
    curveLineSize->Disable();
    curveLineSize->SetToolTip(_("press Left/Right or 1/2"));
    curveLineSize->SetHelpText(_("press Left/Right or 1/2"));
    FlexGridSizer12->Add(curveLineSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer17->Add(FlexGridSizer12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    showCapacity = new wxCheckBox(this, ID_SHOWCAPACITY, _("Show Capacity"), wxDefaultPosition, wxSize(91,13), 0, wxDefaultValidator, _T("ID_SHOWCAPACITY"));
    showCapacity->SetValue(false);
    showCapacity->Disable();
    FlexGridSizer17->Add(showCapacity, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    showConstraint = new wxCheckBox(this, ID_SHOWCONSTRAINT, _("Show Constraint"), wxDefaultPosition, wxSize(98,13), 0, wxDefaultValidator, _T("ID_SHOWCONSTRAINT"));
    showConstraint->SetValue(false);
    showConstraint->Disable();
    FlexGridSizer17->Add(showConstraint, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer15->Add(FlexGridSizer17, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer18 = new wxFlexGridSizer(0, 1, 0, 0);
    showCycles = new wxCheckBox(this, ID_SHOWCYCLES, _("&Cycles"), wxDefaultPosition, wxSize(114,14), 0, wxDefaultValidator, _T("ID_SHOWCYCLES"));
    showCycles->SetValue(true);
    showCycles->Disable();
    showCycles->SetToolTip(_("press C"));
    showCycles->SetHelpText(_("press C"));
    FlexGridSizer18->Add(showCycles, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    wxString __wxRadioBoxChoices_2[2] = 
    {
    	_("Random color"),
    	_("Color by cost")
    };
    showCycleMode = new wxRadioBox(this, ID_RADIOBOXCYCLEMODE, wxEmptyString, wxDefaultPosition, wxSize(120,63), 2, __wxRadioBoxChoices_2, 1, 0, wxDefaultValidator, _T("ID_RADIOBOXCYCLEMODE"));
    showCycleMode->Disable();
    showCycleMode->SetToolTip(_("press shift+C"));
    showCycleMode->SetHelpText(_("press shift+C"));
    FlexGridSizer18->Add(showCycleMode, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer16 = new wxFlexGridSizer(0, 2, 0, 0);
    cycleSizeText = new wxStaticText(this, ID_STATICTEXTCYCLESIZE, _("Cycles Size"), wxDefaultPosition, wxSize(60,14), 0, _T("ID_STATICTEXTCYCLESIZE"));
    cycleSizeText->Disable();
    FlexGridSizer16->Add(cycleSizeText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    showCycleSize = new wxSpinButton(this, ID_SHOWCYCLESIZE, wxDefaultPosition, wxSize(57,16), wxSP_HORIZONTAL|wxSP_ARROW_KEYS, _T("ID_SHOWCYCLESIZE"));
    showCycleSize->SetValue(9);
    showCycleSize->SetRange(1, 10);
    showCycleSize->Disable();
    showCycleSize->SetToolTip(_("press Left/Right or 1/2"));
    showCycleSize->SetHelpText(_("press Left/Right or 1/2"));
    FlexGridSizer16->Add(showCycleSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer18->Add(FlexGridSizer16, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer20 = new wxFlexGridSizer(0, 2, 0, 0);
    lineSizeText = new wxStaticText(this, ID_STATICTEXT1, _("Line Size"), wxDefaultPosition, wxSize(60,14), 0, _T("ID_STATICTEXT1"));
    lineSizeText->Disable();
    FlexGridSizer20->Add(lineSizeText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    lineSize = new wxSpinButton(this, ID_LINESIZE, wxDefaultPosition, wxSize(57,16), wxSP_HORIZONTAL|wxSP_ARROW_KEYS, _T("ID_LINESIZE"));
    lineSize->SetValue(2);
    lineSize->SetRange(1, 10);
    lineSize->Disable();
    lineSize->SetToolTip(_("prese Up/Down or 3/4"));
    lineSize->SetHelpText(_("prese Up/Down or 3/4"));
    FlexGridSizer20->Add(lineSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer18->Add(FlexGridSizer20, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    cycleRandomColor = new wxButton(this, ID_CYCLERANDOMCOLOR, _("Random Color"), wxDefaultPosition, wxSize(117,20), 0, wxDefaultValidator, _T("ID_CYCLERANDOMCOLOR"));
    cycleRandomColor->Disable();
    FlexGridSizer18->Add(cycleRandomColor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    showSingleCycle = new wxCheckBox(this, ID_SHOWSINGLECYCLE, _("Si&ngle Cycle"), wxDefaultPosition, wxSize(103,14), 0, wxDefaultValidator, _T("ID_SHOWSINGLECYCLE"));
    showSingleCycle->SetValue(false);
    showSingleCycle->Disable();
    showSingleCycle->SetToolTip(_("press V"));
    showSingleCycle->SetHelpText(_("press V"));
    FlexGridSizer18->Add(showSingleCycle, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Cycle subdivision"));
    FlexGridSizer22 = new wxFlexGridSizer(0, 1, 0, 0);
    showNewCycles = new wxCheckBox(this, ID_CHECKBOX1, _("Show new cycles"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    showNewCycles->SetValue(false);
    FlexGridSizer22->Add(showNewCycles, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer23 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT3, _("Thres:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer23->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    cycleSubdivideParam_Curvature = new wxTextCtrl(this, ID_TEXTCTRL1, _("0.1"), wxDefaultPosition, wxSize(35,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    FlexGridSizer23->Add(cycleSubdivideParam_Curvature, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Button1 = new wxButton(this, ID_BUTTON1, _("Do"), wxDefaultPosition, wxSize(38,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer23->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer22->Add(FlexGridSizer23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    StaticBoxSizer1->Add(FlexGridSizer22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer18->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer15->Add(FlexGridSizer18, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer19 = new wxFlexGridSizer(0, 1, 0, 0);
    showSurface = new wxCheckBox(this, ID_SHOWSURFACE, _("&Surfaces"), wxDefaultPosition, wxSize(103,14), 0, wxDefaultValidator, _T("ID_SHOWSURFACE"));
    showSurface->SetValue(false);
    showSurface->Disable();
    showSurface->SetToolTip(_("press S"));
    showSurface->SetHelpText(_("press S"));
    FlexGridSizer19->Add(showSurface, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    wxString __wxRadioBoxChoices_3[2] = 
    {
    	_("Wireframe"),
    	_("Solid")
    };
    showSurfaceMode = new wxRadioBox(this, ID_SURFACEMODE, wxEmptyString, wxDefaultPosition, wxSize(120,63), 2, __wxRadioBoxChoices_3, 1, 0, wxDefaultValidator, _T("ID_SURFACEMODE"));
    showSurfaceMode->SetSelection(1);
    showSurfaceMode->Disable();
    showSurfaceMode->SetToolTip(_("press shift + S"));
    showSurfaceMode->SetHelpText(_("press shift + S"));
    FlexGridSizer19->Add(showSurfaceMode, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer15->Add(FlexGridSizer19, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer8->Add(FlexGridSizer15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(135,2), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
    FlexGridSizer8->Add(StaticLine3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(FlexGridSizer8, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    FlexGridSizer9 = new wxFlexGridSizer(1, 1, 0, 0);
    FlexGridSizer9->AddGrowableCol(1);
    FlexGridSizer9->AddGrowableRow(1);
    FlexGridSizer9->Add(0,15,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    int GLCanvasAttributes_1[] = {
    	WX_GL_RGBA,
    	WX_GL_DOUBLEBUFFER,
    	WX_GL_DEPTH_SIZE,      16,
    	WX_GL_STENCIL_SIZE,    0,
    	0, 0 };
    m_openGLView = new SketchGLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxSize(1000,700), wxSIMPLE_BORDER|wxWANTS_CHARS, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
    FlexGridSizer9->Add(m_openGLView, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(FlexGridSizer9, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer5->Add(34,20,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer13 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer13->Add(86,47,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    mainMenu = new wxMenuBar();
    Menu1 = new wxMenu();
    Menu3 = new wxMenu();
    MenuItemOpenCurve = new wxMenuItem(Menu3, idMenuCurveFile, _("Curve File"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItemOpenCurve);
    Menu1->Append(ID_MENUITEM2, _("Open"), Menu3, wxEmptyString);
    MenuItem4 = new wxMenu();
    MenuItemSaveModifiedFile = new wxMenuItem(MenuItem4, idMenuSaveModifiedFile, _("New Curve"), wxEmptyString, wxITEM_NORMAL);
    MenuItem4->Append(MenuItemSaveModifiedFile);
    MenuItem9 = new wxMenuItem(MenuItem4, ID_MENUITEM6, _("New Capacity"), wxEmptyString, wxITEM_NORMAL);
    MenuItem4->Append(MenuItem9);
    MenuItem6 = new wxMenuItem(MenuItem4, idMenuSaveParameters, _("Algorithm Parameters"), wxEmptyString, wxITEM_NORMAL);
    MenuItem4->Append(MenuItem6);
    Menu1->Append(ID_MENUITEM3, _("Save"), MenuItem4, wxEmptyString);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    mainMenu->Append(Menu1, _("&File"));
    MenuItem7 = new wxMenu();
    MenuItemResetVisualization = new wxMenuItem(MenuItem7, idMenuResetVisualization, _("Reset Visualization"), wxEmptyString, wxITEM_NORMAL);
    MenuItem7->Append(MenuItemResetVisualization);
    MenuItemResetAll = new wxMenuItem(MenuItem7, idMenuResetAll, _("Reset ALL"), wxEmptyString, wxITEM_NORMAL);
    MenuItem7->Append(MenuItemResetAll);
    mainMenu->Append(MenuItem7, _("&Reset"));
    MenuItem5 = new wxMenu();
    MenuItem3 = new wxMenu();
    MenuItemRandomRotationGraph = new wxMenuItem(MenuItem3, idMenuRandomRotationGraph, _("Random Rotation Graph"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->Append(MenuItemRandomRotationGraph);
    MenuItemRotationGraph = new wxMenuItem(MenuItem3, idMenuRotationGraph, _("Angel"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->Append(MenuItemRotationGraph);
    MenuItemRotationGraphExtended = new wxMenuItem(MenuItem3, idMenuRotationGraphExtended, _("Angle + Dihedral"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->Append(MenuItemRotationGraphExtended);
    MenuItemRotationGraphSuperGraph = new wxMenuItem(MenuItem3, idMenuRotationGraphSuperGraph, _("Global Optimization"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->Append(MenuItemRotationGraphSuperGraph);
    MenuItemRotationGraphSuperGraphEx = new wxMenuItem(MenuItem3, idMenuRotationGraphSuperGraphEx, _("Almost Global Optimization"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->Append(MenuItemRotationGraphSuperGraphEx);
    MenuItem5->Append(ID_MENUITEM1, _("Rotation Graph"), MenuItem3, wxEmptyString);
    MenuItemCycleDiscovery = new wxMenuItem(MenuItem5, idMenuCycleDiscovery, _("Cycle construction"), wxEmptyString, wxITEM_NORMAL);
    MenuItem5->Append(MenuItemCycleDiscovery);
    MenuItemCycleBreaking = new wxMenuItem(MenuItem5, idMenuCycleBreaking, _("Cycle Breaking"), wxEmptyString, wxITEM_NORMAL);
    MenuItem5->Append(MenuItemCycleBreaking);
    MenuItemTriangulation = new wxMenuItem(MenuItem5, idMenuTriangulation, _("Triangulation"), wxEmptyString, wxITEM_NORMAL);
    MenuItem5->Append(MenuItemTriangulation);
    mainMenu->Append(MenuItem5, _("&Compute"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("Show Rotation Graph"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    mainMenu->Append(Menu2, _("Instruction"));
    SetMenuBar(mainMenu);
    SetMenuBar(NULL);
    statusBarMain = new wxStatusBar(this, ID_STATUSBARMAIN, wxSIMPLE_BORDER|wxDOUBLE_BORDER, _T("ID_STATUSBARMAIN"));
    int __wxStatusBarWidths_1[4] = { -15, -15, -15, -50 };
    int __wxStatusBarStyles_1[4] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL };
    statusBarMain->SetFieldsCount(4,__wxStatusBarWidths_1);
    statusBarMain->SetStatusStyles(4,__wxStatusBarStyles_1);
    SetStatusBar(statusBarMain);
    FlexGridSizer1->SetSizeHints(this);
    
    Connect(ID_BUTTONLOADFILE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnOpenCurveFile);
    Connect(ID_BUTTONSAVEFILE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnSaveAll);
    Connect(ID_CYCLECONSTRAINT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnCycleConstraintClick);
    Connect(ID_BUTTONCAPACITY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnCapacityBtnClick);
    Connect(ID_BUTTONPARAMETERS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnParametersClick);
    Connect(ID_SHOWCURVENETWORK,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowCurveNetworkClick);
    Connect(ID_RADIOBOXCURVEMODE,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowCurveModeSelect);
    Connect(ID_CURVELINESIZE,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OncurveLineSizeChange);
    Connect(ID_SHOWCAPACITY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnshowCapacityClick);
    Connect(ID_SHOWCONSTRAINT,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnshowConstraintClick);
    Connect(ID_SHOWCYCLES,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowCyclesClick);
    Connect(ID_RADIOBOXCYCLEMODE,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowCycleModeSelect);
    Connect(ID_SHOWCYCLESIZE,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowCycleSizeChange);
    Connect(ID_LINESIZE,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnlineSizeChange);
    Connect(ID_CYCLERANDOMCOLOR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnCycleRandomColorClick);
    Connect(ID_SHOWSINGLECYCLE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowSingleCycleSelect);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowNewCyclesClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnSubdivideCycles);
    Connect(ID_SHOWSURFACE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnShowSurfaceClick);
    Connect(ID_SURFACEMODE,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnSurfaceModeSelect);
    m_openGLView->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnGLViewKeyDown,0,this);
    m_openGLView->Connect(wxEVT_KEY_UP,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnGLViewKeyUp,0,this);
    m_openGLView->Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnGLViewLeftDClick,0,this);
    m_openGLView->Connect(wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnGLViewMouseWheel,0,this);
    Connect(idMenuCurveFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnOpenCurveFile);
    Connect(idMenuSaveModifiedFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnSaveNewCurve);
    Connect(ID_MENUITEM6,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnSaveCurveWithNewCapacity);
    Connect(idMenuSaveParameters,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnSaveParameters);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnQuit);
    Connect(idMenuResetVisualization,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnResetVisualization);
    Connect(idMenuResetAll,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnResetAll);
    Connect(idMenuRandomRotationGraph,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnMenuItemRandomRotationGraph);
    Connect(idMenuRotationGraph,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeRotationGraph);
    Connect(idMenuRotationGraphExtended,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeRotationGraphExtended);
    Connect(idMenuRotationGraphSuperGraph,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeRotationGraphSuperGraph);
    Connect(idMenuRotationGraphSuperGraphEx,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeRotationGraphSuperGraphEx);
    Connect(idMenuCycleDiscovery,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeCycleSet);
    Connect(idMenuCycleBreaking,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeCycleBreaking);
    Connect(idMenuTriangulation,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnComputeTriangulation);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnAbout);
    Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnKeyDown);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&SketchSurfaceMakerFrame::OnResize);
    //*)


	m_cycleUtils = new cycleUtils();
	*(m_cycleUtils->getStatusBar()) = statusBarMain;
	*(m_cycleUtils->m_cycleDiscovery.getStatusBar()) = statusBarMain;
	*(m_openGLView->getCycleUtils()) = m_cycleUtils;
	PushBackAll();
	initStatus = allStatus;
	preShowMode = NORMAL;
//	_CrtDumpMemoryLeaks();

}

SketchSurfaceMakerFrame::~SketchSurfaceMakerFrame()
{
    //(*Destroy(SketchSurfaceMakerFrame)
    //*)
}

void SketchSurfaceMakerFrame::OnAbout(wxCommandEvent& event)
{
/*
    wxString msg = "1: load the file\n2: compute the Rotation Graph\n3: Find Cycles";//wxbuildinfo(long_f);
    wxMessageBox(msg, _("Instruction"));
*/
}

void SketchSurfaceMakerFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void SketchSurfaceMakerFrame::OnOpenCurveFile(wxCommandEvent& event)
{
	if(!loadFile->IsEnabled()|| !loadFile->IsShown()) return;
	FileIOHelper* fileIOHelper = new FileIOHelper();

	wxString fileName = wxFileSelector(wxT("Open Curve File"), (const wxChar *) NULL,
		(const wxChar *) NULL, wxT("CURVE"),
		wxT("curve files (*.curve)|*.curve|contour files (*.contour)|*.contour"),wxOPEN);//|maya files (*.ma)|*.ma|crv files (*.crv)|*.crv|all files (*.*)|*.*

// 	fileIOHelper->ProcessContourData(fileName.mb_str());
// 	return;

	if(strlen(fileName)==0)
	{
		return;
	}

	OnResetAll(event);

	m_cycleUtils->m_filename = fileName;

	fileIOHelper->ReadCurve(fileName.mb_str(), m_cycleUtils->m_cycleDiscovery.m_curves, m_cycleUtils->m_cycleDiscovery.m_curveCapacitys);

	if (m_cycleUtils->m_cycleDiscovery.m_curves.empty())
	{
		wxMessageBox( _("File reading fails!"),_("Warn"),wxOK | wxICON_INFORMATION, this );
		OnResetAll(event);
		return;
	}

	m_cycleUtils->m_cycleDiscovery.constructNetwork();

	m_openGLView->Render();

	//check if mesh is available,then read it;
	char FileName[200];
	strcpy(FileName,fileName.mb_str());
	for(int i=strlen(FileName)-1;i>=0;i--)
	{
		if(FileName[i] == '.'){
			FileName[i]='\0';
			break;
		}
	}
	char tfi[400];   // my change
	strcpy(tfi,FileName);
	strcat(tfi,".cycle");
	strcat(FileName,".mesh");

	bool isCycleGood, isMeshGood;
	isCycleGood = isMeshGood = true;

	std::ifstream reader;
	reader.open(tfi,ios::in|ios::binary);
	if (!reader.good())
		isCycleGood= false;

	reader.close();
	reader.open(FileName,ios::in|ios::binary);
	if (!reader.good())
		isMeshGood= false;
	reader.close();

	if(isCycleGood&&isMeshGood){
		if(wxMessageBox( _("\nCycles and Mesh are already existed!\t\t\n\n\tDo you want to Load?\n"),_("Info"),wxYES_NO | wxICON_INFORMATION )==wxYES){
			fileIOHelper->ReadCycle(tfi, m_cycleUtils->m_cycleDiscovery.m_cycleSetBreaked);
			fileIOHelper->ReadMesh(FileName, m_cycleUtils->m_triangleSurface);
			showCurveNetwork->Enable();
			showCurveMode->Enable();
			showCycles->Enable();
			showCycles->Enable();
			showCycleSize->Enable();
			cycleSizeText->Enable();
			lineSizeText->Enable();
			showSurface->Enable();
			showSurfaceMode->Enable();
			return;
		}
	}
	else if(isCycleGood&&!isMeshGood){
		if(wxMessageBox( _("\nCycles is already existed!\t\t\n\nDo you want to Load?\n"),_("Info"),wxYES_NO | wxICON_INFORMATION)==wxYES){
			fileIOHelper->ReadCycle(tfi, m_cycleUtils->m_cycleDiscovery.m_cycleSetBreaked);
			m_cycleUtils->surfaceBuilding();
			showCurveNetwork->Enable();
			showCurveMode->Enable();
			showCycles->Enable();
			showCycleSize->Enable();
			cycleSizeText->Enable();
			lineSizeText->Enable();
			lineSize->Enable();
			showSurface->Enable();
			showSurfaceMode->Enable();
			return;
		}
	}
	else if(!isCycleGood&&isMeshGood){
		if(wxMessageBox( _("\nMesh is already existed!\t\t\n\nDo you want to Load?\n"),_("Info"),wxYES_NO | wxICON_INFORMATION)==wxYES){
			fileIOHelper->ReadMesh(FileName, m_cycleUtils->m_triangleSurface);
			showCurveNetwork->Enable();
			showCurveMode->Enable();
			showSurface->Enable();
			showSurfaceMode->Enable();
			return;
		}
	}

	clock_t  time_str = clock();

	statusBarMain->SetStatusText("construct rotation graph ",0);
	m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraphEx();
	statusBarMain->SetStatusText("construct cycles ",0);
	m_cycleUtils->m_cycleDiscovery.constructCycles();
	m_cycleUtils->m_cycleDiscovery.cycleBreaking();
	statusBarMain->SetStatusText("done!",0);
	EnableAll();
	saveFile->Show(true);
	m_openGLView->Render();

	clock_t  time_end = clock();
	clock_t m_timeConsuming = time_end - time_str;
	char tbuf[100];
	_itoa(int(m_timeConsuming),tbuf,10);
	char tx1[400];
	strcpy(tx1,"Time Used: ");
	char tx2[400];
	strcpy(tx2,tbuf);
	strcpy(tbuf,tx1);
	strcat(tbuf,tx2);
	strcat(tbuf,"mm");
	statusBarMain->SetStatusText(tbuf,0);
}

void SketchSurfaceMakerFrame::OnRecomputeAllClick(wxCommandEvent& event)
{
	clock_t  time_str = clock();
	statusBarMain->SetStatusText("construct rotation graph ",0);
	m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraphEx();
	statusBarMain->SetStatusText("construct cycles ",0);
	m_cycleUtils->m_cycleDiscovery.constructCycles();
	m_cycleUtils->m_cycleDiscovery.cycleBreaking();
	statusBarMain->SetStatusText("done!",0);
	EnableAll();
	saveFile->Show(true);
	m_openGLView->Render();

	clock_t  time_end = clock();
	clock_t m_timeConsuming = time_end - time_str;
	char tbuf[100];
	_itoa(int(m_timeConsuming),tbuf,10);

	char tx1[400];
	strcpy(tx1,"Time Used: ");
	char tx2[400];
	strcpy(tx2,tbuf);
	strcpy(tbuf,tx1);
	strcat(tbuf,tx2);
	strcat(tbuf,"mm");
	statusBarMain->SetStatusText(tbuf,0);
}

void SketchSurfaceMakerFrame::OnSaveNewCurve(wxCommandEvent& event)
{
    FileIOHelper* fileIOHelper = new FileIOHelper();
	wxString fileName= m_cycleUtils->m_filename;
	char FileName[400];
	strcpy(FileName,fileName.mb_str());
	for(int i=strlen(FileName)-1;i>=0;i--){
		if(FileName[i] == '.')
		{
			FileName[i]='\0';
			break;
		}
	}
	if(strlen(FileName)==0)
		return;
	else{
		strcat(FileName,"_new");
		strcat(FileName,".curve");
		fileIOHelper->WriteCurve(FileName, m_cycleUtils->m_cycleDiscovery.m_curves, m_cycleUtils->m_cycleDiscovery.m_curveCapacitys);
	}
}
void SketchSurfaceMakerFrame::OnSaveCycles(wxCommandEvent& event)
{
	FileIOHelper* fileIOHelper = new FileIOHelper();
	wxString fileName= m_cycleUtils->m_filename;
	char FileName[400];
	strcpy(FileName,fileName.mb_str());
	for(int i=strlen(FileName)-1;i>=0;i--){
		if(FileName[i] == '.')
		{
			FileName[i]='\0';
			break;
		}
	}
	if(strlen(FileName)==0)
		return;
	else
	{
		strcat(FileName,".cycle");
		fileIOHelper->WriteCycle(FileName, m_cycleUtils->m_cycleDiscovery.m_cycleSetBreaked);
	}
}

void SketchSurfaceMakerFrame::OnSaveParameters(wxCommandEvent& event)
{
	m_cycleUtils->writeConfigurations();
}

void SketchSurfaceMakerFrame::OnSaveMesh(wxCommandEvent& event)
{
	FileIOHelper* fileIOHelper = new FileIOHelper();
	wxString fileName= m_cycleUtils->m_filename;
	char FileName[400];
	strcpy(FileName,fileName.mb_str());
	for(int i=strlen(FileName)-1;i>=0;i--){
		if(FileName[i] == '.')
		{
			FileName[i]='\0';
			break;
		}
	}
	if(strlen(FileName)==0)
		return;
	else{
		strcat(FileName,".off");
		fileIOHelper->WriteMesh(FileName, m_cycleUtils->m_triangleSurface, m_cycleUtils->m_triangleSurfaceNormal);
	}

}
void SketchSurfaceMakerFrame::OnSaveAll(wxCommandEvent& event)
{
	if(!saveFile->IsEnabled()|| !saveFile->IsShown()) return;
	statusBarMain->SetStatusText("save parameters",0);
	OnSaveParameters(event);
//	OnSaveCurveWithNewCapacity(event);
	OnSaveCycles(event);
	OnSaveMesh(event);
//	m_openGLView->screenShot();
	statusBarMain->SetStatusText("all saved",0);
}
void SketchSurfaceMakerFrame::OnSaveCurveWithNewCapacity(wxCommandEvent& event)
{
	FileIOHelper* fileIOHelper = new FileIOHelper();
	wxString fileName = m_cycleUtils->m_filename;
	char FileName[400];
	strcpy(FileName,fileName);
	for(int i=strlen(FileName)-1;i>=0;i--){
		if(FileName[i] == '.')
		{
			FileName[i]='\0';
			break;
		}
	}
	if(strlen(FileName)==0)
		return;
	else
	{
		strcat(FileName,"_withCapacity");
		strcat(FileName,".curve");
		fileIOHelper->WriteCurve(FileName, m_cycleUtils->m_cycleDiscovery.m_curves, m_cycleUtils->m_cycleDiscovery.m_curveCapacitys);
	}
}

void SketchSurfaceMakerFrame::OnMenuItemRandomRotationGraph(wxCommandEvent& event)
{
	m_cycleUtils->m_cycleDiscovery.constructRandomRotationGraph();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeRotationGraph(wxCommandEvent& event)
{
	m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyAngleMetric();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeRotationGraphExtended(wxCommandEvent& event)
{
	m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyAngleDihedralMetric();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeRotationGraphSuperGraph(wxCommandEvent& event)
{
	HyperGraph dialog(this);
    double *twistNormNum = dialog.getTwistNormNum();
    double *angleWeight = dialog.getAngleWeight();
    double *twistWeight = dialog.getTwistWeight();
    double *curveWeight = dialog.getCurveWeight();
    double *nodeWeight = dialog.getNodeWeight();
    double *bestNeightboreNum = dialog.getBestNeightboreNum();
    double *rotationGraphNum = dialog.getRotationGraphNum();
    double *dihedralWeight = dialog.getDihedralWeight();
    double *stateNum = dialog.getStateNum();
	dialog.ShowModal();

	m_cycleUtils->m_cycleDiscovery.m_twistNormNum = int(*twistNormNum);
	m_cycleUtils->m_cycleDiscovery.m_angleWeight = *angleWeight;
	m_cycleUtils->m_cycleDiscovery.m_twistWeight = *twistWeight;
	m_cycleUtils->m_cycleDiscovery.m_curveWeight = *curveWeight;
	m_cycleUtils->m_cycleDiscovery.m_nodeWeight = *nodeWeight;
	m_cycleUtils->m_cycleDiscovery.m_bestNeightboreNum = int(*bestNeightboreNum);
	m_cycleUtils->m_cycleDiscovery.m_rotationGraphNum = int(*rotationGraphNum);
	m_cycleUtils->m_cycleDiscovery.m_dihedralWeight = *dihedralWeight;
	m_cycleUtils->m_cycleDiscovery.m_stateNum = int(*stateNum);

	m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraph();
	 m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeRotationGraphSuperGraphEx(wxCommandEvent& event)
{
	HyperGraph dialog(this);
	double *twistNormNum = dialog.getTwistNormNum();
	double *angleWeight = dialog.getAngleWeight();
	double *twistWeight = dialog.getTwistWeight();
	double *curveWeight = dialog.getCurveWeight();
	double *nodeWeight = dialog.getNodeWeight();
	double *bestNeightboreNum = dialog.getBestNeightboreNum();
	double *rotationGraphNum = dialog.getRotationGraphNum();
	bool *rographConnect = dialog.getRotationGraphConnect();
	double *dihedralWeight = dialog.getDihedralWeight();
	double *stateNum = dialog.getStateNum();
	dialog.ShowModal();

	m_cycleUtils->m_cycleDiscovery.m_twistNormNum = int(*twistNormNum);
	m_cycleUtils->m_cycleDiscovery.m_angleWeight = *angleWeight;
	m_cycleUtils->m_cycleDiscovery.m_twistWeight = *twistWeight;
	m_cycleUtils->m_cycleDiscovery.m_curveWeight = *curveWeight;
	m_cycleUtils->m_cycleDiscovery.m_nodeWeight = *nodeWeight;
	m_cycleUtils->m_cycleDiscovery.m_bestNeightboreNum = int(*bestNeightboreNum);
	m_cycleUtils->m_cycleDiscovery.m_rotationGraphNum = int(*rotationGraphNum);
	m_cycleUtils->m_cycleDiscovery.m_isRoGraphConnect = *rographConnect;
	m_cycleUtils->m_cycleDiscovery.m_dihedralWeight = *dihedralWeight;
	m_cycleUtils->m_cycleDiscovery.m_stateNum = int(*stateNum);

	m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraphEx();
	 m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeCycleSet(wxCommandEvent& event)
{
	m_cycleUtils->m_cycleDiscovery.constructCycles();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeCycleBreaking(wxCommandEvent& event)
{
	m_cycleUtils->m_cycleDiscovery.cycleBreaking();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnComputeTriangulation(wxCommandEvent& event)
{
    m_cycleUtils->surfaceBuilding();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnShowCurveNetworkClick(wxCommandEvent& event)
{
	if(!showCurveNetwork->IsEnabled()|| !showCurveNetwork->IsShown()) return;
	m_cycleUtils->m_showCurveNetwork = showCurveNetwork->GetValue();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnShowSingleCycleSelect(wxCommandEvent& event)
{
	if(!showSingleCycle->IsEnabled()|| !showSingleCycle->IsShown()) return;
	m_cycleUtils->m_showSingleCycle = showSingleCycle->GetValue();
	m_openGLView->Render();
}
void SketchSurfaceMakerFrame::OnShowCyclesClick(wxCommandEvent& event)
{
	if(!showCycles->IsEnabled()|| !showCycles->IsShown()) return;
	m_cycleUtils->m_showCycles = showCycles->GetValue();
	 if(showCycles->GetValue())
		 showSingleCycle->Enable();
	 else
		 showSingleCycle->Disable();
	 m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnShowSurfaceClick(wxCommandEvent& event)
{
	if(!showSurface->IsEnabled()|| !showSurface->IsShown()) return;
	m_cycleUtils->m_showSurface = showSurface->GetValue();
	 if(m_cycleUtils->m_triangleSurface.empty()&&showSurface->GetValue())
	 {
		 statusBarMain->SetStatusText("construct mesh ");
		 clock_t  time_str = clock();
		 m_cycleUtils->surfaceBuilding();
		 clock_t  time_end = clock();
		 clock_t m_timeConsuming = time_end - time_str;
		 char tbuf[100];
		 _itoa(int(m_timeConsuming),tbuf,10);
		 char *tx1 = "Time Used: ";
		 char *tx2 = new char[strlen(tbuf)];
		 strcpy(tx2,tbuf);
		 strcpy(tbuf,tx1);
		 strcat(tbuf,tx2);
		 strcat(tbuf,"mm");
		 statusBarMain->SetStatusText(tbuf);
	 }
	 m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnShowCycleSizeChange(wxSpinEvent& event)
{
	if(!showCycleSize->IsEnabled()|| !showCycleSize->IsShown()) return;
	m_cycleUtils->m_selectedCycleSize = showCycleSize->GetValue();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OncurveLineSizeChange(wxSpinEvent& event)
{
	if(!curveLineSize->IsEnabled()|| !curveLineSize->IsShown()) return;
	m_cycleUtils->m_curveLineSize = curveLineSize->GetValue();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnlineSizeChange(wxSpinEvent& event)//cycle line size
{
	if(!lineSize->IsEnabled()|| !lineSize->IsShown()) return;
	m_cycleUtils->m_lineSize = lineSize->GetValue();
	m_openGLView->Render();
}
void SketchSurfaceMakerFrame::OnCycleRandomColorClick(wxCommandEvent& event)
{
	if(!cycleRandomColor->IsEnabled()|| !cycleRandomColor->IsShown()) return;
	m_cycleUtils->changeCycleColor();
	m_openGLView->Render();

}
void SketchSurfaceMakerFrame::OnShowCurveModeSelect(wxCommandEvent& event)
{
	if(!showCurveMode->IsEnabled()|| !showCurveMode->IsShown()) return;
	m_cycleUtils->m_showArcMode = showCurveMode->GetSelection();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnShowCycleModeSelect(wxCommandEvent& event)
{
	if(!showCurveMode->IsEnabled()|| !showCycleMode->IsShown()) return;
	m_cycleUtils->m_showCycleMode = showCycleMode->GetSelection();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnSurfaceModeSelect(wxCommandEvent& event)
{
	if(!showSurfaceMode->IsEnabled()|| !showSurfaceMode->IsShown()) return;
	m_cycleUtils->m_showSurfaceMode = showSurfaceMode->GetSelection();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnshowCapacityClick(wxCommandEvent& event)
{
	if(!showCapacity->IsEnabled()|| !showCapacity->IsShown()) return;
	m_cycleUtils->resetUpdateArcs();
	m_cycleUtils->m_showSingleCurve = showCapacity->GetValue();
	m_cycleUtils->m_showCapacity = showCapacity->GetValue();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnshowConstraintClick(wxCommandEvent& event)
{
	if(!showConstraint->IsEnabled()|| !showConstraint->IsShown()) return;
	m_cycleUtils->resetUpdateArcs();
	m_cycleUtils->m_showSingleCurve = showConstraint->GetValue();
	m_cycleUtils->m_showConstraint = showConstraint->GetValue();
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnCycleConstraintClick(wxCommandEvent& event)
{
	if(!cycleConstraint->IsEnabled()|| !cycleConstraint->IsShown()) return;
	if(cycleConstraint->GetLabel()=="&Constraint"){
		PushBackAll();
		DisableAll();
		showCycles->Enable();
		showCycleMode->Enable();
		showCycleSize->Enable();
		cycleSizeText->Enable();
		showCycles->SetValue(false);
		m_cycleUtils->m_showCyclesBreaked = false;
		showCycleMode->SetSelection(1);

		m_cycleUtils->m_showCycleMode = 1;

		showCurveNetwork->Enable();
		showCurveMode->Enable();
		showCurveNetwork->SetValue(true);
		m_cycleUtils->m_showCurveNetwork = true;
		showCurveMode->SetSelection(0);
		m_cycleUtils->m_showArcMode = 0;

		showSurface->Enable();
		showSurfaceMode->Enable();
		showSurface->SetValue(false);
		m_cycleUtils->m_showSurface = false;

		cycleConstraint->Enable();
		cycleConstraint->SetLabel("Done");
		m_cycleUtils->m_showConstraint = true;
		m_cycleUtils->m_showSingleCurve = true;
		m_cycleUtils->resetUpdateArcs();
		m_openGLView->Render();
	}
	else if(cycleConstraint->GetLabel()=="Done"){
		//the call makes other calls.
		AML::double3 temp=AML::double3(0,0,0);
		AML::double3 temp2=AML::double3(1,1,1);
		m_cycleUtils->pickArc(temp,temp2,ADD);
		cycleConstraint->SetLabel("&Constraint");

		m_cycleUtils->m_showConstraint = false;
		m_cycleUtils->m_showSingleCurve = false;

		clock_t  time_str = clock();
		m_cycleUtils->resetUpdateNodes();
		statusBarMain->SetStatusText("construct rotation graph ",0);
		m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraphEx();
		if (m_cycleUtils->m_cycleDiscovery.m_rotationGraph.empty())
			return;
		statusBarMain->SetStatusText("construct cycles ",0);
		m_cycleUtils->m_cycleDiscovery.constructCycles();
		m_cycleUtils->m_cycleDiscovery.cycleBreaking();
//		m_openGLView->Render();
		clock_t  time_end = clock();
		clock_t m_timeConsuming = time_end - time_str;
		char tbuf[100];
		_itoa(int(m_timeConsuming),tbuf,10);
		char *tx1 = "Time Used: ";
		char *tx2 = new char[strlen(tbuf)];
		strcpy(tx2,tbuf);
		strcpy(tbuf,tx1);
		strcat(tbuf,tx2);
		strcat(tbuf,"mm");
		statusBarMain->SetStatusText(tbuf,0);
		PopBackAll();
		showSurface->SetValue(false);
		m_cycleUtils->m_showSurface = false;
	}
}

void SketchSurfaceMakerFrame::OnCapacityBtnClick(wxCommandEvent& event)
{
	if(!capacityBtn->IsEnabled()|| !capacityBtn->IsShown()) return;
	if(capacityBtn->GetLabel()=="C&Apacity"){
		PushBackAll();

		DisableAll();
		m_cycleUtils->m_showCurveNetwork=true;
		m_cycleUtils->m_showArcMode=1;
		showCycles->Enable();
		showCycleMode->Enable();
		showCycleSize->Enable();
		cycleSizeText->Enable();
		showSurface->Enable();
		showSurfaceMode->Enable();
		showCycles->SetValue(false);
		m_cycleUtils->m_showCycles=(false);
		showCycleMode->SetSelection(1);
		m_cycleUtils->m_showCycleMode=(1);
		showSurface->SetValue(false);
		m_cycleUtils->m_showSurface=(false);

		capacityBtn->Enable();
		capacityBtn->SetLabel("Done");
		m_cycleUtils->m_showCapacity=(true);
		m_cycleUtils->m_showSingleCurve=(true);
		m_cycleUtils->resetUpdateArcs();
		m_openGLView->Render();
	}
	else if(capacityBtn->GetLabel()=="Done"){
		bool isRight = m_cycleUtils->updateCapacity();
		if(!isRight){
			if(wxMessageBox( _("\nInvalid Capacity Setting!\t\t\n\nDo you want to stay here?\n"),_("Warn"),wxYES_NO | wxICON_INFORMATION)==wxYES)
				return;
			else{
				m_cycleUtils->resetUpdateArcs();
				m_cycleUtils->resetUpdateNodes();
			}
		}
/*
		EnableAll();
		showCurveNetwork->SetValue(true);
		m_cycleUtils->setShowCurveNetwork(true);
		showCycleMode->SetSelection(0);
		m_cycleUtils->setShowArcMode(0);
		showCycles->SetValue(true);
		m_cycleUtils->setShowCycles(true);
		showCycleMode->SetSelection(0);
		m_cycleUtils->setShowCylceMode(0);

*/
		capacityBtn->SetLabel("C&Apacity");		m_cycleUtils->m_showCapacity=false;
		m_cycleUtils->m_showSingleCurve=false;
		if(isRight){
			clock_t  time_str = clock();
			m_cycleUtils->resetUpdateNodes();
			statusBarMain->SetStatusText("construct rotation graph ",0);
			m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraphEx();
			if (m_cycleUtils->m_cycleDiscovery.m_rotationGraph.empty())
				return;
			statusBarMain->SetStatusText("construct cycles ",0);
			m_cycleUtils->m_cycleDiscovery.constructCycles();
			m_cycleUtils->m_cycleDiscovery.cycleBreaking();
//			m_openGLView->Render();
			clock_t  time_end = clock();
			clock_t m_timeConsuming = time_end - time_str;
			char tbuf[100];
			_itoa(int(m_timeConsuming),tbuf,10);
			char *tx1 = "Time Used: ";
			char *tx2 = new char[strlen(tbuf)];
			strcpy(tx2,tbuf);
			strcpy(tbuf,tx1);
			strcat(tbuf,tx2);
			strcat(tbuf,"mm");
			statusBarMain->SetStatusText(tbuf,0);
		}
		PopBackAll();
		showSurface->SetValue(false);
		m_cycleUtils->m_showSurface = false;
	}
}

void SketchSurfaceMakerFrame::OnParametersClick(wxCommandEvent& event)
{
	if(!parameters->IsEnabled()|| !parameters->IsShown()) return;
	HyperGraph dialog(this);
	double *twistNormNum = dialog.getTwistNormNum();
	double *angleWeight = dialog.getAngleWeight();
	double *twistWeight = dialog.getTwistWeight();
	double *curveWeight = dialog.getCurveWeight();
	double *nodeWeight = dialog.getNodeWeight();
	double *bestNeightboreNum = dialog.getBestNeightboreNum();
	double *rotationGraphNum = dialog.getRotationGraphNum();
	bool *rographConnect = dialog.getRotationGraphConnect();
	double *dihedralWeight = dialog.getDihedralWeight();
	double *stateNum = dialog.getStateNum();

	bool *isCycleBreak = dialog.getCycleBreak();

	double* weightTri = dialog.getSurfaceTriWeight();
	double* weightEdge = dialog.getSurfaceEdgeWeight();
	double* weightBiTri = dialog.getSurfaceBiTriWeight();
	double* weightTriBd = dialog.getSurfaceTriBdWeight();
	double* weightWorsDih = dialog.getSurfaceWorsDihWeight();
	bool* surfaceSmooth	= dialog.getSurfaceSmooth();
	double* subdivisonSmooth=dialog.getSurfaceSubdivision();
	double* laplacianSmooth=dialog.getSurfaceLaplacian();

	*dialog.getTwistNormNum() = m_cycleUtils->m_cycleDiscovery.m_twistNormNum;
	*dialog.getAngleWeight() = m_cycleUtils->m_cycleDiscovery.m_angleWeight;
	*dialog.getTwistWeight() = m_cycleUtils->m_cycleDiscovery.m_twistWeight;
	*dialog.getCurveWeight() = m_cycleUtils->m_cycleDiscovery.m_curveWeight;
	*dialog.getNodeWeight() = m_cycleUtils->m_cycleDiscovery.m_nodeWeight;
	*dialog.getBestNeightboreNum() = m_cycleUtils->m_cycleDiscovery.m_bestNeightboreNum;
	*dialog.getRotationGraphNum() = m_cycleUtils->m_cycleDiscovery.m_rotationGraphNum;
	*dialog.getRotationGraphConnect() = m_cycleUtils->m_cycleDiscovery.m_isRoGraphConnect;
	*dialog.getDihedralWeight() = m_cycleUtils->m_cycleDiscovery.m_dihedralWeight;
	*dialog.getStateNum() = m_cycleUtils->m_cycleDiscovery.m_stateNum;
	*dialog.getCycleBreak() = m_cycleUtils->m_cycleDiscovery.m_isDoCycleBreak;

	*dialog.getSurfaceTriWeight() = m_cycleUtils->m_weightTri;
	*dialog.getSurfaceEdgeWeight() = m_cycleUtils->m_weightEdge;
	*dialog.getSurfaceBiTriWeight() = m_cycleUtils->m_weightBiTri;
	*dialog.getSurfaceTriBdWeight() = m_cycleUtils->m_weightTriBd;
	*dialog.getSurfaceWorsDihWeight() = m_cycleUtils->m_weightWorsDih;
	*dialog.getSurfaceSmooth() = m_cycleUtils->m_SurfaceSmooth;
	*dialog.getSurfaceSubdivision() = m_cycleUtils->m_subdivisonSmooth;
	*dialog.getSurfaceLaplacian() = m_cycleUtils->m_laplacianSmooth;

	dialog.setShow();

	dialog.ShowModal();
	if(dialog.isReturn==true){
		clock_t  time_str = clock();

		m_cycleUtils->m_cycleDiscovery.m_twistNormNum = int(*twistNormNum);
		m_cycleUtils->m_cycleDiscovery.m_angleWeight = *angleWeight;
		m_cycleUtils->m_cycleDiscovery.m_twistWeight = *twistWeight;
		m_cycleUtils->m_cycleDiscovery.m_curveWeight = *curveWeight;
		m_cycleUtils->m_cycleDiscovery.m_nodeWeight = *nodeWeight;
		m_cycleUtils->m_cycleDiscovery.m_bestNeightboreNum = int(*bestNeightboreNum);
		m_cycleUtils->m_cycleDiscovery.m_rotationGraphNum = int(*rotationGraphNum);
		m_cycleUtils->m_cycleDiscovery.m_isRoGraphConnect = *rographConnect;
		m_cycleUtils->m_cycleDiscovery.m_dihedralWeight = *dihedralWeight;
		m_cycleUtils->m_cycleDiscovery.m_stateNum = int(*stateNum);
		m_cycleUtils->m_cycleDiscovery.m_isDoCycleBreak = *isCycleBreak;

		m_cycleUtils->m_weightTri = *weightTri;
		m_cycleUtils->m_weightEdge = *weightEdge;
		m_cycleUtils->m_weightBiTri = *weightBiTri;
		m_cycleUtils->m_weightTriBd = *weightTriBd;
		m_cycleUtils->m_weightWorsDih = *weightWorsDih;
		m_cycleUtils->m_SurfaceSmooth = *surfaceSmooth;
		m_cycleUtils->m_subdivisonSmooth = int(*subdivisonSmooth);
		m_cycleUtils->m_laplacianSmooth = int(*laplacianSmooth);


		statusBarMain->SetStatusText("construct rotation graph ",0);
		m_cycleUtils->m_cycleDiscovery.constructRotationGraphbyPoleGraphEx();
		statusBarMain->SetStatusText("construct cycles ",0);
		if (m_cycleUtils->m_cycleDiscovery.m_rotationGraph.empty())
			return;
		m_cycleUtils->m_cycleDiscovery.constructCycles();
		m_cycleUtils->m_cycleDiscovery.cycleBreaking();
		if(showSurface->GetValue())
			m_cycleUtils->surfaceBuilding();

//		m_openGLView->Render();

		clock_t  time_end = clock();
		clock_t m_timeConsuming = time_end - time_str;
		char tbuf[100];
		_itoa(int(m_timeConsuming),tbuf,10);
		char *tx1 = "Time Used: ";
		char *tx2 = new char[strlen(tbuf)];
		strcpy(tx2,tbuf);
		strcpy(tbuf,tx1);
		strcat(tbuf,tx2);
		strcat(tbuf,"mm");
		statusBarMain->SetStatusText(tbuf,0);
	//	showSurface->SetValue(false);
	//	m_cycleUtils->setShowSurface(false);
	}

}

/*
void SketchSurfaceMakerFrame::OnColorSetClick(wxCommandEvent& event)
{
	if(!colorSet->IsEnabled() || !colorSet->IsShown())return;
	unsigned int sel=showCycleMode->GetSelection();
	bool cyc = sel==0?false:true;
	if(!cyc&&!showCycles->GetValue()){
        wxMessageBox( _("Please Enable Cycles Cost"),_("Warn"),wxOK | wxICON_INFORMATION, this );
		return;
	}

	std::vector<double> color1,color2;
	wxColourData data;
	data.SetChooseFull(true);
	for (int i = 0; i < 16; i++)
	{
		wxColour colour(i*16, i*16, i*16);
		data.SetCustomColour(i, colour);
	}
	wxColourDialog dialog(this, &data);
	dialog.SetTitle("First Color");
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColourData retData = dialog.GetColourData();
		wxColour col = retData.GetColour();
		color1.push_back(col.Red());color1.push_back(col.Green());color1.push_back(col.Blue());

		dialog.SetTitle("Second Color");
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColourData retData = dialog.GetColourData();
			wxColour col = retData.GetColour();
			color2.push_back(col.Red());color2.push_back(col.Green());color2.push_back(col.Blue());
		}
	}
	if(color1.empty()||color2.empty())
		return;
	if(cyc&&showCycles->GetValue()){
		m_cycleUtils->setCycleColors(color1,color2);}
	m_openGLView->Render();
}
*/
void SketchSurfaceMakerFrame::PushBackAll()
{
	allStatus.clear();
	wxStatus m_stat;

	//	wxButton* loadFile;
	m_stat.isEnable=loadFile->IsEnabled();
	m_stat.isShow=loadFile->IsShown();
	allStatus.push_back(m_stat);
	//	wxButton* saveFile;
	m_stat.isEnable=saveFile->IsEnabled();
	m_stat.isShow=saveFile->IsShown();
	allStatus.push_back(m_stat);
	//	wxButton* cycleConstraint;
	m_stat.isEnable=cycleConstraint->IsEnabled();
	m_stat.isShow=cycleConstraint->IsShown();
	allStatus.push_back(m_stat);
	//	wxButton* capacityBtn;
	m_stat.isEnable=capacityBtn->IsEnabled();
	m_stat.isShow=capacityBtn->IsShown();
	allStatus.push_back(m_stat);
	//	wxButton* parameters;
	m_stat.isEnable=parameters->IsEnabled();
	m_stat.isShow=parameters->IsShown();
	allStatus.push_back(m_stat);
	//	wxCheckBox* showCurveNetwork;
	m_stat.isCheck=showCurveNetwork->IsChecked();
	m_stat.isEnable=showCurveNetwork->IsEnabled();
	m_stat.isShow=showCurveNetwork->IsShown();
	allStatus.push_back(m_stat);
	//	wxRadioBox* showCurveMode;
	m_stat.sel=showCurveMode->GetSelection();
	m_stat.isEnable=showCurveMode->IsEnabled();
	m_stat.isShow=showCurveMode->IsShown();
	allStatus.push_back(m_stat);
	//	wxCheckBox* showCycles;
	m_stat.isCheck=showCycles->IsChecked();
	m_stat.isEnable=showCycles->IsEnabled();
	m_stat.isShow=showCycles->IsShown();
	allStatus.push_back(m_stat);
	//	wxRadioBox* showCycleMode;
	m_stat.sel=showCycleMode->GetSelection();
	m_stat.isEnable=showCycleMode->IsEnabled();
	m_stat.isShow=showCycleMode->IsShown();
	allStatus.push_back(m_stat);
	//	wxStaticText* cycleSizeText;
	m_stat.isEnable=cycleSizeText->IsEnabled();
	m_stat.isShow=cycleSizeText->IsShown();
	allStatus.push_back(m_stat);
	//	wxSpinButton* showCycleSize;
	m_stat.sel=showCycleSize->GetValue();
	m_stat.isEnable=showCycleSize->IsEnabled();
	m_stat.isShow=showCycleSize->IsShown();
	allStatus.push_back(m_stat);
	//	wxStaticText* lineSizeText;
	m_stat.isEnable=lineSizeText->IsEnabled();
	m_stat.isShow=lineSizeText->IsShown();
	allStatus.push_back(m_stat);
	//	wxSpinButton* lineSize;
	m_stat.sel=lineSize->GetValue();
	m_stat.isEnable=lineSize->IsEnabled();
	m_stat.isShow=lineSize->IsShown();
	allStatus.push_back(m_stat);
	//	wxCheckBox* showSingleCycle;
	m_stat.isCheck=showSingleCycle->IsChecked();
	m_stat.isEnable=showSingleCycle->IsEnabled();
	m_stat.isShow=showSingleCycle->IsShown();
	allStatus.push_back(m_stat);
	//	wxCheckBox* showSurface;
	m_stat.isCheck=showSurface->IsChecked();
	m_stat.isEnable=showSurface->IsEnabled();
	m_stat.isShow=showSurface->IsShown();
	allStatus.push_back(m_stat);
	//	wxRadioBox* showSurfaceMode;
	m_stat.sel=showSurfaceMode->GetSelection();
	m_stat.isEnable=showSurfaceMode->IsEnabled();
	m_stat.isShow=showSurfaceMode->IsShown();
	allStatus.push_back(m_stat);
	// wxCheckBox* showCapacity;
	m_stat.sel=showCapacity->GetValue();
	m_stat.isEnable=showCapacity->IsEnabled();
	m_stat.isShow=showCapacity->IsShown();
	allStatus.push_back(m_stat);
	// wxCheckBox* showConstraint;
	m_stat.sel=showConstraint->GetValue();
	m_stat.isEnable=showConstraint->IsEnabled();
	m_stat.isShow=showConstraint->IsShown();
	allStatus.push_back(m_stat);
	// wxSpinButton* curveLineSize;
	m_stat.sel=curveLineSize->GetValue();
	m_stat.isEnable=curveLineSize->IsEnabled();
	m_stat.isShow=curveLineSize->IsShown();
	allStatus.push_back(m_stat);
	// wxStaticText* curveLineSizeTex;
	m_stat.isEnable=curveLineSizeTex->IsEnabled();
	m_stat.isShow=curveLineSizeTex->IsShown();
	allStatus.push_back(m_stat);
	// wxButton* cycleRandomColor;
	m_stat.isEnable=cycleRandomColor->IsEnabled();
	m_stat.isShow=cycleRandomColor->IsShown();
	allStatus.push_back(m_stat);

}
void SketchSurfaceMakerFrame::PopBackAll()
{
	std::vector<wxStatus>::iterator itStat = allStatus.begin();
	//	wxButton* loadFile;
	itStat->isEnable ? loadFile->Enable() : loadFile->Disable();
	itStat->isShow ? loadFile->Show() : loadFile->Hide();
	itStat++;
	//	wxButton* saveFile;
	itStat->isEnable ? saveFile->Enable() : saveFile->Disable();
	itStat->isShow ? saveFile->Show() : saveFile->Hide();
	itStat++;
	//	wxButton* cycleConstraint;
	itStat->isEnable ? cycleConstraint->Enable() : cycleConstraint->Disable();
	itStat->isShow ? cycleConstraint->Show() : cycleConstraint->Hide();
	itStat++;
	//	wxButton* capacityBtn;
	itStat->isEnable ? capacityBtn->Enable() : capacityBtn->Disable();
	itStat->isShow ? capacityBtn->Show() : capacityBtn->Hide();
	itStat++;
	//	wxButton* parameters;
	itStat->isEnable ? parameters->Enable() : parameters->Disable();
	itStat->isShow ? parameters->Show() : parameters->Hide();
	itStat++;
	//	wxCheckBox* showCurveNetwork;
	itStat->isCheck ? showCurveNetwork->SetValue(true) : showCurveNetwork->SetValue(false);
	itStat->isEnable ? showCurveNetwork->Enable() : showCurveNetwork->Disable();
	itStat->isShow ? showCurveNetwork->Show() : showCurveNetwork->Hide();
	itStat++;
	//	wxRadioBox* showCurveMode;
	showCurveMode->SetSelection(itStat->sel);
	itStat->isEnable ? showCurveMode->Enable() : showCurveMode->Disable();
	itStat->isShow ? showCurveMode->Show() : showCurveMode->Hide();
	itStat++;
	//	wxCheckBox* showCycles;
	itStat->isCheck ? showCycles->SetValue(true) : showCycles->SetValue(false);
	itStat->isEnable ? showCycles->Enable() : showCycles->Disable();
	itStat->isShow ? showCycles->Show() : showCycles->Hide();
	itStat++;
	//	wxRadioBox* showCycleMode;
	showCycleMode->SetSelection(itStat->sel);
	itStat->isEnable ? showCycleMode->Enable() : showCycleMode->Disable();
	itStat->isShow ? showCycleMode->Show() : showCycleMode->Hide();
	itStat++;
	//	wxStaticText* cycleSizeText;
	itStat->isEnable ? cycleSizeText->Enable() : cycleSizeText->Disable();
	itStat->isShow ? cycleSizeText->Show() : cycleSizeText->Hide();
	itStat++;
	//	wxSpinButton* showCycleSize;
	showCycleSize->SetValue(itStat->sel);
	itStat->isEnable ? showCycleSize->Enable() : showCycleSize->Disable();
	itStat->isShow ? showCycleSize->Show() : showCycleSize->Hide();
	itStat++;
	//	wxStaticText* lineSizeText;
	itStat->isEnable ? lineSizeText->Enable() : lineSizeText->Disable();
	itStat->isShow ? lineSizeText->Show() : lineSizeText->Hide();
	itStat++;
	//	wxSpinButton* lineSize;
	lineSize->SetValue(itStat->sel);
	itStat->isEnable ? lineSize->Enable() : lineSize->Disable();
	itStat->isShow ? lineSize->Show() : lineSize->Hide();
	itStat++;
	//	wxCheckBox* showSingleCycle;
	itStat->isCheck ? showSingleCycle->SetValue(true) : showSingleCycle->SetValue(false);
	itStat->isEnable ? showSingleCycle->Enable() : showSingleCycle->Disable();
	itStat->isShow ? showSingleCycle->Show() : showSingleCycle->Hide();
	itStat++;
	//	wxCheckBox* showSurface;
	itStat->isCheck ? showSurface->SetValue(true) : showSurface->SetValue(false);
	itStat->isEnable ? showSurface->Enable() : showSurface->Disable();
	itStat->isShow ? showSurface->Show() : showSurface->Hide();
	itStat++;
	//	wxRadioBox* showSurfaceMode;
	showSurfaceMode->SetSelection(itStat->sel);
	itStat->isEnable ? showSurfaceMode->Enable() : showSurfaceMode->Disable();
	itStat->isShow ? showSurfaceMode->Show() : showSurfaceMode->Hide();
	itStat++;
	// wxCheckBox* showCapacity;
	itStat->isCheck ? showCapacity->SetValue(true) : showCapacity->SetValue(false);
	itStat->isEnable ? showCapacity->Enable() : showCapacity->Disable();
	itStat->isShow ? showCapacity->Show() : showCapacity->Hide();
	itStat++;
	// wxCheckBox* showConstraint;
	itStat->isCheck ? showConstraint->SetValue(true) : showConstraint->SetValue(false);
	itStat->isEnable ? showConstraint->Enable() : showConstraint->Disable();
	itStat->isShow ? showConstraint->Show() : showConstraint->Hide();
	itStat++;
	// wxSpinButton* curveLineSize;
	curveLineSize->SetValue(itStat->sel);
	itStat->isEnable ? curveLineSize->Enable() : curveLineSize->Disable();
	itStat->isShow ? curveLineSize->Show() : curveLineSize->Hide();
	itStat++;
	// wxStaticText* curveLineSizeTex;
	itStat->isEnable ? curveLineSizeTex->Enable() : curveLineSizeTex->Disable();
	itStat->isShow ? curveLineSizeTex->Show() : curveLineSizeTex->Hide();
	itStat++;
	// wxButton* cycleRandomColor;
	itStat->isEnable ? cycleRandomColor->Enable() : cycleRandomColor->Disable();
	itStat->isShow ? cycleRandomColor->Show() : cycleRandomColor->Hide();

	m_cycleUtils->m_showArcMode=(showCurveMode->GetSelection());
	m_cycleUtils->m_showCurveNetwork=(showCurveNetwork->GetValue());
	m_cycleUtils->m_showSingleCycle=(showSingleCycle->GetValue());
	m_cycleUtils->m_showCycleMode=(showCycleMode->GetSelection());
	m_cycleUtils->m_showCycles=(showCycles->GetValue());
	m_cycleUtils->m_showSurface=(showSurface->GetValue());
	m_cycleUtils->m_selectedCycleSize=(showCycleSize->GetValue());
	m_cycleUtils->m_lineSize=(lineSize->GetValue());
	m_cycleUtils->m_showSurfaceMode=(showSurfaceMode->GetSelection());
	m_cycleUtils->m_showCapacity=(showCapacity->GetValue());
	m_cycleUtils->m_showConstraint=(showConstraint->GetValue());
	m_cycleUtils->m_curveLineSize=(curveLineSize->GetValue());

	m_openGLView->Render();
}
void SketchSurfaceMakerFrame::EnableAll()
{
	loadFile->Enable();
	saveFile->Enable();
	cycleConstraint->Enable();
	capacityBtn->Enable();
	parameters->Enable();
	showCurveNetwork->Enable();
	showCurveMode->Enable();
	showCycles->Enable();
	showCycleMode->Enable();
	showCycleSize->Enable();
	cycleSizeText->Enable();
	lineSizeText->Enable();
	lineSize->Enable();
	showSingleCycle->Enable();
	showSurface->Enable();
	showSurfaceMode->Enable();
	showCapacity->Enable();
	showConstraint->Enable();
	curveLineSize->Enable();
	curveLineSizeTex->Enable();
	cycleRandomColor->Enable();
}
void SketchSurfaceMakerFrame::DisableAll()
{
	loadFile->Disable();
	saveFile->Disable();
	cycleConstraint->Disable();
	capacityBtn->Disable();
	parameters->Disable();
	showCurveNetwork->Disable();
	showCurveMode->Disable();
	showCycles->Disable();
	showCycleMode->Disable();
	showCycleSize->Disable();
	cycleSizeText->Disable();
	lineSizeText->Disable();
	lineSize->Disable();
	showSingleCycle->Disable();
	showSurface->Disable();
	showSurfaceMode->Disable();
	showCapacity->Disable();
	showConstraint->Disable();
	curveLineSize->Disable();
	curveLineSizeTex->Disable();
	cycleRandomColor->Disable();

}
void SketchSurfaceMakerFrame::setShowMode()
{
	if(showMode==CURVE || showMode==SURFACE ||showMode==CYCLE)
	{
		if(preShowMode==NORMAL)
			PushBackAll();
		preShowMode = showMode;
		DisableAll();
		m_cycleUtils->m_showSingleCurve=(false);
		if(showMode==CURVE){
			showCapacity->Enable();
			showConstraint->Enable();
			m_cycleUtils->m_showCurveNetwork=(true);
			m_cycleUtils->m_showCycles=(false);
			m_cycleUtils->m_showSurface=(false);
		}
		else if(showMode ==CYCLE){
			m_cycleUtils->m_showCurveNetwork=(false);
			m_cycleUtils->m_showCycles=(true);
			m_cycleUtils->m_showSurface=(false);
		}
		else if(showMode == SURFACE){
			m_cycleUtils->m_showCurveNetwork=(false);
			m_cycleUtils->m_showCycles=(false);
			m_cycleUtils->m_showSurface=(true);
		}
	}
	else{
		if(preShowMode!=NORMAL)
			PopBackAll();
		preShowMode = showMode;
	}

}
void SketchSurfaceMakerFrame::OnResetAll(wxCommandEvent& event)
{
	m_cycleUtils->resetAll();
	OnResetVisualization(event);
}
void SketchSurfaceMakerFrame::OnResetVisualization(wxCommandEvent& event)
{
	m_openGLView->Initialize();
	m_cycleUtils->resetAllVisualization();
	allStatus = initStatus;
	PopBackAll();
}
void SketchSurfaceMakerFrame::OnResize(wxSizeEvent& event)
{
	wxSize windowSize = GetClientSize();
	wxSize canvaseSize = wxSize(windowSize.GetX()*1000/1250,windowSize.GetY()*700/810);
	m_openGLView->SetSize(canvaseSize);
}

void SketchSurfaceMakerFrame::OnPopupClick(wxCommandEvent &evt)
 {
/*
 	void *data=static_cast<wxMenu *>(evt.GetEventObject())->GetClientData();
	if(evt.GetId()==ID_CAPACITY_ONE)
		m_cycleUtils->setCapacity(1);
	if(evt.GetId()==ID_CAPACITY_TWO)
		m_cycleUtils->setCapacity(2);
	if(evt.GetId()==ID_CAPACITY_THREE)
		m_cycleUtils->setCapacity(3);
	if(evt.GetId()==ID_CAPACITY_FOUR)
		m_cycleUtils->setCapacity(4);
	if(evt.GetId()==ID_CAPACITY_FIVE)
		m_cycleUtils->setCapacity(5);
	if(evt.GetId()==ID_CAPACITY_SIX)
		m_cycleUtils->setCapacity(6);

	m_openGLView->Render();
*/
 }
void SketchSurfaceMakerFrame::OnGLViewLeftDClick(wxMouseEvent& event)
{
/*
	if(capacityBtn->GetLabel()=="Capacity")
		return;
 	wxMenu mnu;
	wxString a = capacityBtn->GetLabel();
 	mnu.Append(ID_CAPACITY_ONE, 	"---1---");
 	mnu.Append(ID_CAPACITY_TWO, 	"---2---");
 	mnu.Append(ID_CAPACITY_THREE, 	"---3---");
 	mnu.Append(ID_CAPACITY_FOUR, 	"---4---");
 	mnu.Append(ID_CAPACITY_FIVE, 	"---5---");
 	mnu.Append(ID_CAPACITY_SIX, 	"---6---");
 	mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SketchSurfaceMakerFrame::OnPopupClick, NULL, this);
	PopupMenu(&mnu);
*/
}
void SketchSurfaceMakerFrame::OnGLViewMouseWheel(wxMouseEvent& event)
{
	if(capacityBtn->GetLabel()=="Capacity")
		return;
	if(!event.ControlDown())
		return;

	int indicate = event.GetWheelRotation();
	int capacity = m_cycleUtils->getSelectArcCapacity();
	if(indicate>0 && capacity<8)
		capacity++;
	else if(indicate<0 && capacity>1)
		capacity--;
	m_cycleUtils->setCapacity(capacity);
	m_openGLView->Render();
}
void SketchSurfaceMakerFrame::OnGLViewKeyUp(wxKeyEvent& event)
{

}
void SketchSurfaceMakerFrame::OnGLViewKeyDown(wxKeyEvent& event)
{
//	m_openGLView->SetFocus();
	if (event.ControlDown()){		switch(event.GetKeyCode()){			case 'L': OnOpenCurveFile(wxCommandEvent(WXTYPE(0),0));break;			case 'S': OnSaveAll(wxCommandEvent(WXTYPE(0),0));break;			case 'C': OnCycleConstraintClick(wxCommandEvent(WXTYPE(0),0));break;			case 'A': OnCapacityBtnClick(wxCommandEvent(WXTYPE(0),0));break;			case 'P': OnParametersClick(wxCommandEvent(WXTYPE(0),0));break;			case WXK_UP:				if(capacityBtn->GetLabel()=="Done"){
					int cap = m_cycleUtils->getSelectArcCapacity();
					if(cap<8)
						cap++;
					m_cycleUtils->setCapacity(cap);
					m_openGLView->Render();
				}
				break;
			case WXK_DOWN:				if(capacityBtn->GetLabel()=="Done"){
					int cap = m_cycleUtils->getSelectArcCapacity();
					if(cap>1)
						cap--;
					m_cycleUtils->setCapacity(cap);
					m_openGLView->Render();				}				break;			default: break;		}		return;	}	if (event.ShiftDown()){		switch(event.GetKeyCode()){			case 'A':				if(!showCurveMode->IsEnabled()|| !showCurveMode->IsShown()) break;				showCurveMode->SetSelection((1+showCurveMode->GetSelection())%2);				OnShowCurveModeSelect(wxCommandEvent(WXTYPE(0),0));break;			case 'C':				if(!showCycleMode->IsEnabled()|| !showCycleMode->IsShown()) break;				showCycleMode->SetSelection((1+showCycleMode->GetSelection())%2);				OnShowCycleModeSelect(wxCommandEvent(WXTYPE(0),0));break;			case 'S':				if(!showSurfaceMode->IsEnabled()|| !showSurfaceMode->IsShown()) break;				showSurfaceMode->SetSelection((1+showSurfaceMode->GetSelection())%2);				OnSurfaceModeSelect(wxCommandEvent(WXTYPE(0),0));break;			default: break;		}		return;	}	switch(event.GetKeyCode()){			case WXK_HOME:	IsFullScreen()?ShowFullScreen(false):ShowFullScreen(true); break;			case 'A':				if(!showCurveNetwork->IsEnabled()|| !showCurveNetwork->IsShown()) break;				showCurveNetwork->SetValue(!showCurveNetwork->GetValue());				OnShowCurveNetworkClick(wxCommandEvent(WXTYPE(0),0));break;			case 'C':				if(!showCycles->IsEnabled()|| !showCycles->IsShown()) break;				showCycles->SetValue(!showCycles->GetValue());				OnShowCyclesClick(wxCommandEvent(WXTYPE(0),0));break;			case 'S':				if(!showSurface->IsEnabled()|| !showSurface->IsShown()) break;				showSurface->SetValue(!showSurface->GetValue());				OnShowSurfaceClick(wxCommandEvent(WXTYPE(0),0));break;			case 'N':				if(!showSingleCycle->IsEnabled()|| !showSingleCycle->IsShown()) break;				showSingleCycle->SetValue(!showSingleCycle->GetValue());				OnShowSingleCycleSelect(wxCommandEvent(WXTYPE(0),0));break;			case WXK_LEFT:				if(!showCycleSize->IsEnabled()|| !showCycleSize->IsShown()) break;				showCycleSize->SetValue(showCycleSize->GetValue()-1);				OnShowCycleSizeChange(wxSpinEvent(WXTYPE(0),0));break;			case WXK_RIGHT:				if(!showCycleSize->IsEnabled()|| !showCycleSize->IsShown()) break;				showCycleSize->SetValue(showCycleSize->GetValue()+1);				OnShowCycleSizeChange(wxSpinEvent(WXTYPE(0),0));break;			case WXK_UP: if (m_cycleUtils->m_nodeSize<20) m_cycleUtils->m_nodeSize += 1; break;				if(!lineSize->IsEnabled()|| !lineSize->IsShown()) break;				lineSize->SetValue(lineSize->GetValue()+1);				OnlineSizeChange(wxSpinEvent(WXTYPE(0),0));break;			case WXK_DOWN: if (m_cycleUtils->m_nodeSize>2) m_cycleUtils->m_nodeSize -= 1; break;				if(!lineSize->IsEnabled()|| !lineSize->IsShown()) break;				lineSize->SetValue(lineSize->GetValue()-1);				OnlineSizeChange(wxSpinEvent(WXTYPE(0),0));break;			case WXK_DELETE: m_cycleUtils->rollBack(SHOWSINGLECURVE);break;
			case WXK_ESCAPE: exit(0);
			case '1': showMode=CURVE; setShowMode();break;
			case '2': showMode=CYCLE; setShowMode();break;
			case '3': showMode=SURFACE; setShowMode();break;
			case '4': showMode=NORMAL; setShowMode();break;

			case 'R':m_openGLView->setRotateTimes(1);m_openGLView->setRotate();break;
			case 'T':m_openGLView->setRotateTimes(2); m_openGLView->setRotate();break;

			case '0': m_cycleUtils->m_showNormal = !m_cycleUtils->m_showNormal; break;
			case '8': m_cycleUtils->setShowNewNormal();break;
			case '9':m_openGLView->screenShot();break;
			case 'V':m_openGLView->setViewPort(); break;
			default:break;	}	m_openGLView->Render();	return;}
void SketchSurfaceMakerFrame::OnKeyDown(wxKeyEvent& event)
{
	OnGLViewKeyDown(event);
}
void SketchSurfaceMakerFrame::OnIdle(wxIdleEvent& event)
{
	m_openGLView->OnIdle(event);
}

void SketchSurfaceMakerFrame::OnShowNewCyclesClick(wxCommandEvent& event)
{
	m_cycleUtils->setShowNewCycle(showSingleCycle->GetValue());
	m_openGLView->Render();
}

void SketchSurfaceMakerFrame::OnSubdivideCycles(wxCommandEvent& event)
{
	wxString str = cycleSubdivideParam_Curvature->GetValue();
	double tf; str.ToDouble(&tf);
	m_cycleUtils->cycleCurvatureThres = tf;
	m_cycleUtils->subDivideCycles();
	m_openGLView->Render();
}
