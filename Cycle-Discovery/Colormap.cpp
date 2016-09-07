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


#include "colormap.h"



void Colormap::colormapCustomize(color &c1,color &c2,int &n,std::vector<Colormap::color> &colors)
{
	colors.push_back(c1);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = c1.r*(1-float(i)/float(n)) + c2.r*float(i)/float(n);
		c.g = c1.g*(1-float(i)/float(n)) + c2.g*float(i)/float(n);
		c.b = c1.b*(1-float(i)/float(n)) + c2.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(c2);
}
void Colormap::colormapBSC(unsigned int n, std::vector<Colormap::color> &colors)
{
	unsigned binSize = (unsigned)(ceil((float)(n/6.0f)));
	Colormap::color c;
	c.r = 0;c.g = 0;c.b = 1;
	colors.push_back(c);
	c.r = 0;c.g = 1;c.b = 0;
	colors.push_back(c);
	c.r = 1;c.g = 0;c.b = 0;
	colors.push_back(c);
	c.r = 0;c.g = 1;c.b = 1;
	colors.push_back(c);
	c.r = 1;c.g = 0;c.b = 1;
	colors.push_back(c);
	c.r = 1;c.g = 1;c.b = 0;
	colors.push_back(c);

	for (unsigned int i=1; i< binSize; ++i){
		for (unsigned int j=0; j< 6; ++j){
			c.r = colors[j].r * double(i)/double(binSize) ;
			c.g = colors[j].g * double(i)/double(binSize) ;
			c.b = colors[j].b * double(i)/double(binSize) ;
			colors.push_back(c);
		}
	}
}
void Colormap::colormapHSV(unsigned int n, std::vector<Colormap::color> &colors)
{

		unsigned binSize = (unsigned)(ceil((float)(n/6.0f)));

		// from red to yellow
		// increase green intensity
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = 1.0f;			
			c.g = ((float)i/(float)binSize);
			c.b = 0.0f;
			colors.push_back(c);
		}
		
		// from yellow to green
		// decrease red intensity
		for (unsigned int i=binSize; i >0 ; --i){
			Colormap::color c;
			c.r = ((float)i/(float)binSize);
			c.g = 1.0f;
			c.b = 0.0f;
			colors.push_back(c);
		}

		// from green to cyan
		// increase blue intensity
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = 1.0f;
			c.g = 0.0f;
			c.b =((float)i/(float)binSize);
			//color c(0.0f,1.0f, (i/binSize));
			colors.push_back(c);
		}

		// from cyan to blue
		// decrease green intensity
		for (unsigned int i=binSize; i >0 ; --i){
			Colormap::color c;
			c.r = 0.0f;
			c.g = ((float)i/(float)binSize);
			c.b = 1.0f;
			//color c(0.0f, (i/binSize), 1.0f);
			colors.push_back(c);
		}

		// from blue to magenta
		// increase red intensity
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = ((float)i/(float)binSize);
			c.g = 0.0f;
			c.b = 1.0f;
			//color c((i/binSize), 0.0f, 1.0f);
			colors.push_back(c);
		}

		// from magenta to red
		// decrease blue intensity
		for (unsigned int i=binSize; i > 0 ; --i){
			Colormap::color c;
			c.r = 1.0f;
			c.g = 0.0f;
			c.b = ((float)i/(float)binSize);
			//color c(1.0f, 0.0, (i/binSize));
			colors.push_back(c);
		}
}


