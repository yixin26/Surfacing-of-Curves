#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <float.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "graphSearch.h"

GraphSearch::GraphSearch()
{
}
GraphSearch::~GraphSearch()
{
}

double GraphSearch::computeGeneralHamiltonGraph(const Capacity &org1,
												const ArcWithWeight &org2,
												const bool &org3,
												const bool &org4,
												AllMultipleArcs &tar,
												std::vector<double> &tar2)
{
	Capacity capacity=org1;
	ArcWithWeight arcsWithWeights=org2;
	bool connectRequire = org3;
	bool isFindAllSolutions = org4;
	MultipleArcs usedArcsID;
	AllMultipleArcs allUsedArcsID;
	std::vector<double> nodeWeights;
	usedArcsID.clear();

	State initState;
	std::vector<State > queueState;

	initState.usedArcs.clear();
	initState.availableCapacity = capacity;
	initState.nextArcID = 0;
	initState.weight = 0;

	queueState.push_back(initState);

	double minCost=FLT_MAX;
	while(true){
		double minWeight= FLT_MAX;
		int minID =-1;
		for(int i=0;i<queueState.size();i++){
			if(queueState[i].weight<minWeight){
				minID=i; minWeight=queueState[i].weight;
			}
		}
		if(minID==-1){
			usedArcsID.clear();
			break;}
		State currentState = queueState[minID];
		queueState.erase(queueState.begin()+minID);

		if(*(std::max_element(currentState.availableCapacity.begin(),
			currentState.availableCapacity.end()))==0){

				if(connectRequire){
					usedArcsID = currentState.usedArcs;
					std::vector<std::pair<int,int> > usedArcs;
					for(int i=0;i<usedArcsID.size();i++){
						int n1 = arcsWithWeights[usedArcsID[i].first].second.first;
						int n2 = arcsWithWeights[usedArcsID[i].first].second.second;
						usedArcs.push_back(std::pair<int,int>(n1,n2));
					}
					std::vector<int> usedNodes;
					usedNodes.push_back(usedArcs[0].first);
					usedNodes.push_back(usedArcs[0].second);
					usedArcs.erase(usedArcs.begin());
					while(!usedArcs.empty()){
						bool isChanged = false;
						int nodeSize=usedNodes.size();
						for(int j=0;j<nodeSize;j++){
							for(int k=0;k<usedArcs.size();k++){
								if(usedNodes[j]==usedArcs[k].first){
									if(find(usedNodes.begin(),usedNodes.end(),usedArcs[k].second)
										== usedNodes.end())
										usedNodes.push_back(usedArcs[k].second);
									usedArcs.erase(usedArcs.begin()+k);
									isChanged=true;
									break;
								}
								if(usedNodes[j]==usedArcs[k].second){
									if(find(usedNodes.begin(),usedNodes.end(),usedArcs[k].first)
										== usedNodes.end())
										usedNodes.push_back(usedArcs[k].first);
									usedArcs.erase(usedArcs.begin()+k);
									isChanged=true;
									break;
								}
							}
						}
						if(usedNodes.size()==capacity.size())
							break;
						if(!isChanged)break;
					}
					if(usedNodes.size()!=capacity.size())
						continue;
				}
				usedArcsID = currentState.usedArcs;
				allUsedArcsID.push_back(usedArcsID);
				nodeWeights.push_back(currentState.weight);
				if(isFindAllSolutions==false){
					minCost = currentState.weight;
					break;
				}
		}
		else{
			//expand current state;
			int nodeID[2];
			while(currentState.nextArcID<arcsWithWeights.size()){
				nodeID[0]=arcsWithWeights[currentState.nextArcID].second.first;
				nodeID[1]=arcsWithWeights[currentState.nextArcID].second.second;
				if(	currentState.availableCapacity[nodeID[0]]>=1
					&&currentState.availableCapacity[nodeID[1]]>=1)
					break;
				currentState.nextArcID++;
			}
			if(currentState.nextArcID<arcsWithWeights.size()){
				int degree = currentState.availableCapacity[nodeID[0]]<=
					currentState.availableCapacity[nodeID[1]]?
					currentState.availableCapacity[nodeID[0]]:
				currentState.availableCapacity[nodeID[1]];
				State saveState = currentState;
				int usedArcsID,isUsed=0;
				//currentState.weight += arcsWithWeights[currentState.nextArcID].first;
				currentState.nextArcID++;
				queueState.push_back(currentState);
				currentState=saveState;
				while(degree>0){
					currentState.availableCapacity[nodeID[0]]--;
					currentState.availableCapacity[nodeID[1]]--;
					//	int usedArcsID,isUsed=0;
					for(usedArcsID=0;usedArcsID<currentState.usedArcs.size();usedArcsID++){
						if(currentState.usedArcs[usedArcsID].first == (currentState.nextArcID)){
							isUsed=1;break;}
					}
					if(isUsed==1)
						currentState.usedArcs[usedArcsID].second++;
					else
						currentState.usedArcs.push_back(std::pair<int,int>(currentState.nextArcID,1));

					currentState.weight += arcsWithWeights[currentState.nextArcID].first;
					currentState.nextArcID++;
					queueState.push_back(currentState);
					currentState.nextArcID--;
					degree--;
				}
				currentState.nextArcID++;
			}
		}//end of else
	}//end of while

	tar = allUsedArcsID;
	tar2 = nodeWeights;
	return minCost;
}

