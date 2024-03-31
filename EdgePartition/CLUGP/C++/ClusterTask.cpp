#include "ClusterTask.h"

ClusterTask::ClusterTask(std::shared_ptr<Graph> graph, int taskId) : graph(graph), taskId(taskId) {}

std::shared_ptr<StreamCluster> ClusterTask::call(GlobalConfig& config) {
    std::shared_ptr<StreamCluster> streamCluster = std::make_shared<StreamCluster>(config,graph);
    streamCluster->startSteamCluster(config);
    return streamCluster;
}