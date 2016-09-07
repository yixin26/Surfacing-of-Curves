#include "CycleDiscovery.h"
#include <cstdlib>
#include <float.h>
#include <cmath>
#include <wx/math.h>
#include <wx/msgdlg.h>
#include <algorithm>
#include <map>

void cycleDiscovery::init()
{
	m_curves.clear();
	m_curveCapacitys.clear();
	m_curveNet.nodes.clear();
	m_curveNet.arcs.clear();

	m_twistNormNum = 30;
	m_angleWeight = 1;
	m_twistWeight = 1;
	m_curveWeight = 0;
	m_nodeWeight = 0;
	m_bestNeightboreNum = 1;
	m_rotationGraphNum = 10;
	m_dihedralWeight = .5;
	m_stateNum = 100;
	m_stateUnLimited = false;
	m_isRoGraphConnect = false;
	m_isDoCycleBreak = true;

	m_twistTables.clear();
	m_twistTablesConfidence.clear();
	m_twistTablesIndex.clear();

	m_poleGraphNodes.clear();
	m_poleGraphNodeWeight.clear();
	m_poleGraphArcsWeight.clear();
	m_poleGraphArcsMatch.clear();
	m_expandPoleSequence.clear();

	m_isDihedral = false;

	m_rotationGraph.clear();
	m_rotationGraphUpdate.clear();

	m_isCycleBreak = false;
	m_isCycleCorrect = true;
	m_cycleSet.clear();
	m_cycleSetBreaked.clear();
	m_breakNum = 0;

	m_cyclesCost.clear();
	m_arcsCost.clear();

	m_normalsTable.clear();
	m_cycleNormal.clear();

}
void cycleDiscovery::constructNetwork()
{
	if (m_curves.empty())
	{
		return;
	}
	if (m_curveCapacitys.empty() || m_curveCapacitys.size() != m_curves.size())
	{
		m_curveCapacitys.resize(m_curves.size(), 2);
	}

	LinearCurveNet& curves = m_curves;

#ifdef SMOOTHINPUT //smooth the curves;
	{
		std::vector<std::vector<AML::double3> > fairCurve = curves;
		for (int times = 0; times<3; times++){
			for (int i = 0; i<curves.size(); i++){
				if (curves[i].size()<3)continue;
				for (int j = 1; j<curves[i].size() - 1; j++){
					AML::double3 point = curves[i][j];
					AML::double3 leftPoint = curves[i][j - 1];
					AML::double3 rightPoint = curves[i][j + 1];
					AML::double3 centrePoint = (leftPoint + rightPoint) / AML::Scalar(2);
					point = (point + centrePoint) / AML::Scalar(2);
					fairCurve[i][j] = point;
				}
			}
			curves = fairCurve;
		}
		fairCurve.clear();
	}
#endif

	Graph net;
	std::vector< int> capacity = m_curveCapacitys;

	std::map<StdPoint, unsigned> mapPointToIndex;

	int index = 0;
	int len = curves.size();
	for (int i = 0; i<len; i++)
	{
		Curve curve = curves[i];
		std::vector<Point> endNodes; endNodes.push_back(curve.front()); endNodes.push_back(curve.back());
		curve.erase(curve.begin());
		int sizeCurve = curve.size();
		curve.erase(curve.begin() + sizeCurve - 1);

		int currentIndex[2];
		for (int j = 0; j<2; j++){
			StdPoint stdPoint(endNodes[j]);
			auto itMap = mapPointToIndex.find(stdPoint);
			if (itMap != mapPointToIndex.end())
			{
				currentIndex[j] = itMap->second;
				net.nodes[currentIndex[j]].arcID.push_back(i);
				net.nodes[currentIndex[j]].arcDirection.push_back(j + 1);
			}
			else
			{
				mapPointToIndex[stdPoint] = index;
				currentIndex[j] = index;
				index++;
				IntersectPoint node;
				node.pos = endNodes[j];
				node.arcID.push_back(i);
				node.arcDirection.push_back(j + 1);
				net.nodes.push_back(node);
			}
		}
		GraphStructure::Arc arc;
		arc.vertexList = curve;
		arc.capacity = capacity[i];
		arc.endNodesID.first = currentIndex[0];
		arc.endNodesID.second = currentIndex[1];
		if (currentIndex[0] == currentIndex[1])
			arc.posInNode.first = net.nodes[currentIndex[0]].arcID.size() - 2;
		else
			arc.posInNode.first = net.nodes[currentIndex[0]].arcID.size() - 1;
		arc.posInNode.second = net.nodes[currentIndex[1]].arcID.size() - 1;
		net.arcs.push_back(arc);
	}
	m_curveNet.nodes.swap(net.nodes);
	m_curveNet.arcs.swap(net.arcs);
}
void cycleDiscovery::computeCurveNormal(const std::vector<AML::double3> &org, AML::double3 &tar)
{
	std::vector<double> weights(org.size() - 1);
	double size = double(org.size());
	for (int i = 0; i<org.size() - 1; i++)
		weights[i] = pow(1 - double(i) / ((size - 1.0) == 0 ? 1 : (size - 1.0)), 5);
	double sum = 0.;
	for (int i = 0; i<weights.size(); i++)
		sum += weights[i];
	for (int i = 0; i<weights.size(); i++)
		weights[i] /= sum;
	std::vector<AML::double3> vector(org.size() - 1);
	for (int i = 0; i<org.size() - 1; i++){
		vector[i] = (org[i + 1] - org[i]);
		vector[i].normalize();
	}
	for (int i = 0; i<3; i++){
		sum = 0;
		for (int j = 0; j<weights.size(); j++)
			sum += weights[j] * vector[j][i];
		tar[i] = sum;
	}
	tar.normalize();
}
void cycleDiscovery::computeTransportMatrix(const std::vector<AML::double3> &org, std::vector<double> &tar)
{
	AML::double3 e1 = (org[1] - org[0]);
	e1.normalize();
	AML::double3 e2 = (org.back() - org[org.size() - 2]);
	e2.normalize();
	AML::double3 randomVector;
	while (true){
		for (int i = 0; i<3; i++)
			randomVector[i] = (double)(rand() % 1000) / 1000.;
		randomVector.normalize();
		if (randomVector.length() != 0 && randomVector != e1 && -randomVector != e1)
			break;
	}
	AML::double3 coord[2];
	coord[0] = (e1.cross(randomVector));
	coord[0].normalize();
	coord[1] = e1.cross(coord[0]);
	coord[1].normalize();
	AML::double3 coordTrans[] = { coord[0], coord[1] };
	std::vector<AML::double3> vector(org.size() - 1);
	for (int i = 0; i<org.size() - 1; i++){
		vector[i] = (org[i + 1] - org[i]);
		vector[i].normalize();
	}
	AML::double3 tangent;
	for (int j = 0; j<vector.size() - 1; j++){
		tangent = (vector[j] + vector[j + 1]);
		tangent.normalize();
		for (int i = 0; i<2; i++){
			coordTrans[i] -= 2 * (coordTrans[i].dot(tangent))*tangent;
			coordTrans[i].normalize();
		}
	}
	AML::double3 leftMatrix[3], rightMatrix[3];
	for (int i = 0; i<3; i++){
		leftMatrix[i].x = e1[i]; leftMatrix[i].y = coord[0][i]; leftMatrix[i].z = coord[1][i];
		rightMatrix[i].x = e2[i]; rightMatrix[i].y = coordTrans[0][i]; rightMatrix[i].z = coordTrans[1][i];
	}
	for (int i = 0; i<3; i++){
		for (int j = 0; j<3; j++){
			tar.push_back(leftMatrix[i].dot(rightMatrix[j]));
		}
	}
}
void cycleDiscovery::computeTransportMatrixAll(const std::vector<AML::double3> &org, std::vector<std::vector<double> >  &tar)
{
	int sizeCurve = org.size();
	int sizeVector = sizeCurve - 1;

	AML::double3 e1 = (org[1] - org[0]);
	e1.normalize();
	AML::double3 randomVector;
	randomVector.x = 1;
	randomVector.y = exp(1.);
	randomVector.z = M_PI;
	while (true){
		if (randomVector.length() != 0 && randomVector != e1 && -randomVector != e1)
			break;
		for (int i = 0; i<3; i++)
			randomVector[i] = (double)(rand() % 1000) / 1000.;
		randomVector.normalize();
	}
	std::vector<AML::double3> coord(2);
	coord[0] = randomVector.cross(e1);
	coord[0].normalize();
	coord[1] = e1.cross(coord[0]);

	std::vector<std::vector<AML::double3> > coordTrans(2);
	coordTrans[0].push_back(coord[0]);
	coordTrans[1].push_back(coord[1]);
	std::vector<AML::double3> pointVector(sizeVector);
	for (int i = 0; i<sizeVector; i++){
		pointVector[i] = org[i + 1] - org[i];
		pointVector[i].normalize();
	}
	AML::double3 tangent;
	for (int j = 0; j<sizeVector - 1; j++){
		tangent = (pointVector[j] + pointVector[j + 1]);
		tangent.normalize();
		AML::double3 tvec = coordTrans[0].back() - 2 * (coordTrans[0].back().dot(tangent))*tangent;
		tvec.normalize();
		coordTrans[0].push_back(tvec);
		tvec = pointVector[j + 1].cross(tvec);
		coordTrans[1].push_back(tvec);
	}

	std::vector<AML::double3> leftMatrix(3);
	for (int i = 0; i<3; i++){
		leftMatrix[i].x = e1[i];
		leftMatrix[i].y = coord[0][i];
		leftMatrix[i].z = coord[1][i];
	}

	for (int v = 0; v<pointVector.size(); v++){
		AML::double3 rightMatrix[3];
		for (int i = 0; i<3; i++){
			rightMatrix[i].x = pointVector[v][i];
			rightMatrix[i].y = coordTrans[0][v][i];
			rightMatrix[i].z = coordTrans[1][v][i];
		}
		std::vector<double> tmax;
		for (int i = 0; i<3; i++){
			for (int j = 0; j<3; j++){
				double temp = leftMatrix[i].dot(rightMatrix[j]);
				tmax.push_back(temp);
			}
		}
		tar.push_back(tmax);
	}
}
double cycleDiscovery::computeAngle(const AML::double3 &org1, const AML::double3 &org2)
{
	AML::double3 ang_str = org1;
	AML::double3 ang_end = org2;
	ang_str.normalize();
	ang_end.normalize();
	double res = ang_str.dot(ang_end);
	res = res < -1.0 ? -1.0 : res;
	res = res > 1.0 ? 1.0 : res;
	return acos(res);
}
double cycleDiscovery::computeAnglebetweenTwoCurves(const int &i, const int &j, const std::vector<Curve> &pointLists)
{
	AML::double3 v[2];
	computeCurveNormal(pointLists[i], v[0]);
	computeCurveNormal(pointLists[j], v[1]);
	return computeAngle(v[0], v[1]);
}
AML::double3 cycleDiscovery::computeNormalPlanebetweenTwoCurves(const int &i, const int &j, const std::vector<Curve> &pointLists)
{
	AML::double3 v[2];
	computeCurveNormal(pointLists[i], v[0]);
	computeCurveNormal(pointLists[j], v[1]);
	return v[0].cross(v[1]);
}

