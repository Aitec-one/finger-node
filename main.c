#include <windows.h>
#include <stdio.h>
#include <string.h>


#include "zkinterface.h"
#include "libzkfperrdef.h"
#include "libzkfptype.h"
#include "libzkfp.h"

#define ENROLLCNT 3

int userId = 100;
int stop = 1;
int opType = 0;

int enrollIdx;
unsigned char enrollTries[ENROLLCNT][MAX_TEMPLATE_SIZE];
unsigned int enrollTriesLen[ENROLLCNT];

HANDLE device;
unsigned char *imgBuf;
int width;
int height;

HANDLE dbCache;
unsigned char lastRegTemplate[MAX_TEMPLATE_SIZE];
unsigned int lastRegTempLen;

int fp_connect() {
    if (ZKFPM_Init() != ZKFP_ERR_OK) {
        printf("Init ZKFPM fail\n");
        return 0;
    }
    printf("Init done\n");

    if ((device = ZKFPM_OpenDevice(0)) == NULL) {
        printf("Open sensor fail\n");
        ZKFPM_Terminate();
        return 0;
    }
    printf("Device is open\n");

    dbCache = ZKFPM_DBInit();
    if (NULL == dbCache) {
        printf("Create DBCache fail\n");
        ZKFPM_CloseDevice(device);
        ZKFPM_Terminate();
        return 0;
    }
    printf("DB loaded\n");

    unsigned int size = 4;
    ZKFPM_GetParameters(device, 1, (unsigned char *) &width, &size);
    size = 4;
    ZKFPM_GetParameters(device, 2, (unsigned char *) &height, &size);
    imgBuf = malloc(width * height * sizeof(unsigned char));

    return 1;
}

int fp_enroll(unsigned char *temp, unsigned int len) {

    if (enrollIdx > 0) {
        if (0 >= ZKFPM_DBMatch(dbCache,
                               enrollTries[enrollIdx - 1],
                               enrollTriesLen[enrollIdx - 1], temp,
                               len)) {
            enrollIdx = 0;
            printf("Please press the same finger while registering!\n");
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
                printf("Register succ\n");
            } else {
                printf("Register fail, because add to db fail, ret=%d\n", ret);
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

int fp_verify(unsigned char *temp, unsigned int len) {
    unsigned int tid = 0;
    unsigned int score = 0;
    int ret = ZKFPM_DBIdentify(dbCache, temp, len, &tid, &score);
    if (ret != ZKFP_ERR_OK) {
        return 0;
    }

    printf("Verified: %d, %d\n", tid, score);
    return 1;
}

int fp_match(unsigned char *temp, unsigned int len) {
    int ret = ZKFPM_DBMatch(dbCache, lastRegTemplate, lastRegTempLen, temp, len);
    if (ret <= 0) {
        return 0;
    }
    printf("Match");
    return 1;
}

void fp_mainLoop() {
    while (stop) {
        unsigned char szTemplate[MAX_TEMPLATE_SIZE];
        unsigned int tempLen = MAX_TEMPLATE_SIZE;
        int ret = ZKFPM_AcquireFingerprint(device, imgBuf,
                                           width * height,
                                           szTemplate, &tempLen);
        if (ret == ZKFP_ERR_OK) {
            if (opType == 0) {
                if (fp_enroll(szTemplate, tempLen) == 1) {
                    printf("Registered!");
                    opType = 1;
                }

            } else if (opType == 1) {
                if (fp_verify(szTemplate, tempLen) == 1) {
                    printf("Verified!");
                } else {
                    printf("Not verified!");
                }

            } else if (opType == 2) {
                fp_match(szTemplate, tempLen);
            }
        }
        Sleep(10);
    }
}

int main() {
    printf("Starting fingerprint demo...\n");
    if (fp_connect() == 1) {
        printf("Starting main loop...");
        fp_mainLoop();
    }
    return 0;
}
