#ifndef CYCLE_UTILS
#define CYCLE_UTILS

#include <vector>
#include "amlVec.h"
#include <wx/string.h>
#include <wx/statusbr.h>
#include "GraphStructure.h"
#include <time.h>
#include "colormap.h"
#include "glFontSystem.h"
#include "CycleDiscovery.h"

//the operation or visualization changes environment for current work.  
enum GeometryType{SHOWSINGLENODE,SHOWSINGLECURVE,SHOWSINGLECYCLE,SHOWALLCYCLE};
enum OperationType{VIEW,SELECT,ADD,MODIFY,DEL};
enum{NODE,POLE};

using namespace GraphStructure;

class cycleUtils{
public:
	cycleUtils();
	~cycleUtils();

	/*
		data structure definition;
	*/
		//in file GraphStructure.h; shared with some other classes;

public:

	wxString m_filename;
	wxStatusBar* m_statusBar;

	cycleDiscovery m_cycleDiscovery;

	double m_weightTri,m_weightEdge,m_weightBiTri,m_weightTriBd,m_weightWorsDih;
	bool m_SurfaceSmooth;
	int m_subdivisonSmooth,m_laplacianSmooth;
	bool m_isUpdateSubdivision,m_isUpdateLaplacian;

    TriangleSurface m_triangleSurface;
    TriangleSurface m_triangleSurfaceNormal;

	/*
		visualization
	*/
	freetype::font_data our_font;
	std::vector<Colormap::color> m_curveColors;
	std::vector<Colormap::color> m_selectArcColors;
	std::vector<Colormap::color> m_cycleColors;
	int m_colorInterval;
	std::vector<Colormap::color> m_colorsCycle;
	std::vector<int> m_colorsCycleInd;
	std::vector<Colormap::color> m_colorsCycleBreak;
	std::vector<int> m_colorsCycleBreakInd;

	bool m_showCurveNetwork;
	bool m_showIntersectionPoints;
	bool m_showRotationGraph;
	bool m_showCycles;
    bool m_showSurface;

	bool m_showCyclesBreaked;

	bool m_showSingleNode;
	bool m_showSingleCurve;
	bool m_showSingleCycle;

	int m_showCycleMode;
	int m_showArcMode;
	int m_showSurfaceMode;
	int m_brightness;

	bool m_showCapacity;
	bool m_showConstraint;

	int m_showNormal;

	bool m_isViewing;

	std::vector<unsigned> m_meshListWire,m_meshListSolid;

	void drawNode(int nodeID);
	void drawIntersectionPoints();
	void drawArc(int arcID);
	void drawSeletedArcs();
	void drawString(const Point &position,const char* str,const double size);
	void drawCurveNetworkRaw();
	void drawArcRaw(int arcID);
	void drawCurveNetwork();
	void drawElementofRotationGraph(int nodeID,int orgArcID,int conArcID,int orgInstanceID,int conInstanceID,int type);
	void drawRotationGraph();
	void drawCycle(int cycleID);
	void colorMapCycle(bool isbreaked);
	void drawCycles();
	void buildList();
	void drawPatch(int patchID);
	void drawSurface();

	/*
		interaction
	*/
	int m_capacity;

	int m_selectedNodeIDFormer;
	int m_selectedArcIDFormer;
	int m_selectedNodeID;
	int m_selectedArcID;
	int m_selectedCycleID;

	int m_curveLineSize;
    int m_selectedCycleSize;
	int m_lineSize;
	int m_nodeSize;

	int m_selectedRotationGraphPosition;
	int m_selectedRotationGraphLength;
	int m_selectedPoleRotationGraphPosition;
	int m_selectedPoleRotationGraphLength;


	bool updateConstraintList();


public:


	void Render();
	void resetAllVisualization();
	void resetAllData();
	void resetAll();
	wxStatusBar** getStatusBar(){return &m_statusBar;}
		
	void resetUpdateArcs(){
		m_cycleDiscovery.m_latestUpdateArcs.clear();	m_cycleDiscovery.m_latestUpdateArcsCapacity.clear();
		m_cycleDiscovery.m_selectArcList.back().clear(); m_cycleDiscovery.m_errorNode.clear();
	}
	void resetUpdateNodes(){
		m_cycleDiscovery.m_latestUpdateNodes.clear();
		for (int i = 0; i<m_cycleDiscovery.m_latestUpdateArcs.size(); i++){
			m_cycleDiscovery.m_latestUpdateNodes.push_back(m_cycleDiscovery.m_curveNet.arcs[m_cycleDiscovery.m_latestUpdateArcs[i]].endNodesID.first);
			m_cycleDiscovery.m_latestUpdateNodes.push_back(m_cycleDiscovery.m_curveNet.arcs[m_cycleDiscovery.m_latestUpdateArcs[i]].endNodesID.second);
		}
		m_cycleDiscovery.m_latestUpdateArcs.clear();
		for (int i = 0; i<m_cycleDiscovery.m_latestUpdateNodes.size(); i++){
			std::vector<int> tarcs = m_cycleDiscovery.m_curveNet.nodes[m_cycleDiscovery.m_latestUpdateNodes[i]].arcID;
			for(int j=0;j<tarcs.size();j++)
				m_cycleDiscovery.m_latestUpdateArcs.push_back(tarcs[j]);
		}
	}
	int getSelectArcCapacity();
	bool updateCapacity(){
		if (!m_cycleDiscovery.m_errorNode.empty()) return false;
		for (int i = 0; i<m_cycleDiscovery.m_latestUpdateArcs.size(); i++){
			m_cycleDiscovery.m_curveNet.arcs[m_cycleDiscovery.m_latestUpdateArcs[i]].capacity = m_cycleDiscovery.m_latestUpdateArcsCapacity[i];
			m_cycleDiscovery.m_curveCapacitys[m_cycleDiscovery.m_latestUpdateArcs[i]] = m_cycleDiscovery.m_latestUpdateArcsCapacity[i];
		}
		return true;
	}
	void setCurveColors(std::vector<double> &color1,std::vector<double> &color2);
	void setCycleColors(std::vector<double> &color1,std::vector<double> &color2);
	void changeCycleColor();
	void setCapacity(int selection);
	
	void writeConfigurations();

    void surfaceBuilding();

	/*
		user interaction;
	*/
	void pickArc(const AML::double3 rayStr,const AML::double3 rayEnd,enum OperationType oT);
	void pickCycle(const AML::double3 rayStr,const AML::double3 rayEnd,enum OperationType oT);
	void rollBack(enum GeometryType oT);

////////////////////////////////////
	bool m_showNewNormal;
	std::vector<double*> m_newPoints;
	std::vector<float*> m_newNormals;
	std::vector<int> m_newPointNum;
	void setShowNewNormal(){m_showNewNormal = !m_showNewNormal;}
	void renderNewNormal();

	//new 
	bool m_showNewCycle;
	double cycleCurvatureThres;
	void setShowNewCycle(bool selection){ m_showNewCycle = selection; }
	void subDivideCycles();
};

#endif