bool mySort1(std::pair<double, std::pair<int, int> > i, std::pair<double, std::pair<int, int> > j)
{
	return (i.first<j.first);
}
void cycleDiscovery::constructJointRotationGraph()
{
	Graph net = m_curveNet;
	RotationGraph roGraph = m_rotationGraph;

	for (int i = 0; i<net.nodes.size(); i++){
		std::vector<int> capacity;
		std::vector<std::pair<double, std::pair<int, int> > > arcsWithWeights;
		std::vector<Curve> pointLists;
		for (int j = 0; j<net.nodes[i].arcID.size(); j++){
			int arcID = net.nodes[i].arcID[j];
			capacity.push_back(net.arcs[arcID].capacity);
			Curve curve = net.arcs[arcID].vertexList;
			curve.push_back(net.nodes[net.arcs[arcID].endNodesID.second].pos);
			curve.insert(curve.begin(), net.nodes[net.arcs[arcID].endNodesID.first].pos);
			if (net.arcs[arcID].endNodesID.second == i)
				reverse(curve.begin(), curve.end());
			pointLists.push_back(curve);
		}
		std::vector<std::vector<int> > indices(capacity.size(), std::vector<int>(capacity.size(), 0));
		for (int j = 0; j<capacity.size() - 1; j++){
			for (int k = j + 1; k<capacity.size(); k++){
				double weight = computeAnglebetweenTwoCurves(j, k, pointLists);
				arcsWithWeights.push_back(std::pair<double, std::pair<int, int> >(weight, std::pair<int, int>(j, k)));
				indices[j][k] = (arcsWithWeights.size() - 1);
				indices[k][j] = indices[j][k];
			}
		}
		std::vector<std::pair<int, int> > usedArcsID;
		std::vector<std::vector<std::pair<int, int> > > allUsedArcsID;
		std::vector<double> nodeWeights;
		if (m_isDihedral){
			std::vector<std::vector<double> > pairArcsWithWeight(arcsWithWeights.size(), std::vector<double>(arcsWithWeights.size(), 0));
			std::vector<AML::double3> norm;

			for (int j = 0; j<arcsWithWeights.size(); j++){
				AML::double3 normal_ = computeNormalPlanebetweenTwoCurves(arcsWithWeights[j].second.first,
					arcsWithWeights[j].second.second, pointLists);
				normal_.normalize();
				norm.push_back(normal_);
			}
			int count = 0;
			for (int a = 0; a<capacity.size(); a++){
				for (int k = 0; k<capacity.size(); k++){
					for (int j = k; j<capacity.size(); j++){
						if (j != a&&k != a){
							count++;
							if (k<a&&a<j){
								AML::double3 v[2];
								v[0] = norm[indices[a][k]];
								v[1] = norm[indices[a][j]];
								pairArcsWithWeight[indices[a][k]][indices[a][j]] =
									computeAngle(v[0], v[1]);
							}
							else{
								AML::double3 v[2];
								v[0] = norm[indices[a][k]];
								v[1] = -norm[indices[a][j]];
								pairArcsWithWeight[indices[a][k]][indices[a][j]] =
									computeAngle(v[0], v[1]);
							}
							pairArcsWithWeight[indices[a][j]][indices[a][k]]
								= pairArcsWithWeight[indices[a][k]][indices[a][j]];
						}
					}
				}
			}
			m_graphSearch->computeGeneralHamiltonGraph(capacity,
				arcsWithWeights, pairArcsWithWeight, 10, true,
				false, allUsedArcsID, nodeWeights);
		}
		else{
			sort(arcsWithWeights.begin(), arcsWithWeights.end(), mySort1);
			m_graphSearch->computeGeneralHamiltonGraph(capacity,
				arcsWithWeights, true, false, allUsedArcsID, nodeWeights);
		}
		usedArcsID = allUsedArcsID.front();
		if (usedArcsID.empty()){
			roGraph.clear();
			return;
		}

		std::vector<std::vector<std::pair<int, int> > > jointRotationGraph;
		for (int j = 0; j<net.nodes[i].arcID.size(); j++){
			std::vector<std::pair<int, int> > emptyNode;
			jointRotationGraph.push_back(emptyNode);
		}
		for (int j = 0; j<usedArcsID.size(); j++){
			int optArcID = usedArcsID[j].first;
			int currentArcID[] = { arcsWithWeights[optArcID].second.first, arcsWithWeights[optArcID].second.second };
			std::vector<std::pair<int, int> > segRotationGraph;
			for (int k = 0; k<usedArcsID[j].second; k++){
				jointRotationGraph[currentArcID[0]].push_back(std::pair<int, int>
					(currentArcID[1], jointRotationGraph[currentArcID[1]].size()));
				jointRotationGraph[currentArcID[1]].push_back(std::pair<int, int>
					(currentArcID[0], jointRotationGraph[currentArcID[0]].size() - 1));
			}
		}
		roGraph.push_back(jointRotationGraph);
	}
	m_rotationGraph = roGraph;
}
void cycleDiscovery::constructSegmentRotationGraph()
{
	if (m_rotationGraph.empty())
		return;

	Graph net = m_curveNet;
	RotationGraph roGraph = m_rotationGraph;
	//minimize twist
	for (int i = 0; i<net.arcs.size(); i++){
		int nodeID[] = { net.arcs[i].endNodesID.first, net.arcs[i].endNodesID.second };
		int arcID[] = { net.arcs[i].posInNode.first, net.arcs[i].posInNode.second };
		Curve pointList = net.arcs[i].vertexList;
		pointList.push_back(net.nodes[nodeID[1]].pos);
		pointList.insert(pointList.begin(), net.nodes[nodeID[0]].pos);
		Curve reversePointList = pointList;
		reverse(reversePointList.begin(), reversePointList.end());
		AML::double3 v[2];
		computeCurveNormal(pointList, v[0]);
		computeCurveNormal(reversePointList, v[1]);

		std::vector<std::vector<int> > adjacentArcID, adjacentArcIDAbsolute;
		adjacentArcID.resize(2); adjacentArcIDAbsolute.resize(2);
		std::vector<std::vector<Curve> > adjacentPointList;
		adjacentPointList.resize(2);
		std::vector<std::vector<AML::double3> >adjacentVectors, adjacentPlanes;
		adjacentVectors.resize(2); adjacentPlanes.resize(2);
		std::vector<std::vector<AML::Scalar> > adjacentAreas;
		adjacentAreas.resize(2);

		for (int j = 0; j<2; j++){
			for (int k = 0; k<net.arcs[i].capacity; k++){
				adjacentArcID[j].push_back(roGraph[nodeID[j]][arcID[j]][k].first);
				adjacentArcIDAbsolute[j].push_back(net.nodes[nodeID[j]].arcID[adjacentArcID[j][k]]);

				Curve curve = net.arcs[adjacentArcIDAbsolute[j][k]].vertexList;
				curve.push_back(net.nodes[net.arcs[adjacentArcIDAbsolute[j][k]].endNodesID.second].pos);
				curve.insert(curve.begin(), net.nodes[net.arcs[adjacentArcIDAbsolute[j][k]].endNodesID.first].pos);
				if (nodeID[j] == net.arcs[adjacentArcIDAbsolute[j][k]].endNodesID.second)
					reverse(curve.begin(), curve.end());
				adjacentPointList[j].push_back(curve);

				AML::double3 normal;
				computeCurveNormal(adjacentPointList[j][k], normal);
				adjacentVectors[j].push_back(normal);

				if (j == 0)
					normal = normal.cross(v[j]);
				else
					normal = v[j].cross(normal);

				if (normal.length() == 0){
					AML::double3 temp(1., 1., 1.);
					normal = temp;
				}
				adjacentAreas[j].push_back(normal.length());
				normal.normalize();
				adjacentPlanes[j].push_back(normal);
			}
		}
		std::vector<double> transportMatrix;
		computeTransportMatrix(pointList, transportMatrix);
		std::vector<AML::double3> transMatrix;
		for (int j = 0; j<3; j++){
			AML::double3 row;
			for (int k = 0; k<3; k++){
				row[k] = AML::Scalar(transportMatrix[j + k * 3]);
			}
			transMatrix.push_back(row);
		}
		std::vector<int> capacity;
		std::vector<std::pair<double, std::pair<int, int> > > arcsWithWeights;
		capacity.assign(2 * net.arcs[i].capacity, 1);
		for (int j = 0; j<net.arcs[i].capacity; j++){
			AML::double3 transPlane, transVector;
			for (int l = 0; l<3; l++){
				//	transVector[l] = adjacentVectors[0][j].dot(transMatrix[l]);
				transPlane[l] = adjacentPlanes[0][j].dot(transMatrix[l]);
			}
			for (int k = 0; k<net.arcs[i].capacity; k++){
				double weight = adjacentAreas[0][j] * adjacentAreas[1][k];
				weight *= computeAngle(transPlane, adjacentPlanes[1][k]);
				weight = pow(weight, 2);
				arcsWithWeights.push_back(std::pair<double, std::pair<int, int> >(weight, std::pair<int, int>(j, k + net.arcs[i].capacity)));
			}
		}
		sort(arcsWithWeights.begin(), arcsWithWeights.end(), mySort1);

		std::vector<std::pair<int, int> > usedArcsID;
		std::vector<std::vector<std::pair<int, int> > > allUsedArcsID;
		std::vector<double> nodeWeights;
		double minCost = m_graphSearch->computeGeneralHamiltonGraph(capacity, arcsWithWeights, false, false, allUsedArcsID, nodeWeights);

		usedArcsID = allUsedArcsID.front();
		std::vector<int> instanceID;
		instanceID.resize(net.arcs[i].capacity);
		for (int j = 0; j<usedArcsID.size(); j++){
			std::pair<int, int> arc = arcsWithWeights[usedArcsID[j].first].second;
			instanceID[arc.first] = arc.second - net.arcs[i].capacity;
		}
		std::vector<std::pair<int, int> > instances;
		for (int j = 0; j<net.arcs[i].capacity; j++){
			instances.push_back(roGraph[nodeID[1]][arcID[1]][instanceID[j]]);
		}
		roGraph[nodeID[1]][arcID[1]] = instances;
		for (int j = 0; j<net.arcs[i].capacity; j++){
			std::pair<int, unsigned int> connectInstance = roGraph[nodeID[1]][arcID[1]][j];
			roGraph[nodeID[1]][connectInstance.first][connectInstance.second]
				= std::pair<int, unsigned int>(arcID[1], j);
		}

	}
	m_rotationGraph = roGraph;
}
void cycleDiscovery::constructRotationGraphbyAngleMetric()
{
	if (m_curveNet.arcs.empty())
		return;

	m_rotationGraph.clear();
	constructJointRotationGraph();
	constructSegmentRotationGraph();
}
void cycleDiscovery::constructRotationGraphbyAngleDihedralMetric()
{
	if (m_curveNet.arcs.empty())
		return;

	m_rotationGraph.clear();
	m_isDihedral = true;
	constructJointRotationGraph();
	constructSegmentRotationGraph();
	m_isDihedral = false;
}

