#ifndef _truck_h_
#define _truck_h_


#include "nameServer.h"
#include "bottlingPlant.h"

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;    // fwd ref

_Task Truck{
        void main();
        Printer& printer;
        NameServer& nameServer;
        BottlingPlant& plant;
        unsigned int numVendingMachines;
        unsigned int maxStockPerFlavour;
        unsigned int* cargo;
    public:
        Truck(Printer& prt, NameServer& nameServer, BottlingPlant& plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
        ~Truck();
};

#endif // _truck_h_