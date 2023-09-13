#ifndef JEFFERSONDISK_JEFFERSONDISK_H
#define JEFFERSONDISK_JEFFERSONDISK_H

#include <string>

class JeffersonDisk {
private:
    // Disks count
    int size;
    // Shift count
    int shift;
    // Disks array
    unsigned char** diskArray;
public:
    JeffersonDisk(int diskCount, int shiftCount); // Creating obj + first filling letters
    void newRandomDisks(); // Fill diskArray new compilation of letters
    void changeShift(int newShift); // Replace shift number
    void encodeFile(const std::string& inputPath, const std::string& outputPath); // Encode message and return encoded string
    void decodeFile(const std::string& inputPath, const std::string& outputPath);
    void printDisks();
    ~JeffersonDisk(); // Clearing data
};

#endif //JEFFERSONDISK_JEFFERSONDISK_H
