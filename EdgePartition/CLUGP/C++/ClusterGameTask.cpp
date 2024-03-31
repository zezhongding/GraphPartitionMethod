#include "ClusterGameTask.h"


ClusterGameTask::ClusterGameTask(GlobalConfig& config,std::shared_ptr<StreamCluster> streamCluster, int taskId) : streamCluster(streamCluster) {
    int batchSize = config.getBatchSize();
    std::vector<int> clusterList = streamCluster->getClusterList();
    int begin = batchSize * taskId;
    int end = std::min(batchSize * (taskId + 1), static_cast<int>(clusterList.size()));

    cluster.assign(clusterList.begin() + begin, clusterList.begin() + end);
}

std::shared_ptr<ClusterPackGame> ClusterGameTask::call(GlobalConfig& config) {
    std::shared_ptr<ClusterPackGame> clusterPackGame = std::make_shared<ClusterPackGame>(config, streamCluster, cluster);
    clusterPackGame->initGame(config);
    clusterPackGame->startGame(config);
    return clusterPackGame;
}