AML::double3 cycleDiscovery::rotateNormal(const AML::double3 &normal, const AML::double3 &axis, const double &angle)
{
	double d = normal.dot(axis);
	double c = cos(angle);
	double s = sin(angle);
	AML::double3 res;
	res.x = axis.x*d*(1 - c) + normal.x*c + (-axis.z*normal.y + axis.y*normal.z)*s;
	res.y = axis.y*d*(1 - c) + normal.y*c + (axis.z*normal.x - axis.x*normal.z)*s;
	res.z = axis.z*d*(1 - c) + normal.z*c + (-axis.y*normal.x + axis.x*normal.y)*s;
	return res;
}
double cycleDiscovery::twistBisectCostSym(const AML::double3 &e1, const AML::double3 &e2, const AML::double3 &n1)
{
	AML::double3 tang = e1 + e2;
	AML::double3 l1 = n1.cross(tang);
	l1.normalize();
	return 2 * computeAngle(-e1, l1);
}
void cycleDiscovery::computeTwistTables()
{
	if (m_curveNet.arcs.empty())
		return;

	int num_nodes = m_curveNet.nodes.size();
	int num_arcs = m_curveNet.arcs.size();
	graph_t g(num_nodes);
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	for (std::size_t i = 0; i < num_arcs; i++) {
		int nodeID[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		graph_traits < graph_t >::edge_descriptor e; bool inserted;
		boost::tie(e, inserted) = add_edge(nodeID[0], nodeID[1], g);
		weightmap[e] = 0;
	}

	/*
	std::vector<bool> isDoubleArc(m_curveNet.arcs.size());
	std::vector<std::vector<int> > tnodes(m_curveNet.arcs.size());
	for(int i=0;i<m_curveNet.arcs.size();i++){
	tnodes[i].push_back(m_curveNet.arcs[i].endNodesID.first);
	tnodes[i].push_back(m_curveNet.arcs[i].endNodesID.second);
	for(int j=0;j<i;j++){
	if((tnodes[i][0] == tnodes[j][0]&& tnodes[i][1] == tnodes[j][1])
	|| (tnodes[i][0] == tnodes[j][1] && tnodes[i][1] == tnodes[j][0])){
	isDoubleArc[i]=isDoubleArc[j]=true;
	}
	}
	}
	*/


	bool connectRequired = true;

	double wAngle = m_angleWeight;
	double wTwist = m_twistWeight;
	double wCurve = m_curveWeight;
	int nTwistNorm = m_twistNormNum;
	//for each arc twist
	m_normalsTable.clear();
	for (int i = 0; i<m_curveNet.arcs.size(); i++){
		/*
		wxString its = wxString::Format(wxT("compute twist table: %ith arc "), i+1);
		m_statusBar->SetStatusText(its,0);
		*/
		std::vector<AML::double3> pts = m_curveNet.arcs[i].vertexList;
		int nodeID[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		pts.push_back(m_curveNet.nodes[nodeID[1]].pos);
		pts.insert(pts.begin(), m_curveNet.nodes[nodeID[0]].pos);

		std::vector<std::vector<int> > adjArcs(2);
		std::vector<std::vector<int> > adjArcsDirection(2);
		std::vector<std::vector<std::vector<AML::double3> > > adjArcsPts(2);
		//		m_statusBar->SetStatusText("prepare adjacent srcs ",1);
		for (int j = 0; j<2; j++){
			adjArcs[j] = m_curveNet.nodes[nodeID[j]].arcID;
			adjArcsDirection[j] = m_curveNet.nodes[nodeID[j]].arcDirection;
			for (int k = 0; k<adjArcs[j].size(); k++){
				int tempArc = adjArcs[j][k];
				int tempNode[] = { m_curveNet.arcs[tempArc].endNodesID.first, m_curveNet.arcs[tempArc].endNodesID.second };
				std::vector<AML::double3> tempPts = m_curveNet.arcs[adjArcs[j][k]].vertexList;
				tempPts.push_back(m_curveNet.nodes[tempNode[1]].pos);
				tempPts.insert(tempPts.begin(), m_curveNet.nodes[tempNode[0]].pos);
				if (adjArcsDirection[j][k] == 2)
					reverse(tempPts.begin(), tempPts.end());
				adjArcsPts[j].push_back(tempPts);
			}
		}

		std::vector<std::vector<AML::double3> > vecs(2);
		for (int j = 0; j<2; j++){
			for (int k = 0; k<adjArcsPts[j].size(); k++){
				AML::double3 vec = adjArcsPts[j][k][1] - adjArcsPts[j][k][0];
				vec.normalize();
				vecs[j].push_back(vec);
			}
		}

		std::vector<AML::double3> edgeVecs;
		for (int j = 0; j<pts.size() - 1; j++){
			AML::double3 vec = pts[j + 1] - pts[j];
			vec.normalize();
			edgeVecs.push_back(vec);
		}

		AML::double3 v[2] = { edgeVecs.front(), edgeVecs.back() };
		/*
		AML::double3 randomVector;
		while(true){
		for(int i=0;i<3;i++)
		randomVector[i] = (double)(rand()%1000)/1000.;
		randomVector.normalize();
		if(randomVector.length()!=0 && randomVector!=v[0])
		break;
		}
		*/

		AML::double3 randomVector;
		randomVector.x = 1;
		randomVector.y = exp(1.);
		randomVector.z = M_PI;

		AML::double3 initNormal = v[0].cross(randomVector);
		initNormal.normalize();

		//test about the matrices;
		std::vector<std::vector<double> > transportMatrices;
		//		m_statusBar->SetStatusText("normal transport ",1);
		computeTransportMatrixAll(pts, transportMatrices);
		std::vector<std::vector<AML::double3> > transMatrices;
		for (int t = 0; t<transportMatrices.size(); t++){
			std::vector<AML::double3> transMatrix;
			for (int j = 0; j<3; j++){
				AML::double3 row;
				for (int k = 0; k<3; k++){
					row[k] = transportMatrices[t][j + k * 3];
				}
				transMatrix.push_back(row);
			}
			transMatrices.push_back(transMatrix);
		}

		std::vector<std::vector<std::vector<double> > > pairwiseTwistTable;
		std::vector<std::vector<std::vector<std::vector<AML::double3> > > >  endNormals;
		std::vector<std::vector<AML::double3> > midNormals;
		for (int j = 0; j<nTwistNorm; j++){
			double angle = (j + 1) * 2 * M_PI / double(nTwistNorm);
			AML::double3 nm = rotateNormal(initNormal, v[0], angle);
			std::vector<AML::double3> nmlist;
			for (int k = 0; k<transMatrices.size(); k++){
				std::vector<AML::double3> transMatrix = transMatrices[k];
				AML::double3 tnm;
				for (int l = 0; l<3; l++){
					tnm[l] = nm.dot(transMatrix[l]);
				}
				nmlist.push_back(tnm);
			}

			midNormals.push_back(nmlist);

			double costAng = 0;
			for (int k = 0; k<edgeVecs.size() - 1; k++){
				double tc = twistBisectCostSym(edgeVecs[k], edgeVecs[k + 1], nmlist[k]);
				costAng += tc;
			}
			costAng -= (pts.size() - 2)*M_PI;

			double costTwt = 0;
			for (int k = 0; k<edgeVecs.size() - 1; k++){
				double tc = computeAngle(nmlist[k], nmlist[k + 1]);
				costTwt += tc;
			}
			double costCurve = wAngle*costAng + wTwist*costTwt;
			costCurve *= wCurve;

			std::vector<std::vector<double> > pairwiseTwist(vecs[0].size(),
				std::vector<double>(vecs[1].size(), 0));

			std::vector<std::vector<std::vector<AML::double3> > > pairNormals(vecs[0].size(),
				std::vector<std::vector<AML::double3> >(vecs[1].size()));

			//		m_statusBar->SetStatusText("compute full twist ",2);
			AML::double3 nm2 = nmlist.back();
			for (int k1 = 0; k1<vecs[0].size(); k1++){
				for (int k2 = 0; k2<vecs[1].size(); k2++){
					AML::double3 reflect = vecs[0][k1] - v[0];
					reflect.normalize();
					AML::double3 nmT = (-nm) - 2 * ((-nm).dot(reflect))*reflect;

					reflect = v[1] + vecs[1][k2];
					reflect.normalize();
					AML::double3 nm2T = nm2 - 2 * (nm2.dot(reflect))*reflect;

					//compute twistBisectCostSym;
					double costAng1 = twistBisectCostSym(-v[0], vecs[0][k1], -nm);
					double costAng2 = twistBisectCostSym(v[1], vecs[1][k2], nm2);

					//comput arc twist;
					double costTwt1 = computeAngle(-nm, nmT);
					double costTwt2 = computeAngle(nm2, nm2T);

					//whole cost;
					pairwiseTwist[k1][k2] = costCurve + wAngle*(costAng1 + costAng2) / 2 +
						wTwist*(costTwt1 + costTwt2) / 2;

					pairNormals[k1][k2].push_back(nmT);
					pairNormals[k1][k2].push_back(nm2T);
				}
			}
			//			m_statusBar->SetStatusText("",3);
			pairwiseTwistTable.push_back(pairwiseTwist);
			endNormals.push_back(pairNormals);
		}
		// make tables;
		/*
		m_statusBar->SetStatusText(" ",2);
		m_statusBar->SetStatusText("make twist table ",1);
		*/

		std::vector<std::vector<std::vector<double> > >tTable(vecs[0].size(),
			std::vector<std::vector<double> >(vecs[1].size(), std::vector<double>(nTwistNorm, 0)));
		for (int k1 = 0; k1<vecs[0].size(); k1++){
			for (int k2 = 0; k2<vecs[1].size(); k2++){
				for (int n = 0; n<nTwistNorm; n++){
					tTable[k1][k2][n] = pairwiseTwistTable[n][k1][k2];
				}
			}
		}
		std::vector<std::vector<double> > twistTable(vecs[0].size(),
			std::vector<double>(vecs[1].size(), 0));
		std::vector<std::vector<int> > twistTableIndex(vecs[0].size(),
			std::vector<int>(vecs[1].size(), 0));
		std::vector<std::vector<double> > twistTableConfidence(vecs[0].size(),
			std::vector<double>(vecs[1].size(), 0));
		std::vector<std::vector<std::vector<AML::double3> > > normalTable(vecs[0].size(),
			std::vector<std::vector<AML::double3> >(vecs[1].size()));
		for (int k1 = 0; k1<vecs[0].size(); k1++){
			for (int k2 = 0; k2<vecs[1].size(); k2++){
				std::vector<double> tc = tTable[k1][k2];
				std::vector<double>::iterator itc = std::min_element(tc.begin(), tc.end());
				twistTable[k1][k2] = *itc;
				int index = itc - tc.begin();
				twistTableIndex[k1][k2] = index;
				std::vector<AML::double3> tnorm = midNormals[index];
				tnorm.insert(tnorm.begin(), endNormals[index][k1][k2][0]);
				tnorm.push_back(endNormals[index][k1][k2][1]);
				normalTable[k1][k2] = tnorm;
				itc = std::max_element(tc.begin(), tc.end());
				twistTableConfidence[k1][k2] = min(1, (*itc - twistTable[k1][k2]) / M_PI);
			}
		}

		if (/*isDoubleArc[i]==false&&*/connectRequired == true){
			//			m_statusBar->SetStatusText("shortest path ",1);
			std::vector<int> sources;
			std::vector<int> targets;
			std::vector<std::pair<int, int> > tind((adjArcs[0].size())*(adjArcs[1].size()));
			for (int j = 0; j<adjArcs[0].size(); j++){
				int snd;
				if (adjArcsDirection[0][j] == 1)
					snd = m_curveNet.arcs[adjArcs[0][j]].endNodesID.second;
				else
					snd = m_curveNet.arcs[adjArcs[0][j]].endNodesID.first;
				if (adjArcs[0][j] == i)
					snd = nodeID[0];
				sources.push_back(snd);
			}
			for (int k = 0; k<adjArcs[1].size(); k++){
				int tnd;
				if (adjArcsDirection[1][k] == 1)
					tnd = m_curveNet.arcs[adjArcs[1][k]].endNodesID.second;
				else
					tnd = m_curveNet.arcs[adjArcs[1][k]].endNodesID.first;
				if (adjArcs[1][k] == i)
					tnd = nodeID[1];
				targets.push_back(tnd);
			}

			graph_traits < graph_t >::edge_descriptor e; bool sus;
			graph_traits < graph_t >::vertex_descriptor vi1, vi2;
			for (int j = 0; j<2; j++){
				for (int k = 0; k<adjArcs[j].size(); k++){
					int nodeID[] = { m_curveNet.arcs[adjArcs[j][k]].endNodesID.first,
						m_curveNet.arcs[adjArcs[j][k]].endNodesID.second };
					vi1 = vertex(nodeID[0], g);
					vi2 = vertex(nodeID[1], g);
					boost::tie(e, sus) = edge(vi1, vi2, g);
					weightmap[e] = 1;
				}
			}
			std::vector<std::vector<bool> > goodPath(sources.size(), std::vector<bool>(targets.size(), true));

			if (sources.size() <= targets.size()){
				for (int j = 0; j<sources.size(); j++){
					int nod = sources[j];

					/*
					wxString iiits = wxString::Format(wxT("source node %ith"), j+1);
					m_statusBar->SetStatusText(iiits,2);
					*/

					std::vector<vertex_descriptor> p(num_nodes);
					std::vector<int> d(num_nodes);
					vertex_descriptor s = vertex(nod, g);
					dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
					for (int k = 0; k<targets.size(); k++){
						if (d[targets[k]] != 0)
							goodPath[j][k] = false;
					}
				}
			}
			else{
				for (int j = 0; j<targets.size(); j++){
					/*
					wxString iiits = wxString::Format(wxT("source node %ith"), j+1);
					m_statusBar->SetStatusText(iiits,2);

					*/
					int nod = targets[j];
					std::vector<vertex_descriptor> p(num_nodes);
					std::vector<int> d(num_nodes);
					vertex_descriptor s = vertex(nod, g);
					dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
					for (int k = 0; k<sources.size(); k++){
						if (d[sources[k]] != 0)
							goodPath[k][j] = false;
					}
				}
			}
			for (int j = 0; j<2; j++){
				for (int k = 0; k<adjArcs[j].size(); k++){
					int nodeID[] = { m_curveNet.arcs[adjArcs[j][k]].endNodesID.first,
						m_curveNet.arcs[adjArcs[j][k]].endNodesID.second };
					vi1 = vertex(nodeID[0], g);
					vi2 = vertex(nodeID[1], g);
					boost::tie(e, sus) = edge(vi1, vi2, g);
					weightmap[e] = 0;
				}
			}
			//			m_statusBar->SetStatusText("update twist table",2);
			for (int j = 0; j<goodPath.size(); j++){
				for (int k = 0; k<goodPath[j].size(); k++){
					if (!goodPath[j][k]){
						if (sources[j] == targets[k] || (sources[j] == nodeID[1] && targets[k] == nodeID[0]))
							continue;
						twistTable[j][k] = FLT_MAX;
					}
				}
			}
		}//end of shortest path.

		m_twistTables.push_back(twistTable);
		m_twistTablesConfidence.push_back(twistTableConfidence);
		m_twistTablesIndex.push_back(twistTableIndex);
		m_normalsTable.push_back(normalTable);
	}
}
void cycleDiscovery::constructJointRotationGraphbyPoleGraph()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty())
		return;

	Graph net = m_curveNet;
	int maxArcPairNum = m_bestNeightboreNum;
	int maxRoGraphNum = m_rotationGraphNum;
	double wNode = m_nodeWeight;
	bool isConnect = m_isRoGraphConnect;

	for (int i = 0; i<net.nodes.size(); i++){
		if (!m_latestUpdateNodes.empty()){
			if (std::find(m_latestUpdateNodes.begin(), m_latestUpdateNodes.end(), i) == m_latestUpdateNodes.end())
				continue;
		}
		/*
		wxString its = wxString::Format(wxT("compute joint rotation graph: %ith node "), i+1);
		m_statusBar->SetStatusText(its);
		*/

		AML::double3 pt = net.nodes[i].pos;
		std::vector<int> arcs = net.nodes[i].arcID;
		std::vector<int> dirs = net.nodes[i].arcDirection;
		std::vector<std::vector<std::pair<double, std::pair<int, int> > > > tab(arcs.size(),
			std::vector<std::pair<double, std::pair<int, int> >>(arcs.size(),
			std::pair<double, std::pair<int, int> >(FLT_MAX, std::pair<int, int>(0, 0))));
		int id[2];
		//make cost table including each arc pairs;
		for (id[0] = 0; id[0]<arcs.size() - 1; id[0]++){
			for (id[1] = id[0] + 1; id[1]<arcs.size(); id[1]++){
				int arcID[] = { arcs[id[0]], arcs[id[1]] };
				int dir[] = { dirs[id[0]], dirs[id[1]] };
				double cost = 0;
				for (int j = 0; j<2; j++){
					std::vector<std::vector<double> > twistTable = m_twistTables[arcID[j]];
					if (dir[j] == 1){
						std::vector<double> tempTwist = twistTable[id[(j + 1) % 2]];
						tempTwist.erase(tempTwist.begin() + net.arcs[arcID[j]].posInNode.second);
						double tc = *std::min_element(tempTwist.begin(), tempTwist.end());
						if (cost >= FLT_MAX || tc >= FLT_MAX)
							cost = FLT_MAX;
						else
							cost += tc;
					}
					else{
						double tc = FLT_MAX;
						for (int t1 = 0; t1<twistTable.size(); t1++){
							if (t1 == net.arcs[arcID[j]].posInNode.first)
								continue;
							if (tc>twistTable[t1][id[(j + 1) % 2]])
								tc = twistTable[t1][id[(j + 1) % 2]];
						}
						if (cost >= FLT_MAX || tc >= FLT_MAX)
							cost = FLT_MAX;
						else
							cost += tc;
					}
				}
				tab[id[0]][id[1]].first = tab[id[1]][id[0]].first = cost;//make it bigger than zero. then we can use zero cost for userdefine arc pairs;
				tab[id[0]][id[1]].second = std::pair<int, int>(id[0], id[1]);
				tab[id[1]][id[0]].second = std::pair<int, int>(id[1], id[0]);
			}
		}

		for (int j = 0; j<arcs.size(); j++)
			sort(tab[j].begin(), tab[j].end(), mySort1);

		std::vector<int> capacity;
		std::vector<std::pair<int, int> > allArcs;
		std::vector<double> weights;

		for (int j = 0; j<net.nodes[i].arcID.size(); j++){
			int arcID = net.nodes[i].arcID[j];
			capacity.push_back(net.arcs[arcID].capacity);
		}
		std::vector<std::vector<std::pair<int, double> > > rank(arcs.size(), std::vector<std::pair<int, double> >(arcs.size(),
			std::pair<int, double>(0, 0)));
		for (int j = 0; j<arcs.size(); j++){
			for (int k = 0; k<arcs.size(); k++){
				id[0] = tab[j][k].second.first;
				id[1] = tab[j][k].second.second;
				if (id[0] == id[1]) id[0] = id[1] = j;
				rank[id[0]][id[1]].first = k;
				rank[id[0]][id[1]].second = tab[j][k].first;
			}
		}
		//set up arc pairs and weights and capacities;
		std::vector<std::pair<double, std::pair<int, int> > > arcsWithWeights;
		for (int j = 0; j<arcs.size() - 1; j++){
			for (int k = j + 1; k<arcs.size(); k++){
				if (rank[j][k].second == FLT_MAX)
					continue;
				if ((rank[j][k].first<(capacity[j] + maxArcPairNum)) || (rank[k][j].first<(capacity[k] + maxArcPairNum))){
					std::pair<double, std::pair<int, int> > tarcpair;
					tarcpair.first = rank[j][k].second;
					tarcpair.second = std::pair<int, int>(j, k);
					arcsWithWeights.push_back(tarcpair);
					std::pair<int, int> temp; temp.first = j; temp.second = k;
					allArcs.push_back(temp);
					weights.push_back(rank[j][k].second);
				}
			}
		}
		//add constraints 1)decrease capacity;
		std::vector<std::pair<int, int> > userDefinePairs = m_userDefinedPairsInNode[i];
		for (int j = 0; j<userDefinePairs.size(); j++){
			int u1 = userDefinePairs[j].first;
			int u2 = userDefinePairs[j].second;
			capacity[u1]--; capacity[u2]--;
		}
		if (*std::min_element(capacity.begin(), capacity.end())<0){
			m_poleGraphNodes.clear();
			m_poleGraphNodeWeight.clear();
			m_poleGraphArcsWeight.clear();
			m_poleGraphArcsMatch.clear();
			m_selectArcList.clear();
			m_selectArcList.resize(1);
			m_userDefinedPairsInArc.clear();
			m_userDefinedPairsInNode.clear();
			wxMessageBox(_("\nThe number of constraint in an arc is larger than it's capacity."), _("Warn"), wxID_OK | wxICON_INFORMATION);
		}

		// graph search;
		std::vector<std::vector<std::pair<int, int> > > allUsedArcsID;
		std::vector<double> nodeWeights;
		userDefinePairs.clear();
		m_graphSearch->computeGeneralHamiltonGraph(capacity, allArcs, weights, maxRoGraphNum, isConnect, userDefinePairs, allUsedArcsID, nodeWeights);

		if (allUsedArcsID.empty()){
			m_poleGraphNodes.clear();
			m_poleGraphNodeWeight.clear();
			m_poleGraphArcsWeight.clear();
			m_poleGraphArcsMatch.clear();
			m_selectArcList.clear();
			m_selectArcList.resize(1);
			m_userDefinedPairsInArc.clear();
			m_userDefinedPairsInNode.clear();
			wxMessageBox(_("\nNo Rotation Graph Exist.\n\t\nAllow disconnectivity or \nincrease neighbores number."), _("Warn"), wxID_OK | wxICON_INFORMATION);
			return;
		}

		//use constraints 2)add arc pairs;
		userDefinePairs = m_userDefinedPairsInNode[i];
		std::vector<int> ind;
		for (int j = 0; j<userDefinePairs.size(); j++){
			int u1 = userDefinePairs[j].first;
			int u2 = userDefinePairs[j].second;
			bool isfind = false;
			for (int t = 0; t<allArcs.size(); t++){
				int r1 = allArcs[t].first;
				int r2 = allArcs[t].second;
				if ((r1 == u1&&r2 == u2) || (r1 == u2&&r2 == u1)){
					ind.push_back(t);
					isfind = true; break;
				}
			}
			if (isfind == false){
				allArcs.push_back(userDefinePairs[j]);
				ind.push_back(allArcs.size() - 1);
			}
		}
		for (int j = 0; j<userDefinePairs.size(); j++){
			int u1 = userDefinePairs[j].first;
			int u2 = userDefinePairs[j].second;
			for (int t = 0; t<allUsedArcsID.size(); t++){
				bool isfind = false;
				for (int k = 0; k<allUsedArcsID[t].size(); k++){
					int r1 = allArcs[allUsedArcsID[t][k].first].first;
					int r2 = allArcs[allUsedArcsID[t][k].first].second;

					if ((r1 == u1&&r2 == u2) || (r1 == u2&&r2 == u1)){
						allUsedArcsID[t][k].second++; isfind = true; break;
					}
				}
				if (isfind == false){
					std::pair<int, int> temp; temp.first = ind[j]; temp.second = 1;
					allUsedArcsID[t].push_back(temp);
				}
			}
		}

		//convert arc pairs to rotation graph;
		for (int j = 0; j<nodeWeights.size(); j++)
			nodeWeights[j] *= wNode;

		RotationGraph poleGraphNode;
		for (int a = 0; a<allUsedArcsID.size(); a++){
			std::vector<std::vector<std::pair<int, int> > > jointRotationGraph;
			for (int j = 0; j<net.nodes[i].arcID.size(); j++){
				std::vector<std::pair<int, int> > emptyNode;
				jointRotationGraph.push_back(emptyNode);
			}
			std::vector<std::pair<int, int> > usedArcsID = allUsedArcsID[a];
			for (int j = 0; j<usedArcsID.size(); j++){
				int optArcID = usedArcsID[j].first;
				int currentArcID[] = { allArcs[optArcID].first, allArcs[optArcID].second };
				std::vector<std::pair<int, int> > segRotationGraph;
				for (int k = 0; k<usedArcsID[j].second; k++){
					std::pair<int, int> temp; temp.first = currentArcID[1]; temp.second = jointRotationGraph[currentArcID[1]].size();
					jointRotationGraph[currentArcID[0]].push_back(temp);
					temp.first = currentArcID[0]; temp.second = jointRotationGraph[currentArcID[0]].size() - 1;
					jointRotationGraph[currentArcID[1]].push_back(temp);
				}
			}
			poleGraphNode.push_back(jointRotationGraph);
		}
		if (m_poleGraphNodes.size() <= i){
			m_poleGraphNodes.push_back(poleGraphNode);
			m_poleGraphNodeWeight.push_back(nodeWeights);
		}
		else{
			m_poleGraphNodes[i] = poleGraphNode;
			m_poleGraphNodeWeight[i] = nodeWeights;
		}
	}
}

