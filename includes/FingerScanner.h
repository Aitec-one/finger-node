#ifndef ZKTECOTEST_FINGERSCANNER_H
#define ZKTECOTEST_FINGERSCANNER_H

#if defined(_WIN32)
#include <windows.h>
#endif

#include <iostream>

#include "zkinterface.h"
#include "libzkfperrdef.h"
#include "libzkfptype.h"
#include "libzkfp.h"

#define ENROLLCNT 3



class FingerScanner {
public:
    bool connect();

    bool import(const std::string &base64Encoded);

    int acquireFingerprint(unsigned char *szTemplate, unsigned int *tempLen);

    bool enroll(unsigned char *temp, unsigned int len);

    bool verify(unsigned char *temp, unsigned int len);

    bool match(unsigned char *temp, unsigned int len,
               unsigned char *expected, unsigned int expLen);

    std::string getLastRegistered();

private:
    HANDLE device;
    unsigned char *imgBuf;
    int width = 0;
    int height = 0;

    int userId = 100;

    HANDLE dbCache;
    unsigned char lastRegTemplate[MAX_TEMPLATE_SIZE];
    unsigned int lastRegTempLen = 0;

    int enrollIdx = 0;
    unsigned char enrollTries[ENROLLCNT][MAX_TEMPLATE_SIZE];
    unsigned int enrollTriesLen[ENROLLCNT];
};


#endif //ZKTECOTEST_FINGERSCANNER_H
