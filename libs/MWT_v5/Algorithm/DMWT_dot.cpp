#include "DMWT.h"

using namespace std;

void DMWT::setDot(bool isdot1){
	dot = isdot1? 1 : 2;
}

float DMWT::getSize(){
	if(dot==1){
		//DOT1 & DMWT: X-TIL+OPT=EIL,TIL_opt,Vlist,Tlist,Nlist,TiList
		int trivalLists = numoftris*3*sizeof(int)	//tris
			+numofpoints*3*sizeof(double)			//points
			+numofpoints*3*sizeof(float)			//normals
			+(numofpoints-2)*sizeof(int);			//tiling
		int EIL = 0;
		for(int i=0; i<numofedges; i++){			//edgeInfoList
			EIL += edgeInfoList[i]->getSize();
		}
		int OPT = 2*numofedges*triangleInfoList[0]->getOptSize()/3;	//OptSize
		return (float)(trivalLists + EIL + OPT)/1048576;

	}else{
		//DOT2 & DMWT: X=EIL,TIL,Vlist,Tlist,Nlist,TiList
		int trivalLists = numoftris*3*sizeof(int)	//tris
			+numofpoints*3*sizeof(double)			//points
			+numofpoints*3*sizeof(float)			//normals
			+(numofpoints-2)*sizeof(int);			//tiling
		int EIL = 0, TIL = 0;
		for(int i=0; i<numofedges; i++){			//edgeInfoList
			EIL += edgeInfoList[i]->getSize();
		}
		TIL = numoftris*triangleInfoList[0]->getSize();	//triangleInfoList
		return (float)(trivalLists + EIL + TIL)/1048576;
	}
}

void DMWT::statistics(){
	timeTotal = timePreprocess+timeMWT+timeTetgen;
	if(!DO_EXP){
		if(dot==1)	cout<<" [DOT1 DMWT]"<<endl;
		else		cout<<" [DOT2 DMWT]"<<endl;
		cout<<"---------------------------------"<<endl;
		cout<<" File: \t\t"<<filename<<endl;
		cout<<" Vertex:\t"<<numofpoints<<endl;
		cout<<" Edge:\t\t"<<numofedges<<endl;
		cout<<" Triangle:\t"<<numoftris<<endl;
		cout<<" Weights: \t"<<weightTri<<","<<weightEdge<<","<<weightBiTri<<","<<weightTriBd<<endl;
		cout<<""<<endl;
		//cout<<" Read files:\t"<<timeReadIn<<endl;
		cout<<" (T) Call TetGen:\t"<<timeTetgen<<endl;
		cout<<" (T) Preprocess:\t"<<timePreprocess<<endl;
		cout<<" (T) MWT & Tiling:\t"<<timeMWT<<endl;
		cout<<""<<endl;
		cout<<" Total time:\t"<<timeTotal<<"sec"<<endl;
		cout<<" Total space:\t"<<getSize()<<"MB"<<endl;
		cout<<" Optimal Cost:\t"<<optimalCost<<endl;
		cout<<endl;
		cout<<" Intersection:\t"<<intsTriInd[0]<<","<<intsTriInd[1]<<endl;
	}else{
		if(dot==1)	cout<<"DOT1+DMWT==\t";
		else		cout<<"DOT2+DMWT==\t";
		cout<<numofpoints<<"\t"<<numofedges<<"\t"<<numoftris<<"\t"
			<<"w"<<weightTri<<weightEdge<<weightBiTri<<weightTriBd<<"\t"
			<<timeTetgen<<"\t"<<timePreprocess<<"\t"<<timeMWT<<"\t"
			<<timeTotal<<"\t"<<optimalCost<<"\t"
			<<getSize()<<"\t"<<hasIntersect<<"\t"<<hasIntersect2<<"\t"
			<<intsTriInd[0]<<"\t"<<intsTriInd[1]<<"\t"<<filename<<endl;
	}
}

