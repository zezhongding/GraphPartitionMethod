#include "StreamCluster.h"
#include "globalConfig.h" // 确保这是GlobalConfig类的正确包含路径

StreamCluster::StreamCluster(GlobalConfig& config,std::shared_ptr<Graph> graph) {
	this->graph = graph;
	cluster.resize(config.getVCount(), 0);
	degree.resize(config.getVCount(), 0);
	maxVolume = config.getMaxClusterVolume();
}

void StreamCluster::combineCluster(int srcVid, int destVid) {
	if (volume[cluster[srcVid]] >= maxVolume || volume[cluster[destVid]] >= maxVolume)
		return;

	int minVid = (volume[cluster[srcVid]] < volume[cluster[destVid]] ? srcVid : destVid);
	int maxVid = (srcVid == minVid ? destVid : srcVid);

	if ((volume[cluster[maxVid]] + this->degree[minVid]) <= maxVolume) {
		volume[cluster[maxVid]] = volume[cluster[maxVid]] + this->degree[minVid];
		volume[cluster[minVid]] = volume[cluster[minVid]] - this->degree[minVid];
		if (volume[cluster[minVid]] == 0)
			volume.erase(cluster[minVid]);
		cluster[minVid] = cluster[maxVid];
	}
}

void StreamCluster::startSteamCluster(GlobalConfig& config) {
	double averageDegree = 0.0;
	double numsEdge = 0;
	double numsNode = 0;
	int clusterID = 1;

	graph->readGraphFromFile(config);
	Edge edge;
	//First Time
	//while((edge = graph.readStep()) && edge.isNull() == false) {
	while(true){
		edge = graph->readStep();
		if(edge.isNull())
			break;
		numsEdge += 2;
		int src = edge.getSrcVId();
		int dest = edge.getDestVId();
		// allocate cluster
		if (cluster[src] == 0) {
			cluster[src] = clusterID++;
			numsNode += 1;
		}
		if (cluster[dest] == 0) {
			cluster[dest] = clusterID++;
			numsNode += 1;
		}
		averageDegree = 2.0 * numsEdge / numsNode;
		if(averageDegree < 0)
			std::cout << "Error!!! In StreamCluster average degree < 0" << std::endl;
		this->degree[src]++;
		this->degree[dest]++;

		// update volume
		if (volume.find(cluster[src]) == volume.end()) {
			volume[cluster[src]] = 0;
		}
		if (volume.find(cluster[dest]) == volume.end()) {
			volume[cluster[dest]] = 0;
		}
		volume[cluster[src]] = volume[cluster[src]] + 1;
		volume[cluster[dest]] = volume[cluster[dest]] + 1;

		if(volume[cluster[src]] >= maxVolume)
		{
			volume[cluster[src]] = volume[cluster[src]] - this->degree[src];
			cluster[src] = clusterID++;
			volume[cluster[src]] = this->degree[src];
		}

		if(volume[cluster[dest]] >= maxVolume)
		{
			volume[cluster[dest]] = volume[cluster[dest]] - this->degree[dest];
			cluster[dest] = clusterID++;
			volume[cluster[dest]] = this->degree[dest];
		}

		// combine cluster
		combineCluster(src, dest);
	}

	setUpIndex();
	computeEdgeInfo(config);
}

void StreamCluster::setUpIndex(){
	// 首先，我们需要从unordered_map中提取键值对，并将它们放入一个vector中进行排序。
	// 因为std::unordered_map没有直接的排序功能，我们需要将元素转移到一个vector中。
	std::vector<std::pair<int, int>> sortList;
	for (const auto& pair : volume) {
		sortList.push_back(pair);
	}

	// 使用lambda表达式对vector进行排序，基于值(value)进行排序。
	std::sort(sortList.begin(), sortList.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
		return a.second < b.second; // 根据需求选择适当的比较逻辑
	});

	// 遍历排序后的列表，添加非零条目到clusterList
	for (size_t i = 0;i < sortList.size();i++) {
		std::pair<int, int> p = sortList[i];
		if (p.second == 0) continue;
		this->clusterList.push_back(p.first);
	}

	// 清除volume
	volume.clear();
}

