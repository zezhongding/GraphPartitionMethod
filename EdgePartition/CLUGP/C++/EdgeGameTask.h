#ifndef EDGE_GAMETASK_H
#define EDGE_GAMETASK_H

#include "common.h"
#include "Edge.h"
#include "EdgePartitionGame.h"
#include "globalConfig.h"

class EdgeGameTask {
public:
    EdgeGameTask(GlobalConfig& config, std::vector<Edge>& edgeList, int taskId);
    std::unordered_map<Edge, int, EdgeHash> call(GlobalConfig& config);

private:
    std::vector<Edge> edges;
};

#endif // EDGE_GAMETASK_H