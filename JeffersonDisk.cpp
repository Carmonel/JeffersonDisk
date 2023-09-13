#include "JeffersonDisk.h"
#include <ctime>
#include <iostream>
#include <fstream>

JeffersonDisk::JeffersonDisk(int diskCount, int shiftCount){
    this->size = diskCount;
    this->shift = shiftCount;

    diskArray = new unsigned char*[diskCount];
    for (int i = 0; i < 26; i++){
        diskArray[i] = new unsigned char[256];
    }

    newRandomDisks();
}

void JeffersonDisk::newRandomDisks(){
    srand(time(nullptr));

    for (int i = 0; i < size; i++){
        // Checker for used numbers
        bool usedNumbers[256];
        for (int i = 0; i < 256; i++) {
            usedNumbers[i] = false;
        }

        for (int j = 0; j < 256; j++){
            // Finding not used number
            int newNumber = (unsigned char)(rand() % 256);
            while (usedNumbers[newNumber]){
                newNumber = (unsigned char)(rand() % 256);
            }
            usedNumbers[newNumber] = true;

            diskArray[i][j] = newNumber;
        }
    }
}

JeffersonDisk::~JeffersonDisk(){
    for (int i = 0; i < size; i++){
        delete[] diskArray[i];
    }
    delete[] diskArray;
}

void JeffersonDisk::changeShift(int newShift) {
    if ((newShift % 26 == 0) || (newShift < 0)){
        std::cout << "The shift cannot be a multiple of 26, be less than 0, or be 0." << std::endl;
        return;
    }

    shift = newShift;
}

void JeffersonDisk::printDisks() {
    std::cout << "Disks:" << std::endl;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < 256; j++){
            if (diskArray[i][j] < 0x10){
                std::cout << "0";
            }
            std::cout << std::uppercase << std::hex << (int)diskArray[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void JeffersonDisk::encodeFile(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream inputFile;
    try{
        inputFile.open(inputPath, std::ios::binary | std::ios::in);
    }
    catch (std::exception& e){
        std::cout << "Input file error: " << e.what() << std::endl;
    }

    std::ofstream outputFile;
    try{
        outputFile.open(outputPath, std::ios::binary | std::ios::out);
    }
    catch (std::exception& e){
        std::cout << "Output file error: " << e.what() << std::endl;
    }

    if (!inputFile.is_open()){
        std::cout << "Input or output file have not been opened." << std::endl;
        exit(-1);
    }
    if (!outputFile.is_open()){
        std::cout << "Output file have not been opened." << std::endl;
        exit(-1);
    }
    
    ///
    /// Writing keys to file
    ///
    std::cout << "Writing keys to file..." << std::endl;
    outputFile.write(reinterpret_cast<const char *>(&shift), 1);
    outputFile.write(reinterpret_cast<const char *>(&size), 1);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < 256; j++){
            outputFile.write(reinterpret_cast<const char *>(&diskArray[i][j]), 1);
        }
    }


    ///
    /// Writing data to file
    ///
    std::cout << "Writing data to file..." << std::endl;
    // Reading next binary hex
    int diskNumber = 0;
    char readHex;
    while (inputFile.read((char*)(&readHex), 1)){
        // Disk number loop: 0 --> size-1
        if (diskNumber == size) diskNumber = 0;
        std::cout << std::hex << std::uppercase << "Read " << (int)readHex << "['" << readHex <<"']" << " --> ";

        // Finding this hex in current disk
        int readHexPosition = 0;
        while (diskArray[diskNumber][readHexPosition] != readHex) readHexPosition++;

        // Writing shifted hex by current disk
        std::cout << std::hex << std::uppercase << (int)diskArray[diskNumber][(readHexPosition + shift) % 256] << std::endl;
        outputFile.write(reinterpret_cast<const char *>(&diskArray[diskNumber][(readHexPosition + shift) % 256]), 1);
        diskNumber++;
    }

    inputFile.close();
    outputFile.close();
}

void JeffersonDisk::decodeFile(const std::string &inputPath, const std::string &outputPath) {
    std::ifstream inputFile;
    try{
        inputFile.open(inputPath, std::ios::binary | std::ios::in);
    }
    catch (std::exception& e){
        std::cout << "Input file error: " << e.what() << std::endl;
    }

    std::ofstream outputFile;
    try{
        outputFile.open(outputPath, std::ios::binary | std::ios::out);
    }
    catch (std::exception& e){
        std::cout << "Output file error: " << e.what() << std::endl;
    }

    if (!inputFile.is_open()){
        std::cout << "Input or output file have not been opened." << std::endl;
        exit(-1);
    }
    if (!outputFile.is_open()){
        std::cout << "Output file have not been opened." << std::endl;
        exit(-1);
    }

    // Reading size and shift
    char readHex;
    inputFile.read((char*)(&readHex), 1);
    shift = readHex;
    inputFile.read((char*)(&readHex), 1);
    size = readHex;

    std::cout << "Read shift:" << " " << shift << ", size: " << size << std::endl;

    ///
    /// Reading keys from file
    ///
    diskArray = new unsigned char*[size];
    for (int i = 0; i < size; i++){
        diskArray[i] = new unsigned char[256];

        for (int j = 0; j < 256; j++){
            inputFile.read((char*)(&readHex), 1);
            diskArray[i][j] = readHex;
        }
    }
    printDisks();

    ///
    /// Reading + writing data to file
    ///
    int diskNumber = 0;
    while (inputFile.read((char*)(&readHex), 1)){
        // Disk number loop: 0 --> size-1
        if (diskNumber == size) diskNumber = 0;

        // Finding this hex in current disk
        int readHexPosition = 0;
        while (diskArray[diskNumber][readHexPosition] != readHex) readHexPosition++;

        // Writing shifted hex by current disk
        int writeHexPosition = readHexPosition - shift;
        if (writeHexPosition < 0) writeHexPosition = writeHexPosition + 256;
        std::cout << std::hex << std::uppercase << (int)diskArray[diskNumber % size][writeHexPosition % 256] << std::endl;
        outputFile.write(reinterpret_cast<const char *>(&diskArray[diskNumber % size][writeHexPosition % 256]), 1);
        diskNumber++;
    }
}
