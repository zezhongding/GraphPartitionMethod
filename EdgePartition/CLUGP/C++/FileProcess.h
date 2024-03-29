#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include "StreamCluster.h"
#include "globalConfig.h"
#include "common.h"

class FileProcess {
public:
    FileProcess(StreamCluster* streamCluster);

    void savefile(GlobalConfig& config);
    void readfile(GlobalConfig& config);
    StreamCluster* getstreamCluster();

private:
    StreamCluster* streamCluster;
    std::unordered_map<int, std::unordered_map<int, int>> innerAndCutEdge;
    std::vector<int> clusterList;
    std::vector<int> cluster;
};

#endif // FILEPROCESS_H