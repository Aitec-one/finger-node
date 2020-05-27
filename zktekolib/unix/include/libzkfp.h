#ifndef _libzkfp_h
#define _libzkfp_h
/**
*	@file		libzkfp.h
*	@brief		�ӿڶ���
*	@author		scar chen
*	@date		2016-04-12
*	@version	5.0
*	@par	��Ȩ��
*				ZKTeco
*	@par	��ʷ�汾			
*
*	@note
*
*/


#include "libzkfptype.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define ZKFP_DLOPEN
#ifdef ZKFP_DLOPEN

typedef int (*T_ZKFPM_Init)();
typedef int (*T_ZKFPM_Terminate)();
typedef int (*T_ZKFPM_GetDeviceCount)();
typedef HANDLE (*T_ZKFPM_OpenDevice)(int index);
typedef int (*T_ZKFPM_CloseDevice)(HANDLE hDevice);
typedef int (*T_ZKFPM_SetParameters)(HANDLE hDevice, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);
typedef int (*T_ZKFPM_GetParameters)(HANDLE hDevice, int nParamCode, unsigned char* paramValue, unsigned int* cbParamValue);
typedef int (*T_ZKFPM_AcquireFingerprint)(HANDLE hDevice, unsigned char* fpImage, unsigned int cbFPImage, unsigned char* fpTemplate, unsigned int* cbTemplate);
typedef int (*T_ZKFPM_AcquireFingerprintImage)(HANDLE hDevice, unsigned char* fpImage, unsigned int cbFPImage);

typedef HANDLE (*T_ZKFPM_DBInit)();
typedef int (*T_ZKFPM_DBFree)(HANDLE hDBCache);
typedef int (*T_ZKFPM_DBMerge)(HANDLE hDBCache, unsigned char* temp1, unsigned char* temp2, unsigned char* temp3, unsigned char* regTemp, unsigned int* cbRegTemp);
typedef int (*T_ZKFPM_DBAdd)(HANDLE hDBCache, unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);
typedef int (*T_ZKFPM_DBDel)(HANDLE hDBCache, unsigned int fid);
typedef int (*T_ZKFPM_DBClear)(HANDLE hDBCache);
typedef int (*T_ZKFPM_DBCount)(HANDLE hDBCache, unsigned int* fpCount);
typedef int (*T_ZKFPM_DBIdentify)(HANDLE hDBCache, unsigned char* fpTemplate, unsigned int cbTemplate, unsigned int* FID, unsigned int* score);
typedef int (*T_ZKFPM_DBMatch)(HANDLE hDBCache, unsigned char* template1, unsigned int cbTemplate1, unsigned char* template2, unsigned int cbTemplate2);
typedef int (*T_ZKFPM_ExtractFromImage)(HANDLE hDBCache, const char* lpFilePathName, unsigned int DPI, unsigned char* fpTemplate, unsigned int *cbTemplate);


T_ZKFPM_Init ZKFPM_Init;
T_ZKFPM_Terminate ZKFPM_Terminate;
T_ZKFPM_GetDeviceCount ZKFPM_GetDeviceCount;
T_ZKFPM_OpenDevice ZKFPM_OpenDevice;
T_ZKFPM_CloseDevice ZKFPM_CloseDevice;
T_ZKFPM_SetParameters ZKFPM_SetParameters;
T_ZKFPM_GetParameters ZKFPM_GetParameters;
T_ZKFPM_AcquireFingerprint ZKFPM_AcquireFingerprint;
T_ZKFPM_AcquireFingerprintImage ZKFPM_AcquireFingerprintImage;

T_ZKFPM_DBInit ZKFPM_DBInit;
T_ZKFPM_DBFree ZKFPM_DBFree;
T_ZKFPM_DBMerge ZKFPM_DBMerge;
T_ZKFPM_DBAdd ZKFPM_DBAdd;
T_ZKFPM_DBDel ZKFPM_DBDel;
T_ZKFPM_DBClear ZKFPM_DBClear;
T_ZKFPM_DBCount ZKFPM_DBCount;
T_ZKFPM_DBIdentify ZKFPM_DBIdentify;
T_ZKFPM_DBMatch ZKFPM_DBMatch;
T_ZKFPM_ExtractFromImage ZKFPM_ExtractFromImage;

#else

#endif	

#ifdef __cplusplus
};
#endif


#endif	//_libzkfp_h
