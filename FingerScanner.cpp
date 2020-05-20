//
// Created by Vladislav on 5/20/2020.
//

#include "FingerScanner.h"
#include <iostream>

int FingerScanner::connect() {
    if (ZKFPM_Init() != ZKFP_ERR_OK) {
        std::cout << "Init ZKFPM fail" << std::endl;
        return 0;
    }
    std::cout << "Init done" << std::endl;

    if ((device = ZKFPM_OpenDevice(0)) == NULL) {
        std::cout << "Open sensor fail" << std::endl;
        ZKFPM_Terminate();
        return 0;
    }
    std::cout << "Device is open" << std::endl;

    dbCache = ZKFPM_DBInit();
    if (dbCache == NULL) {
        std::cout << "Create DBCache fail" << std::endl;
        ZKFPM_CloseDevice(device);
        ZKFPM_Terminate();
        return 0;
    }
    std::cout << "DB loaded" << std::endl;

    unsigned int size = 4;
    ZKFPM_GetParameters(device, 1, (unsigned char *) &width, &size);
    size = 4;
    ZKFPM_GetParameters(device, 2, (unsigned char *) &height, &size);
    imgBuf = new unsigned char[width * height];
    std::cout << "width: " << width << ", height: " << height << std::endl;
    return 1;
}

int FingerScanner::enroll(unsigned char *temp, unsigned int len) {
    if (enrollIdx > 0) {
        if (0 >= ZKFPM_DBMatch(dbCache,
                               enrollTries[enrollIdx - 1],
                               enrollTriesLen[enrollIdx - 1], temp,
                               len)) {
            enrollIdx = 0;
            std::cout << "Please press the same finger while registering!" << std::endl;
            return 0;
        }
    }
    enrollTriesLen[enrollIdx] = len;
    memcpy(enrollTries[enrollIdx], temp, len);
    enrollIdx++;
    if (enrollIdx >= ENROLLCNT) {

        unsigned char szRegTemp[MAX_TEMPLATE_SIZE] = {0x0};
        unsigned int cbRegTemp = MAX_TEMPLATE_SIZE;
        int ret = ZKFPM_DBMerge(dbCache,
                                enrollTries[0],
                                enrollTries[1],
                                enrollTries[2],
                                szRegTemp, &cbRegTemp);
        enrollIdx = 0;
        if (ret == ZKFP_ERR_OK) {
            ret = ZKFPM_DBAdd(dbCache, userId++, szRegTemp, cbRegTemp);
            if (ret == ZKFP_ERR_OK) {
                memcpy(lastRegTemplate, szRegTemp, cbRegTemp);
                lastRegTempLen = cbRegTemp;
                std::cout << "Register succ" << std::endl;
            } else {
                std::cout << "Register fail, because add to db fail, ret=" << ret << std::endl ;
                return 0;
            }
        }

        enrollIdx = 0;

        return 1;
    } else {
        printf("Pleas put a finger %d more times\n", ENROLLCNT - enrollIdx);
    }
    return 0;
}

int FingerScanner::verify(unsigned char *temp, unsigned int len) {
    unsigned int tid = 0;
    unsigned int score = 0;
    int ret = ZKFPM_DBIdentify(dbCache, temp, len, &tid, &score);
    if (ret != ZKFP_ERR_OK) {
        return 0;
    }

    printf("Verified: %d, %d\n", tid, score);
    return 1;
}

int FingerScanner::match(unsigned char *temp, unsigned int len) {
    int ret = ZKFPM_DBMatch(dbCache, lastRegTemplate, lastRegTempLen, temp, len);
    if (ret <= 0) {
        return 0;
    }
    printf("Match\n");
    return 1;
}

void FingerScanner::mainLoop() {
    while (!stop) {
        unsigned char szTemplate[MAX_TEMPLATE_SIZE];
        unsigned int tempLen = MAX_TEMPLATE_SIZE;
        int ret = ZKFPM_AcquireFingerprint(device, imgBuf,
                                           width * height,
                                           szTemplate, &tempLen);
        if (ret == ZKFP_ERR_OK) {
            if (opType == 0) {
                if (enroll(szTemplate, tempLen) == 1) {
                    printf("Registered!\n");
                    opType = 1;
                }

            } else if (opType == 1) {
                if (verify(szTemplate, tempLen) == 1) {
                    printf("Verified!\n");
                } else {
                    printf("Not verified!\n");
                }

            } else if (opType == 2) {
                match(szTemplate, tempLen);
            }
        }
        Sleep(10);
    }
}