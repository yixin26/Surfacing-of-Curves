#include "cycleUtils.h"
// #include "DrT.h"
#include <iostream>
#include <fstream>


cycleUtils::cycleUtils()
{
	resetAll();
}
void cycleUtils::resetAllData()
{
	m_cycleDiscovery.m_timePre=0;
	m_cycleDiscovery.m_timeRun = 0;
	m_cycleDiscovery.m_timeNewRun = 0;
	m_filename.Clear();

	m_weightTri=m_weightEdge=0;
	m_weightBiTri=m_weightTriBd=1;
	m_weightWorsDih=0;
	m_SurfaceSmooth=true;
	m_subdivisonSmooth=1;
	m_laplacianSmooth=3;

	m_cycleDiscovery.m_isUpdatetwistNormNum = false;
	m_cycleDiscovery.m_isUpdateangleWeight = false;
	m_cycleDiscovery.m_isUpdatetwistWeight = false;
	m_cycleDiscovery.m_isUpdatecurveWeight = false;
	m_cycleDiscovery.m_isUpdatenodeWeight = false;
	m_cycleDiscovery.m_isUpdatebestNeightboreNum = false;
	m_cycleDiscovery.m_isUpdaterotationGraphNum = false;
	m_cycleDiscovery.m_isUpdatedihedralWeight = false;
	m_cycleDiscovery.m_isUpdatestateNum = false;
	m_cycleDiscovery.m_isUpdateRoGraphConnect = false;
	m_cycleDiscovery.m_isUpdateDoCycleBreak = false;
	m_cycleDiscovery.m_isUpdateWeightTri = m_cycleDiscovery.m_isUpdateWeightEdge = m_cycleDiscovery.m_isUpdateWeightBiTri = m_cycleDiscovery.m_isUpdateWeightTriBd = m_cycleDiscovery.isUpdateWeightWorsDih = true;
	m_cycleDiscovery.m_isUpdateSurfaceSmooth = true;
	m_isUpdateSubdivision=m_isUpdateLaplacian=false;
	m_cycleDiscovery.m_isUpdateCycle = false;

	m_cycleDiscovery.m_selectArcList.clear();
	m_cycleDiscovery.m_selectArcList.resize(1);
	m_cycleDiscovery.m_userDefinedPairsInNode.clear();
	m_cycleDiscovery.m_userDefinedPairsInArc.clear();
	m_cycleDiscovery.m_latestUpdateNodes.clear();
	m_cycleDiscovery.m_latestUpdateArcs.clear();
	m_cycleDiscovery.m_latestUpdateArcsCapacity.clear();
	m_cycleDiscovery.m_errorNode.clear();
	m_cycleDiscovery.m_selectedNodeInPole.clear();

	m_showCycleMode=0;
	m_showArcMode=0;
	m_showSurfaceMode=1;
	m_brightness=3;

	m_colorsCycle.clear();
	m_colorsCycleInd.clear();
	m_colorsCycleBreak.clear();
	m_colorsCycleBreakInd.clear();

	m_curveColors.clear();
	m_selectArcColors.clear();
	m_cycleColors.clear();
	m_colorInterval=10;

	m_lineSize=2;
	m_curveLineSize=2;
	m_nodeSize=5;

	m_triangleSurface.clear();
	m_triangleSurfaceNormal.clear();

//////////////////////////////////
	m_showNewNormal=false;
	m_newPoints.clear();
	m_newNormals.clear();
	m_newPointNum.clear();

	//
	m_showNewCycle = false;
}
void cycleUtils::resetAllVisualization()
{
	m_isViewing=false;

	m_showCurveNetwork=true;
	m_showIntersectionPoints = true;
	m_showRotationGraph = false;
	m_showCycles = true;
	m_showCyclesBreaked = true;
	m_showSurface = false;
	m_showSingleNode = false;
	m_showSingleCurve = false;
	m_showSingleCycle = false;
	m_showCapacity = false;
	m_showConstraint = false;

	m_capacity = 2;
	m_selectedNodeIDFormer=0;
	m_selectedArcIDFormer=0;
	m_selectedNodeID=0;
	m_selectedArcID=0;
	m_selectedCycleID=0;
	m_selectedCycleSize=9;//1-10
	m_selectedRotationGraphPosition=20;//10-100
	m_selectedRotationGraphLength=40;//10-100
	m_selectedPoleRotationGraphPosition=20;//10-100
	m_selectedPoleRotationGraphLength=40;//10-100
}
void cycleUtils::resetAll()
{
	our_font.init("times.ttf", 72);
	glColor3ub(0xff,0,0);
	m_showNormal=false;
	resetAllVisualization();
	resetAllData();
}
cycleUtils::~cycleUtils()
{
}