double GraphSearch::computeGeneralHamiltonGraph(const Capacity &org1,
								 const ArcWithWeight &org2, const PairArcsWithWeight & org3,
								 const double org4, const bool &org5, const bool &org6,
								 AllMultipleArcs &tar, std::vector<double> &tar2)
{
	Capacity capacity=org1;
	ArcWithWeight arcsWithWeights=org2;
	PairArcsWithWeight pairArcsWithWeights= org3;
	double balance = org4;
	bool connectRequire = org5;
	bool isFindAllSolutions = org6;
	MultipleArcs usedArcsID;
	AllMultipleArcs allUsedArcsID;
	std::vector<double> nodeWeights;
	usedArcsID.clear();

	StateExtend initState;
	std::vector<StateExtend > queueState;

	initState.usedArcs.clear();
	initState.availableCapacity = capacity;
	initState.nextArcID = 0;
	initState.totalWeight = 0;
	initState.dihedralWeights.assign(capacity.size(),0);
	initState.adjacentArcs.resize(capacity.size());

	queueState.push_back(initState);

	double minCost=FLT_MAX;
	while(true){
		double minWeight= FLT_MAX;
		int minID =-1;
		for(int i=0;i<queueState.size();i++){
			if(queueState[i].totalWeight<minWeight){
				minID=i; minWeight=queueState[i].totalWeight;
			}
		}
		if(minID==-1){
			usedArcsID.clear();	break;}

		StateExtend currentState = queueState[minID];
		queueState.erase(queueState.begin()+minID);

		if(*(std::max_element(currentState.availableCapacity.begin(),
			currentState.availableCapacity.end()))==0){

				if(connectRequire){
					usedArcsID = currentState.usedArcs;
					std::vector<std::pair<int,int> > usedArcs;
					for(int i=0;i<usedArcsID.size();i++){
						int n1 = arcsWithWeights[usedArcsID[i].first].second.first;
						int n2 = arcsWithWeights[usedArcsID[i].first].second.second;
						usedArcs.push_back(std::pair<int,int>(n1,n2));
					}
					std::vector<int> usedNodes;
					usedNodes.push_back(usedArcs[0].first);
					usedNodes.push_back(usedArcs[0].second);
					usedArcs.erase(usedArcs.begin());
					while(!usedArcs.empty()){
						bool isChanged = false;
						int nodeSize=usedNodes.size();
						for(int j=0;j<nodeSize;j++){
							for(int k=0;k<usedArcs.size();k++){
								if(usedNodes[j]==usedArcs[k].first){
									if(find(usedNodes.begin(),usedNodes.end(),usedArcs[k].second)
										== usedNodes.end())
										usedNodes.push_back(usedArcs[k].second);
									usedArcs.erase(usedArcs.begin()+k);
									isChanged=true;
									break;
								}
								if(usedNodes[j]==usedArcs[k].second){
									if(find(usedNodes.begin(),usedNodes.end(),usedArcs[k].first)
										== usedNodes.end())
										usedNodes.push_back(usedArcs[k].first);
									usedArcs.erase(usedArcs.begin()+k);
									isChanged=true;
									break;
								}
							}
						}
						if(usedNodes.size()==capacity.size())
							break;
						if(!isChanged)break;
					}
					if(usedNodes.size()!=capacity.size())
						continue;
				}
				usedArcsID = currentState.usedArcs;
				allUsedArcsID.push_back(usedArcsID);
				nodeWeights.push_back(currentState.totalWeight);
				if(isFindAllSolutions==false){
					minCost = currentState.totalWeight;
					break;
				}
		}
		else{
			//expand current state;
			int nodeID[2];
			while(currentState.nextArcID<arcsWithWeights.size()){
				nodeID[0]=arcsWithWeights[currentState.nextArcID].second.first;
				nodeID[1]=arcsWithWeights[currentState.nextArcID].second.second;
				if(	currentState.availableCapacity[nodeID[0]]>=1
					&&currentState.availableCapacity[nodeID[1]]>=1)
					break;
				currentState.nextArcID++;
			}
			if(currentState.nextArcID<arcsWithWeights.size()){
				int degree = currentState.availableCapacity[nodeID[0]]<=
							currentState.availableCapacity[nodeID[1]]?
							currentState.availableCapacity[nodeID[0]]:
							currentState.availableCapacity[nodeID[1]];
				StateExtend saveState = currentState;
				currentState.nextArcID++;
				queueState.push_back(currentState);
				currentState=saveState;
				while(degree>0){
					currentState.availableCapacity[nodeID[0]]--;
					currentState.availableCapacity[nodeID[1]]--;
					//	int usedArcsID,isUsed=0;
					int usedArcsID,isUsed=0;
					for(usedArcsID=0;usedArcsID<currentState.usedArcs.size();usedArcsID++){
						if(currentState.usedArcs[usedArcsID].first == (currentState.nextArcID)){
							isUsed=1;break;}
					}
					if(isUsed==1)
						currentState.usedArcs[usedArcsID].second++;
					else
						currentState.usedArcs.push_back(std::pair<int,int>(currentState.nextArcID,1));

					currentState.totalWeight += arcsWithWeights[currentState.nextArcID].first;

					//update weight;
					for(int n=0;n<2;n++){
						double w=0;
						for(int i=0;i<currentState.adjacentArcs[nodeID[n]].size();i++){
							if(w<pairArcsWithWeights[currentState.nextArcID][currentState.adjacentArcs[nodeID[n]][i]])
								w=pairArcsWithWeights[currentState.nextArcID][currentState.adjacentArcs[nodeID[n]][i]];
						}
						if(w>currentState.dihedralWeights[nodeID[n]]){
							currentState.totalWeight+=balance*(w-currentState.dihedralWeights[nodeID[n]]);
						}
						else{
							currentState.dihedralWeights[nodeID[n]]=w;
						}
						currentState.adjacentArcs[nodeID[n]].push_back(currentState.nextArcID);
					}

					currentState.nextArcID++;
					queueState.push_back(currentState);
					currentState.nextArcID--;
					degree--;
				}
				currentState.nextArcID++;
			}
		}//end of else
	}//end of while

	tar = allUsedArcsID;
	tar2 = nodeWeights;
	return minCost;
}


