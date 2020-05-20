#ifndef ZKTECOTEST_FINGERSCANNER_H
#define ZKTECOTEST_FINGERSCANNER_H

#include <windows.h>

#include "libzkfperrdef.h"
#include "libzkfptype.h"
#include "libzkfp.h"

#define ENROLLCNT 3

class FingerScanner {
public:
    int connect();

    void mainLoop();

private:
    HANDLE device;
    unsigned char *imgBuf;
    int width = 0;
    int height = 0;
    int userId = 100;
    bool stop = false;
    int opType = 0;

    HANDLE dbCache;
    unsigned char lastRegTemplate[MAX_TEMPLATE_SIZE];
    unsigned int lastRegTempLen = 0;

    int enrollIdx = 0;
    unsigned char enrollTries[ENROLLCNT][MAX_TEMPLATE_SIZE];
    unsigned int enrollTriesLen[ENROLLCNT];

    int enroll(unsigned char *temp, unsigned int len);

    int verify(unsigned char *temp, unsigned int len);

    int match(unsigned char *temp, unsigned int len);
};


#endif //ZKTECOTEST_FINGERSCANNER_H
