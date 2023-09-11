#ifndef JEFFERSONDISK_JEFFERSONDISK_H
#define JEFFERSONDISK_JEFFERSONDISK_H

#include "string"

class JeffersonDisk {
private:
    // Disks count
    int size;
    // Shift count
    int shift;
    // Disks array
    int** diskArray;
public:
    JeffersonDisk(int diskCount, int shiftCount); // Creating obj + first filling letters
    void newRandomDisks(); // Fill diskArray new compilation of letters
    void changeShift(int newShift); // Replace shift number
    std::string encodeMessage(std::string message); // Encode message and return encoded string
    ~JeffersonDisk(); // Clearing data
};

#endif //JEFFERSONDISK_JEFFERSONDISK_H
