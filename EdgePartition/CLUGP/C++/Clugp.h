#ifndef CLUGP_H
#define CLUGP_H

#include "common.h"
#include "globalConfig.h"
#include "Graph.h"
#include "OriginGraph.h"
#include "StreamCluster.h"
#include "CluSP.h"
#include "ClusterPackGame.h"
#include "ClusterGameTask.h"

std::unordered_map<int, int> mergeMaps(const std::vector<std::unordered_map<int, int>>& maps);

class Clugp {
public:
    Clugp(GlobalConfig& config);
    void main(GlobalConfig& config);

private:
    void parallelGame(GlobalConfig& config, std::shared_ptr<StreamCluster> streamCluster, const std::vector<int>& clusterList);
    std::shared_ptr<OriginGraph> graph;
    std::unordered_map<int, int> clusterPartition;
    int roundCnt;
    std::chrono::time_point<std::chrono::system_clock> gameStartTime;
    std::chrono::time_point<std::chrono::system_clock> gameEndTime;
    int taskNum;
    int clusterSize;
};

#endif // CLUGP_H