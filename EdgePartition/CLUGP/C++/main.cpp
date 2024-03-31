#include "Clugp.h"

using namespace std;
std::string inputGraphPath = "/raid/bear/tmp/com_or.bin";

int main() {
	omp_set_num_threads(THREADNUM);
	GlobalConfig config("./project.properties");
	config.printParaInfo();
	
	// auto start = std::chrono::high_resolution_clock::now();
	// int threads = 4;
	// std::vector<std::thread> threadPool;
	// std::vector<std::future<void>> futureList;

	return 0;
}