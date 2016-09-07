#ifndef GRAPH_ALGORITHM
#define GRAPH_ALGORITHM

#include <stdio.h>
#include <vector>
#include "amlVec.h"
#include "GraphStructure.h"

using namespace GraphStructure;

class GraphSearch{
public:
	GraphSearch();
	~GraphSearch();

private:
	typedef std::vector<int> Capacity;
	typedef std::vector<std::pair<double,std::pair<int,int> > > ArcWithWeight;
	typedef std::vector<std::vector<double> > PairArcsWithWeight;
	typedef std::vector<std::pair<int,int> > MultipleArcs;
	typedef std::vector<MultipleArcs> AllMultipleArcs;
	typedef struct{
		std::vector<std::vector<int> > usage;
		Capacity availableCapacity;
		double cost;
		double hcost;
	}StateEx2;
	typedef struct{
		MultipleArcs usedArcs;
		Capacity availableCapacity;
		int nextArcID;
		double weight;
	}State;
	typedef struct{
		MultipleArcs usedArcs;
		Capacity availableCapacity;
		int nextArcID;
		double totalWeight;
		std::vector<double> dihedralWeights;
		std::vector<std::vector<int> > adjacentArcs;
	}StateExtend;

public:

	double computeGeneralHamiltonGraph(const Capacity &org1,
			const ArcWithWeight &org2, const bool &org3, const bool &org4,
			AllMultipleArcs &tar, std::vector<double> &tar2);
	double computeGeneralHamiltonGraph(const Capacity &org1,
			const ArcWithWeight &org2, const PairArcsWithWeight & org3,
			const double org4, const bool &org5, const bool &org6,
			AllMultipleArcs &tar, std::vector<double> &tar2);

	void GraphSearch::computeGeneralHamiltonGraph(const Capacity &capacity,
			const std::vector<std::pair<int,int> > &allArcs,
			const std::vector<double> &weights,
			const int K,const bool isConnect,
			const std::vector<std::pair<int,int> > &predefinedArcs,
			AllMultipleArcs &tar,std::vector<double> &tar2);
	void stableMatching(const std::vector<std::vector<int> > &firstGroup,
			const std::vector<std::vector<int> > &secondGroup,
			const std::vector<std::pair<int,int> > &predefinedPairs,std::vector<int> &res);

};

#endif
