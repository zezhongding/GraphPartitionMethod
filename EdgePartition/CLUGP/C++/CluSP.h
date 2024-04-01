#ifndef CLUSP_H
#define CLUSP_H

#include "PartitionStrategy.h"
#include "StreamCluster.h"
#include "Edge.h"
#include "globalConfig.h"
#include "common.h"
#include "readGraph.h"

inline void output(std::ofstream& fout,const int srcid,const int destid,const int partition);

class CluSP : public PartitionStrategy {
private:
    std::shared_ptr<StreamCluster> streamCluster;
    std::vector<int> partitionLoad;
    std::unordered_map<int, std::unordered_set<int>> replicateTable;
    std::unordered_map<int, int> clusterPartition;
    std::ofstream bufferedWriter;

public:
    CluSP(GlobalConfig& config, std::shared_ptr<StreamCluster> streamCluster, std::unordered_map<int, int>& clusterPartition);
    virtual ~CluSP() {}

    virtual void performStep(GlobalConfig& config) override;
    virtual void clear() override;
    virtual double getReplicateFactor(GlobalConfig& config) override;
    virtual double getLoadBalance(GlobalConfig& config) override;
    void printInfo();
};

#endif // CLUSP_H