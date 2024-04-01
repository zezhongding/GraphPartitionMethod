#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"
#include "Edge.h"
#include "globalConfig.h"

class Graph {
public:
	virtual ~Graph() {}
	virtual void readGraphFromFile(GlobalConfig& config) = 0;
	virtual int getVCount() const = 0;
	virtual int getECount() const = 0;
	virtual void addEdge(int srcVId, int destVId) = 0;
	virtual std::vector<Edge> getEdgeList() const = 0;
	virtual void clear() = 0;
	virtual Edge readStep() = 0;
	virtual void closef() = 0;
	virtual bool iseof() const = 0;
};

#endif // GRAPH_H