#include <iostream>
#include <uPRNG.h>                  // access: set_seed
#include <fstream>
#include <cstdlib>			        // access: rand, srand
#include <cstring>					// access: strcmp

#include "config.h"
#include "printer.h"
using namespace std;

int main(int argc, char* argv[]) {
    int seed = -1, processors = 1;
    string configFile = "soda.config";

    struct cmd_error {};
    // command input
    try {
        switch (argc) {	// don't break. allow fall through
        case 4: if (strcmp(argv[3], "d") != 0) {    // processors
            processors = convert(argv[1]); if (processors <= 0) throw cmd_error();
        }
        case 3: if (strcmp(argv[2], "d") != 0) {    // seed
            seed = convert(argv[2]); if (seed <= 0) throw cmd_error();
        }
        case 2: if (strcmp(argv[1], "d") != 0) {    // config
            configFile = argv[3];
        }
        case 1: {
            break;
        }
        default: { throw cmd_error(); }
        } // switch
    }     // try
    catch (...) {
        cerr << "Usage: " << argv[0] << "  [ config-file | 'd' [ seed ( > 0) | 'd' [ processors (> 0) | 'd'] ] ]" << endl;
        exit(EXIT_FAILURE);
    }   // catch

    uProcessor p[processors - 1] __attribute__((unused)); // create more kernel thread
    if (seed != -1) {
        set_seed(seed);
    }

    // set params
    ConfigParms configParms;  // use method to load these
    processConfigFile(configFile.c_str(), configParms);

    // define each entity:

}
