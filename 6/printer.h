#ifndef printer_h
#define printer_h

_Monitor Printer{
  public:
    // kind enum 
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers);
    ~Printer();
    void print(Kind kind, char state);
    void print(Kind kind, char state, unsigned int value1);
    void print(Kind kind, char state, unsigned int value1, unsigned int value2);
    void print(Kind kind, unsigned int lid, char state);
    void print(Kind kind, unsigned int lid, char state, unsigned int value1);
    void print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2);

    void flush();    // flush the buffered output

  private:
    unsigned int numStudents, numVendingMachines, numCouriers;
    unsigned int numColumns;  // total

    // printer internal buffer data for one voter
    struct ColumnData {
        bool isWrittenTo = false;     // flip when written to to prevent being overwritten

        char state;                   // current state as one unique letter
        unsigned int value1, value2;  // max two numbers to print
        Kind kind;                    // easy to use when printing
    };
    ColumnData* columnDataArr;        // keep track of each column's current info

    // helper functions
    unsigned int getDataArrIndex(Kind kind);
    unsigned int getDataArrIndex(Kind kind, unsigned int lid);
    void printBasic(Kind kind, char state, unsigned int index);
};


#endif      // ifndef