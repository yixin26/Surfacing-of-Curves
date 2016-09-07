/*************************************************************************
*
* ADOBE PROPRIETARY INFORMATION
*
* Use is governed by the license in the attached LICENSE.TXT file
*
*
*  Copyright 2010 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and may be covered by U.S. and Foreign Patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/
//#pragma once
#ifndef __FILE_IO_HELPER_H
#define __FILE_IO_HELPER_H

#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include "amlVec.h"
#include "GraphStructure.h"

using namespace std;
using namespace GraphStructure;

class FileIOHelper
{

public:
	//public:
	FileIOHelper();
	~FileIOHelper();

	bool ReadMayaCurve(
		const AML::Scalar point_closeness_thr,
		const char* fileName,
		std::vector< std::vector<AML::double3> >& curves,
		AML::double3 &boundingBoxMin,
		AML::double3 &boundingBoxMax);

	bool ReadCurve(const char* fileName,
		std::vector< std::vector<AML::double3> >& curves,
		std::vector< int> &capacity);

	bool ReadCrv(	
		const char* fileName,
		std::vector< std::vector<AML::double3> >& curves);

	bool ReadMesh_OFF(
        const char* fileName,
        std::vector<double> &points,
        std::vector<unsigned> &faces);

	bool ReadCycle(
		const char* fileName,
		CycleSet& cycle);

	bool FileIOHelper::ReadMesh(
		const char* fileName,
		TriangleSurface& mesh);


	bool WriteCurve(
		const char* fileName,
		const std::vector< std::vector<AML::double3> >& curves,
		const std::vector< int> &capacity = std::vector<int>(0));

	bool WriteCycle(	
		const char* fileName,
		const CycleSet& cycle);


	bool WriteMesh(
		const char* fileName,
		const TriangleSurface& mesh,
		const TriangleSurface& meshNormal);

	//temp;
	void ProcessContour(const char* fileName);

};

#endif
