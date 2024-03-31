#ifndef CLUSTERTASK_H
#define CLUSTERTASK_H

#include "Graph.h"
#include "StreamCluster.h"
#include "globalConfig.h"
#include "common.h"

class ClusterTask {
public:
    ClusterTask(std::shared_ptr<Graph> graph, int taskId);
    std::shared_ptr<StreamCluster> call(GlobalConfig& config);

private:
    std::shared_ptr<Graph> graph;
    int taskId;
};

#endif // CLUSTERTASK_H