int cycleUtils::getSelectArcCapacity()
{
	if (std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) != m_cycleDiscovery.m_latestUpdateArcs.end()){
		int ind = std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) - m_cycleDiscovery.m_latestUpdateArcs.begin();
		return m_cycleDiscovery.m_latestUpdateArcsCapacity[ind];
	}
	else
		return m_cycleDiscovery.m_curveNet.arcs[m_selectedArcID].capacity;
}
void cycleUtils::setCapacity(int selection){
	m_capacity = selection;
	m_cycleDiscovery.m_errorNode.clear();
	if(m_showSingleCurve){
		if (m_cycleDiscovery.m_curveNet.arcs[m_selectedArcID].capacity == m_capacity)
		{
			if (std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) != m_cycleDiscovery.m_latestUpdateArcs.end())
			{
				int ind = std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) - m_cycleDiscovery.m_latestUpdateArcs.begin();
				m_cycleDiscovery.m_latestUpdateArcs.erase(m_cycleDiscovery.m_latestUpdateArcs.begin() + ind);
				m_cycleDiscovery.m_latestUpdateArcsCapacity.erase(m_cycleDiscovery.m_latestUpdateArcsCapacity.begin() + ind);
			}
		}
		else{
			if (std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) != m_cycleDiscovery.m_latestUpdateArcs.end()){
				int ind = std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) - m_cycleDiscovery.m_latestUpdateArcs.begin();
				m_cycleDiscovery.m_latestUpdateArcsCapacity[ind] = m_capacity;
			}
			else{
				m_cycleDiscovery.m_latestUpdateArcs.push_back(m_selectedArcID);
				m_cycleDiscovery.m_latestUpdateArcsCapacity.push_back(m_capacity);
			}
		}

		std::vector<int> useNode;
		for (int i = 0; i<m_cycleDiscovery.m_latestUpdateArcs.size(); i++){
			int arcID = m_cycleDiscovery.m_latestUpdateArcs[i];
			int nodeID[] = { m_cycleDiscovery.m_curveNet.arcs[arcID].endNodesID.first, m_cycleDiscovery.m_curveNet.arcs[arcID].endNodesID.second };
			if(std::find(useNode.begin(),useNode.end(),nodeID[0])==useNode.end()){
				useNode.push_back(nodeID[0]);
			}
			if(std::find(useNode.begin(),useNode.end(),nodeID[1])==useNode.end()){
				useNode.push_back(nodeID[1]);
			}
		}
		for(int i=0;i<useNode.size();i++){
			std::vector<int> adjArc = m_cycleDiscovery.m_curveNet.nodes[useNode[i]].arcID;
			int cap=0;
			for(int j=0;j<adjArc.size();j++){
				if (std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), adjArc[j])
					!= m_cycleDiscovery.m_latestUpdateArcs.end()){
					int ind = std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), adjArc[j])
						- m_cycleDiscovery.m_latestUpdateArcs.begin();
					cap += m_cycleDiscovery.m_latestUpdateArcsCapacity[ind];
				}
				else
					cap += m_cycleDiscovery.m_curveNet.arcs[adjArc[j]].capacity;
			}
			if((cap%2)==1)
				m_cycleDiscovery.m_errorNode.push_back(useNode[i]);
		}
	}
}
bool cycleUtils::updateConstraintList()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (m_curveNet.arcs.empty())
		return false;
	if (m_cycleDiscovery.m_userDefinedPairsInNode.empty())
		m_cycleDiscovery.m_userDefinedPairsInNode.resize(m_curveNet.nodes.size());
	if (m_cycleDiscovery.m_userDefinedPairsInArc.empty())
		m_cycleDiscovery.m_userDefinedPairsInArc.resize(m_curveNet.arcs.size());

	for (int i = 0; i<m_cycleDiscovery.m_selectArcList.back().size() - 1; i++){
		for (int j = i + 1; j<m_cycleDiscovery.m_selectArcList.back().size(); j++){
			if (m_cycleDiscovery.m_selectArcList.back()[i] == m_cycleDiscovery.m_selectArcList.back()[j]){
				m_cycleDiscovery.m_selectArcList.back().erase(m_cycleDiscovery.m_selectArcList.back().begin() + j);
				j--;
			}
		}
	}
	std::vector<std::vector<int> > jointArcs(m_curveNet.nodes.size());
	std::vector<std::vector<std::vector<int> > > adjArcs(m_curveNet.arcs.size(), std::vector<std::vector<int> >(2));
	std::vector<bool> usedArcs(adjArcs.size(),false);
	for (int i = 0; i<m_cycleDiscovery.m_selectArcList.back().size(); i++){
		int a1 = m_cycleDiscovery.m_selectArcList.back()[i];
		int n[] = { m_curveNet.arcs[a1].endNodesID.first, m_curveNet.arcs[a1].endNodesID.second };
		jointArcs[n[0]].push_back(a1);
		jointArcs[n[1]].push_back(a1);

		usedArcs[a1]=true;
		std::vector<int> leftAdj = m_curveNet.nodes[n[0]].arcID;
		std::vector<int> rightAdj = m_curveNet.nodes[n[1]].arcID;
		std::vector<int> leftAdjDir = m_curveNet.nodes[n[0]].arcDirection;
		std::vector<int> rightAdjDir = m_curveNet.nodes[n[1]].arcDirection;
		for(int j=0;j<leftAdj.size();j++){
			if(leftAdj[j]!=a1){
				adjArcs[leftAdj[j]][leftAdjDir[j]-1].push_back(a1);
			}
		}
		for(int j=0;j<rightAdj.size();j++){
			if(rightAdj[j]!=a1)
				adjArcs[rightAdj[j]][rightAdjDir[j]-1].push_back(a1);
		}
	}

	bool isRightCycle = true;
	// data check;
	for(int i=0;i<jointArcs.size();i++){
		if(jointArcs[i].size()>2){
			isRightCycle = false; break;
		}
	}
	if(!isRightCycle)
		return isRightCycle;

	isRightCycle = false;
	for(int i=0;i<jointArcs.size();i++){
		if(jointArcs[i].empty()||jointArcs[i].size()==1)
			continue;
		isRightCycle=true;
		int a1 = jointArcs[i][0];
		int a2 = jointArcs[i][1];
		int order1,order2;
		if (m_curveNet.arcs[a1].endNodesID.first == i)
			order1 = m_curveNet.arcs[a1].posInNode.first;
		else
			order1 = m_curveNet.arcs[a1].posInNode.second;
		if (m_curveNet.arcs[a2].endNodesID.first == i)
			order2 = m_curveNet.arcs[a2].posInNode.first;
		else
			order2 = m_curveNet.arcs[a2].posInNode.second;
		if(order1<order2){
			std::pair<int,int> temp; temp.first=order1; temp.second=order2;
			m_cycleDiscovery.m_userDefinedPairsInNode[i].push_back(temp);
		}
		else{
			std::pair<int,int> temp; temp.first=order2; temp.second=order1;
			m_cycleDiscovery.m_userDefinedPairsInNode[i].push_back(temp);
		}
		m_cycleDiscovery.m_latestUpdateNodes.push_back(i);
	}
	for(int i=0;i<adjArcs.size();i++){
		if(adjArcs[i][0].size()!=1 || adjArcs[i][1].size()!=1)
			continue;
		if(!usedArcs[i])
			continue;
		int a1 = adjArcs[i][0][0];
		int a2 = adjArcs[i][1][0];
		int n[] = { m_curveNet.arcs[i].endNodesID.first, m_curveNet.arcs[i].endNodesID.second };
		int order1,order2;
		if (m_curveNet.arcs[a1].endNodesID.first == n[0])
			order1 = m_curveNet.arcs[a1].posInNode.first;
		else
			order1 = m_curveNet.arcs[a1].posInNode.second;
		if (m_curveNet.arcs[a2].endNodesID.first == n[1])
			order2 = m_curveNet.arcs[a2].posInNode.first;
		else
			order2 = m_curveNet.arcs[a2].posInNode.second;

		std::pair<int,int> temp; temp.first=order1; temp.second=order2;
		bool isExist=false;
		for (int j = 0; j<m_cycleDiscovery.m_userDefinedPairsInArc[i].size(); j++){
			std::pair<int, int> pairArc = m_cycleDiscovery.m_userDefinedPairsInArc[i][j];
			if(pairArc.first==temp.first && pairArc.second==temp.second)
				isExist=true;
		}
		if(!isExist){
			m_cycleDiscovery.m_userDefinedPairsInArc[i].push_back(temp);
			m_cycleDiscovery.m_latestUpdateArcs.push_back(i);
		}
	}
	return isRightCycle;
}
void cycleUtils::rollBack(enum GeometryType oT)
{
	switch (oT){
		case SHOWSINGLENODE: break;
		case SHOWSINGLECURVE: 
			if (!m_cycleDiscovery.m_selectArcList.back().empty()){
				m_cycleDiscovery.m_selectArcList.back().erase(m_cycleDiscovery.m_selectArcList.back().end() - 1);
				if (!m_cycleDiscovery.m_selectArcList.back().empty())
					m_selectedArcID = m_cycleDiscovery.m_selectArcList.back().back();
				else
					m_selectedArcID=0;
			}
			break;
		case SHOWSINGLECYCLE: break;
		case SHOWALLCYCLE:break;
		default: break;
	}
};
void cycleUtils::pickArc(const AML::double3 rayStr,const AML::double3 rayEnd,enum OperationType oT)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;
	auto& m_selectArcList = m_cycleDiscovery.m_selectArcList;

	double minDistance = FLT_MAX;
	int arcID=0;
	float p0p1LenSquared = (rayEnd-rayStr).dot(rayEnd-rayStr);

	for(int i=0;i<m_curveNet.arcs.size();i++){
		Curve curve = m_curveNet.arcs[i].vertexList;
		int nodeID[]={m_curveNet.arcs[i].endNodesID.first,m_curveNet.arcs[i].endNodesID.second};
		curve.push_back(m_curveNet.nodes[nodeID[1]].pos);
		curve.insert(curve.begin(),m_curveNet.nodes[nodeID[0]].pos);
		for(int j=0;j<curve.size();j++){
			AML::double3 p = curve[j];
			double paramU = (
				((p[0]-rayStr[0])*(rayEnd[0]-rayStr[0])) +
				((p[1]-rayStr[1])*(rayEnd[1]-rayStr[1])) +
				((p[2]-rayStr[2])*(rayEnd[2]-rayStr[2]))
				)/p0p1LenSquared;

			AML::double3 lineP = rayStr + (rayEnd-rayStr)*paramU;
			double diatance = (lineP-p).length();
			if(minDistance>diatance || (minDistance==diatance && curve.size()==2)){
				minDistance = diatance;
				arcID =i;
			}
		}
	}
	if(true/*minDistance<=MINDISTANCE*/)
	{
		if(m_selectedArcID!=arcID)
		{
			m_selectedArcIDFormer = m_selectedCycleID;
			m_selectedArcID = arcID;
		}
		switch (oT)
		{
		case VIEW: break;
		case SELECT:
			m_selectArcList.back().push_back(m_selectedArcID);
			break;
		case ADD:
			if (!m_selectArcList.back().empty()){
				bool isRightCycle = updateConstraintList();
				if (isRightCycle){
					int len = m_selectArcList.size() + 1;
					m_selectArcList.resize(len);
				}
				else
					m_selectArcList.back().clear();
			}
			break;
		case MODIFY:
			break;
		case DELETE:
			if (!m_selectArcList.back().empty())
				m_selectArcList.back().erase(m_selectArcList.back().end() - 1);
			break;
		default: break;
		}
	}
}
bool mySort2(std::pair<double,int> i,std::pair<double,int>  j) 
{ 
	return (i.first<j.first); 
}
void cycleUtils::pickCycle(const AML::double3 rayStr,const AML::double3 rayEnd,enum OperationType oT)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	const double MINDISTANCE = 5.;
	float p0p1LenSquared = (rayEnd-rayStr).dot(rayEnd-rayStr);

	int arcSize = m_curveNet.arcs.size();
	std::vector<std::pair<double,int> > dists(arcSize);
	for(int i=0;i<m_curveNet.arcs.size();i++){
		double minDistance = FLT_MAX;
		for(int j=0;j<m_curveNet.arcs[i].vertexList.size();j++){
			AML::double3 p = m_curveNet.arcs[i].vertexList[j];
			double paramU = (
				((p[0]-rayStr[0])*(rayEnd[0]-rayStr[0])) +
				((p[1]-rayStr[1])*(rayEnd[1]-rayStr[1])) +
				((p[2]-rayStr[2])*(rayEnd[2]-rayStr[2]))
				)/p0p1LenSquared;

			AML::double3 lineP = rayStr + (rayEnd-rayStr)*paramU;
			double diatance = (lineP-p).length();
			if(minDistance>diatance)
				minDistance = diatance;
		}
		dists[i].first=minDistance;
		dists[i].second=i;
	}
	sort(dists.begin(),dists.end(),mySort2);
	std::vector<int> priorities;
	for(int i=0;i<arcSize;i++)
		priorities.push_back(dists[i].second);

	std::vector<int> index(arcSize);
	for(int i=0;i<arcSize;i++)
		index[priorities[i]]=i;

	std::vector<std::vector<int> > rankList(arcSize);
	CycleSet cycles;
	if(m_showCyclesBreaked)
		cycles = m_cycleDiscovery.m_cycleSetBreaked;
	else
		cycles = m_cycleDiscovery.m_cycleSet;

	for(int i=0;i<cycles.size();i++){
		for(int j=0;j<cycles[i].size();j++){
			int arcID = cycles[i][j].arcID;
			int order = index[arcID];
			rankList[order].push_back(i);
		}
	}
	std::vector<int> top;
	for(int i=0;i<arcSize;i++){
		if(rankList[i].empty())
			continue;
		if(top.size()==0){
			top = rankList[i];
			continue;
		}
		if(top.size()==1)
			break;
		std::vector<int> ttop = rankList[i];
		if(ttop.size()==0)
			continue;

		std::vector<int> newTop;
		for(int j=0;j<top.size();j++){
			if(std::find(ttop.begin(),ttop.end(),top[j])!=ttop.end())
				newTop.push_back(j);
		}
		if(newTop.empty())
			continue;
		else{
			for(int j=0;j<newTop.size();j++){
				top[j] =top[newTop[j]];
			}
			top.resize(newTop.size());
		}
	}
	m_selectedCycleID = top.front();
}

