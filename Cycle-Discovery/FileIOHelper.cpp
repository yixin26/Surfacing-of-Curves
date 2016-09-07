#include "fileIOHelper.h"
#include <string.h>
#include <wx/msgdlg.h>
#include <map>
#include "GraphStructure.h"

FileIOHelper::FileIOHelper()
{
	//m_SvgLoader = new SimpleSVGLoader(); //NOSVG
}

FileIOHelper::~FileIOHelper()
{
	//delete m_SvgLoader; //NOSVG
}

//-----------------------------------------------------------
void resample(const std::vector< std::vector<AML::double3> > inputCurves,
			  std::vector< std::vector<AML::double3> > &outputCurves)
{
	/// for each curve see if the smaping is  enough.
	// if they are too distanc add extra points in between
	// the sample points so you have a denser point sampling
	for (unsigned int i=0 ; i < inputCurves.size(); i++){
		std::vector<AML::double3> curve = inputCurves[i];
		double distance=0;
		for (unsigned int j=0 ; j < (curve.size()-1); j++){
			AML::double3 d = curve[j+1] - curve[j];
			double l = (double)(d.length());
			distance = distance + l;
		}
		distance = (double)(distance/(double)(curve.size()));
		//double length = distance.length();
		if (distance > 0.01){ // resample
			std::vector<AML::double3> resampledCurve;
			for (unsigned int j=0 ; j < (curve.size()-1); j++){
				AML::double3 midPoint = (curve[j+1] + curve[j])/(double)(2.0);
				resampledCurve.push_back(curve[j]);
				resampledCurve.push_back(midPoint);
			}
			resampledCurve.push_back(curve[curve.size()-1]);
			outputCurves.push_back(resampledCurve);

		}
		else
			outputCurves.push_back(curve);
	}
}
//-----------------------------------------------------------
void scaleCurveNet(const AML::Scalar point_closeness_thr,
		   std::vector< std::vector<AML::double3> > &allCurves,
			std::vector< std::vector<AML::double3> > &scaledResampledcurves)
{
	AML::double3 bBoxMin(1e10,1e10,1e10);
	AML::double3 bBoxMax(-1e10,-1e10,-1e10);
	AML::double3 centroid(0,0,0);

	unsigned int numPoints = 0;

	for (unsigned int i=0;i<allCurves.size(); i++){
		for (unsigned int j=0;j<allCurves[i].size();j++){
			//Rep::MeshRepousseSolver::Mesh::Point p = curves[i][j];
			AML::double3 p = allCurves[i][j];

			for (unsigned int d=0;d<3;d++){
				if (p[d]>bBoxMax[d])
					bBoxMax[d] = p[d];
				if (p[d]<bBoxMin[d])
					bBoxMin[d] = p[d];
			}
			centroid += p;
			numPoints++;
		}
	}

	centroid *= 1.0/numPoints;

	//find the sides of bbox
	AML::double3 bBox;
	for (unsigned int d=0;d<3;d++)
		bBox[d] = fabs(bBoxMin[d]-bBoxMax[d]);

	//find the largest side
	double largestSide = (bBox[0]>bBox[1]) ? bBox[0]: bBox[1];
	largestSide = (largestSide > bBox[2]) ? largestSide : bBox[2];

	//translate the points so centroid is at origin
	//scale the points so bbox fits in canonical cube
	for (unsigned int i=0;i<allCurves.size(); i++){
		for (unsigned int j=0;j<allCurves[i].size();j++){
			//Rep::MeshRepousseSolver::Mesh::Point p = curves[i][j];
			AML::double3 p = allCurves[i][j];
			p = p - centroid;
			//scale the points so bbox fits in canonical cube
			p *= 1.0/largestSide;
			allCurves[i][j] = p;
		}
	}

	//scale the points so bbox fits in canonical cube
	//for (unsigned int i=0;i<curves.size(); i++){
	//	for (unsigned int j=0;j<curves[i].size();j++){
	//		//Rep::MeshRepousseSolver::Mesh::Point p = curves[i][j];
	//		AML::double3 p = curves[i][j];
	//		p *= 1.0/largestSide;
	//		curves[i][j] = p;
	//	}
	//}

	//-----------------------------------------
	//	Prune the curves that are tooooo small
	//-----------------------------------------
	std::vector< std::vector<AML::double3> > curves;
	AML::double3 max,min;
	AML::double3 boundingBoxMin;
	AML::double3 boundingBoxMax;
	boundingBoxMin.x = boundingBoxMin.y = boundingBoxMin.z = 1e10;
	boundingBoxMax.x = boundingBoxMax.y = boundingBoxMax.z = -1e10;
	//boundingBoxMax(-1e10,-1e10,-1e10);
	for (unsigned int i=0;i<allCurves.size(); i++){

		// compute the bounding box of this curve
		AML::double3 minPt(1e10,1e10,1e10);
		AML::double3 maxPt(-1e10,-1e10,-1e10);
		for (unsigned int j=0;j<allCurves[i].size();j++){
			AML::double3 p = allCurves[i][j];

			if (p.x > maxPt.x)
				maxPt.x = p.x;
			if (p.y > maxPt.y)
				maxPt.y = p.y;
			if (p.z > maxPt.z)
				maxPt.z = p.z;
			if (p.x < minPt.x)
				minPt.x = p.x;
			if (p.y < minPt.y)
				minPt.y = p.y;
			if (p.z < minPt.z)
				minPt.z = p.z;
		}

		double X = maxPt.x - minPt.x;
		double Y = maxPt.y - minPt.y;
		double Z = maxPt.z - minPt.z;

		//double volume = X*Y*Z;
		// prune the curves that are too small, noise, etc.

		if ((X < point_closeness_thr) && (Y < point_closeness_thr) && ( Z< point_closeness_thr)){
			continue;
		}
		else{
			curves.push_back(allCurves[i]);

			// update the bounding box
			for (unsigned int d=0;d<3;d++){

				if (maxPt[d]>boundingBoxMax[d])
					boundingBoxMax[d] = maxPt[d];

				if (minPt[d]<boundingBoxMin[d])
					boundingBoxMin[d] = minPt[d];
			}
		}
	}

	resample(curves,scaledResampledcurves);
}

