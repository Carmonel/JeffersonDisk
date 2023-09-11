#include "JeffersonDisk.h"
#include <ctime>
#include <iostream>

JeffersonDisk::JeffersonDisk(int diskCount, int shiftCount){
    size = diskCount;
    shift = shiftCount;

    diskArray = new int*[diskCount];
    for (int i = 0; i < 26; i++){
        diskArray[i] = new int[26];
    }

    newRandomDisks();
}

void JeffersonDisk::newRandomDisks(){
    srand(time(nullptr));

    for (int i = 0; i < size; i++){
        int alphabet[26] ={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
        for (int j = 0; j < 26; j++){
            int newChar = (int)(rand() % 26);

            // Finding not used letter
            while (alphabet[newChar] != 0){
                newChar = (int)(rand() % 26);
            }

            diskArray[i][j] = alphabet[newChar];
            alphabet[newChar] = 0;
        }
    }
}

JeffersonDisk::~JeffersonDisk(){
    for (int i = 0; i < size; i++){
        delete[] diskArray[i];
    }
    delete[] diskArray;
}

std::string JeffersonDisk::encodeMessage(std::string message) {
}

void JeffersonDisk::changeShift(int newShift) {
    if ((newShift % 26 == 0) || (newShift < 0)){
        std::cout << "The shift cannot be a multiple of 26, be less than 0, or be 0." << std::endl;
        return;
    }

    shift = newShift;
}
