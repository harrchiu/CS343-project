#include "printer.h"

#include <iostream>
using namespace std;

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) :
    numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers),
    numColumns(6 + numStudents + numVendingMachines + numCouriers) {

    // line 1 headers
    cout << "Parent\t";
    cout << "Gropoff\t";
    cout << "WATOff\t";
    cout << "Names\t";
    cout << "Truck\t";
    cout << "Plant";
    for (unsigned int i = 0; i < numStudents; ++i) {
        cout << "\tStud" << i;
    }
    for (unsigned int i = 0; i < numVendingMachines; ++i) {
        cout << "\tMach" << i;
    }
    for (unsigned int i = 0; i < numCouriers; ++i) {
        cout << "\tCour" << i;
    }
    cout << endl;

    // line 2 separators
    cout << "*******";
    for (unsigned int i = 1; i < numColumns; i += 1) {
        cout << "\t" << "*******";
    }
    cout << endl;

    // allocate room for voter states
    // delete at end of life
    columnDataArr = new Printer::ColumnData[numColumns];
}

Printer::~Printer() {
    // destructor cleanup and final message
    flush();
    cout << "***********************" << endl;
    delete[] columnDataArr;
}

// helper to get id from kind
unsigned int Printer::getDataArrIndex(Kind kind) {
    switch (kind) {
    case Parent:
        return 0;
    case Groupoff:
        return 1;
    case WATCardOffice:
        return 2;
    case NameServer:
        return 3;
    case Truck:
        return 4;
    case BottlingPlant:
        return 5;
    default:
        // for Student, Vending, Courier
        cout << "PRINTER ERROR - Kind requires lid" << endl;
        throw;
    }
}
// overload for lid
unsigned int Printer::getDataArrIndex(Kind kind, unsigned int lid) {
    switch (kind) {
    case Student:
        return 6 + lid;
    case Vending:
        return 6 + numStudents + lid;
    case Courier:
        return 6 + numStudents + numVendingMachines + lid;
    default:
        // for those without lid
        cout << "PRINTER ERROR - Kind should not be using lid" << endl;
        throw;
    }
}

// helper function - set basic info given an index
void Printer::printBasic(Kind kind, char state, unsigned int index) {
    // first check if we're about to overwrite, data
    if (columnDataArr[index].isWrittenTo) {
        flush();        // if so, flush first, then update
    }
    columnDataArr[index].isWrittenTo = true; // flip writing state
    columnDataArr[index].state = state;
    columnDataArr[index].kind = kind;
}


// we can assume this is not an object that has an local identifier (e.g. Parent or Truck)
void Printer::print(Kind kind, char state) {
    // first figure out the index of columnDataArr
    unsigned int index = getDataArrIndex(kind);
    printBasic(kind, state, index);
}

void Printer::print(Kind kind, char state, unsigned int value1) {
    unsigned int index = getDataArrIndex(kind);
    printBasic(kind, state, index);
    columnDataArr[index].value1 = value1;
}

void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2) {
    unsigned int index = getDataArrIndex(kind);
    printBasic(kind, state, index);
    columnDataArr[index].value1 = value1;
    columnDataArr[index].value2 = value2;
}

// lid calls
void Printer::print(Kind kind, unsigned int lid, char state) {
    // first figure out the index of columnDataArr
    unsigned int index = getDataArrIndex(kind, lid);
    printBasic(kind, state, index);
}
void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1) {
    unsigned int index = getDataArrIndex(kind, lid);
    printBasic(kind, state, index);
    columnDataArr[index].value1 = value1;
}
void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2) {
    unsigned int index = getDataArrIndex(kind, lid);
    printBasic(kind, state, index);
    columnDataArr[index].value1 = value1;
    columnDataArr[index].value2 = value2;
}

void Printer::flush() {
    for (unsigned int i = 0; i < numColumns; i += 1) {
        if (i >= 1) {
            cout << "\t"; // tab space from left
        }

        Printer::ColumnData data = columnDataArr[i]; // current voter buffered data
        if (!data.isWrittenTo) {    // skip if not written to 
            continue;
        }

        // must have written state in all print calls, print the state
        cout << data.state; // has nothing after: S,
        switch (data.kind) {
        case Parent:
            if (data.state == 'D') cout << data.value1 << ',' << data.value2;
            else if (data.state == 'F') cout << data.value1;
            break;
        case Groupoff:
            if (data.state == 'D') cout << data.value1 << ',' << data.value2;
            break;
        case WATCardOffice:
            if (data.state == 'C' || data.state == 'T') cout << data.value1 << ',' << data.value2;
            break;
        case NameServer:
            if (data.state == 'R') cout << data.value1;
            else if (data.state == 'N') cout << data.value1 << ',' << data.value2;
            break;
        case Truck:
            if (data.state == 'd' || data.state == 'U' || data.state == 'D') cout << data.value1 << ',' << data.value2;
            else if (data.state == 'P') cout << data.value1;
            break;
        case BottlingPlant:
            if (data.state == 'G') cout << data.value1;
            break;
        case Student:
            if (data.state == 'S' || data.state == 'G' || data.state == 'B' || data.state == 'F') cout << data.value1 << ',' << data.value2;
            else if (data.state == 'V' || data.state == 'A') cout << data.value1;
            break;
        case Vending:
            if (data.state == 'B') cout << data.value1 << ',' << data.value2;
            else if (data.state == 'S') cout << data.value1;
            break;
        case Courier:
            if (data.state == 'T' || data.state == 't') cout << data.value1 << ',' << data.value2;
            else if (data.state == 'L') cout << data.value1;
            break;
        default: break;
        } // switch

        // reset for next
        columnDataArr[i].isWrittenTo = false;
    } // for

    // reset for next (just have to mark not dirty)
    cout << endl;
}