std::vector<std::vector<int> > ordering(const std::vector<std::vector<double> > &data)
{
	std::vector<std::vector<int> > rank(data.size(), std::vector<int>(data.size(), 0));
	for (int i = 0; i<data.size(); i++){
		std::vector<double> tdat = data[i];
		for (int j = 0; j<tdat.size(); j++){
			tdat[j] /= 2.;
		}
		for (int j = 0; j<tdat.size(); j++){
			int minInd = j;
			for (int k = 0; k<tdat.size(); k++){
				if (tdat[minInd]>tdat[k])
					minInd = k;
			}
			rank[i][minInd] = j;
			tdat[minInd] = FLT_MAX;
		}
	}
	return rank;
}
std::vector<int> ordering(const std::vector<int> &data)
{
	std::vector<int> rank(data.size());
	std::vector<int> tdat = data;
	for (int j = 0; j<tdat.size(); j++){
		int minInd = j;
		for (int k = 0; k<tdat.size(); k++){
			if (tdat[minInd]>tdat[k])
				minInd = k;
		}
		rank[minInd] = j;
		tdat[minInd] = INT_MAX;
	}
	return rank;
}
double getDihedralCostIndexed(const std::vector<int> &ns, int &twistNormNum)
{
	std::vector<int> ind = ns;
	sort(ind.begin(), ind.end());
	int len = ind.size();
	ind.push_back(ind.front());
	double c = 0;
	double a = 2 * M_PI / twistNormNum;
	double expDih = 2;
	for (int i = 0; i<len; i++){
		int d = abs(ind[i] - ind[i + 1]);
		double tc;
		if ((d * 2)>twistNormNum)
			tc = a*(twistNormNum - d);
		else
			tc = a*d;
		c += pow(M_PI - tc, expDih);	//dihedral between normal i and i+1 when expDih=1;	
	}
	return pow(c / len, 1 / expDih)*len - (len - 2)*M_PI;
}
void cycleDiscovery::constructSegmentRotationGraphbyPoleGraph()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty())
		return;

	Graph net = m_curveNet;
	double wDihedral = m_dihedralWeight;
	int nTwistNorm = m_twistNormNum;


	//minimize twist
	std::sort(m_latestUpdateArcs.begin(), m_latestUpdateArcs.end());
	for (int i = 1; i<m_latestUpdateArcs.size(); i++){
		if (m_latestUpdateArcs[i] == m_latestUpdateArcs[i - 1]){
			m_latestUpdateArcs.erase(m_latestUpdateArcs.begin() + i);
			i--;
		}
	}
	for (int i = 0; i<net.arcs.size(); i++){
		if (!m_latestUpdateArcs.empty() && std::find(m_latestUpdateArcs.begin(), m_latestUpdateArcs.end(), i) == m_latestUpdateArcs.end())
			continue;

		/*
		wxString its = wxString::Format(wxT("compute arc rotation graph: %ith arc "), i+1);
		m_statusBar->SetStatusText(its);
		*/

		int nodeID[] = { net.arcs[i].endNodesID.first, net.arcs[i].endNodesID.second };
		int arcID[] = { net.arcs[i].posInNode.first, net.arcs[i].posInNode.second };
		int cap = net.arcs[i].capacity;

		std::vector<std::vector<double> > twistTable = m_twistTables[i];
		std::vector<std::vector<int> > twistTableIndex = m_twistTablesIndex[i];
		std::vector<std::vector<double> > twistTableConfidence = m_twistTablesConfidence[i];

		std::vector<std::vector<std::vector<std::pair<int, int> > > > matches(m_poleGraphNodes[nodeID[0]].size(),
			std::vector<std::vector<std::pair<int, int> > >(m_poleGraphNodes[nodeID[1]].size()));
		std::vector<std::vector<double> > poleGraphArcWeight(m_poleGraphNodes[nodeID[0]].size(), std::vector<double>(m_poleGraphNodes[nodeID[1]].size(), 0));

		for (int k1 = 0; k1<poleGraphArcWeight.size(); k1++){
			for (int k2 = 0; k2<poleGraphArcWeight[k1].size(); k2++){

				std::vector<std::pair<int, int> > preArcs = m_userDefinedPairsInArc[i];
				if (preArcs.size()>cap){
					m_poleGraphNodes.clear();
					m_poleGraphNodeWeight.clear();
					m_poleGraphArcsWeight.clear();
					m_poleGraphArcsMatch.clear();
					m_selectArcList.clear();
					m_selectArcList.resize(1);
					m_userDefinedPairsInArc.clear();
					m_userDefinedPairsInNode.clear();
					wxMessageBox(_("Too many constraints."), _("Warn"), wxID_OK | wxICON_INFORMATION);
					return;
				}

				std::vector<std::pair<int, int> > preArcsNew;
				std::vector<std::vector<std::pair<int, int> > > rot1 = m_poleGraphNodes[nodeID[0]][k1];
				std::vector<std::vector<std::pair<int, int> > > rot2 = m_poleGraphNodes[nodeID[1]][k2];

				std::vector<std::vector<double> > costs(cap, std::vector<double>(cap));
				std::vector<std::vector<double> > tvars(cap, std::vector<double>(cap));
				std::vector<std::vector<int> > tnmids(cap, std::vector<int>(cap));
				std::vector<bool> useListL(cap, false);
				std::vector<bool> useListR(cap, false);
				for (int j = 0; j<cap; j++){
					for (int k = 0; k<cap; k++){
						if (useListL[j] == true || useListR[k] == true)
							continue;

						int inst1 = rot1[arcID[0]][j].first;
						int inst2 = rot2[arcID[1]][k].first;
						costs[j][k] = twistTable[inst1][inst2];
						tvars[j][k] = twistTableConfidence[inst1][inst2];
						tnmids[j][k] = twistTableIndex[inst1][inst2];


						for (int p = 0; p<preArcs.size(); p++){
							if (preArcs[p].first == inst1 && preArcs[p].second == inst2){
								preArcs.erase(preArcs.begin() + p);
								std::pair<int, int> temp; temp.first = j; temp.second = k;
								preArcsNew.push_back(temp);

								useListL[j] = true; useListR[k] = true;
								break;
							}
						}
					}
				}
				if (!preArcs.empty()){
					continue;
				}
				/*
				if(preArcsNew.size()==cap){
				std::vector<std::pair<int,int> > optpairs(cap);
				for(int j=0;j<cap;j++){
				optpairs[j].first=cap*j+preArcsNew[j].second;
				optpairs[j].second =1;
				}
				poleGraphArcWeight[k1][k2] = 0;
				matches[k1][k2] = optpairs;
				continue;
				}
				*/
				std::vector<std::vector<double> > transposeCosts(cap, std::vector<double>(cap));
				for (int j = 0; j<cap; j++){
					for (int k = 0; k<cap; k++){
						transposeCosts[j][k] = costs[k][j];
					}
				}
				std::vector<std::pair<int, int> > trasposePairs;
				for (int j = 0; j<preArcsNew.size(); j++){
					std::pair<int, int> temp; temp.first = preArcsNew[j].second; temp.second = preArcsNew[j].first;
					trasposePairs.push_back(temp);
				}

				std::vector<std::vector<int> > firstGroup = ordering(costs);
				std::vector<std::vector<int> > secondGroup = ordering(transposeCosts);

				std::vector<int> match1;
				std::vector<int> match2;
				m_graphSearch->stableMatching(firstGroup, secondGroup, preArcsNew, match1);
				m_graphSearch->stableMatching(secondGroup, firstGroup, trasposePairs, match2);

				double cost1, cost2;
				cost1 = cost2 = 0;
				for (int j = 0; j<cap; j++){
					cost1 += costs[j][match1[j]];
					cost2 += transposeCosts[j][match2[j]];
				}

				std::vector<int> optmatch;
				std::vector<std::pair<int, int> > optpairs(cap);
				double optcost;

				if (cost1<cost2){
					optmatch = match1;
					optcost = cost1;
					for (int j = 0; j<cap; j++){
						optpairs[j].first = cap*j + match1[j];
						optpairs[j].second = 1;
					}
				}
				else{
					optmatch = ordering(match2);
					optcost = cost2;
					for (int j = 0; j<cap; j++){
						optpairs[j].first = cap*match2[j] + j;
						optpairs[j].second = 1;
					}
				}

				double var = 0;
				std::vector<int> ns;
				for (int j = 0; j<cap; j++){
					var += tvars[j][optmatch[j]];
					ns.push_back(tnmids[j][optmatch[j]]);
				}
				var /= cap;

				optcost += wDihedral*(var*getDihedralCostIndexed(ns, nTwistNorm) + (1 - var)*M_PI)*(cap - 1);

				if (optcost>FLT_MAX)
					optcost = FLT_MAX;
				poleGraphArcWeight[k1][k2] = optcost;
				matches[k1][k2] = optpairs;
			}
		}
		if (poleGraphArcWeight.empty()){
			m_poleGraphNodes.clear();
			m_poleGraphNodeWeight.clear();
			m_poleGraphArcsWeight.clear();
			m_poleGraphArcsMatch.clear();
			m_selectArcList.clear();
			m_selectArcList.resize(1);
			m_userDefinedPairsInArc.clear();
			m_userDefinedPairsInNode.clear();
			wxMessageBox(_("\nNo Rotation Graph Exist.\n\t\nCheck Parameters/Capacity/Constraint."), _("Warn"), wxID_OK | wxICON_INFORMATION);
			return;
		}

		if (m_poleGraphArcsWeight.size() <= i){
			m_poleGraphArcsWeight.push_back(poleGraphArcWeight);
			m_poleGraphArcsMatch.push_back(matches);
		}
		else{
			m_poleGraphArcsWeight[i] = poleGraphArcWeight;
			m_poleGraphArcsMatch[i] = matches;
		}
	}
}

