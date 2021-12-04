#include "config.h"

using namespace std;

int main(int argc, char* argv[]) {
    ConfigParms cparms;
    processConfigFile(argv[1], cparms);

    uProcessor p[processors - 1]; // number of kernel threads
    if(processors == 1) uThisProcessor().setPreemption(0); // turn off time-slicing for reproducibility
}