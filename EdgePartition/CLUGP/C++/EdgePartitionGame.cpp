#include "EdgePartitionGame.h"

EdgePartitionGame::EdgePartitionGame(GlobalConfig& config,const std::vector<Edge>& edgeList)
	: edgeList(edgeList),partitionLoad(config.getPartitionNum(), 0), beta(0) {
	edgePartition.clear();
	reflectTable.clear();
}

void EdgePartitionGame::initGame(GlobalConfig& config) {
	int vertexCount = 0;

	for (const Edge & edge : edgeList) {
		int src = edge.getSrcVId();
		int dest = edge.getDestVId();

		if (reflectTable.find(src) == reflectTable.end()) reflectTable[src] = vertexCount++;
		if (reflectTable.find(dest) == reflectTable.end()) reflectTable[dest] = vertexCount++;
	}

	vertexRecordTable.resize(vertexCount, std::vector<int>(config.getPartitionNum(), 0));

	std::mt19937 rng(std::random_device{}());

	for (const auto& edge : edgeList) {
		int partition = rng() % config.getPartitionNum();
		int src = reflectTable[edge.getSrcVId()];
		int dest = reflectTable[edge.getDestVId()];

		if (edgePartition.find(edge) == edgePartition.end()) edgePartition[edge] = partition;

		partitionLoad[partition]++;
		vertexRecordTable[src][partition]++;
		vertexRecordTable[dest][partition]++;
	}

	beta = std::pow(config.getPartitionNum(), 3) * static_cast<double>(vertexCount) / (2 * std::pow(edgeList.size(), 2));
}

double EdgePartitionGame::computeCost(GlobalConfig& config,const Edge& edge, int partition) {
	double alpha = config.getAlpha();
	int k = config.getPartitionNum();

	int src = reflectTable[edge.getSrcVId()];
	int dest = reflectTable[edge.getDestVId()];

	int load = partitionLoad[partition];
	double srcDegreeCost = vertexRecordTable[src][partition];
	double destDegreeCost = vertexRecordTable[dest][partition];

	if (partition != edgePartition[edge]) {
		load++;
		srcDegreeCost++;
		destDegreeCost++;
	}

	return alpha * beta / k * load + (1 - alpha) * (1 / srcDegreeCost + 1 / destDegreeCost);
}

void EdgePartitionGame::startGame(GlobalConfig& config) {
	bool finish = false;

	while (!finish) {
		finish = true;
		for (const Edge& edge : edgeList) {
			double minCost = std::numeric_limits<double>::max();
			int originPartition = edgePartition[edge];
			int minPartition = 0;

			for (int i = 0; i < config.getPartitionNum(); i++) {
				double cost = computeCost(config, edge, i);
				if (minCost > cost) {
					minCost = cost;
					minPartition = i;
				} else if (minCost == cost && i < minPartition) {
					minPartition = i;
				}
			}

			if (minPartition != originPartition)
				finish = false;

			partitionLoad[originPartition]--;
			int src = reflectTable[edge.getSrcVId()];
			int dest = reflectTable[edge.getDestVId()];
			vertexRecordTable[src][originPartition]--;
			vertexRecordTable[dest][originPartition]--;
			partitionLoad[minPartition]++;
			vertexRecordTable[src][minPartition]++;
			vertexRecordTable[dest][minPartition]++;
			edgePartition.insert({edge,minPartition});
		}
	}
}

std::unordered_map<Edge, int, EdgeHash> EdgePartitionGame::getEdgePartition() const {
	return this->edgePartition;
}