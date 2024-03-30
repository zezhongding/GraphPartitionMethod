#ifndef PARTITIONSTRATEGY_H
#define PARTITIONSTRATEGY_H

#include "common.h"
#include "globalConfig.h"

class PartitionStrategy {
public:
	virtual ~PartitionStrategy() {}
	virtual void performStep(GlobalConfig& config) = 0;
	virtual void clear() = 0;
	virtual double getReplicateFactor(GlobalConfig& config) = 0;
	virtual double getLoadBalance(GlobalConfig& config) = 0;
};

#endif