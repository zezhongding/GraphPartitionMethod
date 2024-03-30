#include "ClusterTask.h"

ClusterTask::ClusterTask(std::shared_ptr<Graph> graph, int taskId) : graph(graph), taskId(taskId) {}

StreamCluster* ClusterTask::call(GlobalConfig& config) {
    StreamCluster* streamCluster = new StreamCluster(config,graph);
    streamCluster->startSteamCluster(config);
    return streamCluster;
}