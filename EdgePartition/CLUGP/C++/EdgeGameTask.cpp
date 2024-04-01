#include "EdgeGameTask.h"


EdgeGameTask::EdgeGameTask(GlobalConfig& config, std::vector<Edge>& edgeList, int taskId) {
	int batchSize = config.getBatchSize();
	int begin = batchSize * taskId;
	int end = std::min(batchSize * (taskId + 1), static_cast<int>(edgeList.size()));

	for (int i = begin; i < end; ++i) {
		this->edges.push_back(edgeList[i]);
	}
}

std::unordered_map<Edge, int, EdgeHash> EdgeGameTask::call(GlobalConfig& config) {
	EdgePartitionGame game(config, edges);
	game.initGame(config);
	game.startGame(config);
	return game.getEdgePartition();
}