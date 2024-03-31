#ifndef CLUSTERGAMETASK_H
#define CLUSTERGAMETASK_H

#include "common.h"
#include "StreamCluster.h"
#include "globalConfig.h"
#include "ClusterPackGame.h"

class ClusterGameTask {
public:
	ClusterGameTask(GlobalConfig& config,std::shared_ptr<StreamCluster> streamCluster, int taskId);
	std::shared_ptr<ClusterPackGame> call(GlobalConfig& config);

private:
	std::shared_ptr<StreamCluster> streamCluster;
	std::vector<int> cluster;
};

#endif // CLUSTERGAMETASK_H