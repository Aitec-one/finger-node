//
// Created by Andrey Koltochnik on 5/20/2020.
//

#include "FingerScanner.h"
#include "base64.h"

bool FingerScanner::connect() {
    if (ZKFPM_Init() != ZKFP_ERR_OK) {
        std::cout << "Init ZKFPM fail" << std::endl;
        return false;
    }
    std::cout << "Init done" << std::endl;

    if ((device = ZKFPM_OpenDevice(0)) == NULL) {
        std::cout << "Open sensor fail" << std::endl;
        ZKFPM_Terminate();
        return false;
    }
    std::cout << "Device is open" << std::endl;

    dbCache = ZKFPM_DBInit();
    if (dbCache == NULL) {
        std::cout << "Create DBCache fail" << std::endl;
        ZKFPM_CloseDevice(device);
        ZKFPM_Terminate();
        return false;
    }
    std::cout << "DB loaded" << std::endl;

    unsigned int size = 4;
    ZKFPM_GetParameters(device, 1, (unsigned char *) &width, &size);
    size = 4;
    ZKFPM_GetParameters(device, 2, (unsigned char *) &height, &size);
    imgBuf = new unsigned char[width * height];
    std::cout << "width: " << width << ", height: " << height << std::endl;
    return true;
}

bool FingerScanner::enroll(unsigned char *temp, unsigned int len) {
    if (enrollIdx > 0) {
        if (0 >= ZKFPM_DBMatch(dbCache,
                               enrollTries[enrollIdx - 1],
                               enrollTriesLen[enrollIdx - 1], temp,
                               len)) {
            enrollIdx = 0;
            std::cout << "Please press the same finger while registering!" << std::endl;
            return false;
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
                std::cout << "Register fail, because add to db fail, ret=" << ret << std::endl;
                return false;
            }
        }

        enrollIdx = 0;

        return true;
    } else {
        std::cout << "Pleas put a finger " << ENROLLCNT - enrollIdx << " more times" << std::endl;
    }
    return false;
}

bool FingerScanner::import(const std::string &base64Encoded) {
    std::vector<BYTE> decodedData = base64_decode(base64Encoded);
    if (!decodedData.empty()) {
        int ret = ZKFPM_DBAdd(dbCache, userId++,
                              &decodedData[0], decodedData.size());
        if (ret == ZKFP_ERR_OK) {
            return true;
        }
    }
    return false;
}

std::string FingerScanner::getLastRegistered() {
    return base64_encode(&lastRegTemplate[0], lastRegTempLen);
}

bool FingerScanner::verify(unsigned char *temp, unsigned int len) {
    unsigned int tid = 0;
    unsigned int score = 0;
    int ret = ZKFPM_DBIdentify(dbCache, temp, len, &tid, &score);
    if (ret != ZKFP_ERR_OK) {
        return false;
    }

    std::cout << "Verified: userId: " << tid << ", score: " << score << std::endl;
    return true;
}

bool FingerScanner::match(unsigned char *temp, unsigned int len,
                          unsigned char *expected, unsigned int expLen) {
    int ret = ZKFPM_DBMatch(dbCache, expected, expLen, temp, len);
    if (ret <= 0) {
        return false;
    }
    return true;
}

int FingerScanner::acquireFingerprint(unsigned char *szTemplate, unsigned int *tempLen) {
    return ZKFPM_AcquireFingerprint(device, imgBuf,
                                    width * height,
                                    szTemplate, tempLen);
}





