/*************************************************************
*CopyRight(c) Beijing Senselock Inc
*
* [当前版本] 1.0.0.0
*
* [作    者] lining
*
* [完成日期] 2013/04/02 
*
*------------------------------------------------------------
*[历史修订记录]
*
* [修改时间]      [版本]        [作者]         [修改记录说明]
*____________   ___________   __________   ___________________ 
*____________   ___________   __________   ___________________
*____________   ___________   __________   ___________________
*------------------------------------------------------------
*************************************************************/

#pragma once

#ifndef SLC_BYTE
typedef unsigned char   UINT8;
#endif

//锁内程序或者锁外加解密函数执行成功，但加密或者解密出的结果不正确
#define EV_RIGHT_RESULT_ERROR    0xaaaaaaaa 

//锁内程序执行失败
#define EV_RIGHT_EXECUTE_ERROR    0xbbbbbbbb  

//获取SLC key格式密钥的大小时返回0值
#define EV_RIGHT_GETSLCKEYSIZE_ERROR    0xcccccccc 

#define DEVELOPERID   0


#include <stdio.h>
#include <windows.h>
#include "SlcCrypt.h"


void CheckResult( DWORD lRetCode, DWORD lCode, LPSTR lpszMsg );
void CheckResult2( DWORD lRetCode, DWORD lCode, LPSTR lpszMsg );

//AES
void Test_SlcAesEncRaw(unsigned char mode, int key_bits);
void Test_SlcAesDecRaw(unsigned char mode, int key_bits);
void Test_SlcAesGenerateKey( int key_bits, BOOL ishave);
void Test_SlcAesEnc(unsigned char mode, int key_bits);
void Test_SlcAesDec(unsigned char mode, int key_bits);

//DES
void Test_SlcDesEncRaw(unsigned char mode);
void Test_SlcDesDecRaw(unsigned char mode);
void Test_SlcdesGenerateKey(BOOL ishave);
void Test_SlcDesEnc(unsigned char mode);
void Test_SlcDesDec(unsigned char mode);

//TDES
void Test_SlcTDesEncRaw(unsigned char mode);
void Test_SlcTDesDecRaw(unsigned char mode);
void Test_SlcTDesGenerateKey(BOOL ishave);
void Test_SlcTDesEnc(unsigned char mode);
void Test_SlcTDesDec(unsigned char mode);

//HASH1
void Test_HASH1();
//HASH256
void Test_HASH256();
//MD5
void Test_MD5();

//RSA
void Test_SlcRsaGenerateKey(int key_bits);
void Test_SlcRsaPubEnc(unsigned char mode, int key_bits);
void Test_SlcRsaPriDec(unsigned char mode, int key_bits);
void Test_SlcRsaPriEnc(unsigned char mode, int key_bits);
void Test_SlcRsaPubDec(unsigned char mode, int key_bits);
void Test_SlcRsaSign(unsigned char mode, int key_bits);
void Test_SlcRsaVerify(unsigned char mode, int key_bits);

//ECC
void Test_SlcEccGenerateKey(SLC_ULONG ulBits, unsigned char mode);
void Test_SlcEccSign(unsigned char mode);
void Test_SlcEccVerify(unsigned char mode);

//关于密钥
void Test_SlcKeyToAesKey( int key_bits, BOOL ishave, unsigned char mode);
void Test_SlcKeyToDesKey( BOOL ishave, unsigned char mode);
void Test_SlcKeyToTDesKey( BOOL ishave, unsigned char mode);
void Test_SlcCheckKeyType();
void Test_SlcGetKeySize();
void Test_SlcGetKeyBitLength();
void Test_SlcExportKey(int crypt_flag, int key_bits, unsigned char mode);
void Test_SlcImportKey(int crypt_flag, int key_bits, unsigned char mode);
void Test_SlcFreeKey();