void scalePointSet(std::vector<double> &points)
{
	AML::double3 bBoxMin(1e10,1e10,1e10);
	AML::double3 bBoxMax(-1e10,-1e10,-1e10);
	AML::double3 centroid(0,0,0);

	unsigned int numPoints = 0;

	for (unsigned int i=0;i<points.size(); i+=3){
		AML::double3 p(points[i],points[i+1],points[i+2]);
		for (unsigned int d=0;d<3;d++){
			if (p[d]>bBoxMax[d])
				bBoxMax[d] = p[d];
			if (p[d]<bBoxMin[d])
				bBoxMin[d] = p[d];
		}
		centroid += p;
		numPoints++;
	}

	centroid *= 1.0/numPoints;

	//find the sides of bbox
	AML::double3 bBox;
	for (unsigned int d=0;d<3;d++)
		bBox[d] = fabs(bBoxMin[d]-bBoxMax[d]);

	//find the largest side
	double largestSide = (bBox[0]>bBox[1]) ? bBox[0]: bBox[1];
	largestSide = (largestSide > bBox[2]) ? largestSide : bBox[2];

	//translate the points so centroid is at origin
	//scale the points so bbox fits in canonical cube
	for (unsigned int i=0;i<points.size(); i+=3){
		AML::double3 p(points[i],points[i+1],points[i+2]);
		p = p - centroid;
		//scale the points so bbox fits in canonical cube
		p *= 1.0/largestSide;
		for(int j=0;j<3;j++){
			points[i+j]=p[j];
		}
	}
}