void cycleDiscovery::constructExpandSequence()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty() || m_poleGraphArcsWeight.empty())
		return;

	int numOfPole = m_poleGraphNodeWeight.size();

	m_expandPoleSequence.clear();
	m_selectedNodeInPole.clear();

	std::vector<std::vector<int> > adjcentPoles(numOfPole);
	for (int i = 0; i<m_curveNet.arcs.size(); i++){
		int nodeID[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		adjcentPoles[nodeID[0]].push_back(nodeID[1]);
		adjcentPoles[nodeID[1]].push_back(nodeID[0]);
	}
	std::vector<int> unvisitedNeighboreNum(numOfPole);
	for (int i = 0; i<numOfPole; i++)
		unvisitedNeighboreNum[i] = adjcentPoles[i].size();

	std::vector<int> unvisitedNodes(numOfPole);
	for (int i = 0; i<numOfPole; i++)
		unvisitedNodes[i] = i;

	std::vector<long long> frontNodes;

	for (int i = 0; i<numOfPole; i++){
		ExpandPole currentExpandPole;
		long long minStateNum = LLONG_MAX;
		int selectedPole = 0;
		int selectedPoleIndexInUnvisitedNodes = 0;
		std::vector<long long> newFront;
		std::vector<int> newUnvisitedNeighboreNum;

		for (int j = 0; j<unvisitedNodes.size(); j++){
			std::vector<int> tempUnvisitedNeighboreNum = unvisitedNeighboreNum;
			int currentPole = unvisitedNodes[j];
			for (int k = 0; k<adjcentPoles[currentPole].size(); k++){
				tempUnvisitedNeighboreNum[adjcentPoles[currentPole][k]]--;
			}
			std::vector<long long> tempFrontNodes = frontNodes;
			tempFrontNodes.push_back(currentPole);
			std::vector<long long> tempFrontNodes2;
			long long stateNum = 1;
			for (int k = 0; k<tempFrontNodes.size(); k++){
				if (tempUnvisitedNeighboreNum[tempFrontNodes[k]]>0){
					tempFrontNodes2.push_back(tempFrontNodes[k]);
					stateNum *= m_poleGraphNodeWeight[tempFrontNodes[k]].size();
				}
			}
			if (stateNum <0){
				m_statusBar->SetStatusText("over flow error", 5);
			}
			if (stateNum<minStateNum){
				minStateNum = stateNum;
				selectedPole = currentPole;
				selectedPoleIndexInUnvisitedNodes = j;
				newFront = tempFrontNodes2;
				newUnvisitedNeighboreNum = tempUnvisitedNeighboreNum;
			}
		}

		frontNodes = newFront;
		unvisitedNeighboreNum = newUnvisitedNeighboreNum;
		unvisitedNodes.erase(unvisitedNodes.begin() + selectedPoleIndexInUnvisitedNodes);
		currentExpandPole.nodeID = selectedPole;
		currentExpandPole.front = frontNodes;
		currentExpandPole.stateNum = minStateNum;

		m_expandPoleSequence.push_back(currentExpandPole);
	}
}
void getBases(const std::vector<long long> &org, std::vector<long long> &tar)
{
	if (org.empty())
		return;
	tar = org;
	tar.erase(tar.begin());
	tar.push_back(1);
	for (int i = tar.size() - 2; i >= 0; i--)
		tar[i] *= tar[i + 1];
}
long long indicesToIndex(const std::vector<long long> &org, const std::vector<long long> &org2)
{
	if (org.empty())
		return 0;
	long long index = 0;
	for (int i = 0; i<org.size(); i++){
		index += org[i] * org2[i];
	}
	return index;
}
void indexToIndices(const long long &org, const std::vector<long long> &org2, std::vector<long long> &tar)
{
	if (org2.empty())
		return;
	tar = org2;
	long long temp = org;
	for (int i = 0; i<tar.size(); i++){
		long long temp2 = temp%tar[i];
		tar[i] = temp / tar[i];
		temp = temp2;
	}
}
std::vector<int> getMinValuesPos(const std::vector<double> &data, const int &n)
{
	int len = data.size();
	std::vector<int> ind;
	for (int i = 0; i<len; i++){
		int count = 0;
		for (int j = 0; j<len; j++){
			if (data[i]<data[j])
				count++;
		}
		if (count >= (len - n))
			ind.push_back(i);
		count = 0;
	}
	return ind;
}
void cycleDiscovery::searchAlmostMinPoleGraph()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty()
		|| m_poleGraphArcsWeight.empty() || m_expandPoleSequence.empty())
		return;

	int numOfPole = m_poleGraphNodeWeight.size();

	long long nState = m_stateNum;
	std::vector<std::vector<int> > adjcentPoles(numOfPole);
	std::vector<std::vector<std::pair<int, int> > > adjcentArcs(numOfPole);
	for (int i = 0; i<m_curveNet.arcs.size(); i++){
		int nodeID[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		adjcentPoles[nodeID[0]].push_back(nodeID[1]);
		adjcentPoles[nodeID[1]].push_back(nodeID[0]);
		std::pair<int, int> temp; temp.first = i; temp.second = 0;
		adjcentArcs[nodeID[0]].push_back(temp);
		temp.second = 1;
		adjcentArcs[nodeID[1]].push_back(temp);
	}
	std::vector<bool> visitedPole(numOfPole, false);
	std::vector<int> numOfNode(numOfPole);
	for (int i = 0; i<numOfPole; i++)
		numOfNode[i] = m_poleGraphNodeWeight[i].size();

	/*
	std::vector<std::vector<double> > costs(numOfPole);
	std::vector<std::vector<int> > prevs(numOfPole);
	std::vector<std::vector<int> > nodes(numOfPole);
	//the size of state may rise up to millions,that would fail.
	for(int i=0;i<numOfPole;i++){
	costs[i].resize(m_expandPoleSequence[i].stateNum,FLT_MAX);
	prevs[i].resize(m_expandPoleSequence[i].stateNum,-1);
	nodes[i].resize(m_expandPoleSequence[i].stateNum,-1);
	}

	*/
	std::vector<std::vector<long long> > stateInd(numOfPole); //long long is _int64. Crash happens when a state index is larger than 2^64-1;
	std::vector<std::vector<double> > costsVal(numOfPole);
	std::vector<std::vector<long long> > prevsVal(numOfPole);
	std::vector<std::vector<int> > nodesVal(numOfPole);

	for (int i = 0; i<numOfPole; i++){
		int tsn = min(nState, m_expandPoleSequence[i].stateNum);
		stateInd[i].resize(tsn, -1);			costsVal[i].resize(tsn, FLT_MAX);
		nodesVal[i].resize(tsn, -1);			prevsVal[i].resize(tsn, -1);
	}

	std::vector<long long> frontNodes = m_expandPoleSequence[0].front;
	int pole = m_expandPoleSequence[0].nodeID;
	visitedPole[pole] = true;
	/*
	costs[0] = m_poleGraphNodeWeight[pole];
	for(int i=0;i<nodes[0].size();i++)
	nodes[0][i]=i;
	*/
	//init first front state;
	std::vector<int> ind = getMinValuesPos(m_poleGraphNodeWeight[pole], stateInd[0].size());
	for (int i = 0; i<stateInd[0].size(); i++){
		stateInd[0][i] = double(ind[i]);
		costsVal[0][i] = m_poleGraphNodeWeight[pole][ind[i]];
		nodesVal[0][i] = ind[i];
	}

	std::vector<long long> bases;
	std::vector<long long> frontSize = frontNodes;
	for (int i = 0; i<frontSize.size(); i++)
		frontSize[i] = numOfNode[frontSize[i]];
	getBases(frontSize, bases);

	std::vector<long long> statelist = stateInd[0];
	/*
	statelist.push_back(0);
	*/
	for (int i = 1; i<numOfPole; i++){
		/*
		wxString its = wxString::Format(wxT("pole graph searching: %ith iteration "), i+1);
		m_statusBar->SetStatusText(its);
		*/
		std::vector<long long> prevfrontNodes = frontNodes;
		std::vector<long long> prevbases = bases;
		frontNodes = m_expandPoleSequence[i].front;
		pole = m_expandPoleSequence[i].nodeID;
		frontSize = frontNodes;
		for (int j = 0; j<frontSize.size(); j++)
			frontSize[j] = numOfNode[frontSize[j]];
		getBases(frontSize, bases);

		std::vector<long long> pos = frontNodes;
		std::vector<long long> temp = prevfrontNodes;
		temp.push_back(pole);
		for (int j = 0; j<pos.size(); j++){
			for (int k = 0; k<temp.size(); k++){
				if (pos[j] == temp[k]){
					pos[j] = k; break;
				}
			}
		}
		std::vector<int> prevfrontind(numOfPole, -1);
		for (int j = 0; j<prevfrontNodes.size(); j++)
			prevfrontind[prevfrontNodes[j]] = j;

		std::vector<std::pair<int, int> > tarcs;
		std::vector<int> tpoles;
		for (int j = 0; j<adjcentPoles[pole].size(); j++){
			if (visitedPole[adjcentPoles[pole][j]] == true){
				tarcs.push_back(adjcentArcs[pole][j]);
				tpoles.push_back(prevfrontind[adjcentPoles[pole][j]]);
			}
		}

		/*
		std::vector<double> tempcosts(nState,FLT_MAX);
		std::vector<int> tempstates(nState,-1);
		*/
		std::vector<double> tempcosts = costsVal[i];
		std::vector<long long> tempstates = stateInd[i];
		std::vector<long long> tempprevs = prevsVal[i];
		std::vector<int> tempnodes = nodesVal[i];

		for (int s = 0; s<statelist.size(); s++){
			long long state = statelist[s];
			if (state<0) continue;
			int stateid = s;
			std::vector<long long> previnds;
			indexToIndices(state, prevbases, previnds);

			for (int n = 0; n<numOfNode[pole]; n++){
				double c = costsVal[i - 1][stateid] + m_poleGraphNodeWeight[pole][n];
				for (int j = 0; j<tarcs.size(); j++){
					if (tarcs[j].second == 0){
						if (c >= FLT_MAX || m_poleGraphArcsWeight[tarcs[j].first][n][previnds[tpoles[j]]] >= FLT_MAX)
							c = FLT_MAX;
						else
							c += m_poleGraphArcsWeight[tarcs[j].first][n][previnds[tpoles[j]]];
					}
					else{
						if (c >= FLT_MAX || m_poleGraphArcsWeight[tarcs[j].first][previnds[tpoles[j]]][n] >= FLT_MAX)
							c = FLT_MAX;
						else
							c += m_poleGraphArcsWeight[tarcs[j].first][previnds[tpoles[j]]][n];
					}
				}
				std::vector<long long> indices;
				previnds.push_back(long long(n));
				for (int j = 0; j<pos.size(); j++){
					indices.push_back(previnds[pos[j]]);
				}
				previnds.pop_back();
				long long index = indicesToIndex(indices, bases);

				std::vector<long long>::iterator pSt = std::find(tempstates.begin(), tempstates.end(), index);
				if (pSt == tempstates.end()){
					std::vector<double>::iterator imax = std::max_element(tempcosts.begin(), tempcosts.end());
					int maxid = imax - tempcosts.begin();
					if (c<*imax){
						tempcosts[maxid] = c;
						tempstates[maxid] = index;
						tempprevs[maxid] = state;
						tempnodes[maxid] = n;
					}

					if (c >= FLT_MAX && tempcosts[maxid] >= FLT_MAX){
						tempcosts[maxid] = 100000;
						tempstates[maxid] = index;
						tempprevs[maxid] = state;
						tempnodes[maxid] = n;
					}
				}
				else{
					if (tempcosts[pSt - tempstates.begin()] > c){
						int ti = pSt - tempstates.begin();
						tempcosts[ti] = c;
						tempstates[ti] = index;
						tempprevs[ti] = state;
						tempnodes[ti] = n;
					}
				}
			}
		}
		visitedPole[pole] = true;//Very important! when put before get tars&tpoles, and in multi self-loop in the same pole case, it crash...
		costsVal[i] = tempcosts;
		stateInd[i] = tempstates;
		prevsVal[i] = tempprevs;
		nodesVal[i] = tempnodes;
		statelist = tempstates;
	}
	std::vector<int> res(numOfPole);
	long long state = 0;

	for (int i = numOfPole - 1; i >= 0; i--){
		std::vector<long long>::iterator pstate = std::find(stateInd[i].begin(), stateInd[i].end(), state);
		int stateid = pstate - stateInd[i].begin();
		res[m_expandPoleSequence[i].nodeID] = nodesVal[i][stateid];
		state = prevsVal[i][stateid];
	}

	m_selectedNodeInPole = res;
}
void cycleDiscovery::searchMinPoleGraph()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty()
		|| m_poleGraphArcsWeight.empty() || m_expandPoleSequence.empty())
		return;

	int numOfPole = m_poleGraphNodeWeight.size();

	std::vector<std::vector<int> > adjcentPoles(numOfPole);
	std::vector<std::vector<std::pair<int, int> > > adjcentArcs(numOfPole);
	for (int i = 0; i<m_curveNet.arcs.size(); i++){
		int nodeID[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		adjcentPoles[nodeID[0]].push_back(nodeID[1]);
		adjcentPoles[nodeID[1]].push_back(nodeID[0]);
		std::pair<int, int> temp; temp.first = i; temp.second = 0;
		adjcentArcs[nodeID[0]].push_back(temp);
		temp.second = 1;
		adjcentArcs[nodeID[1]].push_back(temp);
	}
	std::vector<bool> visitedPole(numOfPole, false);
	std::vector<int> numOfNode(numOfPole);
	for (int i = 0; i<numOfPole; i++)
		numOfNode[i] = m_poleGraphNodeWeight[i].size();

	std::vector<std::vector<double> > costs(numOfPole);
	std::vector<std::vector<int> > prevs(numOfPole);
	std::vector<std::vector<int> > nodes(numOfPole);
	for (int i = 0; i<numOfPole; i++){
		costs[i].resize(m_expandPoleSequence[i].stateNum, FLT_MAX);
		prevs[i].resize(m_expandPoleSequence[i].stateNum, -1);
		nodes[i].resize(m_expandPoleSequence[i].stateNum, -1);
	}

	std::vector<long long> frontNodes = m_expandPoleSequence[0].front;
	int pole = m_expandPoleSequence[0].nodeID;
	visitedPole[pole] = true;
	costs[0] = m_poleGraphNodeWeight[pole];
	for (int i = 0; i<nodes[0].size(); i++)
		nodes[0][i] = i;

	std::vector<long long> bases;
	std::vector<long long> frontSize = frontNodes;
	for (int i = 0; i<frontSize.size(); i++)
		frontSize[i] = numOfNode[frontSize[i]];
	getBases(frontSize, bases);

	for (int i = 1; i<numOfPole; i++){
		std::vector<long long> prevfrontNodes = frontNodes;
		std::vector<long long> prevbases = bases;
		frontNodes = m_expandPoleSequence[i].front;
		pole = m_expandPoleSequence[i].nodeID;
		visitedPole[pole] = true;
		frontSize = frontNodes;
		for (int j = 0; j<frontSize.size(); j++)
			frontSize[j] = numOfNode[frontSize[j]];
		getBases(frontSize, bases);

		std::vector<long long> pos = frontNodes;
		std::vector<long long> temp = prevfrontNodes;
		temp.push_back(pole);
		for (int j = 0; j<pos.size(); j++){
			for (int k = 0; k<temp.size(); k++){
				if (pos[j] == temp[k]){
					pos[j] = k; break;
				}
			}
		}
		std::vector<int> prevfrontind(numOfPole, -1);
		for (int j = 0; j<prevfrontNodes.size(); j++)
			prevfrontind[prevfrontNodes[j]] = j;

		std::vector<std::pair<int, int> > tarcs;
		std::vector<int> tpoles;
		for (int j = 0; j<adjcentPoles[pole].size(); j++){
			if (visitedPole[adjcentPoles[pole][j]] == true){
				tarcs.push_back(adjcentArcs[pole][j]);
				tpoles.push_back(prevfrontind[adjcentPoles[pole][j]]);
			}
		}

		int prevStateNum = m_expandPoleSequence[i - 1].stateNum;
		for (int s = 0; s<prevStateNum; s++){
			std::vector<long long> previnds;
			indexToIndices(s, prevbases, previnds);

			for (int n = 0; n<numOfNode[pole]; n++){
				double c = costs[i - 1][s] + m_poleGraphNodeWeight[pole][n];
				//update c;
				for (int j = 0; j<tarcs.size(); j++){
					if (tarcs[j].second == 0)
						c += m_poleGraphArcsWeight[tarcs[j].first][n][previnds[tpoles[j]]];
					else
						c += m_poleGraphArcsWeight[tarcs[j].first][previnds[tpoles[j]]][n];
				}
				std::vector<long long> indices;
				previnds.push_back(n);
				for (int j = 0; j<pos.size(); j++){
					indices.push_back(previnds[pos[j]]);
				}
				previnds.pop_back();
				long long index = indicesToIndex(indices, bases);
				if (c<costs[i][index]){
					costs[i][index] = c;
					prevs[i][index] = s;
					nodes[i][index] = n;
				}
			}
		}
	}
	std::vector<int> res(numOfPole);
	int state = 0;
	for (int i = numOfPole - 1; i >= 0; i--){
		res[m_expandPoleSequence[i].nodeID] = nodes[i][state];
		state = prevs[i][state];
	}
	m_selectedNodeInPole = res;
}
bool mySort3(std::pair<int, int> i, std::pair<int, int> j)
{
	return (i.second<j.second);
}
void cycleDiscovery::updateRotationGraphbyPoleGraph()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty()
		|| m_poleGraphArcsWeight.empty() || m_expandPoleSequence.empty() || m_selectedNodeInPole.empty())
		return;

	m_rotationGraph.clear();
	for (int i = 0; i<m_selectedNodeInPole.size(); i++){
		m_rotationGraph.push_back(m_poleGraphNodes[i][m_selectedNodeInPole[i]]);
	}

	Graph net = m_curveNet;
	for (int i = 0; i<net.arcs.size(); i++){
		int nodeID[] = { net.arcs[i].endNodesID.first, net.arcs[i].endNodesID.second };
		int arcID[] = { net.arcs[i].posInNode.first, net.arcs[i].posInNode.second };
		int cap = net.arcs[i].capacity;

		std::vector<std::pair<int, int> > pairs;
		for (int j = 0; j<cap; j++){
			std::pair<int, int> temp;
			for (int k = 0; k<cap; k++){
				temp.first = j; temp.second = k + cap;
				pairs.push_back(temp);
			}
		}
		std::vector<std::pair<int, int> > optpair = m_poleGraphArcsMatch[i][m_selectedNodeInPole[nodeID[0]]][m_selectedNodeInPole[nodeID[1]]];
		std::vector<std::pair<int, int> > tpairs;
		for (int j = 0; j<optpair.size(); j++)
			tpairs.push_back(pairs[optpair[j].first]);
		sort(tpairs.begin(), tpairs.end(), mySort3);
		std::vector<int> order;
		for (int j = 0; j<tpairs.size(); j++)
			order.push_back(tpairs[j].first);

		tpairs = m_rotationGraph[nodeID[0]][arcID[0]];
		for (int j = 0; j<order.size(); j++)
			m_rotationGraph[nodeID[0]][arcID[0]][j] = tpairs[order[j]];

		for (int j = 0; j<cap; j++){
			std::pair<int, int> temp; temp.first = arcID[0]; temp.second = j;
			m_rotationGraph[nodeID[0]][m_rotationGraph[nodeID[0]][arcID[0]][j].first]
				[m_rotationGraph[nodeID[0]][arcID[0]][j].second] = temp;
		}
	}
	m_isUpdateCycle = true;

}
void cycleDiscovery::constructRotationGraphbyPoleGraph()
{
	if (m_curveNet.arcs.empty())
		return;

	/* test data  compare with ground truth, make sure getting right results*/

	/*
	m_poleGraphNodeWeight.resize(4);
	m_poleGraphNodeWeight[0].push_back(.5);
	m_poleGraphNodeWeight[0].push_back(.5);
	m_poleGraphNodeWeight[1].push_back(.5);
	m_poleGraphNodeWeight[2].push_back(1.);
	m_poleGraphNodeWeight[2].push_back(1.);
	m_poleGraphNodeWeight[3].push_back(1.);
	m_poleGraphNodeWeight[3].push_back(2.);
	*/

	/*
	m_poleGraphArcs.resize(4);
	m_poleGraphArcs[0].resize(2);
	m_poleGraphArcs[0][0].push_back(.5);
	m_poleGraphArcs[0][1].push_back(.5);

	m_poleGraphArcs[2].resize(1);
	m_poleGraphArcs[2][0].push_back(.5);
	m_poleGraphArcs[2][0].push_back(0);

	m_poleGraphArcs[1].resize(2);
	m_poleGraphArcs[1][0].push_back(.5);
	m_poleGraphArcs[1][0].push_back(0);
	m_poleGraphArcs[1][1].push_back(.5);
	m_poleGraphArcs[1][1].push_back(0);

	m_poleGraphArcs[3].resize(2);
	m_poleGraphArcs[3][0].push_back(.5);
	m_poleGraphArcs[3][0].push_back(0);
	m_poleGraphArcs[3][1].push_back(.5);
	m_poleGraphArcs[3][1].push_back(0);
	*/

	/*
	int f1[4][4] ={{2, 1, 3, 4}, {4, 1, 2, 3}, {1, 3, 2, 4}, {2, 3, 1, 4}};
	int s2[4][4] ={{1, 3, 2, 4}, {3, 4, 1, 2}, {4, 2, 3, 1}, {3, 2, 1, 4}};
	std::vector<std::vector<int> > firstGroup(4);
	std::vector<std::vector<int> > secondGroup(4);
	for(int i=0;i<4;i++){
	for(int j=0;j<4;j++){
	firstGroup[i].push_back(f1[i][j]-1);
	secondGroup[i].push_back(s2[i][j]-1);
	}
	}
	std::vector<int> res;
	m_graphSearch.stableMatching(firstGroup,secondGroup,res);
	*/

	/*
	int capsi[]={2,1,3};
	std::vector<int> caps;
	for(int i=0;i<3;i++)
	caps.push_back(capsi[i]);
	std::vector<std::vector<int> > res= enumSets(caps,2);
	*/

	/*
	int tempNode[] = {m_curveNet.arcs[0].endNodesID.first,m_curveNet.arcs[0].endNodesID.second};
	std::vector<AML::double3> tempPts = m_curveNet.arcs[0].vertexList;
	tempPts.push_back(m_curveNet.nodes[tempNode[1]].pos);
	tempPts.insert(tempPts.begin(),m_curveNet.nodes[tempNode[0]].pos);
	std::vector<std::vector<double> > transportMatrices;
	computeTransportMatrixAll(tempPts,transportMatrices);
	*/
	m_twistTables.clear();
	m_twistTablesConfidence.clear();
	m_twistTablesIndex.clear();

	m_poleGraphNodes.clear();
	m_poleGraphNodeWeight.clear();
	m_poleGraphArcsWeight.clear();
	m_poleGraphArcsMatch.clear();
	m_expandPoleSequence.clear();
	m_selectedNodeInPole.clear();

	if (m_userDefinedPairsInNode.empty())
		m_userDefinedPairsInNode.resize(m_curveNet.nodes.size());
	if (m_userDefinedPairsInArc.empty())
		m_userDefinedPairsInArc.resize(m_curveNet.arcs.size());

	computeTwistTables();
	constructJointRotationGraphbyPoleGraph();
	constructSegmentRotationGraphbyPoleGraph();
	constructExpandSequence();
	searchMinPoleGraph();
	updateRotationGraphbyPoleGraph();
	clock_t  time_end = clock();
	m_stateUnLimited = true;
}

