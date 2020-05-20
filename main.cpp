#include <windows.h>
#include <iostream>

#include "FingerScanner.h"

bool stop = false;
int opType = 0;

void mainLoop(FingerScanner* scanner) {
    while (!stop) {
        unsigned char szTemplate[MAX_TEMPLATE_SIZE];
        unsigned int tempLen = MAX_TEMPLATE_SIZE;
        int ret = scanner->acquireFingerprint(szTemplate, &tempLen);
        if (ret == ZKFP_ERR_OK) {
            if (opType == 0) {
                if (scanner->enroll(szTemplate, tempLen) == 1) {
                    std::cout << "Registered!" << std::endl;
                    std::string base64 = scanner->getLastRegistered();
                    std::cout << base64 << std::endl;
                    opType = 1;
                }

            } else if (opType == 1) {
                if (scanner->verify(szTemplate, tempLen) == 1) {
                    std::cout << "Verified!" << std::endl;
                } else {
                    std::cout << "Not verified!" << std::endl;
                }

            } else if (opType == 2) {
                scanner->match(szTemplate, tempLen);
            }
        }
        Sleep(10);
    }
}

int main() {
    std::cout << "Starting fingerprint demo..." << std::endl;
    FingerScanner scanner;

    if (scanner.connect()) {
        std::cout << "Starting main loop..." << std::endl;
        mainLoop(&scanner);
    }
    return 0;
}
