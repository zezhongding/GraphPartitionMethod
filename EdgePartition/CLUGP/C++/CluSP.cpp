#include "CluSP.h"

CluSP::CluSP(GlobalConfig& config, std::shared_ptr<StreamCluster> streamCluster, std::unordered_map<int, int>& clusterPartition)
: streamCluster(streamCluster), clusterPartition(clusterPartition) {
	partitionLoad.resize(config.getPartitionNum());
	replicateTable.reserve(config.getVCount());
}

void CluSP::performStep(GlobalConfig& config) {
	double maxLoad = static_cast<double>(config.getECount()) / config.getPartitionNum() * 1.1;

	std::pair<int,int> e(-1,-1);
	TGEngine tgEngine(config.getInputGraphPath(),config.getVCount(),config.getECount());

	while(-1 != tgEngine.readline(e)){
		int src = e.first;
		int dest = e.second;
		if(src < 0 || dest < 0 || src >= config.getVCount() || dest >= config.getVCount())
			continue;
		int srcPartition = clusterPartition[streamCluster->getClusterId(src)];
		int destPartition = clusterPartition[streamCluster->getClusterId(dest)];
		int edgePartition = -1;

		if (!replicateTable.count(src)) replicateTable[src] = std::unordered_set<int>();
		if (!replicateTable.count(dest)) replicateTable[dest] = std::unordered_set<int>();
		if (partitionLoad[srcPartition] > maxLoad && partitionLoad[destPartition] > maxLoad) {
			for (int i = 0; i < config.getPartitionNum(); i++) {
				if (partitionLoad[i] <= maxLoad) {
					edgePartition = i;
					srcPartition = i;
					destPartition = i;
					break;
				}
			}
		} else if (partitionLoad[srcPartition] > partitionLoad[destPartition]) {
			edgePartition = destPartition;
			srcPartition = destPartition;
		} else {
			edgePartition = srcPartition;
			destPartition = srcPartition;
		}
		partitionLoad[edgePartition]++;
		replicateTable[src].insert(srcPartition);
		replicateTable[dest].insert(destPartition);
	}
}

void CluSP::clear() {
	replicateTable.clear();
}

double CluSP::getReplicateFactor(GlobalConfig& config) {
	double sum = 0.0;
	for (const auto& pair : replicateTable) {
		sum += pair.second.size();
	}
	return sum / config.getVCount();
}

double CluSP::getLoadBalance(GlobalConfig& config) {
	double maxLoad = 0.0;
	for (int load : partitionLoad) {
		if (maxLoad < load) {
			maxLoad = load;
		}
	}
	return static_cast<double>(config.getPartitionNum()) / config.getECount() * maxLoad;
}

void CluSP::output(const Edge& edge, int partition) {
	try {
		bufferedWriter << edge.getSrcVId() << "\t" << edge.getDestVId() << "\t" << partition << "\n";
	} catch (const std::exception& ex) {
		// Handle exception or log error
		std::cerr << "output CluSP error!" << std::endl;
	}
}

void CluSP::printInfo() {
	std::cout << "clusterPartition" << std::endl;
	for (const auto& pair : clusterPartition) {
		std::cout << pair.first << " " << pair.second << std::endl;
	}
	std::cout << "end" << std::endl;
}