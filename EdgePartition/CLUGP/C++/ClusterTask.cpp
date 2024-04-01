#include "ClusterTask.h"

ClusterTask::ClusterTask(int taskId) : taskId(taskId) {}

std::shared_ptr<StreamCluster> ClusterTask::call(GlobalConfig& config) {
    std::shared_ptr<StreamCluster> streamCluster = std::make_shared<StreamCluster>(config);
    streamCluster->startSteamCluster(config);
    return streamCluster;
}