void cycleUtils::writeConfigurations()
{
	std::fstream File;
	wxString fileName = m_filename;
	char FileName[400];
	strcpy(FileName, fileName);
	for (int i = strlen(FileName) - 1; i >= 0; i--){
		if (FileName[i] == '.'){
			FileName[i] = '\0';
			break;
		}
	}
	char FileNameExt[400];
	int ind = 0;
	for (int i = strlen(FileName) - 1; i >= 0; i--){
		if (FileName[i] == '//' || FileName[i] == '\\'){
			FileName[i] = '\0';	break;
		}
		FileNameExt[ind] = FileName[i]; ind++;
	}
	FileNameExt[ind] = '\0';
	char ModelName[400];
	for (int i = 0; i<ind; i++){
		ModelName[i] = FileNameExt[ind - i - 1];
	}
	ModelName[ind] = '\0';
	//	strcat(ModelName,".result");


	strcat(FileName, "\\result.txt");
	File.open(FileName, std::ios::app);

	wxString text[] = { "sketch model", "curves", "non-manifold", "nodes", "maxdegree",
		"total time", "preComp", "runtime", "total constraint", "constraint cycle", "genus", "isBreaked" };
	int len = 12;
	std::vector<int> textSize;
	for (int i = 0; i<len; i++){
		int len = strlen(text[i]);
		textSize.push_back(len);
	}
	textSize.front() = 30;
	textSize[10] = 11;

	File.seekg(0, File.end);
	int fileLength = File.tellp();
	if (fileLength == 0){
		for (int i = 0; i<len; i++){
			File.width(textSize[i]);	File << text[i];
			File.width(3);				File << " | ";
		}
		File << endl;
		for (int i = 0; i<len; i++){
			for (int j = 0; j<textSize[i]; j++)
				File << '-';
			if (i != (len - 1)){
				File.width(3);	File << "-|-";
			}
			else{
				File.width(2);	File << "-|";
			}
		}
		File << endl;
	}

	if (strlen(ModelName)>textSize[0])
		ModelName[textSize[0]] = '\0';
	File.width(textSize[0]);	File/*<<std::left*/ << ModelName;	File.width(3);	File << " | ";
	int curveSize = m_cycleDiscovery.m_curveNet.arcs.size();
	File.width(textSize[1]);	File << curveSize;	File.width(3);	File << " | ";
	int nonManifold = 0;
	for (int i = 0; i<m_cycleDiscovery.m_curveCapacitys.size(); i++){
		if (m_cycleDiscovery.m_curveCapacitys[i] != 2) nonManifold++;
	}
	File.width(textSize[2]);	File << nonManifold;	File.width(3);	File << " | ";
	int nodeSize = m_cycleDiscovery.m_curveNet.nodes.size();
	File.width(textSize[3]);	File << nodeSize;	File.width(3);	File << " | ";
	int maxDegree = 2;
	for (int i = 0; i<m_cycleDiscovery.m_curveNet.nodes.size(); i++){
		int d = m_cycleDiscovery.m_curveNet.nodes[i].arcID.size();
		if (maxDegree<d) maxDegree = d;
	}
	File.width(textSize[4]);	File << maxDegree;	File.width(3);	File << " | ";
	File.width(textSize[5] - 2);	File << m_cycleDiscovery.m_timePre + m_cycleDiscovery.m_timeRun; File.width(2);	File << "mm";	File.width(3);	File << " | ";
	File.width(textSize[6] - 2);	File << m_cycleDiscovery.m_timePre; File.width(2);	File << "mm";	File.width(3);	File << " | ";
	File.width(textSize[7] - 2);	File << m_cycleDiscovery.m_timeRun; File.width(2);	File << "mm";	File.width(3);	File << " | ";
	int totalConstraint = 0;
	for (int i = 0; i<m_cycleDiscovery.m_selectArcList.size() - 1; i++)
		totalConstraint += m_cycleDiscovery.m_selectArcList[i].size();
	File.width(textSize[8]);	File << totalConstraint;	File.width(3);	File << " | ";
	int constraintCycle = m_cycleDiscovery.m_selectArcList.size() - 1;
	File.width(textSize[9]);	File << constraintCycle;	File.width(3);	File << " | ";
	int genus = nodeSize - curveSize + m_cycleDiscovery.m_cycleSetBreaked.size() - 2;
	genus /= -2;
	File.width(textSize[10]);	nonManifold == 0 ? genus >= 0 ? File << genus : File << "Disconnect" : File << "NonManifold";	File.width(3);	File << " | ";
	File.width(textSize[11]);	m_cycleDiscovery.m_isCycleBreak == true ? File << m_cycleDiscovery.m_breakNum : File << "No";	File.width(3);	File << " | ";
	File << endl;

	/*
	File<<"Time Consuming:                             "<<m_timeConsuming<<"mm"<<endl;
	File<<"Break Required:                             "<<((m_isCycleBreak==true)? "Yes" : "No")<<endl<<endl;
	File<<"Twist Normal Number:                        "<<m_twistNormNum<<endl;
	File<<"Angle Weight:                               "<<m_angleWeight<<endl;
	File<<"Twist Weight:                               "<<m_twistWeight<<endl;
	File<<"curve Weight:                               "<<m_curveWeight<<endl;
	File<<"Node  Weight:                               "<<m_nodeWeight<<endl;
	File<<"Maximal Adjacent Arcs Size of Single Arc:   "<<m_bestNeightboreNum<<endl;
	File<<"Maximal Rotation Graph Size of Single Node: "<<m_rotationGraphNum<<endl;
	File<<"Dihedral Weight:                            "<<m_dihedralWeight<<endl;
	File<<"Maximal State Num of Graph Search:          "<<m_stateNum<<endl;
	*/
	File.close();
}

