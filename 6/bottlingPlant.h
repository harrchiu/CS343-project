#ifndef _bottling_plant_h_
#define _bottling_plant_h_

#include "printer.h"
#include "nameserver.h"
#include "truck.h"

_Task BottlingPlant {
        Printer & printer;
        NameServer& nameServer;
        unsigned int numVendingMachines;
        unsigned int maxShippedPerFlavour;
        unsigned int maxStockPerFlavour;
        unsigned int timeBetweenShipments;
        
        unsigned int productionStock[Flavours::NUM_OF_FLAVOURS]; // current production stock
        
        uCondition truckWaiting; // condition variable for truck to wait for shipment

        void main();
    public:
        enum Flavours { BluesBlackCherry, ClassicCreamSoda, RockRootBeer, JazzLime, NUM_OF_FLAVOURS }; // flavours of soda, NUM_OF_FLAVOURS is the number of flavours is the number of flavours (4)
        _Exception Shutdown {}; // shutdown plant
        BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                        unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                        unsigned int timeBetweenShipments );
        void getShipment( unsigned int cargo[] );
};

#endif // _botting_plant_h_