bool FileIOHelper::ReadMayaCurve(
		const AML::Scalar point_closeness_thr,
		const char* fileName,
		std::vector< std::vector<AML::double3> >& curves,
		AML::double3 &boundingBoxMin,
		AML::double3 &boundingBoxMax)
{
	if (!fileName)
		return false;

	std::ifstream fin(fileName);
	//std::ifstream fin("testMayaCurve.rtf");

	if (!fin.good())
		return false;

	curves.clear();

	const unsigned MAX_LINE_LENGTH = 1024;
	char line[MAX_LINE_LENGTH]={0};			//the actual line from the input file
	char tempLine[MAX_LINE_LENGTH]={0};		//copy of the above (for tokenizing)
	char lineData[MAX_LINE_LENGTH]={0};		//store individual tokens in here..one by one
	char meshFileName[MAX_LINE_LENGTH]={0};

	double shapeCoeffs[MAX_LINE_LENGTH] = {0};

	unsigned int numCurves = 0;
	std::vector< std::vector<AML::double3> > allCurves;

	while (fin.getline(line, MAX_LINE_LENGTH)) {
		strcpy(tempLine, line);

		//split the line into tokens using '\t' or space as the delimiter
		char * pch;
		pch = strtok(tempLine,"\t ");
		if (pch != NULL) {
			//check if this is the beginning of the 'createNode nurbsCurve'
			if (!strcmp(pch, "createNode"))	{
				pch = strtok (NULL, "\t ");

				if (!strcmp(pch, "nurbsCurve")) {
					//read in this curve
					//std::vector<Rep::MeshRepousseSolver::Mesh::Point> curve;
					std::vector<AML::double3> curve;
					curve.clear();

					//ignore first three lines
					for (unsigned int i=0;i<3;i++)
						fin.getline(line, MAX_LINE_LENGTH);
					numCurves++;

					//now read the # of samples
					int numSamples = 0;
					fin>>numSamples;
					double param = 0; //unused for now
					for (int i=0;i<numSamples;i++)
						fin>>param;

					//now read the # of samples (again)
					fin>>numSamples;
					for (int i=0;i<numSamples;i++){
						double x=0,y=0,z=0;
						fin>>x;
						fin>>y;
						fin>>z;
						// Fatemeh
						if (fabs(x) < 0.000000000000001)
							x = 0;
						if (fabs(y) < 0.000000000000001)
							y = 0;
						if ( fabs(z) < 0.000000000000001)
							z = 0;
						//---------------
						//Rep::MeshRepousseSolver::Mesh::Point p(x,y,z);
						AML::double3 p(x,y,z);
						curve.push_back(p);
					}
					allCurves.push_back(curve);
				} //if found a nurbsCurve
			} //if this was a createNode
		} // pch was not NULL

		//clear the lines
		for (int i=0;i<MAX_LINE_LENGTH;i++)
		{
			line[i]=tempLine[i]=lineData[i]=0;
		}
	}
	fin.close();


	// *** resize the curves so their bounding box has a max. side of length 2 ****

	scaleCurveNet(point_closeness_thr, allCurves, curves);

	return true;
}

bool FileIOHelper::ReadCurve(		 const char* fileName,
									 std::vector< std::vector<AML::double3> >& curves,
									 std::vector< int> &capacity)
{
	wxString fina = fileName;	
	if (fina.length()==0)
		return false;

	std::ifstream reader(fileName);
	//std::ifstream fin("testMayaCurve.rtf");

	if (!reader.good())
		return false;

	curves.clear();

	unsigned int numCurves = 0;
	int numPoints,j,k;
	std::vector< std::vector<AML::double3> > allCurves;


	reader >> numCurves;
	if(numCurves<=0) return false;

	//read point set data
	AML::double3 point;
	capacity.assign(numCurves,2);

	for(k=0;k<numCurves;k++){
		reader >> numPoints >> j >> capacity[k];
		std::vector<AML::double3> curve;
		for (int i=0; i<numPoints; i++) {
			reader >> point.x>>point.y>>point.z;
			curve.push_back(point);
		}
		allCurves.push_back(curve);
	}
//	curves = allCurves;
	reader.close();


	// *** resize the curves so their bounding box has a max. side of length 2 ****
	const double point_closeness_thr = 0;
	scaleCurveNet(point_closeness_thr, allCurves, curves);
	return true;
}