void cycleUtils::surfaceBuilding()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;
	auto& m_cycleSetBreaked = m_cycleDiscovery.m_cycleSetBreaked;
	auto& m_cycleNormal = m_cycleDiscovery.m_cycleNormal;
	auto& m_normalsTable = m_cycleDiscovery.m_normalsTable;

	if(m_curveNet.arcs.empty()||m_cycleSetBreaked.empty())
		return;

	if (!(m_cycleDiscovery.m_isUpdateWeightTri || m_cycleDiscovery.m_isUpdateWeightEdge || m_cycleDiscovery.m_isUpdateWeightBiTri || m_cycleDiscovery.m_isUpdateWeightTriBd ||
		m_cycleDiscovery.isUpdateWeightWorsDih || m_cycleDiscovery.m_isUpdateSurfaceSmooth || m_isUpdateSubdivision || m_isUpdateLaplacian)
		&& !m_cycleDiscovery.m_isUpdateCycle)
		return;


	m_cycleNormal.clear();
	m_newPoints.clear();
	m_newNormals.clear();
	m_newPointNum.clear();

	Graph net = m_curveNet;
	CycleSet cycles = m_cycleSetBreaked;
	TriangleSurface surface;
	TriangleSurface norms;

	for(int c=0;c<cycles.size();c++){
		wxString its = wxString::Format(wxT("surface building: %ith patch "), c+1);
		m_statusBar->SetStatusText(its);
		Cycle cycle = cycles[c];
		std::vector<int> arcs;
		LinearCurveNet curves;
		for(int i=0;i<cycle.size();i++){
			int arcID = cycle[i].arcID;			
			curves.push_back(net.arcs[arcID].vertexList);
			int strNode = m_curveNet.arcs[arcID].endNodesID.first;
			int endNode = m_curveNet.arcs[arcID].endNodesID.second;
			curves.back().insert(curves.back().begin(),m_curveNet.nodes[strNode].pos);
			curves.back().push_back(m_curveNet.nodes[endNode].pos);
			if(cycle[i].strEndID==2){
				reverse(curves.back().begin(),curves.back().end());
			}
		}
		Curve pointList;
		for(int i=0;i<curves.size();i++){
			for(int j=0;j<curves[i].size()-1;j++)
				pointList.push_back(curves[i][j]);
		}
		double* points;
		int point_num= pointList.size();
		points= new double[point_num*3];
		for(int i=0;i<pointList.size();i++){
			points[i*3+0]=pointList[i].x;
			points[i*3+1]=pointList[i].y;
			points[i*3+2]=pointList[i].z;
		}
		double* tile_list;
		int tileNum;
		float weights[]={float(m_weightTri),
			float(m_weightEdge),float(m_weightBiTri),float(m_weightTriBd),float(m_weightWorsDih)};

		int res; bool dosmooth=m_SurfaceSmooth; int subs=m_subdivisonSmooth;int laps=m_laplacianSmooth;
		double* newPoints;
		float* newNormals;
		int newPointNum;

		if(m_normalsTable.empty())
		{
// 			res = delaunayRestrictedTriangulation(points, point_num, &newPoints, &newPointNum,
// 				&tile_list, &tileNum, weights, dosmooth, subs, laps);
		}


//////////////////////////////////////
		if(!m_normalsTable.empty())
		{
			CycleSegment cyl = cycle.back();
			cycle.insert(cycle.begin(),cyl);
			cycle.push_back(cycle[1]);
			LinearCurveNet cycleNormal;
			LinearCurveNet endNorm;
			std::vector<double> variance;
			std::vector<int> dirts;
			for(int j=1;j<cycle.size()-1;j++){
				int arcID=cycle[j].arcID;
				int nodeID[]={m_curveNet.arcs[arcID].endNodesID.first,
					m_curveNet.arcs[arcID].endNodesID.second};
				int a1 = cycle[j-1].arcID;
				int a2 = cycle[j+1].arcID;
				int aind1=0;int aind2=0;
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

				cycleNormal.push_back(m_normalsTable[arcID][aind1][aind2]);

				if(cycle[j].strEndID==1){
					dirts.push_back(1);
				}
				else{
					dirts.push_back(2);
					reverse(cycleNormal.back().begin(),cycleNormal.back().end());
					for(int k=0;k<cycleNormal.back().size();k++){
						cycleNormal.back()[k] = -cycleNormal.back()[k];
					}
				}

				cycleNormal.back().pop_back();
				cycleNormal.back().erase(cycleNormal.back().begin());

				std::vector<AML::double3> temp;
				temp.push_back(cycleNormal.back().front()); temp.push_back(cycleNormal.back().back());
				endNorm.push_back(temp);
				variance.push_back(m_cycleDiscovery.m_twistTablesConfidence[arcID][aind1][aind2]);
			}

			std::vector<AML::double3> tema = endNorm.front();
			std::vector<AML::double3> temb = endNorm.back();
			endNorm.insert(endNorm.begin(),temb);
			endNorm.push_back(tema);
			double tvar = variance.back();
			variance.insert(variance.begin(),tvar);
			variance.push_back(variance[1]);

			for(int j=0;j<cycleNormal.size();j++){
				AML::double3 tempL = endNorm[j][1];
				AML::double3 tempR = endNorm[j+2][0];
				Curve pts = curves[j];

				std::vector<std::vector<double> > transportMatrices;
				cycleDiscovery::computeTransportMatrixAll(pts, transportMatrices);
				std::vector<std::vector<AML::double3> > transMatrices;
				for(int t=0;t<transportMatrices.size();t++){
					std::vector<AML::double3> transMatrix;
					for(int j=0;j<3;j++){
						AML::double3 row;
						for(int k=0;k<3;k++){
							row[k]= transportMatrices[t][j+k*3];
						}
						transMatrix.push_back(row);
					}
					transMatrices.push_back(transMatrix);
				}
				std::vector<AML::double3> nmlistL;
				for(int k=0;k<transMatrices.size();k++){
					std::vector<AML::double3> transMatrix = transMatrices[k];
					AML::double3 tnm;
					for(int l=0;l<3;l++){
						tnm[l] = tempL.dot(transMatrix[l]);
					}
					tnm.normalize();
					nmlistL.push_back(tnm);
				}

				reverse(pts.begin(),pts.end());
				transportMatrices.clear();
				cycleDiscovery::computeTransportMatrixAll(pts,transportMatrices);
				transMatrices.clear();
				for(int t=0;t<transportMatrices.size();t++){
					std::vector<AML::double3> transMatrix;
					for(int j=0;j<3;j++){
						AML::double3 row;
						for(int k=0;k<3;k++){
							row[k]= transportMatrices[t][j+k*3];
						}
						transMatrix.push_back(row);
					}
					transMatrices.push_back(transMatrix);
				}
				std::vector<AML::double3> nmlistR;
				for(int k=0;k<transMatrices.size();k++){
					std::vector<AML::double3> transMatrix = transMatrices[k];
					AML::double3 tnm;
					for(int l=0;l<3;l++){
						tnm[l] = tempR.dot(transMatrix[l]);
					}
					tnm.normalize();
					nmlistR.push_back(tnm);
				}
				reverse(nmlistR.begin(),nmlistR.end());
				for(int k=0;k<cycleNormal[j].size();k++){
					AML::double3 tnm= variance[j]*nmlistL[k]+ variance[j+2]*nmlistR[k]+ cycleNormal[j][k];
					tnm.normalize();
					cycleNormal[j][k]=tnm;
				}
			}

			Curve normalList;
			for(int i=0;i<cycleNormal.size();i++){
				for(int j=0;j<cycleNormal[i].size();j++)
					normalList.push_back(cycleNormal[i][j]);
			}

			float* normals;
			int normal_num= normalList.size();
			normals= new float[normal_num*3];
			for(int i=0;i<normalList.size();i++){
				normals[i*3+0]=normalList[i].x;
				normals[i*3+1]=normalList[i].y;
				normals[i*3+2]=normalList[i].z;
			}

// 			res=delaunayRestrictedTriangulation(points,normals,point_num,&newPoints,&newNormals,&newPointNum,&tile_list,&tileNum,weights,
// 				dosmooth,subs,laps);

			LinearCurveNet cycleNormalForVis = cycleNormal;
			for(int j=0;j<cycleNormalForVis.size();j++){
				AML::double3 temp = AML::double3(0,0,0);
				cycleNormalForVis[j].push_back(temp);
				if(dirts[j]==2)
					reverse(cycleNormalForVis[j].begin(),cycleNormalForVis[j].end());
			}
			m_cycleNormal.push_back(cycleNormalForVis);
	
			m_newPoints.push_back(newPoints);
			m_newNormals.push_back(newNormals);
			m_newPointNum.push_back(newPointNum);
		}
		///////////////////////////////////////

		TriangleCycle triangleCycle;
		TriangleCycle triangleCycleNorm;
		if(res==1){
			for ( int i=0; i<tileNum; i++){
				std::vector<AML::double3> triangle;
				for (int j=0;j<3;j++){
					AML::double3 point;
					point.x = tile_list[i*9+j*3+0];
					point.y = tile_list[i*9+j*3+1];
					point.z = tile_list[i*9+j*3+2];
					triangle.push_back(point);
				}
				triangleCycle.push_back(triangle);
			}
			for ( int i=0; i<tileNum; i++){
				std::vector<AML::double3> triangleNorm;
				for (int j=0;j<3;j++){
					AML::double3 norm;
					norm.x = newNormals[i*9+j*3+0];
					norm.y = newNormals[i*9+j*3+1];
					norm.z = newNormals[i*9+j*3+2];
					triangleNorm.push_back(norm);
				}
				triangleCycleNorm.push_back(triangleNorm);
			}
		}
		surface.push_back(triangleCycle);
		norms.push_back(triangleCycleNorm);
	}

