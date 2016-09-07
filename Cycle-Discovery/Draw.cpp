#include "cycleUtils.h"
#include <GL/glu.h>
#include <GL/glut.h>


void cycleUtils::drawNode(int nodeID)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (m_curveNet.nodes.empty())
		return;

	Point p = m_curveNet.nodes[nodeID].pos;
	glBegin(GL_POINTS);
	glVertex3f(p[0], p[1], p[2]);
	glEnd();
}
void cycleUtils::drawIntersectionPoints()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (m_showSingleNode == true){
		glColor3f(1, 0, 0);
		glPointSize(5.);
		drawNode(m_selectedNodeID);
		glPointSize(1.);
		return;
	}

	glColor3f(0, 0, 1);
	glPointSize(m_nodeSize);
	for (int i = 0; i<m_curveNet.nodes.size(); i++){
		glBegin(GL_POINTS);
		Point p = m_curveNet.nodes[i].pos;
		glVertex3f(p[0], p[1], p[2]);
		glEnd();
	}
	glPointSize(1.0);
}
void cycleUtils::drawArcRaw(int arcID)
{
	auto& m_curves = m_cycleDiscovery.m_curves;

	if (m_curves.empty())
		return;
	Curve curve = m_curves[arcID];
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j<curve.size(); ++j){
		Point p = curve[j];
		glVertex3f(p[0], p[1], p[2]);
	}
	glEnd();
}
void cycleUtils::drawArc(int arcID)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (m_curveNet.nodes.empty())
		return;
	Point first, last;
	first = m_curveNet.nodes[m_curveNet.arcs[arcID].endNodesID.first].pos;
	last = m_curveNet.nodes[m_curveNet.arcs[arcID].endNodesID.second].pos;
	Curve curve = m_curveNet.arcs[arcID].vertexList;
	curve.push_back(last);
	curve.insert(curve.begin(), first);

	/*
	AML::double3 centroid(0,0,0);
	unsigned int numPoints = 0;
	for (unsigned int i=0;i<curve.size(); i++){
	AML::double3 p = curve[i];
	centroid += p;
	numPoints++;
	}
	centroid *= 1.0/numPoints;
	*/

	glBegin(GL_LINE_STRIP);
	for (int j = 0; j<curve.size(); ++j){
		Point p = curve[j];
		/*
		p = p*AML::Scalar(0.9)+centroid*AML::Scalar(0.1);
		*/
		glVertex3f(p[0], p[1], p[2]);
	}
	glEnd();//GL_LINE_STRIP
}
void cycleUtils::drawSeletedArcs()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (m_selectArcColors.empty())
		Colormap::colormapHSV(12, m_selectArcColors);
	else if (m_selectArcColors.size()<m_cycleDiscovery.m_selectArcList.size())
		Colormap::colormapHSV(m_selectArcColors.size() * 2, m_selectArcColors);

	if (m_showConstraint)
	{
		glLineWidth(1 + 2 * m_curveLineSize);
		for (int i = 0; i<m_cycleDiscovery.m_selectArcList.size(); i++){
			Colormap::color cc = m_selectArcColors[i];
			glColor3f(cc.r, cc.g, cc.b);
			for (int j = 0; j<m_cycleDiscovery.m_selectArcList[i].size(); j++){
				drawArc(m_cycleDiscovery.m_selectArcList[i][j]);
			}
		}
		if (m_isViewing){
			glLineWidth(4 + 2 * m_curveLineSize);
			drawArc(m_selectedArcID);
			glLineWidth(1.);
		}
	}
	if (m_showCapacity){
		glColor3f(1, 0, 0);
		AML::double3 p;
		for (int i = 0; i<m_cycleDiscovery.m_errorNode.size(); i++){
			p = m_curveNet.nodes[m_cycleDiscovery.m_errorNode[i]].pos;
			drawString(p, "x", 4);
		}

		int cap;
		if (std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) != m_cycleDiscovery.m_latestUpdateArcs.end()){
			int ind = std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), m_selectedArcID) - m_cycleDiscovery.m_latestUpdateArcs.begin();
			cap = m_cycleDiscovery.m_latestUpdateArcsCapacity[ind];
		}
		else
			cap = m_curveNet.arcs[m_selectedArcID].capacity;
		if (cap == 1)
			glColor3f(0, 0, 1);
		else if (cap == 2)
			glColor3f(0, 0, 0);
		else
			glColor3f(1, 0, 1);
		if (m_isViewing){
			glLineWidth(5);
			drawArc(m_selectedArcID);
			glLineWidth(1.);

			int arcID;
			if (m_curveNet.arcs[m_selectedArcID].vertexList.empty()){
				arcID = m_curveNet.arcs[m_selectedArcID].endNodesID.first;
				p = m_curveNet.nodes[arcID].pos;
				arcID = m_curveNet.arcs[m_selectedArcID].endNodesID.second;
				p += m_curveNet.nodes[arcID].pos;
				p /= 2;
			}
			else
				p = m_curveNet.arcs[m_selectedArcID].vertexList[(m_curveNet.arcs[m_selectedArcID].vertexList.size()) / 2];
			char screenPrint[32];
			_itoa_s(cap, screenPrint, 10);
			drawString(p, screenPrint, 6);
		}
	}
}
void cycleUtils::drawCurveNetworkRaw()
{
	auto& m_curves = m_cycleDiscovery.m_curves;

	glColor3f(0, 1, 0);
	for (int i = 0; i<m_curves.size(); i++){
		Curve curve = m_curves[i];
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j<curve.size(); j++){
			Point p = curve[j];
			glVertex3f(p[0], p[1], p[2]);
		}
		glEnd();//GL_LINE_STRIP
	}
}
void cycleUtils::setCurveColors(std::vector<double> &color1, std::vector<double> &color2)
{
	Colormap::color c1, c2;
	c1.r = color1[0] / 255.; c1.g = color1[1] / 255.; c1.b = color1[2] / 255.;
	c2.r = color2[0] / 255.; c2.g = color2[1] / 255.; c2.b = color2[2] / 255.;
	m_curveColors.clear();
	Colormap::colormapCustomize(c1, c2, m_colorInterval, m_curveColors);
}
void cycleUtils::setCycleColors(std::vector<double> &color1, std::vector<double> &color2)
{
	Colormap::color c1, c2;
	c1.r = color1[0] / 255.; c1.g = color1[1] / 255.; c1.b = color1[2] / 255.;
	c2.r = color2[0] / 255.; c2.g = color2[1] / 255.; c2.b = color2[2] / 255.;
	m_cycleColors.clear();
	Colormap::colormapCustomize(c1, c2, m_colorInterval, m_cycleColors);
}
void cycleUtils::changeCycleColor()
{
	colorMapCycle(true);
}
void cycleUtils::drawString(const Point &p, const char* str, const double size)
{
	glPushMatrix();
	glTranslated(p.x, p.y, p.z);
	glScalef(size*0.0002, size*0.0002, size*0.0002);
	glMatrixMode(GL_MODELVIEW);
	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	float xx = modelview_matrix[0];
	float yx = modelview_matrix[4];
	float zx = modelview_matrix[8];
	float d = sqrt(xx*xx + yx*yx + zx*zx);
	for (int i = 0; i<12; i++){
		//if(i%4==3) continue;
		if (i % 5 == 0) modelview_matrix[i] = d;
		else modelview_matrix[i] = 0;
	}
	glLoadIdentity();
	glMultMatrixf(modelview_matrix);
	double eyeChange = modelview_matrix[14] / -2.0;
	glScalef(eyeChange, eyeChange, eyeChange);

	freetype::print(our_font, 0, 0, str, 0);
	glPopMatrix();
	glFlush();
}
void cycleUtils::drawCurveNetwork()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;
	auto& m_arcsCost = m_cycleDiscovery.m_arcsCost;

	if (m_curveNet.arcs.empty())
		return;
	if (m_curveColors.empty()){
		std::vector<Colormap::color> Colors;
		Colormap::colormapRedToBlack(10, Colors);
		m_curveColors = Colors;
	}
	if (m_arcsCost.empty())
		m_arcsCost.resize(m_curveNet.arcs.size(), .5);

	for (int i = 0; i<m_curveNet.arcs.size(); i++){
		Point first, last;
		first = m_curveNet.nodes[m_curveNet.arcs[i].endNodesID.first].pos;
		last = m_curveNet.nodes[m_curveNet.arcs[i].endNodesID.second].pos;
		Curve curve = m_curveNet.arcs[i].vertexList;
		curve.push_back(last);
		curve.insert(curve.begin(), first);

		int wdth = 0;
		if (m_showCapacity){
			wdth = m_curveNet.arcs[i].capacity;
			if (!m_cycleDiscovery.m_latestUpdateArcsCapacity.empty() && std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), i) !=
				m_cycleDiscovery.m_latestUpdateArcs.end()){
				int ind = std::find(m_cycleDiscovery.m_latestUpdateArcs.begin(), m_cycleDiscovery.m_latestUpdateArcs.end(), i) -
					m_cycleDiscovery.m_latestUpdateArcs.begin();
				wdth = m_cycleDiscovery.m_latestUpdateArcsCapacity[ind];
			}
			if (wdth == 1){
				glColor3f(0, 0, 1); glLineWidth(m_curveLineSize * 2);
			}
			else if (wdth == 2){
				glColor3f(0, 0, 0); glLineWidth(m_curveLineSize * 1);
			}
			else{
				glColor3f(1, 0, 1); glLineWidth(m_curveLineSize * 2);
			}
		}
		else{
			glLineWidth(m_curveLineSize * 1);
			glColor3f(0, 0, 0);
			if (m_showArcMode == 1){
				glLineStipple(2, 0x00FF);
				glEnable(GL_LINE_STIPPLE);
			}
		}

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j<curve.size(); ++j){
			Point p = curve[j];
			glVertex3f(p[0], p[1], p[2]);
		}
		glEnd();//GL_LINE_STRIP
		glDisable(GL_LINE_STIPPLE);
		glLineWidth(1);
		if ((i != m_selectedArcID || !m_isViewing) && wdth != 0 && wdth != 2){
			Point p;
			if (curve.size() == 2){
				p = curve.front() + curve.back(); p /= 2;
			}
			else
				p = curve[curve.size() / 2];
			char screenPrint[32];
			wdth %= 8;
			_itoa_s(wdth, screenPrint, 10);
			drawString(p, screenPrint, 4);
		}
	}
}

