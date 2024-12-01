
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