/*  here, I'm trying make normals consistent along all patches; but something is wrong;
	std::vector<std::vector<unsigned> > adjList(m_curveNet.arcs.size());
	for(unsigned i=0;i<m_cycleSet.size();i++){
		for(unsigned j=0;j<m_cycleSet[i].size();j++){
			unsigned arcID = m_cycleSet[i][j].arcID;
			adjList[arcID].push_back(i);
		}
	}
	for(unsigned i=0;i<adjList.size();i++){
		for(unsigned j=0;j<adjList[i].size();j++)
			 cout<<"  "<<adjList[i][j];
		cout<<endl;
	}

	std::vector<std::vector<unsigned> > adjCycleList(m_cycleSet.size(),std::vector<unsigned>(1,m_cycleSet.size()));
	std::vector<std::vector<unsigned> > arcShared(m_cycleSet.size());
	for(unsigned i=0;i<adjList.size();i++){
		for(unsigned j=0;j<adjList[i].size()-1;j++){
			for(unsigned k=j+1;k<adjList[i].size();k++){
				if(std::find(adjCycleList[adjList[i][j]].begin(),adjCycleList[adjList[i][j]].end(),adjList[i][k])==
					adjCycleList[adjList[i][j]].end()){
					adjCycleList[adjList[i][j]].push_back(adjList[i][k]);
					arcShared[adjList[i][j]].push_back(i);
				}
				if(std::find(adjCycleList[adjList[i][k]].begin(),adjCycleList[adjList[i][k]].end(),adjList[i][j])==
					adjCycleList[adjList[i][k]].end()){
					adjCycleList[adjList[i][k]].push_back(adjList[i][j]);
					arcShared[adjList[i][k]].push_back(i);
				}
			}
		}
	}
	for(unsigned i=0;i<adjCycleList.size();i++){
		adjCycleList[i].erase(adjCycleList[i].begin());
	}
	for(unsigned i=0;i<adjCycleList.size();i++){
		for(unsigned j=0;j<adjCycleList[i].size();j++)
		 cout<<"  "<<adjCycleList[i][j];
		cout<<endl;
		for(unsigned j=0;j<arcShared[i].size();j++)
		 cout<<"  "<<arcShared[i][j];
		cout<<endl;
	}

	std::vector<bool> checkedList(m_cycleSet.size(),false);
	checkedList.front()=true;
	unsigned count=0;
	for(unsigned i=0;i<adjCycleList.size();i++){
		std::vector<unsigned> &adjCycles = adjCycleList[i];
		for(unsigned j=0;j<adjCycles.size();j++){
			if(checkedList[adjCycles[j]]==true) continue;
			unsigned adjArc = arcShared[i][j];
			std::vector<AML::double3> p(3);
			if(!m_curveNet.arcs[adjArc].vertexList.empty())
				p[0] = m_curveNet.arcs[adjArc].vertexList.front();
			else
				p[0] = m_curveNet.nodes[m_curveNet.arcs[adjArc].endNodesID.first].pos; 

			unsigned edgeDirection[2]={3,3};
			std::vector<unsigned> ind;
			for(unsigned t=0;t<surface[i].size();t++){
				std::vector<AML::double3> &triA = surface[i][t];
				unsigned tind=std::find(triA.begin(),triA.end(),p[0])-triA.begin();
				if(tind<3){
					ind.push_back(tind);ind.push_back((tind+1)%3);ind.push_back((tind+2)%3);
					p.push_back(triA[ind[1]]);p.push_back(triA[ind[2]]);
					cout<<"find"<<endl;
				}
			}
			std::vector<unsigned> ind2;
			for(unsigned t=0;t<surface[adjCycles[j]].size();t++){
				std::vector<AML::double3> &triA = surface[adjCycles[j]][t];
				unsigned tind=std::find(triA.begin(),triA.end(),p[0])-triA.begin();
				if(tind<3){
					unsigned tind2=std::find(triA.begin(),triA.end(),p[1])-triA.begin();
					if(tind2<3){
						if((ind[1]-ind[0])==1 || (ind[0]-ind[1])==2)
							edgeDirection[0]=1;
						else
							edgeDirection[0]=2;
						if((tind-tind2)==1 || (tind-tind2)==2)
							edgeDirection[1]=1;
						else
							edgeDirection[1]=2;
						count++;
						break;
					}
					else{
						unsigned tind3=std::find(triA.begin(),triA.end(),p[2])-triA.begin();
						if(tind3<3){
							if((ind[1]-ind[2])==1 || (ind[0]-ind[2])==2)
								edgeDirection[0]=1;
							else
								edgeDirection[0]=2;
							if((tind-tind3)==1 || (tind-tind3)==2)
								edgeDirection[1]=1;
							else
								edgeDirection[1]=2;
							count++;
							break;					
						}
					}
				}

			}
			if(edgeDirection[0]==edgeDirection[1] && edgeDirection[0]!=3 &&edgeDirection[1]!=3 ){
				std::vector<std::vector<AML::double3> > &triCyc = surface[adjCycles[j]];
				std::vector<std::vector<AML::double3> > &normCyc = norms[adjCycles[j]];
				for(unsigned t=0;t<triCyc.size();t++){
					AML::double3 tp=triCyc[t].front();
					triCyc[t][0]=triCyc[t][1];
					triCyc[t][1]=tp;
					normCyc[t][0]=-normCyc[t][0];normCyc[t][1]=-normCyc[t][1];normCyc[t][2]=-normCyc[t][2];
				}
			}
			checkedList[adjCycles[j]]=true;
		}
	}

	cout<<"count:"<<count<<endl;
*/
	m_triangleSurface=surface;
	m_triangleSurfaceNormal=norms;
	m_cycleDiscovery.m_isUpdateWeightTri = m_cycleDiscovery.m_isUpdateWeightEdge = m_cycleDiscovery.m_isUpdateWeightBiTri =
		m_cycleDiscovery.m_isUpdateWeightTriBd = m_cycleDiscovery.isUpdateWeightWorsDih = m_cycleDiscovery.m_isUpdateSurfaceSmooth =
		m_isUpdateSubdivision = m_isUpdateLaplacian = false;
	m_cycleDiscovery.m_isUpdateCycle = false;

	buildList();	
}
void cycleUtils::subDivideCycles()
{
	for (unsigned i = 0; i < m_cycleDiscovery.m_curveNet.arcs.size(); i++)
	{
		auto curve = m_cycleDiscovery.m_curveNet.arcs[i].vertexList;


	}
	//m_curves;
	//m_curveNet;
	//cycleCurvatureThres;
}