std::vector<std::vector<int> >  enumSets(const std::vector<int> &capacity,
										 const int &k)
{
	std::vector<std::vector<int> > res;
	if(capacity.size()==1){
		res.push_back(std::vector<int>(1,k));
		return res;
	}
	else{
		std::vector<int> ncaps = capacity;
		ncaps.erase(ncaps.begin());
		if(capacity.front()==0){
			res = enumSets(ncaps,k);
			for(int i=0;i<res.size();i++)
				res[i].insert(res[i].begin(),0);
			return res;
		}
		else{
			int l=0;
			for(int i=0;i<ncaps.size();i++)
				l+=ncaps[i];
			l = max(0,k-l);
			int h = min(k,capacity.front());
			if(l>h){
				int t = l;
				l = h;
				h = t;
			}
			for(int i=l;i<=h;i++){
				std::vector<std::vector<int> > subres = enumSets(ncaps,k-i);
				for(int j=0;j<subres.size();j++){
					subres[j].insert(subres[j].begin(),i);
					res.push_back(subres[j]);
				}
			}
			return res;
		}
	}
}
void GraphSearch::computeGeneralHamiltonGraph(	const Capacity &capacity,
												const std::vector<std::pair<int,int> > &allArcs,
												const std::vector<double> &weights,
												const int K, const bool isConnect,
												const std::vector<std::pair<int,int> > &predefinedArcs,
												AllMultipleArcs &tar,
												std::vector<double> &tar2)
{
	MultipleArcs usedArcsID;
	AllMultipleArcs allUsedArcsID;
	std::vector<double> nodeWeights;

	//build up adjacency;
	int lenN = capacity.size();
	int lenA = allArcs.size();
	std::vector<std::vector<int> > adjnodes(lenN);
	std::vector<std::vector<int> > adjarcs(lenN);
	std::vector<std::vector<int> > adjnodeinds(lenN);
	std::vector<std::vector<double> > adjcosts(lenN);
	for(int i=0;i<lenA;i++){
		int n1 = allArcs[i].first;
		int n2 = allArcs[i].second;
		adjarcs[n1].push_back(i);
		adjarcs[n2].push_back(i);
		adjcosts[n1].push_back(weights[i]);
		adjcosts[n2].push_back(weights[i]);
		adjnodes[n1].push_back(n2);
		adjnodes[n2].push_back(n1);
		adjnodeinds[n1].push_back(adjnodes[n2].size()-1);
		adjnodeinds[n2].push_back(adjnodes[n1].size()-1);
	}

	double minArcCost = *std::min_element(weights.begin(),weights.end());

	StateEx2 initState;
	std::vector<StateEx2 > queueState;

	initState.usage = adjnodes;
	for(int i=0;i<adjnodes.size();i++){
		for(int j=0;j<adjnodes[i].size();j++){
			initState.usage[i][j] = 0;
		}
	}
	initState.availableCapacity = capacity;
	initState.cost = 0;
	initState.hcost = 0;

	std::vector<std::pair<int,int> > preArcs = predefinedArcs;
	for(int i=0;i<preArcs.size();i++){
		int n1=preArcs[i].first;
		int n2=preArcs[i].second;
		initState.availableCapacity[n1]--;
		initState.availableCapacity[n2]--;

		int n12 = std::find(adjnodes[n1].begin(),adjnodes[n1].end(),n2)-adjnodes[n1].begin();
		int n21 = std::find(adjnodes[n2].begin(),adjnodes[n2].end(),n1)-adjnodes[n2].begin();
		initState.usage[n1][n12]++;
		initState.usage[n2][n21]++;

		/*add weight to predefined arcs; */
		//
	}

	queueState.push_back(initState);

	int count=0;
	while(true){
		if(queueState.empty())
			break;
		count++;
		double minWeight= FLT_MAX;
		double validWeight= FLT_MAX;
		int minID =-1;
		for(int i=0;i<queueState.size();i++){
			if(queueState[i].hcost<minWeight){
				minID=i; minWeight=queueState[i].hcost;
			}
		}
		StateEx2 currentState = queueState[minID];
		queueState.erase(queueState.begin()+minID);

		if(*(std::max_element(currentState.availableCapacity.begin(),
			currentState.availableCapacity.end()))==0){
			usedArcsID.clear();
			for(int i=0;i<lenN;i++){
				for(int j=0;j<adjnodes[i].size();j++){
					if(adjnodes[i][j]>i && currentState.usage[i][j]>0)
						usedArcsID.push_back(std::pair<int,int>(adjarcs[i][j],
							currentState.usage[i][j]));
				}
			}
			allUsedArcsID.push_back(usedArcsID);
			nodeWeights.push_back(currentState.cost);
			if(allUsedArcsID.size()==K)
				break;
			else
				continue;
		}
		else{
			//expand current state;
			std::vector<int> partialNodes;
			std::vector<int> newNode;
			if(count==1)
				partialNodes.push_back(0);
			else{ 
				for(int i=0;i<lenN;i++){
					if(currentState.availableCapacity[i]>0 &&
						currentState.availableCapacity[i]<capacity[i])
						partialNodes.push_back(i);
					else{
						if(currentState.availableCapacity[i]>0)
							newNode.push_back(i);
					}
				}
			}
/*
			if(partialNodes.empty())
				continue;
*/
			int nodeind;
			if(partialNodes.empty()){
				if(isConnect||newNode.empty())
					continue;
				else
					nodeind = newNode.front();
			}
			else
				nodeind = partialNodes.front();

			std::vector<int> adjcaps;
			for(int i=0;i<adjnodes[nodeind].size();i++){
/*	
				if(currentState.usage[nodeind][i]>0) //can't do this if. it fails for jetfighter, when arc(1,2,3) with cap(2,2,4)
													// and a constraint(arc1,arc3).  Crz, in this case no connect between arc(1,3),
													//but the only state will start by using arc(1,3);
					adjcaps.push_back(0);
				else
*/
					adjcaps.push_back(currentState.availableCapacity[adjnodes[nodeind][i]]);
			}
			int totalcap=0;
			for(int i=0;i<adjcaps.size();i++)
				totalcap+=adjcaps[i];
			if(totalcap<currentState.availableCapacity[nodeind])
				continue;

			std::vector<std::vector<int> > nusage;
			nusage = enumSets(adjcaps,currentState.availableCapacity[nodeind]);

			currentState.availableCapacity[nodeind]=0;
			StateEx2 tstate =currentState;
			for(int i=0;i<nusage.size();i++){
				currentState = tstate;
				for(int j=0;j<nusage[i].size();j++)
					currentState.usage[nodeind][j]+=nusage[i][j];

				for(int j=0;j<nusage[i].size();j++){
					if(nusage[i][j]>0){
						currentState.availableCapacity[adjnodes[nodeind][j]]
							-=nusage[i][j];
						currentState.usage[adjnodes[nodeind][j]][adjnodeinds[nodeind][j]]
							+=nusage[i][j];
					}
				}
				for(int j=0;j<nusage[i].size();j++)
					currentState.cost+=nusage[i][j]*adjcosts[nodeind][j];
				int totalavail=0;
				for(int j=0;j<currentState.availableCapacity.size();j++)
					totalavail+=currentState.availableCapacity[j];
				currentState.hcost=currentState.cost+minArcCost*totalavail/2;

				queueState.push_back(currentState);				
			}
		}
	}
	tar = allUsedArcsID;
	tar2 = nodeWeights;
}


