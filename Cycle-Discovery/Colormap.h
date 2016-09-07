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

#ifndef __COLORMAP_H
#define __COLORMAP_H

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;


namespace Colormap{
	
	struct color{
		//color(float r,float g,float b){r = r;g=g;b=b;};
		double r;
		double g;
		double b;
	};

	void colormapCustomize(color &c1,color &c2,int &n,std::vector<Colormap::color> &colors);
	void colormapBSC(unsigned int n, std::vector<color> &colors);
	void colormapHSV(unsigned int n, std::vector<color> &colors);
	void colormapHeatColorReverse(unsigned int n, std::vector<color> &colors);
	void colormapHeatColor(unsigned int n, std::vector<color> &colors);
	void colormapWinter(unsigned int n, std::vector<color> &colors);
	void colormapLightGrayToRed(unsigned int n, std::vector<color> &colors);
	void colormapRedToGray(unsigned int n, std::vector<color> &colors);
	void colormapRedToBlue(unsigned int n, std::vector<color> &colors);
	void colormapRedToYellow(unsigned int n, std::vector<color> &colors);
	void colormapRedToBlack(unsigned int n, std::vector<color> &colors);
	void colormapPurpleToBlue(unsigned int n, std::vector<color> &colors);
	void colormapGreenToYellow(unsigned int n, std::vector<color> &colors);
	void colormapRed(unsigned int n, std::vector<color> &colors);
	void colormapBlue(unsigned int n, std::vector<color> &colors);
	void colormapDistinct1(unsigned int n, std::vector<color> &colors);

	
};

#endif