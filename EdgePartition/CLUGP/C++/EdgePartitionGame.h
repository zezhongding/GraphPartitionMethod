#ifndef EDGE_PARTITION_GAME_H
#define EDGE_PARTITION_GAME_H

#include "GameStrategy.h" // 假设GameStrategy类的定义在这个文件中
#include "globalConfig.h" // GlobalConfig类定义
#include "Edge.h" // Edge类定义
#include "common.h"

class EdgePartitionGame : public GameStrategy {
private:
    std::vector<Edge> edgeList;
    std::unordered_map<int, int> reflectTable;
    std::unordered_map<Edge, int, EdgeHash> edgePartition; // 注意:需要为Edge类型提供一个哈希函数
    std::vector<int> partitionLoad;
    std::vector<std::vector<int>> vertexRecordTable;
    double beta;

public:
    explicit EdgePartitionGame(GlobalConfig& config,const std::vector<Edge>& edgeList);
    void initGame(GlobalConfig& config) override;
    void startGame(GlobalConfig& config) override;
    std::unordered_map<Edge, int, EdgeHash> getEdgePartition() const; // 注意:返回类型需要与成员变量一致

private:
    double computeCost(GlobalConfig& config,const Edge& edge, int partition);
};

#endif // EDGE_PARTITION_GAME_H