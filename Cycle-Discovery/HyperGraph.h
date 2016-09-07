#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

//(*Headers(HyperGraph)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class HyperGraph: public wxDialog
{
	public:

		HyperGraph(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~HyperGraph();

		//(*Declarations(HyperGraph)
		wxStaticText* StaticText10;
		wxStaticText* StaticText9;
		wxTextCtrl* curveWeight;
		wxTextCtrl* laplacianSmooth;
		wxTextCtrl* bestNeighboreNum;
		wxTextCtrl* subDivision;
		wxStaticText* StaticText13;
		wxStaticText* StaticText2;
		wxStaticText* StaticText14;
		wxTextCtrl* nodeWeight;
		wxButton* Button1;
		wxTextCtrl* rotationGraphNum;
		wxStaticText* StaticText6;
		wxTextCtrl* dihedralWeight;
		wxTextCtrl* twistNormalNum;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxTextCtrl* triangleAreaWeight;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxTextCtrl* bestStateNum;
		wxTextCtrl* triangleTriBdWeight;
		wxTextCtrl* angleWeight;
		wxTextCtrl* triangleBiTriWeight;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxCheckBox* surfaceSmooth;
		wxStaticText* StaticText15;
		wxTextCtrl* triangleEdgeWeight;
		wxStaticText* StaticText12;
		wxCheckBox* isRoGraphConnect;
		wxTextCtrl* triangleWorseDihedral;
		wxStaticText* StaticText4;
		wxCheckBox* cycleBreak;
		wxTextCtrl* twistWeight;
		wxStaticText* StaticText16;
		//*)

		double * getTwistNormNum(){return &m_twistNormNum;}
		double * getAngleWeight(){return &m_angleWeight;}
		double * getTwistWeight(){return &m_twistWeight;}
		double * getCurveWeight(){return &m_curveWeight;}
		double * getNodeWeight(){return &m_nodeWeight;}
		double * getBestNeightboreNum(){return &m_bestNeightboreNum;}
		double * getRotationGraphNum(){return &m_rotationGraphNum;}
		bool * getRotationGraphConnect(){return &m_rographConnect;}
		double * getDihedralWeight(){return &m_dihedralWeight;}
		double * getStateNum(){return &m_stateNum;}
		bool * getCycleBreak(){return &m_isCycleBreak;}
		double* getSurfaceTriWeight(){return &m_weightTri;}
		double* getSurfaceEdgeWeight(){return &m_weightEdge;}
		double* getSurfaceBiTriWeight(){return &m_weightBiTri;}
		double* getSurfaceTriBdWeight(){return &m_weightTriBd;}
		double* getSurfaceWorsDihWeight(){return &m_weightWorsDih;}
		bool* getSurfaceSmooth(){return &m_surfaceSmooth;}
		double* getSurfaceSubdivision(){return &m_subdivisonSmooth;}
		double* getSurfaceLaplacian(){return &m_laplacianSmooth;}


		void setShow();

		bool isReturn;

	protected:

		//(*Identifiers(HyperGraph)
		static const long ID_STATICTEXT12;
		static const long ID_TEXTCTRL12;
		static const long ID_STATICTEXT11;
		static const long ID_TEXTCTRL11;
		static const long ID_STATICTEXT10;
		static const long ID_TEXTCTRL10;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_ROGRAPHCONNECT;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL8;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRL9;
		static const long ID_CYCLEBREAK;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL6;
		static const long ID_STATICTEXT7;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT13;
		static const long ID_TEXTCTRL13;
		static const long ID_STATICTEXT15;
		static const long ID_TEXTCTRL15;
		static const long ID_CHECKBOX1;
		static const long ID_STATICTEXT14;
		static const long ID_TEXTCTRL14;
		static const long ID_STATICTEXT16;
		static const long ID_TEXTCTRL16;
		static const long ID_BUTTON1;
		//*)

		double m_twistNormNum;
		double m_angleWeight;
		double m_twistWeight;
		double m_curveWeight;
		double m_nodeWeight;
		double m_bestNeightboreNum;
		double m_rotationGraphNum;
		bool m_rographConnect;
		double m_dihedralWeight;
		double m_stateNum;
		bool m_isCycleBreak;
		double m_weightTri,m_weightEdge,m_weightBiTri,m_weightTriBd,m_weightWorsDih;
		bool m_surfaceSmooth;
		double m_subdivisonSmooth,m_laplacianSmooth;


	private:

		//(*Handlers(HyperGraph)
		void OnOK(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
