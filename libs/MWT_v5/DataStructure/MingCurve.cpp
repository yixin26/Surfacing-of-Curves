#include "MingCurve.h"
#include <algorithm>
MingCurve::MingCurve(const char *file, int limit, bool hasNorm){
	numofpoints = 0; PT_LIMIT = limit; EXPSTOP = false; withNorm = hasNorm;isDeGen=false; badInput=false;perturbNum=0;

	newEdge.push_back(0); newEdge.push_back(0);
	newAdj.push_back(0); newAdj.push_back(0);
	newNorm.push_back(0); newNorm.push_back(0);
	newClip.push_back(0); newClip.push_back(0);
	readOrgCurveFile(file);
	if(withNorm){
		readOrgNormFile();
	}


}

MingCurve::MingCurve(const double* inCurve,const int inNum, int limit, bool hasNorm){
	numofpoints = 0; PT_LIMIT = limit; EXPSTOP = false; withNorm = hasNorm;isDeGen=false;badInput=false;perturbNum=0;
	newEdge.push_back(0); newEdge.push_back(0);
	newAdj.push_back(0); newAdj.push_back(0);
	newNorm.push_back(0); newNorm.push_back(0);
	newClip.push_back(0); newClip.push_back(0);
	loadOrgCurve(inCurve, inNum);
}

MingCurve::MingCurve(const double* inCurve,const float* inNorm, const int inNum, int limit, bool hasNorm){
	numofpoints = 0; PT_LIMIT = limit; EXPSTOP = false; withNorm = hasNorm;isDeGen=false; badInput=false;perturbNum=0;
	newEdge.push_back(0); newEdge.push_back(0);
	newAdj.push_back(0); newAdj.push_back(0);
	newNorm.push_back(0); newNorm.push_back(0);
	newClip.push_back(0); newClip.push_back(0);
	loadOrgCurve(inCurve, inNum);
	if (withNorm) {
		loadOrgNorm(inNorm);
	}
}

MingCurve::~MingCurve(){
	delete [] filename;
	if (!badInput&&!isDeGen){
		delete [] points; 
		// delete more
		if(withNorm){
			delete [] normals;
		}
	}
}

int MingCurve::getNumOfPoints(){ return numofpoints; }
double* MingCurve::getPoints(){ return points; }
double* MingCurve::getDeGenPoints(){ return DeGenPoints; }
char* MingCurve::getFilename(){ return filename; }
float* MingCurve::getNormal() { return normals; }

//=====================================File IO==================================//
// Read in curve files before edge protection, 
// store curve/adj index/norm index information in tempC/tempAdj/tempNorm //
bool MingCurve::readOrgCurveFile(const char* file){
	org_n = 0;
	
	filename = new char[300]; 
	strcpy_s(filename,300,file);
	char* dot=strrchr(filename,'.'); *dot = '\0';
	ifstream reader(file, ifstream::in);
	if ( !reader.good()){
		errors(ERROR_CURVE,filename);
	}
	//read curve description
	reader >> numofcurves;
	reader >> org_n >> isOpen >> capacity;
	radius.reserve(org_n);
	orgradius.reserve(org_n);
	numofpoints = org_n;
	if(org_n>PT_LIMIT) {
		EXPSTOP=true; 
		errors(ERROR_LIMIT,filename);
	}
	n_before = org_n;
	//read point set data
	double * v = new double[3];
	for (int i=0; i<org_n; i++) {
		reader >> v[0] >> v[1] >> v[2];
		tempC.push_back(Point3(v[0],v[1],v[2]));
		tempOrgC.push_back(Point3(v[0],v[1],v[2]));
		newAdj[0] = i+1; newAdj[1] = i-1; 
		tempAdj.push_back(newAdj);
	}
	tempAdj[0][1] = org_n -1;
	tempAdj[org_n-1][0] = 0;
	delete [] v;
	reader.close();
	
	return 1;
}

// Read in curve files before edge protection, 
// store curve/adj index/norm index information in tempC/tempAdj/tempNorm //
bool MingCurve::loadOrgCurve(const double* inCurve,const int inNum){
	const char* file = "null.curve";
	filename = new char[300]; 
	strcpy_s(filename,300,file);

	numofcurves = 1;
	org_n = inNum;
	radius.reserve(org_n);
	orgradius.reserve(org_n);
	numofpoints = org_n;
	isOpen = 0;
	capacity = 2;

	n_before = org_n;
	//read point set data
	for (int i=0; i<org_n; i++) {
		tempC.push_back(Point3(inCurve[i*3],inCurve[i*3 + 1],inCurve[i*3 + 2]));
		tempOrgC.push_back(Point3(inCurve[i*3],inCurve[i*3 + 1],inCurve[i*3 + 2]));
		newAdj[0] = i+1; newAdj[1] = i-1; 
		tempAdj.push_back(newAdj);
	}
	tempAdj[0][1] = org_n -1;
	tempAdj[org_n-1][0] = 0;
	
	return 1;
}

