#ifndef _DRT_H
#define _DRT_H

// For CycleDiscovery Project
// use Sum of Area Metric Only: weight = {1,0,0,0}, use interface 1
// use Sum of Edge Metric Only: weight = {0,1,0,0}, use interface 1
// use Sum of Dihedral Metric Only: weight = {0,0,1,0}, use interface 1
// use Dihedral & Board Normal Metric together: weight = {0,0,1,1} (two of 1's), use interface 2

// return type:
// 0: error, could be (a) bad input (b) no solution from MWT (c) unknown error
// 1: correct results
// 2: degenerated case, input is a plain, no result returned

#include "DMWT.h"
#include "MingCurve.h"
#include <stdio.h>

using namespace std;

//interface 1
int delaunayRestrictedTriangulation(const double* inCurve,const int inNum,double** outCurve,int *outPn, double** outFaces,int *outNum,float* weights, bool dosmooth, int subd, int laps){

	bool withNorm = false;

	bool isdmwt=true, ismwt=false, isliepa=false, isdot1=false;
	float weightTri=weights[0];
	float weightEdge=weights[1];
	float weightBiTri=weights[2];
	float weightTriBd=weights[3];
	float weightWorst=weights[4];
	int   limit = 1000000;

	try{
		MingCurve * myCurve = new MingCurve(inCurve, inNum,limit,withNorm);
		if(!myCurve->edgeProtect(isdmwt)){			
			delete myCurve;
			cout<<"MWT: (0) bad input, not able to protect curve"<<endl;
			return 0; // (0) bad input, not able to protect curve
		}
		//myCurve->statistics();
		int ptn = myCurve->getNumOfPoints();
		double * pts = myCurve->getPoints();
		double * deGenPts = myCurve->getDeGenPoints();

		float * newNms;
		
		if(isdmwt){
			DMWT * myDMWT = new DMWT(ptn,pts,deGenPts,myCurve->isDeGen);
			myDMWT->setWeights(weightTri,weightEdge,weightBiTri,weightTriBd,weightWorst);
			myDMWT->setDot(isdot1);
			myDMWT->preprocess();
			if (!myDMWT->start()){
				delete myDMWT;
				delete myCurve;
				cout<<"MWT: (0) no solution case"<<endl;
				return 0; // (0) no solution case
			}
			myDMWT->getResult(outFaces,outNum,outCurve,&newNms,outPn,dosmooth, subd, laps);
			delete myDMWT;
		}

		delete myCurve;
	}catch(int e){
		cout<<"MWT: Unknown Error!! Exception Nr. "<<e<<endl;
		return 0; // (0) unknown error
	}
	return 1; // (1) correct results
}

//interface 2
int delaunayRestrictedTriangulation(const double* inCurve,const float* inNorm,const int inNum,double** outCurve,float** outNorm,int *outPn,double** outFaces,int *outNum,float* weights, bool dosmooth, int subd, int laps){
	
	bool withNorm = true;

	bool isdmwt=true, ismwt=false, isliepa=false, isdot1=false;
	float weightTri=weights[0];
	float weightEdge=weights[1];
	float weightBiTri=weights[2];
	float weightTriBd=weights[3];
	float weightWorst=weights[4];
	int   limit = 1000000;

	try{
		MingCurve * myCurve = new MingCurve(inCurve, inNorm, inNum,limit,withNorm);
		if(!myCurve->edgeProtect(isdmwt)){
			delete myCurve;
			cout<<"MWT: (0) bad input, not able to protect curve"<<endl;
			return 0; // (0) bad input, not able to protect curve
		}
		//myCurve->statistics();
		int ptn = myCurve->getNumOfPoints();
		double * pts = myCurve->getPoints();
		double * deGenPts = myCurve->getDeGenPoints();
		float * norms = myCurve->getNormal();

		if(isdmwt){
			DMWT * myDMWT = new DMWT(ptn,pts,deGenPts,norms,myCurve->isDeGen);
			myDMWT->setWeights(weightTri,weightEdge,weightBiTri,weightTriBd,weightWorst);
			myDMWT->setDot(isdot1);
			myDMWT->preprocess();
			if (!myDMWT->start()){
				delete myDMWT;
				delete myCurve;
				cout<<"MWT: (0) no solution case"<<endl;
				return 0; // (0) no solution case
			}
			myDMWT->getResult(outFaces,outNum,outCurve,outNorm,outPn,dosmooth, subd, laps);
			delete myDMWT;
		}

		delete myCurve;
	}catch(int e){
		cout<<"MWT: Unknown Error!! Exception Nr. "<<e<<endl;
		return 0; // (0) unknown error
	}
	return 1; // (1) correct results
}


//interface 3
//No smoothing, just return the index of vertices as outFaces
int delaunayRestrictedTriangulation(const double* inCurve,const float* inNorm,const int inNum,double** outCurve,float** outNorm,int *outPn,int** outFaces,int *outNum,float* weights){
	
	bool withNorm = true;

	bool isdmwt=true, ismwt=false, isliepa=false, isdot1=false;
	float weightTri=weights[0];
	float weightEdge=weights[1];
	float weightBiTri=weights[2];
	float weightTriBd=weights[3];
	float weightWorst=weights[4];
	int   limit = 1000000;

	try{
		MingCurve * myCurve = new MingCurve(inCurve, inNorm, inNum,limit,withNorm);
		if(!myCurve->edgeProtect(isdmwt)){
			delete myCurve;
			cout<<"MWT: (0) bad input, not able to protect curve"<<endl;
			return 0; // (0) bad input, not able to protect curve
		}
		//myCurve->statistics();
		int ptn = myCurve->getNumOfPoints();
		double * pts = myCurve->getPoints();
		double * deGenPts = myCurve->getDeGenPoints();
		float * norms = myCurve->getNormal();

		if(isdmwt){
			DMWT * myDMWT = new DMWT(ptn,pts,deGenPts,norms,myCurve->isDeGen);
			myDMWT->setWeights(weightTri,weightEdge,weightBiTri,weightTriBd,weightWorst);
			myDMWT->setDot(isdot1);
			myDMWT->preprocess();
			if (!myDMWT->start()){
				delete myDMWT;
				delete myCurve;
				cout<<"MWT: (0) no solution case"<<endl;
				return 0; // (0) no solution case
			}
			myDMWT->getResult(outFaces,outNum,outCurve,outNorm,outPn);
			delete myDMWT;
		}

		delete myCurve;
	}catch(int e){
		cout<<"MWT: Unknown Error!! Exception Nr. "<<e<<endl;
		return 0; // (0) unknown error
	}
	return 1; // (1) correct results
}


#endif