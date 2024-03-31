// ClusterPackGames.h
#ifndef CLUSTERPACKGAMES_H
#define CLUSTERPACKGAMES_H

#include "GameStrategy.h"
#include "StreamCluster.h"
#include "globalConfig.h"
#include "common.h"

class ClusterPackGame : public GameStrategy {
private:
    std::unordered_map<int, int> clusterPartition; // key: cluster, value: partition
    std::unordered_map<int, int> cutCostValue; // key: cluster, value: cutCost
    std::unordered_map<int, std::unordered_set<int>> clusterNeighbours;
    std::vector<double> partitionLoad;
    std::vector<int> clusterList;
    std::shared_ptr<StreamCluster> streamCluster;
    double beta;
    int roundCnt;

public:
    ClusterPackGame(GlobalConfig& config,std::shared_ptr<StreamCluster> streamCluster, const std::vector<int>& clusterList);
    virtual void initGame(GlobalConfig& config) override;
    virtual void startGame(GlobalConfig& config) override;
    int getRoundCnt() const;
    std::unordered_map<int, int> getClusterPartition() const;

private:
    double computeCost(GlobalConfig& config,int clusterId, int partition);
};

#endif // CLUSTERPACKGAMES_H