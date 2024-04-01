#include "OriginGraph.h"
#include "common.h"

OriginGraph::OriginGraph(GlobalConfig& config){
	vCount = config.getVCount();
	eCount = config.getECount();
}

void OriginGraph::closef() {
	if(bufferedReader.is_open())
		bufferedReader.close();
}

void OriginGraph::readGraphFromFile(GlobalConfig& config) {
	std::string path = config.getInputGraphPath();
	// 清除之前的异常标记
	bufferedReader.clear();
	// 设置异常标志，指示当failbit或badbit被设置时抛出异常
	bufferedReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		bufferedReader.open(path.c_str(),std::ios::in|std::ios::binary);
		if(!bufferedReader.is_open())
			std::cerr << "open error in try " << path << std::endl;
		// 如果需要，这里可以进行其他文件操作
	} catch (const std::ifstream::failure& e) {
		// 处理打开文件失败的情况
		std::cerr << "Exception opening/reading file: " << e.what() << "\t" << std::strerror(errno) << '\n';
		std::cerr << "Path : " << path << std::endl;
		// 在这里你可以决定如何处理异常，比如重新抛出、记录日志或其他错误处理逻辑
	}
}

bool OriginGraph::iseof() const {
	return bufferedReader.eof();
}

Edge OriginGraph::readStep() {
	std::string line;
	if(!bufferedReader.is_open()){
		std::cerr << "open error" << std::endl;
		return Edge();
	}
	while (!bufferedReader.eof()) {
		try {
			std::getline(bufferedReader, line);
			if (line[0] == '#')
				continue;
			if (line.empty())
				return Edge();
			std::istringstream iss(line);
			int srcVid, destVid;
			try{
				iss >> srcVid >> destVid;
			} catch (const std::istringstream::failure& e){
				std::cerr << "wrong format" << std::endl;
				continue;
			}
			if (srcVid == destVid)
				continue;
			return Edge(srcVid, destVid, 1);
		} catch (const std::ifstream::failure& e) {
			if(bufferedReader.eof())
			{
				bufferedReader.clear();
				//std::cout << "read all edges" << std::endl;
				break;
			}
			else
				std::cerr << "Exception opening/reading file: " << e.what() << "\t" << std::strerror(errno) << '\n';
			break;
		}
		// } catch (const std::ios_base::failure& e) {
		// 	std::cerr << "Exception ios_base " << e.what() << "\t" << std::strerror(errno) << '\n';
		// 	break;
		// }
	}
	// 在C++中，我们需要考虑如何处理这种情况，因为我们不能返回null。
	// 一种可能的方式是返回一个标记值或抛出一个异常。
	// 这里我们选择返回一个默认构造的Edge对象。
	return Edge();
}

void OriginGraph::addEdge(int srcVId, int destVId) {
	edgeList.push_back(Edge(srcVId, destVId, 1));
}

std::vector<Edge> OriginGraph::getEdgeList() const {
	return edgeList;
}

int OriginGraph::getVCount() const {
	return vCount;
}

int OriginGraph::getECount() const {
	return eCount;
}

void OriginGraph::clear() {
	edgeList.clear();
}