void StreamCluster::computeEdgeInfo(GlobalConfig& config) {
	// compute inner and cut edge
	// Second Time
	graph->readGraphFromFile(config);
	//std::unordered_map<int, std::unordered_set<int>> replicateTable;
	Edge edge;
	double sum = 0.0;
	//while((edge = graph.readStep()) && edge.isNull() == false){
	while(1){
		edge = graph->readStep();
		if(edge.isNull())
			break;
		int src = edge.getSrcVId();
		int dest = edge.getDestVId();
		if(cluster[src] != cluster[dest]) sum++;
		if (innerAndCutEdge.find(cluster[src]) == innerAndCutEdge.end())
			innerAndCutEdge.insert({cluster[src], std::unordered_map<int,int>()});
		if (innerAndCutEdge[cluster[src]].find(cluster[dest]) == innerAndCutEdge[cluster[src]].end())
			innerAndCutEdge[cluster[src]][cluster[dest]] = 0;
		int oldValue = innerAndCutEdge[cluster[src]][cluster[dest]];
		innerAndCutEdge[cluster[src]][cluster[dest]] = oldValue + edge.getWeight();
	}

	std::cout << "Cluster rep: " << (sum + config.getVCount()) / config.getVCount() << std::endl;
}

void StreamCluster::PrintInformation(GlobalConfig& config) {
	// 由于C++没有直接等价于Java的System.out.println，这里需要使用iostream库
	for (size_t i = 0; i < config.getVCount(); ++i) {
		std::cout << "vertex: " << i << ", clusterID: " << cluster[i] << std::endl;
	}
	for (size_t i = 0; i < clusterList.size(); i++) {
		std::cout << "clusterList: " << i << ": " << clusterList[i] << std::endl;
	}

	// 遍历innerAndCutEdge打印信息，类似于Java中的forEach
	for (const auto& key1_pair : innerAndCutEdge) {
		std::cout << key1_pair.first << " ";
		for (const auto& key2_pair : key1_pair.second) {
			std::cout << key2_pair.first << " " << key2_pair.second << " ";
		}
		std::cout << std::endl;
	}
}

std::vector<int> StreamCluster::getClusterList() const {
	return clusterList;
}

std::vector<int> StreamCluster::getCluster() const {
	return cluster;
}

std::unordered_map<int, std::unordered_map<int, int>> StreamCluster::getInnerAndCutEdge() const {
	return innerAndCutEdge;
}

int StreamCluster::getEdgeNum(int cluster1, int cluster2) const {
	if(this->innerAndCutEdge.find(cluster1) ==  this->innerAndCutEdge.end()
		|| this->innerAndCutEdge.find(cluster1)->second.find(cluster2) == this->innerAndCutEdge.find(cluster1)->second.end())
		return 0;
	return this->innerAndCutEdge.at(cluster1).at(cluster2);
}

std::string StreamCluster::toString() const{
	std::stringstream volumeStr;
	std::stringstream clusterStr;
	for(auto & kv: volume){
		volumeStr << "cluster " << kv.first << " volume: " << kv.second << std::endl;
	}
	for (int i = 0; i < graph->getVCount(); i++) {
        clusterStr << "vid : " << i << " cluster: " << cluster[i] << std::endl;
    }
	std::string ret = volumeStr.str() + clusterStr.str();
	return ret;
}

int StreamCluster::getClusterId(int vId) const{
	return cluster[vId];
}

std::unordered_map<int, int> StreamCluster::getVolume() const{
	return volume;
}

void StreamCluster::changeCluster(const std::vector<int>& cluster) {
	this->cluster.resize(cluster.size());
	std::copy(cluster.begin(),cluster.end(),this->cluster.begin());
}

void StreamCluster::changeClusterList(const std::vector<int>& clusterList) {
	this->clusterList.resize(clusterList.size());
	std::copy(clusterList.begin(),clusterList.end(),this->clusterList.begin());
}

void StreamCluster::reprocess(GlobalConfig& config) {
	computeEdgeInfo(config);
}