bool FileIOHelper::ReadCrv(	
				 const char* fileName,
				 std::vector< std::vector<AML::double3> >& curves)
{
	if (!fileName)
		return false;

	std::ifstream reader;
	reader.open(fileName,ios::in|ios::binary);
	//std::ifstream fin("testMayaCurve.rtf");

	if (!reader.good())
		return false;

	curves.clear();

	unsigned int numCorners = 0;
	unsigned int numEdges = 0;
	unsigned int numPatches=0;
	std::vector<CVR::Corner> allCorners;
/*
	CVR::Patch patch;
*/
	reader.read((char*)(&numCorners),4);
	reader.read((char*)(&numEdges),4);
	reader.read((char*)(&numPatches),4);
	//read corner;
	for(int i=0;i<numCorners;i++){
		CVR::Corner corner;
		reader.read((char*)(&corner.numEdges),4);
		corner.edges = new int[corner.numEdges];
		for(int j=0;j<corner.numEdges;j++)
			reader.read((char*)(&corner.edges[j]),4);
		reader.read((char*)(&corner.numPatches),4);
		corner.patches = new int[corner.numPatches];
		for(int j=0;j<corner.numPatches;j++)
			reader.read((char*)(&corner.patches[j]),4);
		reader.read((char*)(&corner.pos.x),4);
		reader.read((char*)(&corner.pos.y),4);
		reader.read((char*)(&corner.pos.z),4);
		reader.read((char*)(&corner.numColinear),4);
		corner.colinear = new CVR::EdgePair[corner.numColinear];
		for(int j=0;j<corner.numColinear;j++){
			reader.read((char*)(&corner.colinear[j].e1),4);
			reader.read((char*)(&corner.colinear[j].e2),4);
		}
		allCorners.push_back(corner);
	}

	for(int i=0;i<numEdges;i++){
		CVR::Edge edge;
		reader.read((char*)(&edge.sharp),4);
		reader.read((char*)(&edge.c1),4);
		reader.read((char*)(&edge.c2),4);
		reader.read((char*)(&edge.numVerts),4);
		edge.verts = new CVR::Pos[edge.numVerts];
		for(int j=0;j<edge.numVerts;j++){
			reader.read((char*)(&edge.verts[j].x),4);
			reader.read((char*)(&edge.verts[j].y),4);
			reader.read((char*)(&edge.verts[j].z),4);
		}
		
		std::vector<AML::double3> curve;
		{AML::double3 p(allCorners[edge.c1].pos.x,allCorners[edge.c1].pos.y,allCorners[edge.c1].pos.z);
		curve.push_back(p);}
		for(int j=0;j<edge.numVerts;j++){
			AML::double3 p(edge.verts[j].x,edge.verts[j].y,edge.verts[j].z);
			curve.push_back(p);
		}
		AML::double3 p(allCorners[edge.c2].pos.x,allCorners[edge.c2].pos.y,allCorners[edge.c2].pos.z);
		curve.push_back(p);
		curves.push_back(curve);
	}

	reader.close();
	return true;
}


bool FileIOHelper::ReadMesh_OFF(
                                const char* fileName,
                                std::vector<double> &points,
                                std::vector<unsigned> &faces)
{
 	if (!fileName) //*.mesh
		return false;

	std::ifstream reader(fileName);

	if (!reader.good())
		return false;

	points.clear();
	faces.clear();

	char* fileType;
	int count=strlen(fileName)-1;
	while(count>=0)
	{
		if(fileName[count]=='.')
			break;
		count--;
	}
	fileType = new char[strlen(fileName)-count-1];
	for(int i=0;i<strlen(fileType);i++){
		fileType[i]=fileName[count+i+1];
	}
	if(strcmp(fileType,"off")==0)
		reader.getline(fileType,10);

	unsigned int numPoints = 0;
	unsigned int numFaces = 0;
	unsigned int van;

	reader >> numPoints;
	reader >> numFaces;
	reader >> van;

	if(numPoints<=0) return false;

	for(int i=0;i<numPoints;i++)
	{
	    for(int j=0;j<3;j++){
	        double p;
            reader >> p;
            points.push_back(p);
	    }
	}
	for(int i=0;i<numFaces;i++)
	{
		reader >> van;
	    for(int j=0;j<3;j++){
	        unsigned p;
            reader >> p;
            faces.push_back(p);
	    }
	}

	reader.close();

	scalePointSet(points);

    return true;
}

bool FileIOHelper::WriteCurve(
		const char* fileName,
		const std::vector< std::vector<AML::double3> >& curves,
		const std::vector< int> &capacity)
{
	std::ofstream File;
	File.open(fileName,std::ios::out);

	File<<curves.size()<<endl;
	for(int i=0;i<curves.size();i++)
	{
		if (capacity.empty())
		{
			File << curves[i].size() << ' ' << '0' << ' ' << 2 << endl;
		}
		else
		{
			File << curves[i].size() << ' ' << '0' << ' ' << capacity[i] << endl;
		}
		for(int j= 0;j<curves[i].size();j++)
		{
			AML::double3 point = curves[i][j];
			File<<point[0]<<' '<<point[1]<<' '<<point[2]<<endl;
		}
	}
	File.close();

	return true;
}

bool FileIOHelper::ReadCycle(	
				   const char* fileName,
				   CycleSet& cycles)
{
	wxString fina = fileName;	
	if (fina.length()==0)
		return false;

	std::ifstream reader(fileName);
	if (!reader.good())
		return false;

	cycles.clear();
	unsigned int numCycle;
	reader >> numCycle;
	if(numCycle<=0) return false;
	for(int i=0;i<numCycle;i++){
		Cycle cycle;
		int numCurve;
		reader >> numCurve;
		for(int j=0;j<numCurve;j++) {
			CycleSegment cycleseg;
			reader >>cycleseg.arcID>>cycleseg.strEndID>>cycleseg.instanceID;
			cycle.push_back(cycleseg);
		}
		cycles.push_back(cycle);
	}
	reader.close();
	return true;
}

