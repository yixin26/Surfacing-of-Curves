#ifndef CYCLE_DISCOVERY
#define CYCLE_DISCOVERY

#include <vector>

#include "GraphStructure.h"
#include "graphSearch.h"
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <wx/statusbr.h>

using namespace GraphStructure;
using namespace boost;

class cycleDiscovery{
public:
	cycleDiscovery(){ init(); };
	~cycleDiscovery(){};

	void init();
	/*
	data structure definition;
	*/
	//in file GraphStructure.h; shared with some other classes;

private:

	typedef adjacency_list < vecS, vecS, undirectedS,
		no_property, property < edge_weight_t, int > > graph_t;
	typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
	typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
	typedef std::pair<int, int> Edge;


public:

	wxStatusBar* m_statusBar;

	clock_t m_timePre;
	clock_t m_timeRun;
	clock_t m_timeNewRun;

	LinearCurveNet m_curves;
	std::vector<int> m_curveCapacitys;

	Graph m_curveNet;

	int m_twistNormNum;
	double m_angleWeight;
	double m_twistWeight;
	double m_curveWeight;
	double m_nodeWeight;
	int m_bestNeightboreNum;
	int m_rotationGraphNum;
	double m_dihedralWeight;
	int m_stateNum;
	bool m_stateUnLimited;
	bool m_isRoGraphConnect;
	bool m_isDoCycleBreak;

	std::vector<std::vector<std::vector<double> > > m_twistTables;
	std::vector<std::vector<std::vector<double> > > m_twistTablesConfidence;
	std::vector<std::vector<std::vector<int> > > m_twistTablesIndex;
	bool m_isDihedral;

	RotationGraph m_rotationGraph;
	RotationGraph m_rotationGraphUpdate;

	CycleSet m_cycleSet;
	CycleSet m_cycleSetBreaked;
	bool m_isCycleBreak;
	bool m_isCycleCorrect;
	int m_breakNum;
	std::vector<double> m_arcsCost;
	std::vector<double> m_cyclesCost;

	std::vector<std::vector<std::vector<AML::double3> > > m_cycleNormal;
	std::vector<std::vector<std::vector<std::vector<AML::double3> > > > m_normalsTable;

	PoleGraphNodes m_poleGraphNodes;
	PoleGraphNodeWeight m_poleGraphNodeWeight;
	PoleGraphArcs m_poleGraphArcsWeight;
	PoleGraphArcsPair m_poleGraphArcsMatch;
	ExpandPoleSequence m_expandPoleSequence;

	std::vector<int> m_selectedNodeInPole;

	//parameter changing
	bool m_isUpdatetwistNormNum;
	bool m_isUpdateangleWeight;
	bool m_isUpdatetwistWeight;
	bool m_isUpdatecurveWeight;
	bool m_isUpdatenodeWeight;
	bool m_isUpdatebestNeightboreNum;
	bool m_isUpdaterotationGraphNum;
	bool m_isUpdatedihedralWeight;
	bool m_isUpdatestateNum;
	bool m_isUpdateRoGraphConnect;
	bool m_isUpdateDoCycleBreak;
	bool m_isUpdateWeightTri, m_isUpdateWeightEdge, m_isUpdateWeightBiTri, m_isUpdateWeightTriBd, isUpdateWeightWorsDih;
	bool m_isUpdateSurfaceSmooth;
	bool m_isUpdateCycle;


	//user added constraints;
	std::vector<std::vector<int> > m_selectArcList;
	std::vector<std::vector<std::pair<int, int> > > m_userDefinedPairsInNode;
	std::vector<std::vector<std::pair<int, int> > > m_userDefinedPairsInArc;
	std::vector<int> m_latestUpdateNodes;
	std::vector<int> m_latestUpdateArcs;
	std::vector<int> m_latestUpdateArcsCapacity;
	std::vector<int> m_errorNode;


private:

	void constructJointRotationGraph();
	void constructSegmentRotationGraph();

	AML::double3 rotateNormal(const AML::double3 &normal, const AML::double3 &tangent, const double &angle);
	double twistBisectCostSym(const AML::double3 &e1, const AML::double3 &e2, const AML::double3 &n1);
	void computeTwistTables();
	void constructJointRotationGraphbyPoleGraph();
	void constructSegmentRotationGraphbyPoleGraph();
	void constructExpandSequence();
	void searchMinPoleGraph();
	void searchAlmostMinPoleGraph();
	void updateRotationGraphbyPoleGraph();

	void computeArcCost();
	void computeCycleCost();
	GraphSearch *m_graphSearch;


public:
	static void computeCurveNormal(const std::vector<AML::double3> &org, AML::double3 &tar);
	static void computeTransportMatrix(const std::vector<AML::double3> &org, std::vector<double> &tar);
	static void computeTransportMatrixAll(const std::vector<AML::double3> &org, std::vector<std::vector<double> > &tar);
	static double computeAngle(const AML::double3 &org1, const AML::double3 &org2);

	static double computeAnglebetweenTwoCurves(const int &i, const int &j, const std::vector<Curve> &pointLists);
	static AML::double3 computeNormalPlanebetweenTwoCurves(const int &i, const int &j, const std::vector<Curve> &pointLists);

	wxStatusBar** getStatusBar(){ return &m_statusBar; }

	void constructNetwork();
	void constructRandomRotationGraph();
	void constructRotationGraphbyAngleMetric();
	void constructRotationGraphbyAngleDihedralMetric();
	void constructRotationGraphbyPoleGraph();
	void constructRotationGraphbyPoleGraphEx();
	void constructCycles();
	void cycleBreaking();
};
#endif
