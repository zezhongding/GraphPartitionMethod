#include "OriginGraph.h"
#include "common.h"

OriginGraph::OriginGraph(GlobalConfig& config){
	vCount = config.getVCount();
	eCount = config.getECount();
}

void OriginGraph::readGraphFromFile(GlobalConfig& config) {
	try {
		std::string path = config.getInputGraphPath();
		bufferedReader.open(path);
	} catch (const std::ifstream::failure& e) {
		std::cerr << "Exception opening/reading file" << std::endl;
	}
}

Edge OriginGraph::readStep() {
	std::string line;
	while (std::getline(bufferedReader, line)) {
		if (line[0] == '#')
			continue;
		if (line.empty())
			return Edge();
		std::istringstream iss(line);
		int srcVid, destVid;
		try{
			iss >> srcVid >> destVid;
		} catch (const std::istringstream::failure& e){
			std::cerr << "wrong format" << std::endl;
			continue;
		}
		if (srcVid == destVid)
			continue;
		return Edge(srcVid, destVid, 1);
	}
	// 在C++中，我们需要考虑如何处理这种情况，因为我们不能返回null。
	// 一种可能的方式是返回一个标记值或抛出一个异常。
	// 这里我们选择返回一个默认构造的Edge对象。
	return Edge();
}

void OriginGraph::addEdge(int srcVId, int destVId) {
	edgeList.push_back(Edge(srcVId, destVId, 1));
}

std::vector<Edge> OriginGraph::getEdgeList() const {
	return edgeList;
}

int OriginGraph::getVCount() const {
	return vCount;
}

int OriginGraph::getECount() const {
	return eCount;
}

void OriginGraph::clear() {
	edgeList.clear();
}