void cycleDiscovery::constructRotationGraphbyPoleGraphEx()
{
	if (m_curveNet.arcs.empty())
		return;

	if (m_isUpdatetwistNormNum || m_isUpdateangleWeight || m_isUpdatetwistWeight || m_isUpdatecurveWeight){
		m_twistTables.clear();
		m_twistTablesConfidence.clear();
		m_twistTablesIndex.clear();

		m_latestUpdateNodes.clear();
		m_latestUpdateArcs.clear();

		m_poleGraphNodes.clear();
		m_poleGraphNodeWeight.clear();
		m_poleGraphArcsWeight.clear();
		m_poleGraphArcsMatch.clear();
		m_expandPoleSequence.clear();
		m_selectedNodeInPole.clear();
	}
	else if (m_isUpdatenodeWeight || m_isUpdatebestNeightboreNum || m_isUpdaterotationGraphNum || m_isUpdateRoGraphConnect){
		m_latestUpdateNodes.clear();
		m_latestUpdateArcs.clear();
		m_poleGraphNodes.clear();
		m_poleGraphNodeWeight.clear();
		m_poleGraphArcsWeight.clear();
		m_poleGraphArcsMatch.clear();
		m_expandPoleSequence.clear();
		m_selectedNodeInPole.clear();
	}
	else if (m_isUpdatedihedralWeight){
		m_latestUpdateArcs.clear();
		m_poleGraphArcsWeight.clear();
		m_poleGraphArcsMatch.clear();
		m_expandPoleSequence.clear();
		m_selectedNodeInPole.clear();
	}
	else if (m_isUpdatestateNum){
		m_selectedNodeInPole.clear();
	}

	if (m_userDefinedPairsInNode.empty())
		m_userDefinedPairsInNode.resize(m_curveNet.nodes.size());
	if (m_userDefinedPairsInArc.empty())
		m_userDefinedPairsInArc.resize(m_curveNet.arcs.size());


	if (m_twistTables.empty()){
		m_statusBar->SetStatusText("construct twist table ", 0);
		clock_t  time_str = clock();
		computeTwistTables();
		if (m_timePre == 0)
			m_timePre = clock() - time_str;
	}
	m_timeNewRun = clock();
	if (m_poleGraphNodeWeight.empty() || !m_latestUpdateNodes.empty()){
		m_statusBar->SetStatusText("construct joint rotation graph ", 0);
		constructJointRotationGraphbyPoleGraph();
		m_statusBar->SetStatusText("construct arc rotation graph ", 0);
		constructSegmentRotationGraphbyPoleGraph();
		m_statusBar->SetStatusText("construct front node sequence ", 0);
		constructExpandSequence();
		m_statusBar->SetStatusText("pole graph searching ");
		searchAlmostMinPoleGraph();
		m_statusBar->SetStatusText("construct final rotation graph ", 0);
		updateRotationGraphbyPoleGraph();
// 		m_triangleSurface.clear();
	}
	else if (m_poleGraphArcsWeight.empty() || !m_latestUpdateArcs.empty()){
		m_statusBar->SetStatusText("construct arc rotation graph ", 0);
		constructSegmentRotationGraphbyPoleGraph();
		m_statusBar->SetStatusText("construct front node sequence ", 0);
		constructExpandSequence();
		m_statusBar->SetStatusText("pole graph xearching ");
		searchAlmostMinPoleGraph();
		m_statusBar->SetStatusText("construct final rotation graph ", 0);
		updateRotationGraphbyPoleGraph();
// 		m_triangleSurface.clear();
	}
	else if (m_selectedNodeInPole.empty()){
		m_statusBar->SetStatusText("pole graph searching ", 0);
		searchAlmostMinPoleGraph();
		m_statusBar->SetStatusText("construct final rotation graph ", 0);
		updateRotationGraphbyPoleGraph();
// 		m_triangleSurface.clear();
	}
	m_stateUnLimited = false;
	m_isUpdatetwistNormNum = false;
	m_isUpdateangleWeight = false;
	m_isUpdatetwistWeight = false;
	m_isUpdatecurveWeight = false;
	m_isUpdatenodeWeight = false;
	m_isUpdatebestNeightboreNum = false;
	m_isUpdaterotationGraphNum = false;
	m_isUpdatedihedralWeight = false;
	m_isUpdatestateNum = false;
	m_isUpdateRoGraphConnect = false;
}
void cycleDiscovery::constructRandomRotationGraph()
{
	if (m_curveNet.arcs.empty())
		return;

	Graph net = m_curveNet;
	RotationGraph roGraph;

	//initialization
	std::vector<std::vector<std::vector<bool> > > usedList;
	for (int i = 0; i<net.nodes.size(); i++){
		std::vector<std::vector<bool> > usedNode;
		int nodeSize = net.nodes[i].arcID.size();
		for (int j = 0; j<nodeSize; j++){
			std::vector<bool> usedNodeArcs;
			usedNodeArcs.assign(net.arcs[net.nodes[i].arcID[j]].capacity, false);
			usedNode.push_back(usedNodeArcs);
		}
		usedList.push_back(usedNode);
	}
	for (int i = 0; i<net.nodes.size(); i++){
		std::vector<std::vector<std::pair<int, int> > > jointRoGraph;
		for (int j = 0; j<net.nodes[i].arcID.size(); j++){
			std::vector<std::pair<int, int> > segRoGraph;
			segRoGraph.assign(net.arcs[net.nodes[i].arcID[j]].capacity,
				std::pair<int, int>(0, 0));
			jointRoGraph.push_back(segRoGraph);
		}
		roGraph.push_back(jointRoGraph);
	}
	//end of initialization

	for (int i = 0; i<net.nodes.size(); i++){
		int nodeSize = net.nodes[i].arcID.size();
		std::vector<int> nodeArcsCapacity;
		for (int j = 0; j<nodeSize; j++){
			nodeArcsCapacity.push_back(net.arcs[net.nodes[i].arcID[j]].capacity);
		}
		for (int j = 0; j<nodeSize; j++){
			for (int k = 0; k<nodeArcsCapacity[j]; k++){
				if (usedList[i][j][k] == true)
					continue;
				usedList[i][j][k] = true;
				std::pair<int, unsigned int> connectSeg;
				//random {E,i}
				while (true){
					connectSeg.first = rand() % nodeSize;
					connectSeg.second = rand() % (nodeArcsCapacity[connectSeg.first]);
					if (!usedList[i][connectSeg.first][connectSeg.second]) break;
				}
				usedList[i][connectSeg.first][connectSeg.second] = true;
				roGraph[i][j][k] = connectSeg;
				roGraph[i][connectSeg.first][connectSeg.second] = std::pair<int, unsigned int>(j, k);
			}
		}
	}
	m_rotationGraph = roGraph;
}
void cycleDiscovery::constructCycles()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty() || m_poleGraphArcsWeight.empty()
		|| m_expandPoleSequence.empty() || m_selectedNodeInPole.empty() || m_rotationGraph.empty())
		return;

	Graph net = m_curveNet;
	RotationGraph roGraph = m_rotationGraph;
	CycleSet cycles;

	std::vector<std::vector<bool> > usedList;
	for (int i = 0; i<net.arcs.size(); i++){
		std::vector<bool> usedInstances;
		usedInstances.assign(net.arcs[i].capacity, false);
		usedList.push_back(usedInstances);
	}

	for (int i = 0; i<net.arcs.size(); i++){
		for (int j = 0; j<net.arcs[i].capacity; j++){
			if (usedList[i][j])
				continue;
			Cycle cycle;
			int currentArcID = i;
			int currentArcInstance = j;
			int currentDirection = 1;
			while (!usedList[currentArcID][currentArcInstance]){
				usedList[currentArcID][currentArcInstance] = true;
				CycleSegment cycleSegment;
				cycleSegment.arcID = currentArcID;
				cycleSegment.instanceID = currentArcInstance;
				cycleSegment.strEndID = currentDirection;
				cycle.push_back(cycleSegment);

				int nodeID, arcID;
				if (currentDirection == 1){
					nodeID = net.arcs[currentArcID].endNodesID.second;
					arcID = net.arcs[currentArcID].posInNode.second;
				}
				else{
					nodeID = net.arcs[currentArcID].endNodesID.first;
					arcID = net.arcs[currentArcID].posInNode.first;
				}
				int nextArcID;
				nextArcID = roGraph[nodeID][arcID][currentArcInstance].first;
				currentArcInstance = roGraph[nodeID][arcID][currentArcInstance].second;
				currentArcID = net.nodes[nodeID].arcID[nextArcID];
				currentDirection = net.nodes[nodeID].arcDirection[nextArcID];
			}
			cycles.push_back(cycle);
		}
	}
	m_cycleSet = cycles;
	m_isCycleBreak = false;

