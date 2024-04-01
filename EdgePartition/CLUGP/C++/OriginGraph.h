#ifndef ORIGINGRAPH_H
#define ORIGINGRAPH_H

#include "Graph.h"
#include "common.h"

class OriginGraph : public Graph {
private:
	std::vector<Edge> edgeList;
	int vCount;
	int eCount;
	std::ifstream bufferedReader;
public:

	OriginGraph(GlobalConfig& config);
	virtual ~OriginGraph() {}
	void readGraphFromFile(GlobalConfig& config) override;
	int getVCount() const override;
	int getECount() const override;
	void addEdge(int srcVId, int destVId) override;
	std::vector<Edge> getEdgeList() const override;
	void clear() override;
	Edge readStep() override;
	void closef() override;
	bool iseof() const override;
};

#endif // ORIGINGRAPH_H