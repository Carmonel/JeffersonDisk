#include <iostream>
#include "JeffersonDisk.h"
#include "string"

int main() {
    auto* disk = new JeffersonDisk(6, 4);
    disk->printDisks();
}