// 	colorMapCycle(m_isCycleBreak);
}

void cycleDiscovery::cycleBreaking()
{
	if (m_curveNet.arcs.empty() || m_twistTables.empty() || m_poleGraphNodes.empty() || m_poleGraphArcsWeight.empty()
		|| m_expandPoleSequence.empty() || m_selectedNodeInPole.empty() || m_rotationGraph.empty() || m_cycleSet.empty())
		return;

	Graph net = m_curveNet;
	CycleSet cyclesOrig = m_cycleSet;
	RotationGraph roGraph = m_rotationGraph;
	m_rotationGraphUpdate = m_rotationGraph;
	m_cycleSetBreaked = m_cycleSet;

	bool found;
	bool isbreak = false;
	std::vector<int> tbad;
	m_breakNum = 0;

	do{
		tbad.clear();
		found = false;
		for (int i = 0; i<cyclesOrig.size(); i++){
			std::vector<int > nodeID;
			std::vector<std::vector<std::pair<int, int> > > arcPairs;
			for (int j = 0; j<cyclesOrig[i].size(); j++){
				CycleSegment currentSeg = cyclesOrig[i][j];
				int currentPosition, arcID, instanceID;
				if (currentSeg.strEndID == 1){
					currentPosition = net.arcs[currentSeg.arcID].endNodesID.second;
					arcID = net.arcs[currentSeg.arcID].posInNode.second;
				}
				else{
					currentPosition = net.arcs[currentSeg.arcID].endNodesID.first;
					arcID = net.arcs[currentSeg.arcID].posInNode.first;
				}
				int index = nodeID.size();
				for (int k = 0; k<nodeID.size(); k++){
					if (nodeID[k] == currentPosition){
						index = k; break;
					}
				}
				if (index == nodeID.size()){
					nodeID.push_back(currentPosition);
					std::vector<std::pair<int, int> > arcPair;
					arcPairs.push_back(arcPair);
				}
				std::pair<int, int> temp; temp.first = arcID; temp.second = currentSeg.instanceID;
				arcPairs[index].push_back(temp);
				int nextArcID = roGraph[currentPosition][arcID][currentSeg.instanceID].first;
				instanceID = roGraph[currentPosition][arcID][currentSeg.instanceID].second;
				temp.first = nextArcID; temp.second = instanceID;
				arcPairs[index].push_back(temp);
			}
			for (int j = 0; j<nodeID.size(); j++){
				arcPairs[j].push_back(arcPairs[j].front());
				arcPairs[j].erase(arcPairs[j].begin());
				for (int k = 1; k<arcPairs[j].size(); k++){
					if (arcPairs[j][k].first == arcPairs[j][k - 1].first){
						arcPairs[j].clear();
						tbad.push_back(i); break;
					}
				}
			}
			int	size = 0;
			int index = 0;
			for (int j = 0; j<nodeID.size(); j++){
				if (size<arcPairs[j].size()){
					size = arcPairs[j].size();
					index = j;
				}
			}
			if (size <= 2) continue;
			found = true;
			if (!m_isDoCycleBreak)
				break;

			for (int j = 0; j<arcPairs[index].size(); j += 2){
				roGraph[nodeID[index]][arcPairs[index][j].first][arcPairs[index][j].second] = arcPairs[index][j + 1];
				roGraph[nodeID[index]][arcPairs[index][j + 1].first][arcPairs[index][j + 1].second] = arcPairs[index][j];
			}
		}

		if (!m_isDoCycleBreak)
			break;

		if (found){
			m_breakNum++;
			m_rotationGraphUpdate = m_rotationGraph;
			m_rotationGraph = roGraph;
			m_cycleSetBreaked = m_cycleSet;
			constructCycles();
			cyclesOrig = m_cycleSet;
			m_cycleSet = m_cycleSetBreaked;
			m_cycleSetBreaked = cyclesOrig;
			m_rotationGraph = m_rotationGraphUpdate;
			m_rotationGraphUpdate = roGraph;
			isbreak = true;
		}
	} while (found);


	m_isCycleBreak = isbreak;
// 	colorMapCycle(true);

	//	computeArcCost();
	computeCycleCost();
	for (int i = 0; i<tbad.size(); i++){
		m_cyclesCost[tbad[i]] = 1;
	}

	if (m_timeRun == 0){
		m_timeRun = clock() - m_timeNewRun;
	}
}
void cycleDiscovery::computeArcCost()
{
	m_arcsCost.clear();
	double totalCost = 0;
	int totalNum = 0;
	double maxN = 0;
	double minN = FLT_MAX;
	std::vector<double> possiWorses(m_curveNet.arcs.size(), 0);
	for (int i = 0; i<m_curveNet.arcs.size(); i++){
		int nodeID[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		int pos[] = { m_selectedNodeInPole[nodeID[0]], m_selectedNodeInPole[nodeID[1]] };
		double cost = m_poleGraphArcsWeight[i][pos[0]][pos[1]];
		m_arcsCost.push_back(cost);
		int cap = m_curveNet.arcs[i].capacity;
		double possiWorse = m_twistWeight*M_PI + m_angleWeight*M_PI + m_dihedralWeight*M_PI * 2 * (cap - 1);
		possiWorses[i] = possiWorse;
		if (cost<FLT_MAX){
			totalCost += cost;
			totalNum++;
			if (cost>maxN)
				maxN = cost;
			if (cost<minN)
				minN = cost;
		}
	}
	//	double margin = maxN - minN;
	//	double possiWorse = *std::max_element(possiWorses.begin(),possiWorses.end());
	double possiWorse = maxN - minN;
	for (int i = 0; i<m_arcsCost.size(); i++){
		if (m_arcsCost[i]<FLT_MAX)
			m_arcsCost[i] = min(1, (m_arcsCost[i] - minN) / possiWorse);
		else
			m_arcsCost[i] = 1;
		//		m_arcsCost[i] = 1-m_arcsCost[i];
	}
}
void cycleDiscovery::computeCycleCost()
{
	m_cyclesCost.clear();
	double totalCost = 0;
	int totalNum = 0;
	double maxN = 0;
	double minN = FLT_MAX;

	//	std::vector<double> possiWorses(m_cycleSetBreaked.size(),0);
	for (int i = 0; i<m_cycleSetBreaked.size(); i++){
		double tc = 0;
		Cycle tcycle = m_cycleSetBreaked[i];
		/*
		tcycle.insert(tcycle.begin(),tcycle.back());
		tcycle.push_back(tcycle[1]);
		*/
		for (int j = 0; j<tcycle.size(); j++){
			int arcID = tcycle[j].arcID;
			int nodeID[] = { m_curveNet.arcs[arcID].endNodesID.first,
				m_curveNet.arcs[arcID].endNodesID.second };
			int pos[] = { m_selectedNodeInPole[nodeID[0]], m_selectedNodeInPole[nodeID[1]] };
			if (tc == FLT_MAX || m_poleGraphArcsWeight[arcID][pos[0]][pos[1]] == FLT_MAX)
				tc = FLT_MAX;
			else
				tc += m_poleGraphArcsWeight[arcID][pos[0]][pos[1]];

			/*
			int a1 = tcycle[j-1].arcID;
			int a2 = tcycle[j+1].arcID;
			int aind1,aind2;
			if(m_curveNet.arcs[a1].endNodesID.first==nodeID[0]){
			aind1 = m_curveNet.arcs[a1].posInNode.first;
			}
			else if(m_curveNet.arcs[a1].endNodesID.second==nodeID[0]){
			aind1 = m_curveNet.arcs[a1].posInNode.second;
			}
			else if(m_curveNet.arcs[a2].endNodesID.first==nodeID[0]){
			aind1 = m_curveNet.arcs[a2].posInNode.first;
			}
			else if(m_curveNet.arcs[a2].endNodesID.second==nodeID[0]){
			aind1 = m_curveNet.arcs[a2].posInNode.second;
			}

			if(m_curveNet.arcs[a1].endNodesID.first==nodeID[1]){
			aind2 = m_curveNet.arcs[a1].posInNode.first;
			}
			else if(m_curveNet.arcs[a1].endNodesID.second==nodeID[1]){
			aind2 = m_curveNet.arcs[a1].posInNode.second;
			}
			else if(m_curveNet.arcs[a2].endNodesID.first==nodeID[1]){
			aind2 = m_curveNet.arcs[a2].posInNode.first;
			}
			else if(m_curveNet.arcs[a2].endNodesID.second==nodeID[1]){
			aind2 = m_curveNet.arcs[a2].posInNode.second;
			}
			tc+=m_twistTables[arcID][aind1][aind2];
			*/
		}
		double mincost = (tcycle.size() - 2)*M_PI*m_angleWeight;
		m_cyclesCost.push_back(abs(tc) - mincost);

		if (abs(tc)<FLT_MAX){
			if (tc>maxN)
				maxN = tc;
			if (tc<minN)
				minN = tc;
		}
	}
	//	double margin = maxN - minN;
	//	double possiWorse = *std::max_element(possiWorses.begin(),possiWorses.end());
	//	double possiWorse = *std::max_element(m_cyclesCost.begin(),m_cyclesCost.end());
	for (int i = 0; i<m_cyclesCost.size(); i++){
		if (m_cyclesCost[i]<FLT_MAX){
			m_cyclesCost[i] = min(1, (m_cyclesCost[i]) / maxN);
			m_cyclesCost[i] = max(0, m_cyclesCost[i]);
		}
		else
			m_cyclesCost[i] = 1;
		//		m_cyclesCost[i] = 1-m_cyclesCost[i];
	}
}
