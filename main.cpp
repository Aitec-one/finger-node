#include <windows.h>
#include <iostream>

#include "FingerScanner.h"

int main() {
    std::cout << "Starting fingerprint demo..." << std::endl;
    FingerScanner scanner;
    if (scanner.connect() == 1) {
        std::cout << "Starting main loop..." << std::endl;
        scanner.mainLoop();
    }
    return 0;
}
