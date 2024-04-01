#ifndef CLUSTERTASK_H
#define CLUSTERTASK_H

#include "StreamCluster.h"
#include "globalConfig.h"
#include "common.h"

class ClusterTask {
public:
    ClusterTask(int taskId);
    std::shared_ptr<StreamCluster> call(GlobalConfig& config);

private:
    int taskId;
};

#endif // CLUSTERTASK_H