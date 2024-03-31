#include "ClusterPackGame.h"

ClusterPackGame::ClusterPackGame(GlobalConfig& config,std::shared_ptr<StreamCluster> streamCluster, const std::vector<int>& clusterList)
	: streamCluster(streamCluster), clusterList(clusterList), beta(0.0), roundCnt(0) {
	partitionLoad.resize(config.getPartitionNum(), 0.0);
}

void ClusterPackGame::initGame(GlobalConfig& config) {
	int partition = 0;
	for (auto clusterId : clusterList) {
		double minLoad = (double)(config.getECount());
		for (int i = 0; i < config.getPartitionNum(); i++) {
			if (partitionLoad[i] < minLoad) {
				minLoad = partitionLoad[i];
				partition = i;
			}
		}
		clusterPartition[clusterId] = partition;
		partitionLoad[partition] += streamCluster->getEdgeNum(clusterId, clusterId);
	}

	double sizePart = 0.0, cutPart = 0.0;
	for (auto cluster1 : clusterList) {
		sizePart += streamCluster->getEdgeNum(cluster1, cluster1);
		for (auto cluster2 : clusterList) {
			int innerCut = 0;
			int outerCut = 0;
			if (cluster1 != cluster2) {
				innerCut = streamCluster->getEdgeNum(cluster2, cluster1);
				outerCut = streamCluster->getEdgeNum(cluster1, cluster2);
				if (innerCut != 0 || outerCut != 0) {
					if (clusterNeighbours.find(cluster1) == clusterNeighbours.end())
						clusterNeighbours.insert({cluster1, std::unordered_set<int>()});
					if (clusterNeighbours.find(cluster2) == clusterNeighbours.end())
						clusterNeighbours.insert({cluster2, std::unordered_set<int>()});
	
					clusterNeighbours[cluster1].insert(cluster2);
					clusterNeighbours[cluster2].insert(cluster1);
				}
				cutPart += outerCut;
			}
			if(cutCostValue.find(cluster1) == cutCostValue.end())
				cutCostValue[cluster1] = 0;
			cutCostValue[cluster1] += (innerCut + outerCut);
		}
	}

	beta = config.getPartitionNum() * config.getPartitionNum() * cutPart / (sizePart * sizePart);
}

void ClusterPackGame::startGame(GlobalConfig& config) {
	bool finish = false;
	while (!finish) {
		finish = true;
		for (auto clusterId : clusterList) {
			double minCost = std::numeric_limits<double>::max();
			int minPartition = clusterPartition[clusterId];
			for (int j = 0; j < config.getPartitionNum(); j++) {
				double cost = computeCost(config, clusterId, j);
				if (cost <= minCost) {
					minCost = cost;
					minPartition = j;
				}
			}
			if (minPartition != clusterPartition[clusterId]) {
				finish = false;
				partitionLoad[minPartition] += streamCluster->getEdgeNum(clusterId, clusterId);
				partitionLoad[clusterPartition[clusterId]] -= streamCluster->getEdgeNum(clusterId, clusterId);
				clusterPartition[clusterId] = minPartition;
			}
		}
		roundCnt++;
	}
}

double ClusterPackGame::computeCost(GlobalConfig& config,int clusterId, int partition) {
	double loadPart = 0.0;
	int edgeCutPart = cutCostValue[clusterId];
	int old_partition = clusterPartition[clusterId];

	loadPart = partitionLoad[old_partition];
	if (partition != old_partition)
		loadPart = partitionLoad[partition] + streamCluster->getEdgeNum(clusterId, clusterId);

	if (clusterNeighbours.count(clusterId)) {
		for (auto neighbour : clusterNeighbours[clusterId]) {
			if (clusterPartition[neighbour] == partition)
				edgeCutPart -= (streamCluster->getEdgeNum(clusterId, neighbour) + streamCluster->getEdgeNum(neighbour, clusterId));
		}
	}

	double alpha = config.getAlpha();
	double k = config.getPartitionNum();
	double m = streamCluster->getEdgeNum(clusterId, clusterId);

	return alpha * beta / k * loadPart * m + (1 - alpha) / 2 * edgeCutPart;
}

int ClusterPackGame::getRoundCnt() const {
	return roundCnt;
}

std::unordered_map<int, int> ClusterPackGame::getClusterPartition() const {
	return clusterPartition;
}