#ifndef CLUSTERGAMETASK_H
#define CLUSTERGAMETASK_H

#include "common.h"
#include "StreamCluster.h"
#include "globalConfig.h"
#include "ClusterPackGame.h"

class ClusterGameTask {
public:
	ClusterGameTask(GlobalConfig& config,StreamCluster* streamCluster, int taskId);
	ClusterPackGame* call(GlobalConfig& config);

private:
	StreamCluster* streamCluster;
	std::vector<int> cluster;
};

#endif // CLUSTERGAMETASK_H