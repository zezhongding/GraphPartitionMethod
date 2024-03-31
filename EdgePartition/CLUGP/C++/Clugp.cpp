#include "Clugp.h"

std::unordered_map<int, int> mergeMaps(const std::vector<std::unordered_map<int, int>>& maps) {
	std::unordered_map<int, int> result;

	#pragma omp parallel
	{
		std::unordered_map<int, int> localResult;

		#pragma omp for nowait
		for (size_t i = 0; i < maps.size(); ++i) {
			for (const auto& pair : maps[i]) {
				localResult[pair.first] += pair.second; // Handle duplicate keys by adding their values
			}
		}

		#pragma omp critical
		for (const auto& pair : localResult) {
			result[pair.first] += pair.second;
		}
	}

	return result;
}

Clugp::Clugp(GlobalConfig& config)
{
	graph = std::make_shared<OriginGraph>(config);
	roundCnt = 0;
	gameStartTime = 0;
	gameEndTime = 0;
	taskNum = 0;
	clusterSize = 0;
}


void Clugp::main(GlobalConfig& config) {

	std::cout << "---------------start-------------" << std::endl;
	auto startTime = std::chrono::system_clock::now();

	StreamCluster streamCluster(config, graph);
	streamCluster.startSteamCluster(config);
	std::vector<int> clusterList = streamCluster.getClusterList();

	parallelGame(config, streamCluster, clusterList);

	CluSP cluSP(config, graph, &streamCluster, clusterPartition);
	cluSP.performStep(config);

	auto endTime = std::chrono::system_clock::now();

	double rf = cluSP.getReplicateFactor(config);
	double lb = cluSP.getLoadBalance(config);

	graph->clear();
	cluSP.clear();

	std::cout << "Partition num:" << config.getPartitionNum() << std::endl;
	std::cout << "Partition time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms" << std::endl;
	std::cout << "Relative balance load:" << lb << std::endl;
	std::cout << "Replicate factor: " << rf << std::endl;
	std::cout << "Total game round:" << roundCnt << std::endl;
	auto gameDuration = std::chrono::milliseconds(gameEndTime - gameStartTime);
	std::cout << "Cluster game time: " << gameDuration.count() << " ms" << std::endl;
	std::cout << "---------------end-------------" << std::endl;
}

void Clugp::parallelGame(GlobalConfig& config, StreamCluster& streamCluster, const std::vector<int>& clusterList) {
	int clusterSize = clusterList.size();
	std::cout << "Cluster Size: " << clusterSize << std::endl;
	int taskNum = (clusterSize + config.getBatchSize() - 1) / config.getBatchSize();
	std::cout << "TaskNum: " << taskNum << std::endl;
	std::cout << "Cluster num: " << clusterSize << std::endl;

	auto gameStartTime = std::chrono::system_clock::now();

	std::vector<ClusterPackGame*> games(taskNum); // 假设有默认构造函数

	int roundCnt = 0;

	// 使用OpenMP并行化for循环
	#pragma omp parallel for
	for (int i = 0; i < taskNum; ++i) {
		games[i] = ClusterGameTask(config, &streamCluster, i).call(config);
	}

	std::vector<std::unordered_map<int, int>> parts(taskNum);
	for (int i = 0; i < taskNum; ++i) {
		parts[i] = games[i]->getClusterPartition(); // 假设这是正确的方法获取数据
		//clusterPartition.insert(part.begin(), part.end());
		//std::unordered_map<int, int>::iterator it = part.begin();
		//std::merge(map1.begin(),map1.end(),map2.begin(),map2.end(),inserter(part,it));
		roundCnt += games[i]->getRoundCnt();
	}

	std::unordered_map<int, int> clusterPartition = mergeMaps(parts);

	auto gameEndTime = std::chrono::system_clock::now();
}