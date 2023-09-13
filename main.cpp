#include <iostream>
#include "JeffersonDisk.h"
#include "string"

int main() {
    auto* disk = new JeffersonDisk(2, 1);
    disk->printDisks();
    disk->encodeFile("C:\\Users\\Ярослав\\Documents\\in", "C:\\Users\\Ярослав\\Documents\\out");

    std::cout << std::endl << std::endl << std::endl;
    disk->decodeFile("C:\\Users\\Ярослав\\Documents\\out", "C:\\Users\\Ярослав\\Documents\\res");
}