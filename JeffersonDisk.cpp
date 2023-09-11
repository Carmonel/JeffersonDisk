#include "JeffersonDisk.h"
#include <ctime>
#include <iostream>
#include <fstream>

std::string clearMessage(std::string message){
    for (int i = 0; i < message.length(); i++){
        // Change lowcase to uppercase
        if ((message[i] >= 97) && (message[i] <= 122)){
            message[i] = message[i] - 32;
            continue;
        }
        // Remove other symbols
        if ((message[i] >= 91) || (message[i] <= 64)){
            message.erase(i);
        }
    }

    return message;
}

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
            std::cout << std::uppercase << std::hex << diskArray[i][j] << " ";
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

    if ((!inputFile.is_open()) || (!outputFile.is_open())){
        std::cout << "Input or output file have not been opened." << std::endl;
    }

    char* ch;
    unsigned char negativeCh;
    int diskQueue = 0;
    // Reading next binary hex
    while (inputFile.read(ch, 1)){
        negativeCh = (unsigned char)*ch; // binary hex -> char

        // Finding this hex in current disk
        int chPos = 0;
        while (diskArray[diskQueue % size][chPos] != negativeCh){
            chPos++;
        }

        // Creating new hex
        char* writeCh = (char*)&diskArray[diskQueue % size][chPos + shift];
        outputFile.write(writeCh, 1); // Writing shifted hex by current disk
    }

    inputFile.close();
    outputFile.close();
}
