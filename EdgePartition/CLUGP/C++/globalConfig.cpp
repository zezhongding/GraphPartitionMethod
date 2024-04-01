#include "globalConfig.h"

GlobalConfig::GlobalConfig(std::string filepath) {
	std::ifstream configFile(filepath);
	std::string line;
	while (std::getline(configFile, line, '\n')) {
		if (line.empty() || line[0] == '#')
			continue;

		size_t delimiterPos = line.find('=');
		if (delimiterPos == std::string::npos) {
			std::cerr << "Error: Invalid line format in the configuration file: " << line << std::endl;
			continue;
		}

		std::string key = line.substr(0, delimiterPos);
		std::string value = line.substr(delimiterPos + 1);
		std::cout << key  << " = " << value << std::endl;
		properties[key] = value;
	}

	k = std::stoi(properties["k"]);
	l = std::stoi(properties["l"]);
	hashNum = std::stoi(properties["hashNum"]);
	compressionRate = std::stoi(properties["compressionRate"]);
	vCount = std::stoi(properties["vCount"]);
	eCount = std::stoi(properties["eCount"]);
	inputGraphPath = properties["inputGraphPath"];
	inputGraphPath.pop_back(); //会读入换行符到文件
	outputGraphPath = properties["outputGraphPath"];
	outputGraphPath.pop_back();
	alpha = std::stof(properties["alpha"]);
	partitionNum = std::stoi(properties["partitionNum"]);
	batchSize = std::stoi(properties["batchSize"]);
	threads = std::stoi(properties["threads"]);
}

int GlobalConfig::getHashNum() const {
	return hashNum;
}

int GlobalConfig::getCompressionRate() const {
	return compressionRate;
}

std::string GlobalConfig::getInputGraphPath() const {
	return inputGraphPath;
}

int GlobalConfig::getVCount() const {
	return vCount;
}

int GlobalConfig::getECount() const {
	return eCount;
}

int GlobalConfig::getPartitionNum() const {
	return partitionNum;
}

int GlobalConfig::getMaxClusterVolume() const {
	return  eCount / partitionNum;
}

float GlobalConfig::getAlpha() const {
	return alpha;
}

int GlobalConfig::getK() const {
	return k;
}

int GlobalConfig::getL() const {
	return l;
}

int GlobalConfig::getBatchSize() const {
	return batchSize;
}

int GlobalConfig::getThreads() const {
	return threads;
}

std::string GlobalConfig::getOutputGraphPath() const {
	return outputGraphPath;
}

void GlobalConfig::printParaInfo() const {
	std::cout << "input graph: " << this->inputGraphPath << std::endl;
	std::cout << "vCount: " << this->vCount << std::endl;
	std::cout << "eCount: " << this->eCount << std::endl;
	std::cout << "partitionNum: " << this->partitionNum << std::endl;
	std::cout << "alpha: " << this->alpha << std::endl;
	std::cout << "k: " << this->k << std::endl;
	std::cout << "l: " << this->l << std::endl;
	std::cout << "batchSize: " << this->batchSize << std::endl;
	std::cout << "threads: " << this->threads << std::endl;
	std::cout << "MaxClusterVolume: " << this->getMaxClusterVolume() << std::endl;
	std::cout << "compressionRate: " << this->compressionRate << std::endl;
	std::cout << "hashNum: " << this->hashNum << std::endl;
}