/*
This main function tiles a segment of the input curve on the side of the edge whose index is eind, 
with a "proceeding" triangle whose index in the edge's triangle list is ti. If ti is -1, 
that means eind is a boundary edge (i.e., at the beginning of the algorithm). 
The function returns the optimal tiling cost, and the index of the first triangle in the tiling 
(adjacent to eind) in the triangle list of eind (-1 if eind is on the boundary).
*/
bool DMWT::tileSegment(int eind, char side, int ti, float& thisCost, int& thisTile){
	float optCost = FLT_MAX, subCost=0.0, subCostSum;
	int tnum, tind, tindofti=-1, ei, ejind, ejtnum;
	int optTile = -1, subTile = -1;
	int ev1, ev2, tv3, v3;
	int *tlist; int *ejtlist; char *elist; char newside; char *ejelist;
	thisCost = optCost; thisTile = optTile;
	EdgeInfo * einfo; TriangleInfo * tinfo; EdgeInfo * ejinfo;

	bool isboard=false, hasSolution = true; float costtribd, costbitri;

	//for worstbitri
	float worstbitri = -1;//,bitriAngle;
	//int ejv1,ejv2,ejv3,ejtv3,ejtind;

	//ev1, ev2 are two end vertices of the edge
	einfo = edgeInfoList[eind];
	ev1=einfo->v1; ev2=einfo->v2;
	//get the list of abutting tris on the "side" of current edge
	if(side==RIGHT) {
		tlist = einfo->rightTris;
		tnum  = einfo->rightsize;
		elist = einfo->rightEdgeInd;
		if(ti>=0) {  //not the beginning of algorithm
			tindofti = einfo->leftTris[ti]; 
			tv3=tris[tindofti*3]+tris[tindofti*3+1]+tris[tindofti*3+2]-ev1-ev2;
		}
		isboard = einfo->v2-einfo->v1==1; //eg. (v1,v2)=(3,4), null in (3,4)
		if((tnum==0)&&!isboard) hasSolution=false; //no candidate triangle found
	} else {
		tlist = einfo->leftTris;
		tnum = einfo->leftsize;
		elist = einfo->leftEdgeInd;
		if(ti>=0) {
			tindofti = einfo->rightTris[ti];
			tv3=tris[tindofti*3]+tris[tindofti*3+1]+tris[tindofti*3+2]-ev1-ev2;
		}
		isboard = einfo->v2-einfo->v1==numofpoints-1;//eg. (v1,v2)=(n-1,0), null in (0,0)&(n-1,n-1)
		if((tnum==0)&&!isboard) hasSolution=false;
	}
	if(!hasSolution) return false;
	if(isboard){
		costtribd = 0.0f;
		if(tindofti!=-1){
			if(side==RIGHT) costtribd = costTriBd(measureTriBd(ev1,ev2,tv3,ev1));
			else			costtribd = costTriBd(measureTriBd(ev2,ev1,tv3,ev2));
		}else{
			//cout<<"Error: Left and Right are both empty for edge <v1,v2>=<"<<ev1<<","<<ev2<<">."<<endl;
		}
		if(useWorstDihedral){
			optCost = 0.0;
			worstbitri = 0.0;//worst angle = 0 for a board edge
		} else {
			optCost = costEdge(measureEdge(ev1,ev2)) + costtribd;
		}
	}else{
		for(int t=0; t<tnum; t++){
			tind = tlist[t]; ei = elist[t];
			tinfo = triangleInfoList[tind];
			v3 = tris[tind*3]+tris[tind*3+1]+tris[tind*3+2]-ev1-ev2;
			if (useWorstDihedral) {
				worstbitri = -1;
			} else {
				subCostSum = costEdge(measureEdge(ev1,ev2))+costTri(measureTri(ev1,ev2,v3));
			}
			if(ti==-1){
				if (useWorstDihedral) {
					worstbitri = 0.0;
				}else{
					if(weightTriBd!=0.0f){
						if(side==LEFT)  costtribd = costTriBd(measureTriBd(ev1,ev2,v3,ev1));
						else			costtribd = costTriBd(measureTriBd(ev2,ev1,v3,ev2));
						subCostSum+=costtribd;
					}
				}
			}else{
				//dot1 doesn't consider the bi-tri property 
				if(weightBiTri!=0.0f&&dot==2){
					costbitri = costBiTri(measureBiTri(ev1,ev2,v3,tv3));
					if (useWorstDihedral) {
						worstbitri = costbitri > worstbitri ? costbitri : worstbitri;
					} else {
						subCostSum+=costbitri;
					}
				}
			}
			for(int ej=0; ej<3; ej++){
				if(ej==ei) continue;
				//need to compute tiling
				if(tinfo->optCost[ej]==FLT_MIN){
					//vertices are stored in order, e.g. (0,3,7), and edge0=(0,3),edge1=(3,7),edge2=(0,7),
					//the third vertex is on the (0,0,1) side for each edge
					newside = ej==2? 1 : 0;
					tileSegment(tinfo->edgeIndex[ej],1-newside,tinfo->triIndex[ej],subCost,subTile);
					tinfo->optCost[ej] = subCost;
					tinfo->optTile[ej] = subTile;
					//push optCost&Tile to all triangles that use this edge if useBiTri is true
					if(dot==1){
						//if(!useBiTri){
						ejind = tinfo->edgeIndex[ej];
						ejinfo = edgeInfoList[ejind];
						if (newside==RIGHT){
							ejtnum = ejinfo->rightsize;
							ejtlist = ejinfo->rightTris;
							ejelist = ejinfo->rightEdgeInd;
						}else{
							ejtnum = ejinfo->leftsize;
							ejtlist = ejinfo->leftTris;
							ejelist = ejinfo->leftEdgeInd;
						}
						for(int i=0; i<ejtnum; i++){
							TriangleInfo * ejtinfo = triangleInfoList[ejtlist[i]];
							ejtinfo->optCost[ejelist[i]] = subCost;
							ejtinfo->optTile[ejelist[i]] = subTile;
						}
						//}
					}
				}//end for computing new subproblem
// if use worst-bitri, and algo is dot1, need to consider 2 more bi-tri angles
				if (useWorstDihedral) {
					worstbitri = tinfo->optCost[ej] > worstbitri ? tinfo->optCost[ej] : worstbitri;
				} else {
					subCostSum+=tinfo->optCost[ej];
				}
			}
			if (useWorstDihedral) {
				if(worstbitri < optCost){optCost = worstbitri; optTile=t;}
			} else {
				if(subCostSum < optCost){optCost = subCostSum; optTile=t;}	
			}
		}
	}
	thisCost = optCost;
	thisTile = optTile;
	return true;
}