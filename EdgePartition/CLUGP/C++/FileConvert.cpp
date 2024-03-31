#include "FileConvert.h"
//#include "globalConfig.h"

void convertFile(const std::string& inputFilePath, const std::string& outputFilePath) {
	std::unordered_map<int, int> ht;
	std::ifstream inputFile(inputFilePath);
	std::ofstream outputFile(outputFilePath);
	std::string line;
	int lineNum = 0;
	int id = 0;

	while (std::getline(inputFile, line)) {
		if (line[0] == '#') continue;
		std::istringstream iss(line);
		int srcVid, endVid;
		iss >> srcVid >> endVid;
		int newSrcVid, newEndVid;

		// Check for srcVid
		auto srcIt = ht.find(srcVid);
		if (srcIt != ht.end()) {
			newSrcVid = srcIt->second;
		} else {
			newSrcVid = id++;
			ht[srcVid] = newSrcVid;
		}

		// Check for endVid
		auto endIt = ht.find(endVid);
		if (endIt != ht.end()) {
			newEndVid = endIt->second;
		} else {
			newEndVid = id++;
			ht[endVid] = newEndVid;
		}
		ht.clear();
		if (lineNum != 0) {
			outputFile << "\n";
		}
		outputFile << newSrcVid << "\t" << newEndVid;
		lineNum++;
	}

	std::cout << "Number of Nodes: " << id << std::endl;
	std::cout << "Number of Edges: " << lineNum << std::endl;

	inputFile.close();
	outputFile.close();
}