void Colormap::colormapHeatColorReverse(unsigned int n, std::vector<color> &colors)
{
		unsigned binSize = (unsigned)(ceil((float)(n/4.0f)));

		// from Blue to Cyan		
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = 0.0f;			
			c.g = ((float)i/(float)binSize);
			c.b = 1.0f;
			colors.push_back(c);
		}
		
		// from Cyan to Green
		for (unsigned int i=binSize; i >0 ; --i){
			Colormap::color c;
			c.r = 0.0f;
			c.g = 1.0f;
			c.b = ((float)i/(float)binSize);
			colors.push_back(c);
		}

		// from Green to Yellow
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = ((float)i/(float)binSize);
			c.g = 1.0f;
			c.b = 0.0f;
			colors.push_back(c);
		}

		// from Yellow to Red
		// decrease green intensity
		for (unsigned int i=binSize; i >0 ; --i){
			Colormap::color c;
			c.r = 1.0f;
			c.g = ((float)i/(float)binSize);
			c.b = 0.0f;
			colors.push_back(c);
		}
}
void Colormap::colormapHeatColor(unsigned int n, std::vector<color> &colors)
{
		unsigned binSize = (unsigned)(ceil((float)(n/4.0f)));

		// from R to Y		
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = 1.0f;			
			c.g = ((float)i/(float)binSize);
			c.b = 0.0f;
			colors.push_back(c);
		}
		
		// from Y to Green
		for (unsigned int i=binSize; i > 0 ; --i){
			Colormap::color c;
			c.r = ((float)i/(float)binSize);
			c.g = 1.0f;
			c.b = 0.0f;
			colors.push_back(c);
		}

		// from Green to C
		for (unsigned int i=0; i< binSize; ++i){
			Colormap::color c;
			c.r = 0.0f;
			c.g = 1.0f;
			c.b = ((float)i/(float)binSize);
			colors.push_back(c);
		}

		// from C to B
		for (unsigned int i=binSize; i >0 ; --i){
			Colormap::color c;
			c.r = 0.0f;
			c.g = ((float)i/(float)binSize);
			c.b = 1.0f;
			colors.push_back(c);
		}
}
void Colormap::colormapWinter(unsigned int n, std::vector<Colormap::color> &colors){

	// from Green to Blue : 2 bins

	unsigned binSize = (unsigned)(ceil((float)(n/2.0f)));

	// from Green to Cyan
	// increase green intensity
	for (unsigned int i=0; i< binSize; ++i){
		Colormap::color c;
		c.r = 0.0f;
		c.g = 1.0f;
		c.b = ((float)i/(float)binSize);
		colors.push_back(c);
	}

	// from Cyan to Blue
	// decrease green intensity
	for (unsigned int i = binSize; i > 0 ; --i){
		Colormap::color c;
		c.r = 0.0;
		c.g = ((float)i/(float)binSize);
		c.b = 1.0f;
		colors.push_back(c);
	}
}
void Colormap::colormapLightGrayToRed(unsigned int n, std::vector<Colormap::color> &colors)
{
	Colormap::color gray_;
	gray_.r = .9;gray_.g = .9;gray_.b = .9;
	Colormap::color red_;
	red_.r = 1;red_.g = 0;red_.b = 0;

	colors.push_back(red_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = red_.r*(1-float(i)/float(n)) + gray_.r*float(i)/float(n);
		c.g = red_.g*(1-float(i)/float(n)) + gray_.g*float(i)/float(n);
		c.b = red_.b*(1-float(i)/float(n)) + gray_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(gray_);
}
void Colormap::colormapRedToGray(unsigned int n, std::vector<Colormap::color> &colors)
{
	Colormap::color red_;
	red_.r = 1;red_.g = 0;red_.b = 0;
	Colormap::color gray_;
	gray_.r = .5;gray_.g = .5;gray_.b = .5;

	colors.push_back(red_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = red_.r*(1-float(i)/float(n)) + gray_.r*float(i)/float(n);
		c.g = red_.g*(1-float(i)/float(n)) + gray_.g*float(i)/float(n);
		c.b = red_.b*(1-float(i)/float(n)) + gray_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(gray_);
}
void Colormap::colormapRedToBlue(unsigned int n, std::vector<Colormap::color> &colors)
{
	unsigned binSize = (unsigned)(ceil((float)(n/6.0f)));
	Colormap::color red_;
	red_.r = 1;red_.g = 0;red_.b = 0;
	Colormap::color blue_;
	blue_.r = 0;blue_.g = 0;blue_.b = 1;

	colors.push_back(red_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = red_.r*(1-float(i)/float(n)) + blue_.r*float(i)/float(n);
		c.g = red_.g*(1-float(i)/float(n)) + blue_.g*float(i)/float(n);
		c.b = red_.b*(1-float(i)/float(n)) + blue_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(blue_);
}
void Colormap::colormapRedToYellow(unsigned int n, std::vector<Colormap::color> &colors)
{
	Colormap::color red_;
	red_.r = 1;red_.g = 0;red_.b = 0;
	Colormap::color yellow_;
	yellow_.r = 1;yellow_.g = 1;yellow_.b =0;

	colors.push_back(red_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = red_.r*(1-float(i)/float(n)) + yellow_.r*float(i)/float(n);
		c.g = red_.g*(1-float(i)/float(n)) + yellow_.g*float(i)/float(n);
		c.b = red_.b*(1-float(i)/float(n)) + yellow_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(yellow_);
}
void Colormap::colormapRedToBlack(unsigned int n, std::vector<Colormap::color> &colors){

	Colormap::color red_;
	red_.r = 1;red_.g = 0;red_.b = 0;
	Colormap::color black_;
	black_.r = 1;black_.g = 1;black_.b =0;

	colors.push_back(red_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = red_.r*(1-float(i)/float(n)) + black_.r*float(i)/float(n);
		c.g = red_.g*(1-float(i)/float(n)) + black_.g*float(i)/float(n);
		c.b = red_.b*(1-float(i)/float(n)) + black_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(black_);
}

void Colormap::colormapGreenToYellow(unsigned int n, std::vector<Colormap::color> &colors)
{
	Colormap::color green_;
	green_.r = 176./255.;green_.g = 219./255.;green_.b = 140./255.;
	Colormap::color yellow_;
	yellow_.r = 204./255.;yellow_.g = 228./255.;yellow_.b =103./255.;

	colors.push_back(green_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = green_.r*(1-float(i)/float(n)) + yellow_.r*float(i)/float(n);
		c.g = green_.g*(1-float(i)/float(n)) + yellow_.g*float(i)/float(n);
		c.b = green_.b*(1-float(i)/float(n)) + yellow_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(yellow_);
}


void Colormap::colormapPurpleToBlue(unsigned int n, std::vector<Colormap::color> &colors)
{
	Colormap::color purple_;
	purple_.r = 232./255.;purple_.g = 175./255.;purple_.b = 211./255.;
	Colormap::color blue_;
	blue_.r = 176./255.;blue_.g = 198./255.;blue_.b =228./255.;

	colors.push_back(purple_);
	for (unsigned int i = 1; i<n-1 ; i++){
		Colormap::color c;
		c.r = purple_.r*(1-float(i)/float(n)) + blue_.r*float(i)/float(n);
		c.g = purple_.g*(1-float(i)/float(n)) + blue_.g*float(i)/float(n);
		c.b = purple_.b*(1-float(i)/float(n)) + blue_.b*float(i)/float(n);
		colors.push_back(c);
	}
	colors.push_back(blue_);
}


void Colormap::colormapRed(unsigned int n, std::vector<Colormap::color> &colors){

	unsigned binSize = (unsigned)(ceil((float)(n/9.0f)));
	std::vector<Colormap::color> reds(9);
	reds[0].r = 51./255.;		reds[0].g = 0;		reds[0].b = 0;
	reds[1].r = 102./255.;	reds[1].g = 0;		reds[1].b = 0;
	reds[2].r = 153./255.;	reds[2].g = 0;		reds[2].b = 0;
	reds[3].r = 204./255.;	reds[3].g = 0;		reds[3].b = 0;
	reds[4].r = 255./255.;	reds[4].g = 0;		reds[4].b = 0;
	reds[5].r = 255./255.;	reds[5].g = 51./255.;	reds[5].b = 51./255.;
	reds[6].r = 255./255.;	reds[6].g = 102./255.;reds[6].b = 102./255.;
	reds[7].r = 255./255.;	reds[7].g = 153./255.;reds[7].b = 153./255.;
	reds[8].r = 255./255.;	reds[8].g = 204./255.;reds[8].b = 204./255.;


	colors = reds;
/*
	for (unsigned int i = 1; i<n-1 ; i++){
		for(int j=0;j<9;j++){
			Colormap::color c;
			c.r = red_.r*(1-float(i)/float(n)) + black_.r*float(i)/float(n);
			c.g = red_.g*(1-float(i)/float(n)) + black_.g*float(i)/float(n);
			c.b = red_.b*(1-float(i)/float(n)) + black_.b*float(i)/float(n);
			colors.push_back(c);
		}
	}
*/
}
void Colormap::colormapBlue(unsigned int n, std::vector<Colormap::color> &colors){

	unsigned binSize = (unsigned)(ceil((float)(n/9.0f)));
	std::vector<Colormap::color> blues(9);
	blues[0].r = 0;	blues[0].g = 0;		blues[0].b = 51./255.;
	blues[1].r = 0;	blues[1].g = 0;		blues[1].b = 102./255.;
	blues[2].r = 0;	blues[2].g = 0;		blues[2].b = 153./255.;
	blues[3].r = 0;	blues[3].g = 0;		blues[3].b = 204./255.;
	blues[4].r = 0;	blues[4].g = 0;		blues[4].b = 255./255.;
	blues[5].r = 51./255.;	blues[5].g = 51./255.;blues[5].b = 255/255;
	blues[6].r = 102./255.;	blues[6].g = 102./255.;blues[6].b = 255/255;
	blues[7].r = 153./255.;	blues[7].g = 153./255.;blues[7].b = 255/255;
	blues[8].r = 204./255.;	blues[8].g = 204./255.;blues[8].b = 255/255;


	colors = blues;
/*
	for (unsigned int i = 1; i<n-1 ; i++){
		for(int j=0;j<9;j++){
			Colormap::color c;
			c.r = red_.r*(1-float(i)/float(n)) + black_.r*float(i)/float(n);
			c.g = red_.g*(1-float(i)/float(n)) + black_.g*float(i)/float(n);
			c.b = red_.b*(1-float(i)/float(n)) + black_.b*float(i)/float(n);
			colors.push_back(c);
		}
	}
*/
}

void Colormap::colormapDistinct1(unsigned int n, std::vector<Colormap::color> &colors){

	colors.resize(8);
	colors[0].r = 1;	colors[0].g = 0;		colors[0].b = 0;
	colors[1].r = 255./255.;	colors[1].g = 153./255.;		colors[1].b = 153./255.;
	colors[2].r = 255./255.;	colors[2].g = 255./255.;		colors[2].b = 102./255.;
	colors[3].r = 0;	colors[3].g = 1;		colors[3].b = 0;
	colors[4].r = 153./255.;	colors[4].g = 255./255.;		colors[4].b = 153./255.;
	colors[5].r = 102./255.;	colors[5].g = 255./255.;colors[5].b = 255./255.;
	colors[6].r = 0;	colors[6].g = 0;colors[6].b = 1;
	colors[7].r = 153./255.;	colors[7].g = 153./255.;colors[7].b = 255./255.;
}


