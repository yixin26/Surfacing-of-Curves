#ifndef _TRIANGLE_INFO_H_
#define _TRIANGLE_INFO_H_
#include <float.h>

class TriangleInfo {
public:
	TriangleInfo();
	~TriangleInfo();

	int edgeIndex [3];		//index of {1st,2nd,3rd} edge in edgeInfoList;
	//char side [3];			//side of this triangle to {1st,2nd,3rd} edge, 1 or 2;
	int triIndex [3];		//index of this triangle in the $side list of edge {1,2,3}
	float optCost [3];	//optimal cost, init as BIGNUM
	int	optTile [3];	//optimal tiling, init as 0
	int getSize();
	int getOptSize();
private:
};

#endif