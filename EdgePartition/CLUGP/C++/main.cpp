#include "Clugp.h"
using namespace std;

int main() {
	omp_set_num_threads(THREADNUM);
	GlobalConfig config("../project.properties");
	config.printParaInfo();
	Clugp clugp(config);
	clugp.main(config);
	return 0;
}