bool FileIOHelper::WriteCycle(	
					const char* fileName,
					const CycleSet& cycles)
{
	if(cycles.empty())
		return false;
	if(strlen(fileName)==0)
		return false;

	std::ofstream File;
	File.open(fileName,std::ios::out);

	File<<cycles.size()<<endl;
	for(int i=0;i<cycles.size();i++){
		File<<cycles[i].size()<<endl;
		for(int j= 0;j<cycles[i].size();j++){
			CycleSegment cycleseg = cycles[i][j];
			File<<cycleseg.arcID<<' '<<cycleseg.strEndID<<' '<<cycleseg.instanceID<<endl;
		}
	}
	File.close();
	return true;
}

bool FileIOHelper::ReadMesh(
					const char* fileName,
					TriangleSurface& mesh)
{
	wxString fina = fileName;	
	if (fina.length()==0)
		return false;

	std::ifstream reader(fileName);
	if (!reader.good())
		return false;

	mesh.clear();

	unsigned int numPatch;
	reader >> numPatch;
	if(numPatch<=0) return false;
	for(int i=0;i<numPatch;i++)
	{
		int numTri;
		reader >> numTri;
		TriangleCycle patch;
		for(int j=0;j<numTri;j++) 
		{
			std::vector<AML::double3> triangle;
			for(int k=0;k<3;k++)
			{
				AML::double3 point;
				reader >> point.x >> point.y >> point.z;
				triangle.push_back(point);
			}
			patch.push_back(triangle);
		}
		mesh.push_back(patch);
	}
	reader.close();
	return true;
}
bool FileIOHelper::WriteMesh(
					const char* fileName,
					const TriangleSurface& mesh,
					const TriangleSurface& meshNormal)
{
	if(mesh.empty())
		return false;
	if(strlen(fileName)==0)
		return false;

	bool ishaveNormal = meshNormal.empty()?false:true;
	ishaveNormal=false;
	//make point list and triangle list from input;
	std::vector<Point> pointList; pointList.push_back(mesh[0][0][0]);
	std::vector<Point> pointNormals;if(ishaveNormal) pointNormals.push_back(meshNormal[0][0][0]);
	std::vector<std::vector<unsigned> > faceList;
	unsigned pSize=0;
	for(unsigned i=0;i<mesh.size();i++){
		const TriangleCycle &patch = mesh[i];
		for(unsigned j=0;j<patch.size();j++){
			const std::vector<Point> &triangle = patch[j];
			std::vector<unsigned> tf;
			for(unsigned k=0;k<triangle.size();k++){
				const Point &tp = triangle[k];
				unsigned pID;
				if(std::find(pointList.begin(),pointList.end(),tp)==pointList.end()){
					pointList.push_back(tp); pID = pointList.size()-1;
					if(ishaveNormal) pointNormals.push_back(meshNormal[i][j][k]);
				}
				else
					pID = std::find(pointList.begin(),pointList.end(),tp) - pointList.begin();
				tf.push_back(pID);
			}
			faceList.push_back(tf);
		}
	}

	std::ofstream File;
	File.open(fileName,std::ios::out);

	File<<"OFF"<<endl;
	File<<pointList.size()<<' '<<faceList.size()<<' '<<0<<endl;
	for(unsigned i=0;i<pointList.size();i++){
		Point &coord = pointList[i];
		File<<coord.x<<' '<<coord.y<<' '<<coord.z<<endl;
	}
	//File<<"# "<<pointList.size()<<" vertices"<<endl<<endl;

	if(ishaveNormal){
		for(unsigned i=0;i<pointNormals.size();i++){
			Point &norm = pointNormals[i];
			File<<"vn "<<norm.x<<' '<<norm.y<<' '<<norm.z<<endl;
		}	
		File<<"# "<<pointNormals.size()<<" normals"<<endl<<endl;

	}
	for(unsigned i=0;i<faceList.size();i++){
		std::vector<unsigned> &tri = faceList[i];
		File<<"3 "<<' '<<tri[0]<<' '<<tri[1]<<' '<<tri[2]<<endl;
	}
	//File<<"# "<<faceList.size()<<" faces"<<endl;
	File.close();
	return true;
}

