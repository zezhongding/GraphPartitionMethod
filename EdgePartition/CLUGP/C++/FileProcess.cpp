#include "FileProcess.h"

FileProcess::FileProcess(StreamCluster* streamCluster)
	: streamCluster(streamCluster), innerAndCutEdge(streamCluster->getInnerAndCutEdge()),
		clusterList(streamCluster->getClusterList()), cluster(streamCluster->getCluster()) {}

void FileProcess::savefile(GlobalConfig& config) {
	std::string path = config.getOutputGraphPath() + "streamCluster.txt";
	std::cout << path << std::endl;

	std::ofstream out(path);
	if (!out.is_open()) {
		// Handle file open error
		std::cerr << "open output file " << path << " failed." << std::endl;
		return;
	}

	for (int i = 0; i < cluster.size(); ++i) {
		out << cluster[i] << " ";
	}
	out << "\n";

	for (int i = 0; i < clusterList.size(); ++i) {
		out << clusterList[i] << " ";
	}
	out << "\n";

	for (const auto& pair1 : innerAndCutEdge) {
		out << pair1.first << " ";
		for (const auto& pair2 : pair1.second) {
			out << pair2.first << " " << pair2.second << " ";
		}
		out << "\n";
	}
	out.close();
}

void FileProcess::readfile(GlobalConfig& config) {
	std::string path = config.getOutputGraphPath() + "streamCluster.txt";
	std::ifstream in(path);
	if (!in.is_open()) {
		std::cerr << "open input file " << path << " failed." << std::endl;
		return;
	}

	std::string line;

	// Read first line for cluster
	if (getline(in, line)) {
		std::istringstream iss(line);
		int num;
		while (iss >> num) {
			cluster.push_back(num);
		}
	}

	// Read second line for clusterList
	if (getline(in, line)) {
		std::istringstream iss(line);
		int num;
		while (iss >> num) {
			clusterList.push_back(num);
		}
	}

	// Read remaining lines for innerAndCutEdge
	while (getline(in, line)) {
		std::istringstream iss(line);
		int key1;
		iss >> key1;
		std::unordered_map<int, int> temp;
		int key2, value;
		while (iss >> key2 >> value) {
			temp[key2] = value;
		}
		innerAndCutEdge[key1] = temp;
	}
	in.close();
}

StreamCluster* FileProcess::getstreamCluster() {
	this->streamCluster->changeCluster(this->cluster);
	this->streamCluster->changeClusterList(this->clusterList);
	return this->streamCluster;
}