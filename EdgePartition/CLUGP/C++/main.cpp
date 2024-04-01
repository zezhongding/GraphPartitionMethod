#include "Clugp.h"
using namespace std;

int main(int argc,char** argv) {
	if(argc != 2)
		exit(-1);
	GlobalConfig config(argv[1]);
	config.printParaInfo();
	omp_set_num_threads(config.getThreads());
	Clugp clugp(config);
	clugp.main(config);
	return 0;
}