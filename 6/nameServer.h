#ifndef _name_server_h_
#define _name_server_h_

_Monitor Printer;
_Task VendingMachine;   // forward ref

_Task NameServer{
    Printer & printer;
    unsigned int numVendingMachines, numStudents;

    unsigned int* modulos;  // keep track of mods for each student to visit every machine

    // list of VMs, in order of registration
    // we should be able to answer student demands by just getting the sid^th VM from here
    // since VM1 goes to the 1st student, etc.
    VendingMachine** vendingMachines;
    unsigned int numRegisteredMachines = 0;


    void main();
  public:
    NameServer(Printer& prt, unsigned int numVendingMachines, unsigned int numStudents);
    ~NameServer();
    void VMregister(VendingMachine* vendingmachine);

    // does the lookup on student id and machine
    // also updates modulos for the next time that student requests a machine
    VendingMachine* getMachine(unsigned int id) __attribute__((warn_unused_result));
    VendingMachine** getMachineList() __attribute__((warn_unused_result));
};


#endif // ifndef