void cycleUtils::drawElementofRotationGraph(int nodeID, int orgArcID, int conArcID, int orgInstanceID, int conInstanceID, int type)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (m_curveNet.nodes.empty())
		return;
	AML::double3 centre = m_curveNet.nodes[nodeID].pos;
	AML::double3 str, end;
	if (m_curveNet.arcs[orgArcID].endNodesID.first == nodeID){
		if (m_curveNet.arcs[orgArcID].vertexList.empty())
			str = m_curveNet.nodes[m_curveNet.arcs[orgArcID].endNodesID.second].pos;
		else
			str = m_curveNet.arcs[orgArcID].vertexList.front();
	}
	else{
		if (m_curveNet.arcs[orgArcID].vertexList.empty())
			str = m_curveNet.nodes[m_curveNet.arcs[orgArcID].endNodesID.first].pos;
		else
			str = m_curveNet.arcs[orgArcID].vertexList.back();
	}

	if (m_curveNet.arcs[conArcID].endNodesID.first == nodeID){
		if (m_curveNet.arcs[conArcID].vertexList.empty())
			end = m_curveNet.nodes[m_curveNet.arcs[conArcID].endNodesID.second].pos;
		else
			end = m_curveNet.arcs[conArcID].vertexList.front();
	}
	else{
		if (m_curveNet.arcs[conArcID].vertexList.empty())
			end = m_curveNet.nodes[m_curveNet.arcs[conArcID].endNodesID.first].pos;
		else
			end = m_curveNet.arcs[conArcID].vertexList.back();
	}
	AML::double3 strArrow = str - centre;
	AML::double3 endArrow = end - centre;
	str = strArrow;
	end = endArrow;

	double pos, length;
	if (type == NODE){
		pos = exp(((double)(m_selectedRotationGraphPosition)-50) / 20);
		length = exp(((double)(m_selectedRotationGraphLength)-50) / 20);
	}
	else{
		pos = exp(((double)(m_selectedPoleRotationGraphPosition)-50) / 20);
		length = exp(((double)(m_selectedPoleRotationGraphLength)-50) / 20);
	}

	if (str != end)
		centre += (strArrow + endArrow)*AML::Scalar(pos)*AML::Scalar(1. + double(orgInstanceID));
	else
		centre += (strArrow + endArrow)*AML::Scalar(double(orgInstanceID) / 10);


	str = centre + str*AML::Scalar(length);
	end = centre + end*AML::Scalar(length);

	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex3f(centre[0], centre[1], centre[2]);
	glVertex3f(str[0], str[1], str[2]);
	glVertex3f(centre[0], centre[1], centre[2]);
	glVertex3f(end[0], end[1], end[2]);
	glEnd();
	glLineWidth(1.0);
	glPointSize(6.0);
	glBegin(GL_POINTS);
	glColor3f(orgInstanceID % 2, 0, (orgInstanceID + 1) % 2);
	glVertex3f(str[0], str[1], str[2]);
	glColor3f(conInstanceID % 2, 0, (conInstanceID + 1) % 2);
	glVertex3f(end[0], end[1], end[2]);
	glEnd();
	glPointSize(1.0);
}
void cycleUtils::drawRotationGraph()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	RotationGraph rotationGraph;
	if (!m_showCyclesBreaked)
		rotationGraph = m_cycleDiscovery.m_rotationGraph;
	else
		rotationGraph = m_cycleDiscovery.m_rotationGraphUpdate;

	if (rotationGraph.empty())
		return;

	std::vector<std::vector<std::vector<bool> > > usedList;
	for (int i = 0; i<rotationGraph.size(); i++){
		std::vector<std::vector<bool> > usedNode;
		int nodeSize = rotationGraph[i].size();
		for (int j = 0; j<nodeSize; j++){
			std::vector<bool> usedNodeArcs;
			usedNodeArcs.assign(rotationGraph[i][j].size(), false);
			usedNode.push_back(usedNodeArcs);
		}
		usedList.push_back(usedNode);
	}

	for (int i = 0; i<rotationGraph.size(); i++){
		std::vector<Colormap::color> colors;
		unsigned int index = 0;
		Colormap::colormapHeatColor(rotationGraph[i].size() * 3, colors);

		for (int j = 0; j<rotationGraph[i].size(); j++){
			Colormap::color cc = colors[index];
			glColor3f(cc.r, cc.g, cc.b);
			index += 3;
			int orgArcID = m_curveNet.nodes[i].arcID[j];
			for (int k = 0; k<rotationGraph[i][j].size(); k++){
				int conArcIDRelative = rotationGraph[i][j][k].first;
				int conInstanceID = rotationGraph[i][j][k].second;
				int conArcID = m_curveNet.nodes[i].arcID[conArcIDRelative];
				//draw angle
				if (usedList[i][j][k] == false && usedList[i][conArcIDRelative][conInstanceID] == false){
					drawElementofRotationGraph(i, orgArcID, conArcID, k, conInstanceID, NODE);
					usedList[i][j][k] = true;
					usedList[i][conArcIDRelative][conInstanceID] = true;
				}
			}
		}
	}
}
void cycleUtils::drawCycle(int cycleID)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	Cycle cycle;
	/*
	if(!m_showCyclesBreaked){
	if(m_cycleSet.empty())
	return;
	cycleID = cycleID%m_cycleSet.size();
	cycle = m_cycleSet[cycleID];
	}
	else{
	if(m_cycleSetBreaked.empty())
	return;
	cycleID = cycleID%m_cycleSetBreaked.size();
	cycle = m_cycleSetBreaked[cycleID];
	}
	*/

	/*
	if(m_cycleSetBreaked.empty()){
	if(m_cycleSet.empty())
	return;
	cycle = m_cycleSet[cycleID];
	}
	else{
	*/
	if (m_showCyclesBreaked)
		cycle = m_cycleDiscovery.m_cycleSetBreaked[cycleID];
	else
		cycle = m_cycleDiscovery.m_cycleSet[cycleID];
	/*
	}
	*/
	LinearCurveNet curves;
	for (int i = 0; i<cycle.size(); i++){
		int arcID = cycle[i].arcID;
		Point first, last;
		first = m_curveNet.nodes[m_curveNet.arcs[arcID].endNodesID.first].pos;
		last = m_curveNet.nodes[m_curveNet.arcs[arcID].endNodesID.second].pos;
		Curve curve = m_curveNet.arcs[arcID].vertexList;
		curve.push_back(last);
		curve.insert(curve.begin(), first);
		curves.push_back(curve);
	}

	AML::double3 centroid(0, 0, 0);
	unsigned int numPoints = 0;
	for (int i = 0; i<curves.size(); i++){
		for (int j = 0; j<curves[i].size(); j++){
			AML::double3 p = curves[i][j];
			centroid += p;
			numPoints++;
		}
	}
	centroid *= 1.0 / numPoints;

	for (int i = 0; i<curves.size(); i++){
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j<curves[i].size(); j++){
			Point p = curves[i][j];
			double cycleSize = double(m_selectedCycleSize) / 10.;
			p = p*AML::Scalar(cycleSize) + centroid*AML::Scalar(1 - cycleSize);
			glVertex3f(p[0], p[1], p[2]);
		}
		glEnd();//GL_LINE_STRIP
	}

	if (/*m_showSurface&&*/m_showNormal&&!m_cycleDiscovery.m_cycleNormal.empty()){
		glColor3f(0, 0, 0);
		for (int i = 0; i<m_cycleDiscovery.m_cycleNormal[cycleID].size(); i++){
			glBegin(GL_LINES);
			for (int j = 0; j<m_cycleDiscovery.m_cycleNormal[cycleID][i].size(); j++){
				Point p = m_cycleDiscovery.m_cycleNormal[cycleID][i][j];
				p /= 20;
				Point q = curves[i][j];
				p = p + q;
				glVertex3f(p[0], p[1], p[2]);
				glVertex3f(q[0], q[1], q[2]);
			}
			glEnd();//GL_LINE_STRIP
		}
	}
}
void cycleUtils::colorMapCycle(bool isbreaked)
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;

	if (!isbreaked){
		if (!m_colorsCycle.empty())
			return;
	}

	CycleSet cycleSet = isbreaked ? m_cycleDiscovery.m_cycleSetBreaked : m_cycleDiscovery.m_cycleSet;
	int cycleSize = cycleSet.size();

	int arcSize = m_curveNet.arcs.size();
	std::vector<std::vector<int> > cycleInArcs(arcSize);
	for (int i = 0; i<cycleSize; i++){
		for (int j = 0; j<cycleSet[i].size(); j++){
			cycleInArcs[cycleSet[i][j].arcID].push_back(i);
		}
	}

	int nodeSize = m_curveNet.nodes.size();
	std::vector<std::vector<int> > cycleInNodes(nodeSize);
	for (int i = 0; i<cycleSize; i++){
		for (int j = 0; j<cycleSet[i].size(); j++){
			int node = m_curveNet.arcs[cycleSet[i][j].arcID].endNodesID.first;
			cycleInNodes[node].push_back(i);
			node = m_curveNet.arcs[cycleSet[i][j].arcID].endNodesID.second;
			cycleInNodes[node].push_back(i);
		}
	}

	std::vector<std::vector<bool> > cycleConnected(cycleSize, std::vector<bool>(cycleSize, false));
	for (int i = 0; i<arcSize; i++){
		for (int j = 0; j<cycleInArcs[i].size() - 1; j++){
			for (int k = j + 1; k<cycleInArcs[i].size(); k++){
				cycleConnected[cycleInArcs[i][j]][cycleInArcs[i][k]] = true;
				cycleConnected[cycleInArcs[i][k]][cycleInArcs[i][j]] = true;
			}
		}
	}
	for (int i = 0; i<nodeSize; i++){
		for (int j = 0; j<cycleInNodes[i].size() - 1; j++){
			for (int k = j + 1; k<cycleInNodes[i].size(); k++){
				cycleConnected[cycleInNodes[i][j]][cycleInNodes[i][k]] = true;
				cycleConnected[cycleInNodes[i][k]][cycleInNodes[i][j]] = true;
			}
		}
	}
	std::vector<int> colorMap(cycleSize);
	std::vector<int> colorInd;
	int newColor = 0;
	colorInd.push_back(newColor);
	colorMap[0] = colorInd[0];
	for (int i = 1; i<cycleSize; i++){
		std::vector<int> tcolors = colorInd;
		for (int j = 0; j<i; j++){
			if (cycleConnected[i][j]){
				tcolors[colorMap[j]] = -1;
			}
		}
		if (*std::max_element(tcolors.begin(), tcolors.end())<0 || i<4){
			newColor++;
			colorInd.push_back(newColor);
			colorMap[i] = newColor;
		}
		else{
			int randInd = -1;
			while (randInd<0){
				randInd = tcolors[rand() % tcolors.size()];
			}
			colorMap[i] = tcolors[randInd];
			/*
			colorMap[i] = *std::max_element(tcolors.begin(),tcolors.end());
			*/
		}
	}

	std::vector<Colormap::color> colors;
	Colormap::colormapBSC(colorInd.size(), colors);
	/*
	for(int i=0;i<colorInd.size();i++){
	colors[i] = colors[i+1];
	}
	colors.resize(colorInd.size());
	*/

	if (isbreaked){
		m_colorsCycleBreak = colors;
		m_colorsCycleBreakInd = colorMap;
	}
	else{
		m_colorsCycle = colors;
		m_colorsCycleInd = colorMap;
	}
}
void cycleUtils::drawCycles()
{
	auto& m_curveNet = m_cycleDiscovery.m_curveNet;
	auto& m_cyclesCost = m_cycleDiscovery.m_cyclesCost;

	int cycleSize;
	if (m_showCyclesBreaked){
		if (m_cycleDiscovery.m_cycleSetBreaked.empty())
			cycleSize = m_cycleDiscovery.m_cycleSet.size();
		else
			cycleSize = m_cycleDiscovery.m_cycleSetBreaked.size();
	}
	else{
		cycleSize = m_cycleDiscovery.m_cycleSet.size();
	}

	if (cycleSize == 0)
		return;

	if (m_showCyclesBreaked&&m_cycleColors.empty()){
		std::vector<Colormap::color> Colors;
		Colormap::colormapLightGrayToRed(10, Colors);
		m_cycleColors = Colors;
	}
	if (m_cyclesCost.empty())
		m_cyclesCost.resize(m_cycleDiscovery.m_cycleSetBreaked.size(), 0);
	if (m_colorsCycleBreak.empty() && !m_cycleDiscovery.m_cycleSetBreaked.empty())
		colorMapCycle(true);
	/*
	std::vector<Colormap::color> colors = !m_cycleSetBreaked.empty()? m_colorsCycleBreak:m_colorsCycle;
	std::vector<int> colorInd = !m_cycleSetBreaked.empty()? m_colorsCycleBreakInd:m_colorsCycleInd;
	*/
	std::vector<Colormap::color> colors = m_showCyclesBreaked ? m_colorsCycleBreak : m_colorsCycle;
	std::vector<int> colorInd = m_showCyclesBreaked ? m_colorsCycleBreakInd : m_colorsCycleInd;

	if (m_showCycleMode == 1 && !m_showSurface){
		int worseID = std::max_element(m_cyclesCost.begin(), m_cyclesCost.end()) - m_cyclesCost.begin();
		Cycle cycle;
		cycle = m_cycleDiscovery.m_cycleSetBreaked[worseID];
		LinearCurveNet curves;
		for (int i = 0; i<cycle.size(); i++){
			int arcID = cycle[i].arcID;
			Point first, last;
			first = m_curveNet.nodes[m_curveNet.arcs[arcID].endNodesID.first].pos;
			last = m_curveNet.nodes[m_curveNet.arcs[arcID].endNodesID.second].pos;
			Curve curve = m_curveNet.arcs[arcID].vertexList;
			curve.push_back(last);
			curve.insert(curve.begin(), first);
			curves.push_back(curve);
		}
		AML::double3 centroid(0, 0, 0);
		double maxDist = 0;
		unsigned int numPoints = 0;
		for (int i = 0; i<curves.size(); i++){
			for (int j = 0; j<curves[i].size(); j++){
				AML::double3 p = curves[i][j];
				centroid += p;
				numPoints++;
			}
		}
		centroid *= 1.0 / numPoints;
		for (int i = 0; i<curves.size(); i++){
			for (int j = 0; j<curves[i].size(); j++){
				AML::double3 p = curves[i][j] - centroid;
				if (p.length()>maxDist)
					maxDist = p.length();
			}
		}


		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		float modelview_matrix1[16];//we only want the retation part;    
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix1);
		float modelview_matrix2[16];//we only parts only without rotation;    
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix2);
		modelview_matrix2[14] = -2;
		modelview_matrix2[15] = 1;
		glMultMatrixf(modelview_matrix2);
		glTranslatef(.8, 0.6, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix2);

		glTranslatef(-0.15, -0.1, 0);
		glColor3f(0, 0, 0);
		char screenPrint[32];
		strcpy(screenPrint, "Worst Cycle: ");
		char nc[32];
		int n = int(m_cyclesCost[worseID]);
		_itoa_s(n, nc, 10);
		screenPrint[13] = nc[0];
		screenPrint[14] = '.';

		n = (int(m_cyclesCost[worseID] * 10)) % 10;
		_itoa_s(n, nc, 10);
		screenPrint[15] = nc[0];
		n = (int(m_cyclesCost[worseID] * 100)) % 10;
		_itoa_s(n, nc, 10);
		screenPrint[16] = nc[0];
		screenPrint[17] = '\0';
		AML::double3 pos = AML::double3(0, 0, 0);
		drawString(pos, screenPrint, 2);

		modelview_matrix2[0] = modelview_matrix1[0];
		modelview_matrix2[1] = modelview_matrix1[1];
		modelview_matrix2[2] = modelview_matrix1[2];
		modelview_matrix2[4] = modelview_matrix1[4];
		modelview_matrix2[5] = modelview_matrix1[5];
		modelview_matrix2[6] = modelview_matrix1[6];
		modelview_matrix2[8] = modelview_matrix1[8];
		modelview_matrix2[9] = modelview_matrix1[9];
		modelview_matrix2[10] = modelview_matrix1[10];

		glLoadIdentity();
		glMultMatrixf(modelview_matrix2);
		glLineWidth(2);

		int index = 9 - int(m_cyclesCost[worseID] * 10);
		if (index<0) index = 0;
		Colormap::color cc = m_cycleColors[index];
		glColor3f(cc.r, cc.g, cc.b);

		glScalef(.15 / maxDist, .15 / maxDist, .15 / maxDist);
		for (int i = 0; i<curves.size(); i++){
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j<curves[i].size(); j++){
				Point p = curves[i][j] - centroid;
				glVertex3f(p[0], p[1], p[2]);
			}
			glEnd();//GL_LINE_STRIP
		}
		glPopMatrix();
	}

	if (m_showSingleCycle == true){
		glLineWidth(m_lineSize);
		if (m_showCyclesBreaked){
			if (m_showCycleMode == 0){
				Colormap::color cc = colors[colorInd[m_selectedCycleID]];
				glColor3f(cc.r, cc.g, cc.b);
			}
			else{
				int index = 9 - int(m_cyclesCost[m_selectedCycleID] * 10);
				if (index<0) index = 0;
				Colormap::color cc = m_cycleColors[index];
				glColor3f(cc.r, cc.g, cc.b);
			}
			drawCycle(m_selectedCycleID);
		}
		else{
			Colormap::color cc = colors[colorInd[m_selectedCycleID]];
			glColor3f(cc.r, cc.g, cc.b);
		}
		glLineWidth(1.);
		return;
	}

	glLineWidth(m_lineSize);
	for (int i = 0; i<cycleSize; i++){
		if (m_showCyclesBreaked){
			if (m_showCycleMode == 0){
				Colormap::color cc = colors[colorInd[i]];
				glColor3f(cc.r, cc.g, cc.b);
			}
			else{
				int index = 9 - int(m_cyclesCost[i] * 10);
				if (index<0) index = 0;
				Colormap::color cc = m_cycleColors[index];
				glColor3f(cc.r, cc.g, cc.b);

			}
			drawCycle(i);
		}
		else{
			Colormap::color cc = colors[colorInd[i]];
			glColor3f(cc.r, cc.g, cc.b);
			drawCycle(i);
		}
	}
	glLineWidth(1.0);
}
void cycleUtils::buildList()
{
	if (m_triangleSurface.empty()) return;

	m_meshListWire.resize(m_triangleSurface.size());
	m_meshListSolid.resize(m_triangleSurface.size());

	unsigned anyT = glGenLists(m_meshListWire.size() + m_meshListSolid.size());
	for (unsigned i = anyT; i<anyT + m_meshListWire.size(); i++){
		m_meshListWire[i - anyT] = i;
		cout << " " << m_meshListWire[i - anyT];
	}

	for (unsigned i = anyT + m_meshListWire.size(); i<anyT + m_meshListWire.size() + m_meshListSolid.size(); i++){
		m_meshListSolid[i - anyT - m_meshListWire.size()] = i;
		cout << " " << m_meshListSolid[i - anyT - m_meshListWire.size()];
	}

	for (unsigned c = 0; c<m_meshListWire.size(); c++){
		glNewList(m_meshListWire[c], GL_COMPILE);

		TriangleCycle triangleCycle = m_triangleSurface[c];
		TriangleCycle triangleCycleNormal;
		if (!m_triangleSurface.empty()) triangleCycleNormal = m_triangleSurface[c];

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i<triangleCycle.size(); i++){
			std::vector<AML::double3> triangle = triangleCycle[i];
			std::vector<AML::double3> verticesNormal;
			if (!triangleCycleNormal.empty()) verticesNormal = triangleCycleNormal[i];
			for (int j = 0; j<triangle.size(); j++){
				AML::double3 position = triangle[j];
				if (!verticesNormal.empty()){
					AML::double3 norm = verticesNormal[j];
					glNormal3f(norm.x, norm.y, norm.z);
				}
				glVertex3f(position.x, position.y, position.z);
			}
		}
		glEnd();
		glEndList();
	}
	for (unsigned c = 0; c<m_meshListSolid.size(); c++){
		glNewList(m_meshListSolid[c], GL_COMPILE);

		TriangleCycle triangleCycle = m_triangleSurface[c];
		TriangleCycle triangleCycleNormal;
		if (!m_triangleSurface.empty()) triangleCycleNormal = m_triangleSurface[c];

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i<triangleCycle.size(); i++){
			std::vector<AML::double3> triangle = triangleCycle[i];
			std::vector<AML::double3> verticesNormal;
			if (!triangleCycleNormal.empty()) verticesNormal = triangleCycleNormal[i];
			for (int j = 0; j<triangle.size(); j++){
				AML::double3 position = triangle[j];
				if (!verticesNormal.empty()){
					AML::double3 norm = verticesNormal[j];
					glNormal3f(norm.x, norm.y, norm.z);
				}
				glVertex3f(position.x, position.y, position.z);
			}
		}
		glEnd();
		glEndList();
	}
}
void cycleUtils::drawPatch(int patchID)
{
	if (m_triangleSurface.size() == 0)
		return;

	/*	if(m_showSurfaceMode==0)
	glCallList(m_meshListWire[patchID]);
	else
	glCallList(m_meshListSolid[patchID]);

	return;
	*/
	const TriangleCycle &triangleCycle = m_triangleSurface[patchID];
	const TriangleCycle &triangleCycleNormal = m_triangleSurfaceNormal[patchID];
	if (m_showSurfaceMode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i<triangleCycle.size(); i++){
		const std::vector<AML::double3> &triangle = triangleCycle[i];
		const std::vector<AML::double3> &verticesNormal = triangleCycleNormal[i];
		/*		AML::double3 norm = triangle[1]-triangle[0];
		norm = norm.cross(triangle[2]-triangle[0]);
		norm.normalize();
		glNormal3f(norm.x,norm.y,norm.z);
		*/
		for (int j = 0; j<triangle.size(); j++){
			const AML::double3 &position = triangle[j];
			const AML::double3 &norm = verticesNormal[j];
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex3f(position.x, position.y, position.z);
		}
	}
	glEnd();
}
void cycleUtils::drawSurface()
{
	auto& m_cyclesCost = m_cycleDiscovery.m_cyclesCost;

	if (m_triangleSurface.size() == 0)
		return;
	//surface patches index are identical to cycles;
	std::vector<Colormap::color> colors;
	std::vector<int> colorsInd;
	unsigned int index = 0;
	if (!m_cycleColors.empty() && m_showCycleMode == 1)
		colors = m_cycleColors;
	else if (m_showCycleMode == 0 && !m_colorsCycleBreak.empty()){
		colors = m_colorsCycleBreak;
		colorsInd = m_colorsCycleBreakInd;
	}
	else
		Colormap::colormapHeatColor(m_triangleSurface.size(), colors);

	if (m_showCycles&&m_showSingleCycle){
		Colormap::color cc = m_cycleColors[int(1 - m_cyclesCost[m_selectedCycleID]) * 9];
		if (m_showCycleMode == 0)
			cc = m_colorsCycleBreak[m_colorsCycleBreakInd[m_selectedCycleID]];
		glColor3f(cc.r, cc.g, cc.b);
		drawPatch(m_selectedCycleID);

	}
	else{
		for (int i = 0; i<m_triangleSurface.size(); i++){
			Colormap::color cc;
			if (!m_cycleColors.empty() && m_showCycleMode == 1){
				int index = 9 - int(m_cyclesCost[i] * 10);
				if (index<0) index = 0;
				cc = colors[index];
			}
			else if (m_showCycleMode == 0 && !m_colorsCycleBreak.empty())
				cc = colors[colorsInd[i]];
			else
				cc = colors[i];

			glColor3f(cc.r, cc.g, cc.b);
			drawPatch(i);
		}
	}
}
void cycleUtils::renderNewNormal()
{
	if (m_showSingleCycle){
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		double * newPnt = m_newPoints[m_selectedCycleID];
		float * newNmls = m_newNormals[m_selectedCycleID];
		for (int j = 0; j < m_newPointNum[m_selectedCycleID]; j++){
			Point p, q;
			p.x = newPnt[j * 3 + 0]; p.y = newPnt[j * 3 + 1]; p.z = newPnt[j * 3 + 2];
			q.x = newNmls[j * 3 + 0]; q.y = newNmls[j * 3 + 1]; q.z = newNmls[j * 3 + 2];
			q /= 20;	q = p + q;
			glVertex3f(p[0], p[1], p[2]);
			glVertex3f(q[0], q[1], q[2]);
		}
		glEnd();
	}
	else{
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for (int i = 0; i < m_newPointNum.size(); i++){
			int cycN = 0;
			for (int s = 0; s < m_cycleDiscovery.m_cycleNormal[i].size(); s++)
				cycN += m_cycleDiscovery.m_cycleNormal[i][s].size() - 1;
			if (m_newPointNum[i] != cycN){
				//cout<<endl<<m_newPointNum[i]==cycN? true: false << " newNormalNum"<<i<<":"<<m_newPointNum[i]<< " cylNormNum:"<<cycN<<endl;
				cout << "==========================" << " newNormalNum" << i << ":" << m_newPointNum[i] << " cylNormNum:" << cycN << endl;
			}
			double * newPnt = m_newPoints[i];
			float * newNmls = m_newNormals[i];
			for (int j = 0; j < m_newPointNum[i]; j++){
				Point p, q;
				p.x = newPnt[j * 3 + 0]; p.y = newPnt[j * 3 + 1]; p.z = newPnt[j * 3 + 2];
				q.x = newNmls[j * 3 + 0]; q.y = newNmls[j * 3 + 1]; q.z = newNmls[j * 3 + 2];
				q /= 20;	q = p + q;
				glVertex3f(p[0], p[1], p[2]);
				glVertex3f(q[0], q[1], q[2]);
			}
		}
		glEnd();
	}
}

void cycleUtils::Render()
{
	if (m_showSurface){
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		/*float brt = ((float)m_brightness)*0.1;
		GLfloat As[4] = {brt, brt, brt, 1.0f };*/
		GLfloat As[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, As);
		GLfloat mat_ambient_front[] = { 0.15f, 0.12f, .02f, 1.0f };
		glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_front);
	}
	else{
		glEnable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		GLfloat As[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, As);
		GLfloat mat_ambient_back[] = { 0.15f, 0.12f, .02f, 1.0f };
		glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_back);
	}

	if (m_showCurveNetwork/*m_showIntersectionPoints*/)
		drawIntersectionPoints();
	if (m_showSingleCurve == true)
		drawSeletedArcs();
	if (m_showCurveNetwork)
		drawCurveNetwork();
	if (m_showRotationGraph)
		drawRotationGraph();
	if (m_showCycles)
		drawCycles();
	if (m_showSurface)
		drawSurface();
	if (m_showNewNormal)
		renderNewNormal();
}

