_Task VendingMachine{
	void main();
  public:
	_Exception Funds {};					// insufficient funds
	_Exception Stock {};					// flavour out of stock
	_Exception Free {};						// free, advertisement
	VendingMachine(Printer& prt, NameServer& nameServer, unsigned int id, unsigned int sodaCost);
	void buy(BottlingPlant::Flavours flavour, WATCard& card);
	unsigned int* inventory() __attribute__((warn_unused_result));
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

_Task NameServer{
	void main();
  public:
	NameServer(Printer& prt, unsigned int numVendingMachines, unsigned int numStudents);
	void VMregister(VendingMachine* vendingmachine);
	VendingMachine* getMachine(unsigned int id) __attribute__((warn_unused_result));
	VendingMachine** getMachineList() __attribute__((warn_unused_result));
};

_Task BottlingPlant{
	void main();
  public:
	enum Flavours { ..., NUM_OF_FLAVOURS };	// flavours of soda (YOU DEFINE)
	_Exception Shutdown {};					// shutdown plant
	BottlingPlant(Printer& prt, NameServer& nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments);
	void getShipment(unsigned int cargo[]);
};

_Task Truck{
	void main();
  public:
	Truck(Printer& prt, NameServer& nameServer, BottlingPlant& plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};
