#include <windows.h>
#include <stdio.h>
#include <string.h>


#include "zkinterface.h"
#include "libzkfperrdef.h"
#include "libzkfptype.h"
#include "libzkfp.h"

#define ENROLLCNT 3

int m_Tid = 100;
int stop = 1;
int opType = 0;

int m_bRegister;
int m_enrollIdx;
unsigned char m_arrPreRegTemps[ENROLLCNT][MAX_TEMPLATE_SIZE];
unsigned int m_arrPreTempsLen[3];

HANDLE m_hDevice;
unsigned char *m_pImgBuf;
int m_imgFPWidth;
int m_imgFPHeight;

HANDLE m_hDBCache;
unsigned char m_szLastRegTemplate[MAX_TEMPLATE_SIZE];
unsigned int m_nLastRegTempLen;

int fp_connect() {
    if (ZKFPM_Init() != ZKFP_ERR_OK) {
        printf("Init ZKFPM fail\n");
        return 0;
    }
    printf("Init done\n");

    if ((m_hDevice = ZKFPM_OpenDevice(0)) == NULL) {
        printf("Open sensor fail\n");
        ZKFPM_Terminate();
        return 0;
    }
    printf("Device is open\n");

    m_hDBCache = ZKFPM_DBInit();
    if (NULL == m_hDBCache) {
        printf("Create DBCache fail\n");
        ZKFPM_CloseDevice(m_hDevice);
        ZKFPM_Terminate();
        return 0;
    }
    printf("DB loaded\n");

    unsigned int size = 4;
    ZKFPM_GetParameters(m_hDevice, 1, (unsigned char *) &m_imgFPWidth, &size);
    size = 4;
    ZKFPM_GetParameters(m_hDevice, 2, (unsigned char *) &m_imgFPHeight, &size);
//    m_pImgBuf = new unsigned char[m_imgFPWidth*m_imgFPHeight];
    m_pImgBuf = malloc(m_imgFPWidth * m_imgFPHeight * sizeof(unsigned char));

    return 1;
}

int fp_enroll(unsigned char *temp, unsigned int len) {
//    if (m_enrollIdx >= ENROLLCNT) {
//        m_enrollIdx = 0;    //restart enroll
//        return 0;
//    }
    if (m_enrollIdx > 0) {
        if (0 >= ZKFPM_DBMatch(m_hDBCache,
                               m_arrPreRegTemps[m_enrollIdx - 1],
                               m_arrPreTempsLen[m_enrollIdx - 1], temp,
                               len)) {
            m_enrollIdx = 0;
            m_bRegister = 0;
            printf("Please press the same finger while registering!\n");
            return 0;
        }
    }
    m_arrPreTempsLen[m_enrollIdx] = len;
    memcpy(m_arrPreRegTemps[m_enrollIdx], temp, len);
    m_enrollIdx++;
    if (m_enrollIdx >= ENROLLCNT) {

        unsigned char szRegTemp[MAX_TEMPLATE_SIZE] = {0x0};
        unsigned int cbRegTemp = MAX_TEMPLATE_SIZE;
        int ret = ZKFPM_DBMerge(m_hDBCache,
                                m_arrPreRegTemps[0],
                                m_arrPreRegTemps[1],
                                m_arrPreRegTemps[2],
                                szRegTemp, &cbRegTemp);
        m_enrollIdx = 0;
        m_bRegister = FALSE;
        if (ret == ZKFP_ERR_OK) {
            ret = ZKFPM_DBAdd(m_hDBCache, m_Tid++, szRegTemp, cbRegTemp);
            if (ret == ZKFP_ERR_OK) {
                memcpy(m_szLastRegTemplate, szRegTemp, cbRegTemp);
                m_nLastRegTempLen = cbRegTemp;
                printf("Register succ\n");
            } else {
                printf("Register fail, because add to db fail, ret=%d\n", ret);
                return 0;
            }
        }

        m_enrollIdx = 0;

        return 1;
    } else {
        printf("Pleas put a finger %d more times\n", ENROLLCNT - m_enrollIdx);
    }
    return 0;
}

int fp_verify(unsigned char *temp, unsigned int len) {
    unsigned int tid = 0;
    unsigned int score = 0;
    int ret = ZKFPM_DBIdentify(m_hDBCache, temp, len, &tid, &score);
    if (ret != ZKFP_ERR_OK) {
//        printf("Not verified\n");
        return 0;
    }

    printf("Verified: %d, %d\n", tid, score);
    return 1;
}

int fp_match(unsigned char *temp, unsigned int len) {
    int ret = ZKFPM_DBMatch(m_hDBCache, m_szLastRegTemplate, m_nLastRegTempLen, temp, len);
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
        int ret = ZKFPM_AcquireFingerprint(m_hDevice, m_pImgBuf,
                m_imgFPWidth * m_imgFPHeight, szTemplate, &tempLen);
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
        } else {
//            printf("Finger print error: %d\n", ret);
        }
        Sleep(100);
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
