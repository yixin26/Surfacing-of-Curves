/***************************************************************
 * Name:      SketchSurfaceMakerMain.h
 * Purpose:   Defines Application Frame
 * Author:    Yixin Zhuang (yixin.zhuang@gmail.com)
 * Created:   2012-12-04
 * Copyright: Yixin Zhuang (none)
 * License:
 **************************************************************/

#ifndef SKETCHSURFACEMAKERMAIN_H
#define SKETCHSURFACEMAKERMAIN_H
//(*Headers(SketchSurfaceMakerFrame)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/glcanvas.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/spinbutt.h>
#include <wx/statusbr.h>
//*)
#include <vector>

class SketchGLCanvas;//in charge of visualization;
class cycleUtils;//in charge of curve data and processing;

enum SHOWMODE{CURVE,CYCLE,SURFACE,NORMAL};

class SketchSurfaceMakerFrame: public wxFrame
{
    public:

        SketchSurfaceMakerFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~SketchSurfaceMakerFrame();

    private:

		void OnPopupClick(wxCommandEvent& event);
		void PushBackAll();
		void PopBackAll();
		void EnableAll();
		void DisableAll();
		void setShowMode();
		void OnIdle(wxIdleEvent& event);
        //(*Handlers(SketchSurfaceMakerFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpenCurveFile(wxCommandEvent& event);
        void OnComputeRotationGraph(wxCommandEvent& event);
        void OnComputeCycleSet(wxCommandEvent& event);
        void OnShowCurveNetworkClick(wxCommandEvent& event);
        void OnShowCyclesClick(wxCommandEvent& event);
        void OnComputeCycleBreaking(wxCommandEvent& event);
        void OnMenuItemRandomRotationGraph(wxCommandEvent& event);
        void OnResetAll(wxCommandEvent& event);
        void OnCapacitySelect(wxCommandEvent& event);
        void OnSaveNewCurve(wxCommandEvent& event);
        void OnShowSurfaceClick(wxCommandEvent& event);
        void OnComputeTriangulation(wxCommandEvent& event);
        void OnComputeRotationGraphExtended(wxCommandEvent& event);
        void OnOpenMMFile(wxCommandEvent& event);
        void OnSaveEmbeddedGraph(wxCommandEvent& event);
        void OnOpenMesh(wxCommandEvent& event);
        void OnResetVisualization(wxCommandEvent& event);
        void OnShowCycleSizeChange(wxSpinEvent& event);
        void OnComputeRotationGraphSuperGraph(wxCommandEvent& event);
        void OnComputeRotationGraphSuperGraphEx(wxCommandEvent& event);
        void OnSaveParameters(wxCommandEvent& event);
        void OnSaveMesh(wxCommandEvent& event);
        void OnRadioButton1Select(wxCommandEvent& event);
        void OnIsResultRightSelect(wxCommandEvent& event);
        void OnShowSelectCycleClick(wxCommandEvent& event);
        void OnSaveCurveWithNewCapacity(wxCommandEvent& event);
        void OnCycleConstraintClick(wxCommandEvent& event);
        void OnCapacityBtnClick(wxCommandEvent& event);
        void OnParametersClick(wxCommandEvent& event);
        void OnSaveCycles(wxCommandEvent& event);
        void OnShowCurveModeSelect(wxCommandEvent& event);
        void OnShowCycleModeSelect(wxCommandEvent& event);
        void OnShowSingleCycleSelect(wxCommandEvent& event);
        void OnGLViewLeftDClick(wxMouseEvent& event);
        void OnGLViewPaint(wxPaintEvent& event);
        void OnGLViewMouseWheel(wxMouseEvent& event);
        void OnSurfaceModeSelect(wxCommandEvent& event);
        void OnSaveAll(wxCommandEvent& event);
        void OnparametersClick(wxCommandEvent& event);
        void OnlineSizeChange(wxSpinEvent& event);
        void OnResize(wxSizeEvent& event);
        void OnGLViewResize(wxSizeEvent& event);
        void OnGLViewKeyUp(wxKeyEvent& event);
        void OnGLViewKeyDown(wxKeyEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnRecomputeAllClick(wxCommandEvent& event);
        void OnCycleBreakingClick(wxCommandEvent& event);
        void OnshowCapacityClick(wxCommandEvent& event);
        void OnshowConstraintClick(wxCommandEvent& event);
        void OnshowCycleBreakingClick(wxCommandEvent& event);
        void OncurveLineSizeChange(wxSpinEvent& event);
        void OnCycleRandomColorClick(wxCommandEvent& event);
        void OnShowNewCyclesClick(wxCommandEvent& event);
        void OnSubdivideCycles(wxCommandEvent& event);
        //*)

        //(*Identifiers(SketchSurfaceMakerFrame)
        static const long ID_BUTTONLOADFILE;
        static const long ID_BUTTONSAVEFILE;
        static const long ID_STATICLINE2;
        static const long ID_CYCLECONSTRAINT;
        static const long ID_BUTTONCAPACITY;
        static const long ID_BUTTONPARAMETERS;
        static const long ID_STATICLINE1;
        static const long ID_SHOWCURVENETWORK;
        static const long ID_RADIOBOXCURVEMODE;
        static const long ID_STATICTEXT2;
        static const long ID_CURVELINESIZE;
        static const long ID_SHOWCAPACITY;
        static const long ID_SHOWCONSTRAINT;
        static const long ID_SHOWCYCLES;
        static const long ID_RADIOBOXCYCLEMODE;
        static const long ID_STATICTEXTCYCLESIZE;
        static const long ID_SHOWCYCLESIZE;
        static const long ID_STATICTEXT1;
        static const long ID_LINESIZE;
        static const long ID_CYCLERANDOMCOLOR;
        static const long ID_SHOWSINGLECYCLE;
        static const long ID_CHECKBOX1;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON1;
        static const long ID_SHOWSURFACE;
        static const long ID_SURFACEMODE;
        static const long ID_STATICLINE3;
        static const long ID_GLCANVAS;
        static const long idMenuCurveFile;
        static const long ID_MENUITEM2;
        static const long idMenuSaveModifiedFile;
        static const long ID_MENUITEM6;
        static const long idMenuSaveParameters;
        static const long ID_MENUITEM3;
        static const long idMenuQuit;
        static const long idMenuResetVisualization;
        static const long idMenuResetAll;
        static const long idMenuRandomRotationGraph;
        static const long idMenuRotationGraph;
        static const long idMenuRotationGraphExtended;
        static const long idMenuRotationGraphSuperGraph;
        static const long idMenuRotationGraphSuperGraphEx;
        static const long ID_MENUITEM1;
        static const long idMenuCycleDiscovery;
        static const long idMenuCycleBreaking;
        static const long idMenuTriangulation;
        static const long idMenuAbout;
        static const long ID_STATUSBARMAIN;
        //*)

		static const long ID_CAPACITY_ONE;
		static const long ID_CAPACITY_TWO;
		static const long ID_CAPACITY_THREE;
		static const long ID_CAPACITY_FOUR;
		static const long ID_CAPACITY_FIVE;
		static const long ID_CAPACITY_SIX;

        //(*Declarations(SketchSurfaceMakerFrame)
        wxMenuItem* MenuItemOpenCurve;
        wxButton* saveFile;
        wxRadioBox* showCycleMode;
        wxSpinButton* curveLineSize;
        wxStaticText* curveLineSizeTex;
        wxCheckBox* showCycles;
        wxMenuBar* mainMenu;
        wxCheckBox* showCurveNetwork;
        wxMenu* Menu3;
        wxButton* Button1;
        wxRadioBox* showSurfaceMode;
        wxMenu* MenuItem3;
        wxMenuItem* MenuItemRandomRotationGraph;
        wxSpinButton* showCycleSize;
        wxStaticText* cycleSizeText;
        wxMenuItem* MenuItemRotationGraphSuperGraphEx;
        wxMenuItem* MenuItemCycleBreaking;
        wxRadioBox* showCurveMode;
        wxMenu* MenuItem4;
        wxStaticText* StaticText1;
        wxCheckBox* showSurface;
        wxMenuItem* MenuItemRotationGraph;
        wxMenuItem* MenuItemResetVisualization;
        wxMenuItem* MenuItemResetAll;
        wxStaticLine* StaticLine2;
        wxButton* loadFile;
        wxMenu* MenuItem7;
        wxMenuItem* MenuItemCycleDiscovery;
        wxMenuItem* MenuItemRotationGraphExtended;
        wxTextCtrl* cycleSubdivideParam_Curvature;
        wxCheckBox* showCapacity;
        wxButton* cycleRandomColor;
        wxStaticLine* StaticLine3;
        wxStaticLine* StaticLine1;
        wxCheckBox* showConstraint;
        wxButton* cycleConstraint;
        wxCheckBox* showSingleCycle;
        wxCheckBox* showNewCycles;
        wxMenuItem* MenuItemSaveModifiedFile;
        wxStaticText* lineSizeText;
        wxMenu* MenuItem5;
        wxStatusBar* statusBarMain;
        wxButton* parameters;
        wxButton* capacityBtn;
        wxMenuItem* MenuItem9;
        wxMenuItem* MenuItemTriangulation;
        SketchGLCanvas* m_openGLView;
        wxMenuItem* MenuItemRotationGraphSuperGraph;
        wxSpinButton* lineSize;
        //*)

        cycleUtils* m_cycleUtils;

		class wxStatus {
		public:
			bool isCheck;	int sel;
			bool isEnable;	bool isShow;
		public:
			wxStatus(){}
			wxStatus(const bool &isCheck, const int &sel,const bool &isEnable,const bool &isShow)
				:isCheck(isCheck),sel(sel),isEnable(isEnable),isShow(isShow)
			{}
			wxStatus(const wxStatus& that)
			{
				this->isCheck = that.isCheck;		this->sel = that.sel;
				this->isEnable = that.isEnable;		this->isShow = that.isShow;
			}
			~wxStatus()
			{}
			wxStatus& operator=(const wxStatus& that)
			{
				this->isCheck = that.isCheck;		this->sel = that.sel;
				this->isEnable = that.isEnable;		this->isShow = that.isShow;
				return *this;
			}
		};
		std::vector<wxStatus> allStatus;
		std::vector<wxStatus> initStatus;

		SHOWMODE showMode;
		SHOWMODE preShowMode;
		
        DECLARE_EVENT_TABLE()
};

#endif // SKETCHSURFACEMAKERMAIN_H
