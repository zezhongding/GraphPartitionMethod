#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include "common.h"
class GlobalConfig {
private:
	std::unordered_map<std::string, std::string> properties;
	int k;
	int l;
	int hashNum;
	int compressionRate;
	std::string inputGraphPath;
	std::string outputGraphPath;
	int vCount;
	int eCount;
	float alpha;
	int partitionNum;
	int batchSize;
	int threads;

public:
	GlobalConfig() {};
	GlobalConfig(std::string filepath);
	int getHashNum() const;
	int getCompressionRate() const;
	std::string getInputGraphPath() const;
	int getVCount() const;
	int getECount() const;
	int getPartitionNum() const;
	int getMaxClusterVolume() const;
	float getAlpha() const;
	int getK() const;
	int getL() const;
	int getBatchSize() const;
	int getThreads() const;
	std::string getOutputGraphPath() const;
	void printParaInfo() const;
};

#endif // GLOBALCONFIG_H