void FileIOHelper::ProcessContour(const char* fileName)
{
	wxString fina = fileName;
	if (fina.length() == 0)
		return ;

	std::ifstream reader(fileName);
	//std::ifstream fin("testMayaCurve.rtf");

	if (!reader.good())
		return ;

	unsigned int numContoures = 0;
	reader >> numContoures;
	std::vector<std::vector<AML::double3>> contours;

	if (numContoures <= 0) return;

	double tdummy;
	AML::double3 tp;

	for (unsigned i = 0; i < numContoures; i++)
	{
		for (unsigned t = 0; t < 4; t++) reader >> tdummy;
		int numPoints, numEdges;
		reader >> numPoints >> numEdges;
		std::vector<AML::double3> tps;
		for (unsigned j = 0; j < numPoints; j++)
		{
			reader >> tp.x >> tp.y >> tp.z;
			tps.push_back(tp);
		}
		std::vector<std::pair<int, int> >edges(numEdges);
		int v1, v2;
		int v0;
		for (unsigned t = 0; t < numEdges; t++)
		{
			reader >> v1 >> v2;
			reader >> tdummy >> tdummy;
			edges[t].first = v1;
			edges[t].second= v2;
		}
		unsigned count = 0;
		while (count < numEdges)
		{
			std::vector<AML::double3> tc;

			tc.push_back(tps[edges[count].first]);
			tc.push_back(tps[edges[count].second]);
			unsigned t = count+1;
			for (; t < numEdges; t++)
			{
				if (((t+1) < numEdges) && edges[t-1].second != edges[t].first)
				{
					break;
				}
				tc.push_back(tps[edges[t].second]);
			}
			count = t;
			contours.push_back(tc);		
		}

	}
	reader.close();

	//turn contour into curves;
	std::vector<std::map<StdPoint, unsigned> > firstCounts(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		auto& tf = firstCounts[i];
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (tf.find(StdPoint(contours[i][j])) == tf.end())
			{
				tf[StdPoint(contours[i][j])] = 1;
			}
		}
	}

	std::map<StdPoint, unsigned> countJoints;
	for (int i = 0; i < firstCounts.size(); i++)
	{
		auto& tf = firstCounts[i];
		for (auto j = tf.begin(); j != tf.end(); j++)
		{
			if (countJoints.find(j->first) == countJoints.end())
			{
				countJoints[j->first] = 1;
			}
			else
			{
				countJoints[j->first] = countJoints[j->first] + 1;
				cout << "i:" << i << " j:" << j->first.m_val.x << " " << j->first.m_val.y << " " << j->first.m_val.z << endl;
			}
		}
	}
// 	for (int i = 0; i < contours.size(); i++)
// 	{
// 		for (int j = 0; j < contours[i].size(); j++)
// 		{
// 			if (countJoints.find(IncreaseOrder(contours[i][j])) == countJoints.end())
// 			{
// 				countJoints[IncreaseOrder(contours[i][j])] = 1;
// 			}
// 			else
// 			{
// 				countJoints[IncreaseOrder(contours[i][j])] = countJoints[IncreaseOrder(contours[i][j])] + 1;
// 			}
// 		}
// 	}
// 

	std::vector<std::vector<AML::double3>> curves;
	for (int i = 0; i < contours.size(); i++)
	{
		int j = 0;
		while (j<contours[i].size()-1)
		{
			std::vector<AML::double3> tcur;
			tcur.push_back(contours[i][j]);
			int k = j+1;
			for (; k < contours[i].size(); k++)
			{
				tcur.push_back(contours[i][k]);
				if (countJoints[StdPoint(contours[i][k])] != 1)//this is a joint
				{
					cout << "a joint ";
					break;
				}
			}
			curves.push_back(tcur);
			j = k;
		}
// 		curves.push_back(contours[i]);
	}

	//save;
	std::ofstream File;
	char newfile[100];
	strcpy(newfile, fileName);
	strcat(newfile, ".curve");

	File.open(newfile, std::ios::out);

	File << curves.size() << endl;
	for (int i = 0; i < curves.size(); i++)
	{
		File << curves[i].size() << ' ' << '0' << ' ' << '2' << endl;
		for (int j = 0; j < curves[i].size(); j++)
		{
			AML::double3& point = curves[i][j];
			File << point[0] << ' ' << point[1] << ' ' << point[2] << endl;
		}
	}
	File.close();
}