std::vector<std::vector<int> > ordering(const std::vector<std::vector<int> > &data)
{
	std::vector<std::vector<int> > rank(data.size(),std::vector<int>(data.size(),0));
	for(int i=0;i<data.size();i++){
		std::vector<int> tdat = data[i];
		for(int j=0;j<tdat.size();j++){
			int minInd = j;
			for(int k=0;k<tdat.size();k++){
				if(tdat[minInd]>tdat[k])
					minInd = k;
			}
			rank[i][minInd]=j;
			tdat[minInd] = INT_MAX;
		}
	}
	return rank;
}

void GraphSearch::stableMatching(const std::vector<std::vector<int> > &firstGroup,
					const std::vector<std::vector<int> > &secondGroup,
					const std::vector<std::pair<int,int> > &predefinedPairs, std::vector<int> &res)
{
	int size = firstGroup.size();
	std::vector<std::vector<int> > rank1 = ordering(firstGroup);
	std::vector<std::vector<int> > rank2 = ordering(secondGroup);
	std::vector<int> mate1(size,-1);
	std::vector<int> mate2(size,-1);

	std::vector<std::vector<int> > unproposed = firstGroup;
	std::vector<int> freelist(size);
	for(int i=0;i<size;i++)
		freelist[i]=i;

	std::vector<std::pair<int,int> > prePairs = predefinedPairs;
	for(int i=0;i<prePairs.size();i++){
		int m = prePairs[i].first;
		int w = prePairs[i].second;

		freelist.erase(freelist.begin()+(std::find(freelist.begin(),freelist.end(),m)-freelist.begin()));
		unproposed[m].clear();
		for(int j=0;j<unproposed.size();j++){
			if(unproposed[j].empty())
				continue;
			unproposed[j].erase(unproposed[j].begin()+(std::find(unproposed[j].begin(),unproposed[j].end(),w)-unproposed[j].begin()));
		}
		mate2[w]=m;
		mate1[m]=w;
	}

	while(!freelist.empty()){
		int m1 = freelist.front();
		int w = unproposed[m1].front();
		unproposed[m1].erase(unproposed[m1].begin());
		int m2 = mate2[w];
		if(m2==-1){
			mate2[w]=m1;
			mate1[m1]=w;
			freelist.erase(freelist.begin());
		}
		else{
			if(rank2[w][m1]<rank2[w][m2]){
				mate2[w]=m1;
				mate1[m1]=w;
				freelist.erase(freelist.begin());
				freelist.push_back(m2);
			}
		}
	}
	res = mate1;
}
