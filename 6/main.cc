#include <iostream>
#include <uPRNG.h>                  // access: set_seed
#include <fstream>
#include <cstdlib>			        // access: rand, srand
#include <cstring>					// access: strcmp

#include "config.h"
#include "printer.h"
#include "WATcardoffice.h"
#include "bank.h"
#include "groupoff.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "parent.h"
#include "student.h"
#include "bottlingPlant.h"
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

    {
        // set params
        ConfigParms config;  // use method to load these
        processConfigFile(configFile.c_str(), config);

        // define each entity on stack
        // Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
        // Bank bank(config.numStudents);
        // WATCardOffice cardOffice(printer, bank, config.numCouriers);
        // Groupoff groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);
        // Parent parent(printer, bank, config.numStudents, config.parentalDelay);
        // NameServer nameServer(printer, config.numVendingMachines, config.numStudents);
        VendingMachine* vendingMachines[config.numVendingMachines];
        Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
        Bank bank(config.numStudents);

        WATCardOffice* cardOffice = new WATCardOffice(printer, bank, config.numCouriers);
        Groupoff* groupoff = new Groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);
        Parent* parent = new Parent(printer, bank, config.numStudents, config.parentalDelay);
        NameServer* nameServer = new NameServer(printer, config.numVendingMachines, config.numStudents);

        // construct VMs
        for (unsigned int i = 0; i < config.numVendingMachines; i++) {
            vendingMachines[i] = new VendingMachine(printer, *nameServer, i, config.sodaCost);
        }

        // plant needs to be deleted after students and before VMs to finish final deliveries to prevent deadlock
        BottlingPlant* bottlingPlant = new BottlingPlant(printer, *nameServer, config.numVendingMachines,
            config.maxShippedPerFlavour, config.maxStockPerFlavour,
            config.timeBetweenShipments);

        Student* students[config.numStudents];
        for (unsigned int i = 0; i < config.numStudents; i++) {
            students[i] = new Student(printer, *nameServer, *cardOffice, *groupoff, i, config.maxPurchases);  // will start in scope interacting with others
            // students[i] = new Student(printer, nameServer, cardOffice, groupoff, i, config.maxPurchases);  // will start in scope interacting with others
        }
        
        for (unsigned int i = 0; i < config.numStudents; i++) {    // students done
            delete students[i];
        }
        delete bottlingPlant;
        // clean up VMs (only vm array deleted in nameServer, not the VMs)
        for (unsigned int i = 0; i < config.numVendingMachines; ++i) {
            delete vendingMachines[i];
        }

        std::cout << "we are leaving" << std::endl;
        delete parent;
        delete cardOffice;
        delete groupoff;
        delete nameServer;
    }
}
