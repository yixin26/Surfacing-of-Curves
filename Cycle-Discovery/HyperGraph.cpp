#include "HyperGraph.h"
#include "SketchSurfaceMakerMain.h"

//(*InternalHeaders(HyperGraph)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(HyperGraph)
const long HyperGraph::ID_STATICTEXT12 = wxNewId();
const long HyperGraph::ID_TEXTCTRL12 = wxNewId();
const long HyperGraph::ID_STATICTEXT11 = wxNewId();
const long HyperGraph::ID_TEXTCTRL11 = wxNewId();
const long HyperGraph::ID_STATICTEXT10 = wxNewId();
const long HyperGraph::ID_TEXTCTRL10 = wxNewId();
const long HyperGraph::ID_STATICTEXT4 = wxNewId();
const long HyperGraph::ID_TEXTCTRL4 = wxNewId();
const long HyperGraph::ID_STATICTEXT3 = wxNewId();
const long HyperGraph::ID_TEXTCTRL3 = wxNewId();
const long HyperGraph::ID_STATICTEXT2 = wxNewId();
const long HyperGraph::ID_TEXTCTRL2 = wxNewId();
const long HyperGraph::ID_STATICTEXT1 = wxNewId();
const long HyperGraph::ID_TEXTCTRL1 = wxNewId();
const long HyperGraph::ID_ROGRAPHCONNECT = wxNewId();
const long HyperGraph::ID_STATICTEXT8 = wxNewId();
const long HyperGraph::ID_TEXTCTRL8 = wxNewId();
const long HyperGraph::ID_STATICTEXT9 = wxNewId();
const long HyperGraph::ID_TEXTCTRL9 = wxNewId();
const long HyperGraph::ID_CYCLEBREAK = wxNewId();
const long HyperGraph::ID_STATICTEXT5 = wxNewId();
const long HyperGraph::ID_TEXTCTRL5 = wxNewId();
const long HyperGraph::ID_STATICTEXT6 = wxNewId();
const long HyperGraph::ID_TEXTCTRL6 = wxNewId();
const long HyperGraph::ID_STATICTEXT7 = wxNewId();
const long HyperGraph::ID_TEXTCTRL7 = wxNewId();
const long HyperGraph::ID_STATICTEXT13 = wxNewId();
const long HyperGraph::ID_TEXTCTRL13 = wxNewId();
const long HyperGraph::ID_STATICTEXT15 = wxNewId();
const long HyperGraph::ID_TEXTCTRL15 = wxNewId();
const long HyperGraph::ID_CHECKBOX1 = wxNewId();
const long HyperGraph::ID_STATICTEXT14 = wxNewId();
const long HyperGraph::ID_TEXTCTRL14 = wxNewId();
const long HyperGraph::ID_STATICTEXT16 = wxNewId();
const long HyperGraph::ID_TEXTCTRL16 = wxNewId();
const long HyperGraph::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(HyperGraph,wxDialog)
	//(*EventTable(HyperGraph)
	//*)
END_EVENT_TABLE()

