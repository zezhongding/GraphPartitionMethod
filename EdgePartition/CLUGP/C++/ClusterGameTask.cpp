#include "ClusterGameTask.h"


ClusterGameTask::ClusterGameTask(GlobalConfig& config,StreamCluster* streamCluster, int taskId) : streamCluster(streamCluster) {
    int batchSize = config.getBatchSize();
    std::vector<int> clusterList = streamCluster->getClusterList();
    int begin = batchSize * taskId;
    int end = std::min(batchSize * (taskId + 1), static_cast<int>(clusterList.size()));

    cluster.assign(clusterList.begin() + begin, clusterList.begin() + end);
}

ClusterPackGame* ClusterGameTask::call(GlobalConfig& config) {
    ClusterPackGame* clusterPackGame = new ClusterPackGame(config, streamCluster, cluster);
    clusterPackGame->initGame(config);
    clusterPackGame->startGame(config);
    return clusterPackGame;
}