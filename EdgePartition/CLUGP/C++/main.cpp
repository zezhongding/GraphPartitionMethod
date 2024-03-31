#include "Clugp.h"
using namespace std;

int main() {
	GlobalConfig config("../project.properties");
	config.printParaInfo();
	omp_set_num_threads(config.getThreads());
	Clugp clugp(config);
	clugp.main(config);
	return 0;
}