#ifndef STREAMCLUSTER_H
#define STREAMCLUSTER_H
#include "Graph.h"

class StreamCluster {
private:
	std::vector<int> cluster;
	std::vector<int> degree;
	std::unordered_map<int, int> volume;
	std::unordered_map<int, std::unordered_map<int, int>> innerAndCutEdge;
	std::shared_ptr<Graph> graph;
	std::vector<int> clusterList;
	int maxVolume;
public:
	StreamCluster(GlobalConfig& config,std::shared_ptr<Graph> graph);
	void startSteamCluster(GlobalConfig& config);
	void combineCluster(int srcVid, int destVid);
	void setUpIndex();
	void computeEdgeInfo(GlobalConfig& config);
	void PrintInformation(GlobalConfig& config);

	std::vector<int> getClusterList() const;
	std::vector<int> getCluster() const;
	std::unordered_map<int, std::unordered_map<int, int>> getInnerAndCutEdge() const;
	int getEdgeNum(int cluster1, int cluster2) const;
	std::string toString() const;
	int getClusterId(int vId) const;
	std::unordered_map<int, int> getVolume() const;
	void changeCluster(const std::vector<int>& cluster);
	void changeClusterList(const std::vector<int>& clusterList);
	void reprocess(GlobalConfig& config);
};

#endif // STREAMCLUSTER_H