void MingCurve::readOrgNormFile(){
	char * normfile = new char[300];
	strcpy_s(normfile,300,filename);
	strcat_s(normfile,300,".normal");
	std::ifstream reader(normfile, std::ifstream::in);
	if ( !reader.good()){
		errors(ERROR_NORM,filename);
	}
	//read normal description
	reader >> numofcurves;
	reader >> numofnormals;
	if(numofnormals!=numofpoints){
		errors(ERROR_MATCH,filename);
	}
	//read normal set data
	float * v = new float[3];
	for (int i=0; i<numofnormals; i++) {
		reader >> v[0] >> v[1] >> v[2];
		tempNorm.push_back(Vector3(v[0],v[1],v[2]));
		newNorm[0] = i; newNorm[1] = i-1;
		tempAdjNorm.push_back(newNorm);
	}
	tempAdjNorm[0][1] = org_n-1;
	delete [] v;
	reader.close();
	withNorm = true;
}

void MingCurve::loadOrgNorm(const float* inNorm){
	numofnormals = org_n;
	//read normal set data
	for (int i=0; i<numofnormals; i++) {
		tempNorm.push_back(Vector3(inNorm[i*3],inNorm[i*3 + 1],inNorm[i*3 + 2]));
		newNorm[0] = i; newNorm[1] = i-1;
		tempAdjNorm.push_back(newNorm);
	}
	tempAdjNorm[0][1] = org_n-1;

	withNorm = true;
}

void MingCurve::saveCurve(const char* curvefilein, double* pts, int num){
	char * newcurvefile = new char[300];
	strcpy_s(newcurvefile,300,curvefilein);
	strcat_s(newcurvefile,300,".EP.curve");
	ofstream writer(newcurvefile, ofstream::out);
	writer.precision(15);
	delete [] newcurvefile;
	if (!writer.good()) return;
	writer << "1\n" << num << " " << "0 1\n";
	for(int i=0; i<num; i++){
		writer << pts[i*3] << " " << pts[i*3+1] << " " << pts[i*3+2] << "\n";
	}
	writer.close();
}
//=====================================Get Measures==================================//
double MingCurve::getAngle(int p1, int p2, int p3){
	double res = 0.0;
	Point3 v1 = tempC[p1];
	Point3 v2 = tempC[p2];
	Point3 v3 = tempC[p3];
	Vector3 vec1 = v1-v2; Vector3 vec2 = v1-v3;
	vec1.normalize(); vec2.normalize();
	res = vec1*vec2;
	res = res < -1.0 ? -1.0 : res;
	res = res > 1.0 ? 1.0 : res;
	return acos(res);
}
double MingCurve::getPt2LineDist(int p1, int p2, int p3){
	double res = 0.0;
	Point3 v1 = tempC[p1];
	Point3 v2 = tempC[p2];
	Point3 v3 = tempC[p3];
	Vector3 vec12 = v2-v1;
	Vector3 vec23 = v3-v2;
	Vector3 vec13 = v3-v1;
	if(vec12*vec23>0)	return vec12.length();
	if(vec13*vec23<0)	return vec13.length();
	return sqrt(vec13*vec13-pow(vec13*vec23,2.0)/(vec23*vec23));
}

//=====================================Get Measures==================================//
void MingCurve::splitEdge(int p1, int p2ind, const Point3 & newP){
	//add new point
	int p2 = tempAdj[p1][p2ind];
	tempC.push_back(newP); org_n++;
	newAdj[0] = p1; newAdj[1] = p2;
	tempAdj.push_back(newAdj);
	if(withNorm){
		//and its adj norms
		int newNind = tempAdjNorm[p1][p2ind];
		Vector3 newN = tempNorm[newNind];
		tempNorm.push_back(newN);
		newNorm[0] = newNind; newNorm[1] = newNind;
		tempAdjNorm.push_back(newNorm);
	}
	tempAdj[p1][p2ind] = org_n-1;
	int pos = 0;
	if(tempAdj[p2][0]!=p1) pos = 1;
	tempAdj[p2][pos] = org_n-1;
}