HyperGraph::HyperGraph(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(HyperGraph)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticBoxSizer* StaticBoxSizer12;
	wxStaticBoxSizer* StaticBoxSizer21;
	wxStaticBoxSizer* StaticBoxSizer15;
	wxStaticBoxSizer* StaticBoxSizer14;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer3;
	wxStaticBoxSizer* StaticBoxSizer16;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer9;
	wxFlexGridSizer* FlexGridSizer2;
	wxStaticBoxSizer* StaticBoxSizer9;
	wxFlexGridSizer* FlexGridSizer7;
	wxStaticBoxSizer* StaticBoxSizer7;
	wxStaticBoxSizer* StaticBoxSizer17;
	wxStaticBoxSizer* StaticBoxSizer13;
	wxStaticBoxSizer* StaticBoxSizer10;
	wxStaticBoxSizer* StaticBoxSizer19;
	wxStaticBoxSizer* StaticBoxSizer8;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxFlexGridSizer* FlexGridSizer8;
	wxStaticBoxSizer* StaticBoxSizer20;
	wxStaticBoxSizer* StaticBoxSizer11;
	wxStaticBoxSizer* StaticBoxSizer22;
	wxFlexGridSizer* FlexGridSizer6;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* StaticBoxSizer23;
	wxStaticBoxSizer* StaticBoxSizer5;
	wxStaticBoxSizer* StaticBoxSizer18;
	
	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Precompute"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticBoxSizer16 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Twist Normals #"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	StaticBoxSizer16->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	twistNormalNum = new wxTextCtrl(this, ID_TEXTCTRL12, _("30"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL12"));
	StaticBoxSizer16->Add(twistNormalNum, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(StaticBoxSizer16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer15 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Angle Weight"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	StaticBoxSizer15->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	angleWeight = new wxTextCtrl(this, ID_TEXTCTRL11, _("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
	StaticBoxSizer15->Add(angleWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(StaticBoxSizer15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer14 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Twist Weight"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	StaticBoxSizer14->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	twistWeight = new wxTextCtrl(this, ID_TEXTCTRL10, _("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	StaticBoxSizer14->Add(twistWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(StaticBoxSizer14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Curve Weight"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticBoxSizer8->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	curveWeight = new wxTextCtrl(this, ID_TEXTCTRL4, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticBoxSizer8->Add(curveWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(StaticBoxSizer8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer3->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer2->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Graph Node"));
	FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Node Weight"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticBoxSizer7->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	nodeWeight = new wxTextCtrl(this, ID_TEXTCTRL3, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticBoxSizer7->Add(nodeWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(StaticBoxSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Rotation Graph #"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticBoxSizer6->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	rotationGraphNum = new wxTextCtrl(this, ID_TEXTCTRL2, _("10"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticBoxSizer6->Add(rotationGraphNum, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(StaticBoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Neighbores #"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticBoxSizer5->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bestNeighboreNum = new wxTextCtrl(this, ID_TEXTCTRL1, _("2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticBoxSizer5->Add(bestNeighboreNum, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(StaticBoxSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer9 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	isRoGraphConnect = new wxCheckBox(this, ID_ROGRAPHCONNECT, _("  Connect Required"), wxDefaultPosition, wxSize(212,14), 0, wxDefaultValidator, _T("ID_ROGRAPHCONNECT"));
	isRoGraphConnect->SetValue(false);
	StaticBoxSizer9->Add(isRoGraphConnect, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(StaticBoxSizer9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer2->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer2->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Hypergraph Arc"));
	FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticBoxSizer12 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Dihedral Weight"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	StaticBoxSizer12->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	dihedralWeight = new wxTextCtrl(this, ID_TEXTCTRL8, _("0.5"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	StaticBoxSizer12->Add(dihedralWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer1->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer8->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Hypergraph Search"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticBoxSizer13 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Best State #"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	StaticBoxSizer13->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bestStateNum = new wxTextCtrl(this, ID_TEXTCTRL9, _("10"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	StaticBoxSizer13->Add(bestStateNum, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(StaticBoxSizer13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer4->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer8->Add(StaticBoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer1->Add(FlexGridSizer8, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticBoxSizer10 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Cycle Construct"));
	cycleBreak = new wxCheckBox(this, ID_CYCLEBREAK, _("Cycle Break"), wxDefaultPosition, wxSize(106,14), 0, wxDefaultValidator, _T("ID_CYCLEBREAK"));
	cycleBreak->SetValue(true);
	StaticBoxSizer10->Add(cycleBreak, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7->Add(StaticBoxSizer10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticBoxSizer11 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Surface Weights"));
	FlexGridSizer10 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticBoxSizer17 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Area"), wxDefaultPosition, wxSize(39,14), 0, _T("ID_STATICTEXT5"));
	StaticBoxSizer17->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	triangleAreaWeight = new wxTextCtrl(this, ID_TEXTCTRL5, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	StaticBoxSizer17->Add(triangleAreaWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer18 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Edge"), wxDefaultPosition, wxSize(49,14), 0, _T("ID_STATICTEXT6"));
	StaticBoxSizer18->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	triangleEdgeWeight = new wxTextCtrl(this, ID_TEXTCTRL6, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticBoxSizer18->Add(triangleEdgeWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer19 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Bi Triangle"), wxDefaultPosition, wxSize(64,14), 0, _T("ID_STATICTEXT7"));
	StaticBoxSizer19->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	triangleBiTriWeight = new wxTextCtrl(this, ID_TEXTCTRL7, _("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	StaticBoxSizer19->Add(triangleBiTriWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer20 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText14 = new wxStaticText(this, ID_STATICTEXT13, _("TriBd"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	StaticBoxSizer20->Add(StaticText14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	triangleTriBdWeight = new wxTextCtrl(this, ID_TEXTCTRL13, _("30"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL13"));
	StaticBoxSizer20->Add(triangleTriBdWeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer22 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("worseDihedral"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	StaticBoxSizer22->Add(StaticText15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	triangleWorseDihedral = new wxTextCtrl(this, ID_TEXTCTRL15, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
	StaticBoxSizer22->Add(triangleWorseDihedral, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	surfaceSmooth = new wxCheckBox(this, ID_CHECKBOX1, _("Smooth"), wxDefaultPosition, wxSize(212,14), 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	surfaceSmooth->SetValue(true);
	FlexGridSizer10->Add(surfaceSmooth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer21 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText13 = new wxStaticText(this, ID_STATICTEXT14, _("subDivision #"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	StaticBoxSizer21->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	subDivision = new wxTextCtrl(this, ID_TEXTCTRL14, _("2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
	StaticBoxSizer21->Add(subDivision, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer21, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer23 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("laplacianSmooth #"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	StaticBoxSizer23->Add(StaticText16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	laplacianSmooth = new wxTextCtrl(this, ID_TEXTCTRL16, _("3"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
	StaticBoxSizer23->Add(laplacianSmooth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(StaticBoxSizer23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer11->Add(FlexGridSizer10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(StaticBoxSizer11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(63,8,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button1 = new wxButton(this, ID_BUTTON1, _("OK"), wxDefaultPosition, wxSize(115,27), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer1->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&HyperGraph::OnOK);
	//*)

	isReturn= false;

}

HyperGraph::~HyperGraph()
{
	//(*Destroy(HyperGraph)
	//*)
}

void HyperGraph::setShow()
{
	wxString mystring = wxString::Format(wxT("%i"), int(m_twistNormNum));
	twistNormalNum->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_curveWeight);
	curveWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_angleWeight);
	angleWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_twistWeight);
	twistWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_nodeWeight);
	nodeWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%i"), int(m_rotationGraphNum));
	rotationGraphNum->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_dihedralWeight);
	dihedralWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%i"), int(m_bestNeightboreNum));
	bestNeighboreNum->SetValue(mystring);

	mystring = wxString::Format(wxT("%i"), int(m_stateNum));
	bestStateNum->SetValue(mystring);	
	
	cycleBreak->SetValue(m_isCycleBreak);
	isRoGraphConnect->SetValue(m_rographConnect);

	mystring = wxString::Format(wxT("%.2f"), m_weightTri);
	triangleAreaWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_weightBiTri);
	triangleBiTriWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_weightEdge);
	triangleEdgeWeight->SetValue(mystring);

	mystring = wxString::Format(wxT("%.0f"), m_weightWorsDih);
	triangleWorseDihedral->SetValue(mystring);

	mystring = wxString::Format(wxT("%.2f"), m_weightTriBd);
	triangleTriBdWeight->SetValue(mystring);

	surfaceSmooth->SetValue(m_surfaceSmooth);

	mystring = wxString::Format(wxT("%i"), int(m_subdivisonSmooth));
	subDivision->SetValue(mystring);

	mystring = wxString::Format(wxT("%i"), int(m_laplacianSmooth));
	laplacianSmooth->SetValue(mystring);

}

void HyperGraph::OnOK(wxCommandEvent& event)
{
    wxString tn = twistNormalNum->GetValue();
	wxString aw = angleWeight->GetValue();
	wxString tw = twistWeight->GetValue();
	wxString cw = curveWeight->GetValue();
	wxString nw = nodeWeight->GetValue();
	wxString bn = bestNeighboreNum->GetValue();
	wxString rg = rotationGraphNum->GetValue();
	wxString dw = dihedralWeight->GetValue();
	wxString sn = bestStateNum->GetValue();

	tn.ToDouble(&m_twistNormNum);
	aw.ToDouble(&m_angleWeight);
	tw.ToDouble(&m_twistWeight);
	cw.ToDouble(&m_curveWeight);
	nw.ToDouble(&m_nodeWeight);
	bn.ToDouble(&m_bestNeightboreNum);
	rg.ToDouble(&m_rotationGraphNum);
	m_rographConnect = isRoGraphConnect->GetValue();
	dw.ToDouble(&m_dihedralWeight);
	sn.ToDouble(&m_stateNum);
	m_isCycleBreak = cycleBreak->GetValue();

	wxString taw = triangleAreaWeight->GetValue();
	wxString tbw = triangleBiTriWeight->GetValue();
	wxString tew = triangleEdgeWeight->GetValue();
	wxString tww = triangleWorseDihedral->GetValue();
	wxString ttw = triangleTriBdWeight->GetValue();
	
	taw.ToDouble(&m_weightTri);
	tbw.ToDouble(&m_weightBiTri);
	tew.ToDouble(&m_weightEdge);
	tww.ToDouble(&m_weightWorsDih);
	ttw.ToDouble(&m_weightTriBd);

	m_surfaceSmooth=surfaceSmooth->GetValue();
	wxString ssd = subDivision->GetValue();
	wxString sla = laplacianSmooth->GetValue();
	ssd.ToDouble(&m_subdivisonSmooth);
	sla.ToDouble(&m_laplacianSmooth);

	Close();
	isReturn = true;
}
