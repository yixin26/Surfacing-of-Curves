#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE

#include "amlVec.h"
#include <vector>

const AML::Scalar POINT_CLOSENESS_THRESHOLD = 1e-3;//1e-3;

namespace GraphStructure{

	class StdPoint //extern of vector
	{
	public: StdPoint(AML::double3& p) : m_val(p){}
			~StdPoint(){}
			bool operator==(const StdPoint& other) const{ return (m_val.x == other.m_val.x&&m_val.y == other.m_val.y&&m_val.z == other.m_val.z); }
			bool operator>(const StdPoint& other) const{
				return
					(m_val.x > other.m_val.x) ||
					(m_val.x == other.m_val.x && m_val.y > other.m_val.y) ||
					(m_val.x == other.m_val.x && m_val.y == other.m_val.y && m_val.z > other.m_val.z);
			}
			bool operator<(const StdPoint& other) const{
				return
					m_val.x < other.m_val.x ||
					(m_val.x == other.m_val.x && m_val.y < other.m_val.y) ||
					(m_val.x == other.m_val.x && m_val.y == other.m_val.y && m_val.z < other.m_val.z);

			}
	public:
		AML::double3	m_val;
	};

	//curve network;
	typedef AML::double3 Point;
	typedef std::vector<Point> Curve;
	typedef std::vector<Curve> LinearCurveNet;

	class IntersectPoint{
	public:
		Point pos;
		std::vector<int> arcID;
		std::vector<int> arcDirection;
	public:
		IntersectPoint()
		{

		}
		IntersectPoint(const Point &pos, const std::vector<int> &arcID,const std::vector<int> &arcDirection)
			:pos(pos),arcID(arcID),arcDirection(arcDirection)
		{
		}
		IntersectPoint(const IntersectPoint& that)
		{
			this->pos = that.pos;
			this->arcID = that.arcID;
			this->arcDirection = that.arcDirection;
		}
		~IntersectPoint()
		{
			std::vector<int>().swap(arcID);
			std::vector<int>().swap(arcDirection);
		}
		IntersectPoint& operator=(const IntersectPoint& that)
		{
			this->pos = that.pos;
			this->arcID = that.arcID;
			this->arcDirection = that.arcDirection;
			return *this;
		}
	};

	class Arc{
	public:
		Curve vertexList;
		int capacity;
		std::pair<int,int> endNodesID;
		std::pair<int,int> posInNode;
	public:
		Arc()
		{

		}
		Arc(const Curve &vertexList, const int &capacity,
			const std::pair<int,int>  &endNodesID,const std::pair<int,int> &posInNode)
			:vertexList(vertexList),capacity(capacity),endNodesID(endNodesID),posInNode(posInNode)
		{
		}
		Arc(const Arc& that)
		{
			this->vertexList = that.vertexList;
			this->capacity = that.capacity;
			this->endNodesID = that.endNodesID;
			this->posInNode = that.posInNode;
		}
		~Arc()
		{
			Curve().swap(vertexList);
			std::pair<int,int>().swap(endNodesID);
			std::pair<int,int>().swap(posInNode);
		}
		Arc& operator=(const Arc& that)
		{
			this->vertexList = that.vertexList;
			this->capacity = that.capacity;
			this->endNodesID = that.endNodesID;
			this->posInNode = that.posInNode;
			return *this;
		}
	};

	class Graph{
	public:
		std::vector<IntersectPoint> nodes;
		std::vector<Arc> arcs;
	public:
		Graph()
		{

		}
		Graph(const std::vector<IntersectPoint> &nodes, const std::vector<Arc> &arcs)
			:nodes(nodes),arcs(arcs)
		{
		}
		Graph(const Graph& that)
		{
			this->nodes = that.nodes;
			this->arcs = that.arcs;
		}
		~Graph()
		{
			std::vector<IntersectPoint>().swap(nodes);
			std::vector<Arc>().swap(arcs);
		}
		Graph& operator=(const Graph& that)
		{
			this->nodes = that.nodes;
			this->arcs = that.arcs;
			return *this;
		}
	};

	//rotation graph;
	typedef std::vector<std::vector<
		std::vector<std::pair<int,int> > > > RotationGraph;

	//pole graph;
	typedef std::vector<RotationGraph> PoleGraphNodes;
	typedef std::vector<std::vector<double> > PoleGraphNodeWeight;
	typedef std::vector<std::vector<std::vector<double> > > PoleGraphArcs;
	typedef std::vector<std::vector<std::vector<std::vector<std::pair<int,int> > > > > PoleGraphArcsPair;
	class ExpandPole{
	public:
		int nodeID;
		std::vector<long long> front;
		long long stateNum;
	public:
		ExpandPole()
		{

		}
		ExpandPole(const int &nodeID, const std::vector<long long> &front, const long long &stateNum)
			:nodeID(nodeID),front(front),stateNum(stateNum)
		{
		}
		ExpandPole(const ExpandPole& that)
		{
			this->nodeID = that.nodeID;
			this->front = that.front;
			this->stateNum = that.stateNum;
		}
		~ExpandPole()
		{
			std::vector<long long>().swap(front);
		}
		ExpandPole& operator=(const ExpandPole& that)
		{
			this->nodeID = that.nodeID;
			this->front = that.front;
			this->stateNum = that.stateNum;
			return *this;
		}
	};
	typedef std::vector<ExpandPole> ExpandPoleSequence;

	//cycle;
	class CycleSegment{
	public:
		int arcID;
		int strEndID;
		int instanceID;
	public:
		CycleSegment()
		{

		}
		CycleSegment(const int &arcID, const int &strEndID,const int &instanceID)
			:arcID(arcID),strEndID(strEndID),instanceID(instanceID)
		{
		}
		CycleSegment(const CycleSegment& that)
		{
			this->arcID = that.arcID;
			this->strEndID = that.strEndID;
			this->instanceID = that.instanceID;
		}
		~CycleSegment()
		{
		}
		CycleSegment& operator=(const CycleSegment& that)
		{
			this->arcID = that.arcID;
			this->strEndID = that.strEndID;
			this->instanceID = that.instanceID;
			return *this;
		}
	};
	typedef std::vector<CycleSegment> Cycle;
	typedef std::vector<Cycle> CycleSet;

	//triangle mesh;
	typedef std::vector<std::vector<AML::double3> > TriangleCycle;
	typedef std::vector<TriangleCycle > TriangleSurface;
}

namespace CVR{
	//when use in a vector, should define construct and destruct function.
	//read cvr files;
	struct EdgePair
	{
		int e1, e2;
	};
	struct Pos
	{
		float x, y, z;
	};
	struct Corner
	{
		int numEdges;
		int *edges;
		int numPatches;
		int *patches;
		Pos pos;
		int numColinear;
		EdgePair *colinear;
	};
	struct Edge
	{
		int sharp;
		int c1, c2;
		int numVerts;
		Pos *verts;
	};
	struct Patch
	{
		int numVerts;
		int *verts;
	};
}
#endif