void MingCurve::splitEdge(int p1, int p2ind, const Point3 &newP, const Point3 &newOrgP){
	//add new point
	int p2 = tempAdj[p1][p2ind];
	tempC.push_back(newP); org_n++;
	tempOrgC.push_back(newOrgP);
	newAdj[0] = p1; newAdj[1] = p2;
	tempAdj.push_back(newAdj);
	if(withNorm){
		//and its adj norms
		int newNind = tempAdjNorm[p1][p2ind];
		Vector3 newN = tempNorm[newNind];
		tempNorm.push_back(newN);
		newNorm[0] = newNind; newNorm[1] = newNind;
		tempAdjNorm.push_back(newNorm);
	}
	tempAdj[p1][p2ind] = org_n-1;
	int pos = 0;
	if(tempAdj[p2][0]!=p1) pos = 1;
	tempAdj[p2][pos] = org_n-1;
}
//Protect acute corners
void MingCurve::protectCorner(){
	radius.clear();
	cliped.clear();
	vector<int> acuteList;
	
	int sizeCliped = org_n;
	for(int i=0; i<org_n; i++){
		newClip[0] = 0; newClip[1] = 0;
		cliped.push_back(newClip);
		radius.push_back(FLT_MAX);
	}
	if (isDeGen){
		orgradius.clear();
		for(int i=0; i<org_n; i++){
			orgradius.push_back(FLT_MAX);
		}
	}
	//gather acute corners
	for (int i=0; i<org_n; i++){
		if(getAngle(i,tempAdj[i][0],tempAdj[i][1])<hfPI){
			acuteList.push_back(i);
			cliped[i][0] = 1; cliped[i][1] = 1;
		}
	}
	//computer distance to other edges (circular protection)
	int acsize = acuteList.size();
	int p1,p2,p3; int end=org_n-1; double dist;
	for (int i=0; i<acsize; i++){
		p1 = acuteList[i];
		for (int j=0; j<org_n; j++){
			p2 = j; p3 = j+1;
			if(j==end){ p2=0; p3=end; }
			if(p1==p2||p1==p3)	continue;
			dist = getPt2LineDist(p1,p2,p3);
			radius[p1] = std::min(dist,radius[p1]);
		}
	}
	//computer cut positons std::min(dist,S,L/3,2D/3)
	for (int i=0; i<acsize; i++){
		double M = FLT_MAX;
		double L = FLT_MAX;
		double D = FLT_MAX;
		double S = FLT_MAX;

		double orgM = FLT_MAX;
		double orgL = FLT_MAX;
		double orgD = FLT_MAX;
		double orgS = FLT_MAX;
		int pos;
		p1 = acuteList[i];
		for (int j=0; j<2; j++){
			p2 = tempAdj[p1][j];

			M = (tempC[p1]-tempC[p2]).length();
			S = std::min(S,M);
			if(cliped[p1][j])	L=std::min(L,M);
			//int pos = 0;
			if(tempAdj[p2][0]!=p1) pos = 1;
			if(cliped[p2][pos])	D = std::min(D,M);

			if(isDeGen){
				orgM = (tempOrgC[p1]-tempOrgC[p2]).length();
				orgS = std::min(orgS,orgM);
				if(cliped[p1][j])	orgL=std::min(orgL,orgM);
				if(cliped[p2][pos])	orgD = std::min(orgD,orgM);
			}
		}
		//TODO: consider reduce it a little if it makes program more robust
		radius[p1] = std::min(std::min(radius[p1],S),std::min(L/3.0,D*2.0/3.0));
		if (isDeGen){
			orgradius[p1] = std::min(std::min(orgradius[p1],orgS),std::min(orgL/3.0,orgD*2.0/3.0));
		}
	}
	//split cliped edges
	for (int i=0; i<acsize; i++){
		p1 = acuteList[i];
		for (int j=0; j<2; j++){
			p2 = tempAdj[p1][j];

			Point3 newP(tempC[p1]);
			Vector3 dir(tempC[p2]-tempC[p1]);
			dir.normalize();
			newP += dir*(radius[p1]*0.9);

			if (isDeGen){
				Point3 newOrgP(tempOrgC[p1]);
				Vector3 Orgdir(tempOrgC[p2]-tempOrgC[p1]);
				Orgdir.normalize();
				newOrgP += Orgdir*(orgradius[p1]*0.9);
				splitEdge(p1,j,newP,newOrgP);
			} else {
				splitEdge(p1,j,newP);
			}
		}
	}
}
bool MingCurve::passTetGen(){
	tetgenio tetIn;
	tetgenio tetOut;
	tetIn.numberofpoints = org_n;
	double * org_pts = new double[org_n*3];
	Point3 pt;
	for (int i=0; i<org_n; i++){
		pt = tempC[i];
		org_pts[i*3+0] = pt[0];
		org_pts[i*3+1] = pt[1];
		org_pts[i*3+2] = pt[2];
	}
	tetIn.pointlist = org_pts;

	//call tetgen
	try{
		tetrahedralize("f", &tetIn, &tetOut);
	} catch (int e){
		cout<<"MWT: tetgen problem "<<e<<endl;
		return false;
	}

	int tmpTn = tetOut.numberoftrifaces;
	if (tmpTn == 0) 
		return false;
	return true;
}
//Check whether the curve is alread edge-protected
bool MingCurve::isProtected(){
	badEdge.clear();
	char ** used = new char*[org_n];
	for(int i=0; i<org_n; i++){
		used[i] = new char[2];
		used[i][0] = 0; used[i][1] = 0;
	}
	//set tetgenio point set input
	tetgenio tetIn;
	tetgenio tetOut;
	tetIn.numberofpoints = org_n;
	double * org_pts = new double[org_n*3];
	Point3 pt;
	for (int i=0; i<org_n; i++){
		pt = tempC[i];
		org_pts[i*3+0] = pt[0];
		org_pts[i*3+1] = pt[1];
		org_pts[i*3+2] = pt[2];
	}
	tetIn.pointlist = org_pts;

	//call tetgen
	try{
		tetrahedralize("f", &tetIn, &tetOut);
	} catch (int e){
		cout<<"MWT: tetgen problem "<<e<<endl;
		badInput = true;
		delete [] used;
		return true;
	}
	int tmpTn = tetOut.numberoftrifaces;
	int * tmpTris = tetOut.trifacelist;
	int tri[3]; int p1, p2; int pos;
	for (int i=0; i<tmpTn; i++){
		tri[0] = tmpTris[i*3+0];
		tri[1] = tmpTris[i*3+1];
		tri[2] = tmpTris[i*3+2];
		for (int x=0; x<=1; x++){
			for (int y=x+1; y<=2; y++){
				p1 = tri[x]; p2 = tri[y];
				pos = -1;
				if(tempAdj[p1][0]==p2) pos = 0;
				if(tempAdj[p1][1]==p2) pos = 1;
				if(pos!=-1) used[p1][pos] = 1;
				p1 = tri[y]; p2 = tri[x];
				pos = -1;
				if(tempAdj[p1][0]==p2) pos = 0;
				if(tempAdj[p1][1]==p2) pos = 1;
				if(pos!=-1) used[p1][pos] = 1;
			}
		}
	}
	//save badedges for latter splitting
	for(int i=0; i<org_n; i++){
		for (int j=0; j<2; j++) {
			if((!used[i][j])&&i<tempAdj[i][j]){
				//int * newEdge = new int[2]; 
				newEdge[0] = i; newEdge[1] = j;
				badEdge.push_back(newEdge);
			}
		}
	}
	int besize = badEdge.size();
	if(besize>BADEDGE_LIMIT){
		//errors(ERROR_TETGEN,filename);
		badInput = true;
		delete [] used;
		return true;
	}
	delete [] used;
	if(besize>0) return false;
	//save points and tris

	return true;
}
//Insert mid points on edges that are not edge-protected
void MingCurve::insertMidPointsTetgen(){
	int besize, p, q, j; Point3 p1, p2, newP, orgp1,orgp2, newOrgP;
	while(!isProtected()){
		if (badInput) break;
		besize = badEdge.size();
		for (int i=0; i<besize; i++){
			p = badEdge[i][0]; j = badEdge[i][1]; 
			p1 = tempC[p]; q = tempAdj[p][j];
			p2 = tempC[q]; Vector3 vec = p2-Point3();
			newP = p1 + vec; newP*=0.5;

			if (isDeGen){
				orgp1 = tempOrgC[p]; 
				orgp2 = tempOrgC[q]; 
				Vector3 orgvec = orgp2-Point3();
				newOrgP = orgp1 + orgvec; newOrgP*=0.5;
				splitEdge(p,j,newP,newOrgP);
			} else {
				splitEdge(p,j,newP);
			}
		}
	}
}
//For getCurveAfterEP(), to check whether the orientation changes
//if so, normals should be fliped
bool MingCurve::sameOrientation(const vector<int> & newCurve){
	int ncsize = newCurve.size();
	int pos1 = -1,pos2 = -1,pos=0;
	while(pos1==-1||pos2==-1){
		if(newCurve[pos]==1) pos1 = pos;
		if(newCurve[pos]==2) pos2 = pos;
		pos++;
	}
	return pos2>pos1;
}
//Recover ordered curve points
bool MingCurve::getCurveAfterEP(){
	int start = 0, cur, pre, next;
	vector<int> newCurve;
	newCurve.push_back(0);
	pre = 0; cur = tempAdj[0][0];
	while(cur!=start){
		newCurve.push_back(cur);
		next = tempAdj[cur][0]+tempAdj[cur][1]-pre;
		pre = cur; cur = next;
	}
	numofpoints = newCurve.size();
	n_after = numofpoints;
	n_ratio = (float)n_after/n_before;
	points = new double[3 * numofpoints];
	Point3 pt;
	for (int i=0; i<numofpoints; i++) {
		pt = tempC[newCurve[i]];
		points[i*3+0] = pt[0];
		points[i*3+1] = pt[1];
		points[i*3+2] = pt[2];
	}
	if (isDeGen){
		DeGenPoints = new double[3 * numofpoints];
		for (int i=0; i<numofpoints; i++) {
			pt = tempOrgC[newCurve[i]];
			DeGenPoints[i*3+0] = pt[0];
			DeGenPoints[i*3+1] = pt[1];
			DeGenPoints[i*3+2] = pt[2];
		}
	}
	if(withNorm){
		normals = new float[3 * numofpoints];
		Vector3 norm;
		bool sameOri = sameOrientation(newCurve);
		if(sameOri){
			for (int i=0; i<numofpoints; i++) {
				norm = tempNorm[newCurve[i]];
				normals[i*3+0] = (float)norm[0];
				normals[i*3+1] = (float)norm[1];
				normals[i*3+2] = (float)norm[2];
			}
		}else{
			for (int i=0; i<numofpoints; i++) {
				norm = tempNorm[newCurve[i]];
				normals[i*3+0] = (float)-norm[0];
				normals[i*3+1] = (float)-norm[1];
				normals[i*3+2] = (float)-norm[2];
			}
		}
	}
	return true;
}
bool MingCurve::isDeGenCase(){
	if (org_n<=3) {
		isDeGen = true;
		return true;
	}

	Point3 startPt = tempC[0];
	Vector3 startNorm = tempC[1]-tempC[0];
	Vector3 stdNorm = (tempC[1]-tempC[0])^(tempC[2]-tempC[0]);
	stdNorm.normalize();

	Vector3 curNorm;
	Point3 curPt;
	bool breakout = false;
	for (int i=3; i<org_n-1; i++){
		curNorm = (tempC[i]-startPt)^startNorm;
		curNorm.normalize();
		if (curNorm*stdNorm-1 < plainEPS){
			breakout = true;
			break;
		}
	}
	if (breakout){
		isDeGen = false;
		return false;
	}
	isDeGen = true;
	return true;
}
// give some perturbation for tempC
void MingCurve::perturbPts(double ptb){
	for (int i=0; i<org_n; i++){
		tempC[i].pertube(ptb*(i%5));
	}
}
//main procedure
//If the curve is not edge-protected, protect it by 2 steps:
//1.protectCorner; 2. insert mid points.
bool MingCurve::edgeProtect(bool isdmwt){
	while (!passTetGen()){
		isDeGen=true;
		perturbPts(plainPTB);
		perturbNum++;
		if (perturbNum>500){
			badInput = true;
			cout<<"MWT: plain input, >500 perturbation"<<endl;
			break;
		}
	}
	if(!isProtected()){
		if (!badInput){
			protectCorner();
			insertMidPointsTetgen();
		}
	}

	if (badInput){
		return false;
	}
	getCurveAfterEP();

	#if SAVE_NEWCURVE
	saveCurve(filename,points,numofpoints);
	#endif

	return true;
}

void MingCurve::statistics(){
	if(!DO_EXP){
		cout<<"================================="<<endl;
		cout<<" File: \t\t"<<filename<<endl;
		cout<<" N_ratio:\t"<<n_ratio<<endl;
		cout<<""<<endl;
		cout<<" (T) Read files:\t"<<timeReadIn<<endl;
		cout<<" (T) Edge protect:\t"<<timeEdgeProtect<<endl;
	}else{
		cout<<filename<<"\t"<<n_ratio<<"\t"
			<<timeReadIn<<"\t"<<timeEdgeProtect<<"\t";
	}
}
