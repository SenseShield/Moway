/*******************************************************************************
* Copyright (c) 2013 Beijing Senselock Software Technology Co.,Ltd.
* All rights reserved.
*
* filename: SlcCrypto.h
*
* brief: Cryptography library interface declaration.
* 
*******************************************************************************/
#ifndef _SLC_CRYPTO_H_
#define _SLC_CRYPTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


#ifndef SLCAPI
#if defined WIN32 || defined _WIN32 || defined _WIN64
#define SLCAPI __stdcall
#else
#define SLCAPI
#endif  // WIN32 || defined _WIN32 || defined _WIN64
#endif  //SLCAPI


/*******************************************************************************
 type definition
********************************************************************************/
#ifndef SLC_BYTE
typedef unsigned char   SLC_BYTE;
#endif



#ifndef SLC_ULONG
typedef unsigned long   SLC_ULONG;
#endif


typedef void *          SLC_HASH_CTX;
typedef void *          SLC_KEY;



/*******************************************************************************
 Key type IDs
*******************************************************************************/
#define SLC_KEY_SYMMETRIC                   0x01        // symmetric key
#define SLC_KEY_PUBLIC                      0x02        // public key
#define SLC_KEY_PRIVATE                     0x03        // private key
#define SLC_KEY_HMAC                        0x04

/*******************************************************************************
 Cryptograpic algorithm IDs
*******************************************************************************/
#define SLC_CIPHER_ALGO_AES                 0x00        // AES
#define SLC_CIPHER_ALGO_DES                 0x01        // DES
#define SLC_CIPHER_ALGO_TDES                0x02        // TDES
#define SLC_CIPHER_ALGO_ECC                 0x10        // ECC
#define SLC_CIPHER_ALGO_RSA                 0x11        // RSA
#define SLC_CIPHER_ALGO_HMAC_MD5            0x41        // HMAC-MD5
#define SLC_CIPHER_ALGO_HMAC_SHA1           0x42        // HMAC-SHA1
#define SLC_CIPHER_ALGO_HMAC_SHA256         0x43        // HMAC-SHA256

/*******************************************************************************
 Channing mode IDs for AES/DES/TDES algorithms
*******************************************************************************/
#define SLC_MODE_ECB                        0x00        // ECB mode
#define SLC_MODE_CBC                        0x01        // CBC mode

/*******************************************************************************
 HASH algorithm IDs
*******************************************************************************/
#define SLC_HASH_ALGO_SHA1                  0x01        // SHA-1
#define SLC_HASH_ALGO_SHA256                0x02        // SHA-256
#define SLC_HASH_ALGO_MD5                   0x03        // MD5

/*******************************************************************************
 RSA padding mode IDs
*******************************************************************************/
#define SLC_PAD_MODE_NONE                   0x00        // no padding
#define SLC_PAD_MODE_PKCS_1_V1_5            0x01        // PKCS #1 V1.5 

/*******************************************************************************
 Key length in bit
*******************************************************************************/
#define SLC_DES_KEY_BIT_LEN                  56         // DES
#define SLC_TDES_KEY_BIT_LEN                112         // TDES

#define SLC_AES_KEY_BIT_128                 128         // 16 bytes
#define SLC_AES_KEY_BIT_192                 192         // 24 bytes
#define SLC_AES_KEY_BIT_256                 256         // 32 bytes

#define SLC_RSA_KEY_BIT_1024                1024        // 128 bytes
#define SLC_RSA_KEY_BIT_2048                2048        // 256 bytes
#define SLC_RSA_KEY_BIT_4096                4096        // 512 bytes

#define SLC_ECC_KEY_BIT_192                 192         // 24 bytes
#define SLC_ECC_KEY_BIT_256                 256
#define SLC_ECC_KEY_BIT_384                 384

/*******************************************************************************
 Block size in byte for symmetric ciphers
*******************************************************************************/
#define SLC_DES_BLOCK_SIZE                  8           // DES
#define SLC_TDES_BLOCK_SIZE                 8           // TDES
#define SLC_AES_BLOCK_SIZE                  16          // AES

/*******************************************************************************
 HASH result length in byte
*******************************************************************************/
#define SLC_MD5_DIGEST_LEN                  16          // MD5
#define SLC_SHA1_DIGEST_LEN                 20          // SHA-1
#define SLC_SHA256_DIGEST_LEN               32          // SHA-256

/*******************************************************************************
 Signature length in byte
*******************************************************************************/
#define SLC_RSA1024_SIG_LEN                 128         // 
#define SLC_RSA2048_SIG_LEN                 256         // 
#define SLC_RSA4096_SIG_LEN                 512         // 
#define SLC_ECC192_SIG_MAX_LEN              128         // 



/*******************************************************************************
 API's return codes
*******************************************************************************/
#define SLC_SUCCESS                         0x0000      // Succeeded.
#define SLC_ERROR_NO_MEMORY                 0x0001      // Not enough storage is available to process this command.
#define SLC_ERROR_INVALID_PARAMETER         0x0002      // Mostly, a NULL pointer is used as key or buffer.
#define SLC_ERROR_INVALID_HASH_ALGO         0x0003      // An invalid hash algorithm ID is used.
#define SLC_ERROR_INVALID_CIPHER_ALGO       0x0004      // An invalid cipher algorithm ID is used.
#define SLC_ERROR_INVALID_MODE              0x0005      // An invalid padding or encryption mode is used.
#define SLC_ERROR_INPUTDATA_LENGTH          0x0006      // The length of input data is error.
#define SLC_ERROR_INSUFFICIENT_BUFFER       0x0007      // The output buffer is not large enough.
#define SLC_ERROR_INVALID_KEY_SIZE          0x0008      // An invalid key length is used.
#define SLC_ERROR_MAKE_KEY                  0x0009      // The key cannot be created.
#define SLC_ERROR_SIGN                      0x000A      // The signature cannot be created.
#define SLC_ERROR_VERIFY                    0x000B      // The signature don't match with the original data.
#define SLC_ERROR_INVALID_INPUT_DATA        0x000C      // The input data has a bad format.
#define SLC_ERROR_BAD_KEY_FORMAT            0x000D      // The key has a bad format, maybe corrupted.
#define SLC_ERROR_BAD_KEY_VERSION           0x000E      // The key has an unacceptable version number.
#define SLC_ERROR_BAD_KEY_ALGORITHM         0x000F      // The key is not suitable for the algorithm.
#define SLC_ERROR_BAD_KEY_TYPE              0x0010      // The key is not suitable for the usage.
#define SLC_ERROR_BAD_PADDING               0x0011      // The input data has bad format, maybe corrupted.
#define SLC_ERROR_UNKNOW                    0x0012      // Internal error occured.


////////////////////////////////////////////////////////////////////////////////
// AES encryption/decryption/key generation
// 
SLC_ULONG SLCAPI SlcAesEncRaw(
                        IN      SLC_BYTE                byMode, 
                        IN      const SLC_BYTE          * pbKey, 
                        IN      SLC_ULONG               ulKeyBitLen, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcAesDecRaw(
                        IN      SLC_BYTE                byMode, 
                        IN      const SLC_BYTE          * pbKey, 
                        IN      SLC_ULONG               ulKeyBitLen, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcAesGenerateKey(
                        IN      const SLC_BYTE          * pbInitData,
                        IN      SLC_ULONG               ulKeyBitLen,
                        OUT     SLC_KEY                 * pSlcKey
                        );
SLC_ULONG SLCAPI SlcAesEnc(
                        IN      SLC_BYTE                byMode, 
                        IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcAesDec(
                        IN      SLC_BYTE                byMode, 
                        IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// DES encryption/decryption/key generation
// 
SLC_ULONG SLCAPI SlcDesEncRaw(
                        IN      SLC_BYTE                byMode, 
                        IN      const SLC_BYTE          * pbKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcDesDecRaw(
                        IN      SLC_BYTE                byMode, 
                        IN      const SLC_BYTE          * pbKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcDesGenerateKey(
                        IN      const SLC_BYTE          * pbInitData,
                        IN      SLC_ULONG               ulKeyBitLen,
                        OUT     SLC_KEY                 * pSlcKey
                        );
SLC_ULONG SLCAPI SlcDesEnc(
                        IN      SLC_BYTE                byMode, 
                        IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcDesDec(
                        IN      SLC_BYTE                byMode, 
                        IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// TDES encryption/decryption/key generation
// 
SLC_ULONG SLCAPI SlcTDesEncRaw(
                        IN      SLC_BYTE                byMode, 
                        IN      const SLC_BYTE          * pbKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcTDesDecRaw(
                        IN      SLC_BYTE                byMode, 
                        IN      const SLC_BYTE          * pbKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcTDesGenerateKey(
                        IN      const SLC_BYTE          * pbInitData,
                        IN      SLC_ULONG               ulKeyBitLen,
                        OUT     SLC_KEY                 * pSlcKey
                        );
SLC_ULONG SLCAPI SlcTDesEnc(
                        IN      SLC_BYTE                byMode, 
                        IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcTDesDec(
                        IN      SLC_BYTE                byMode, 
                        IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbIv,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// SHA-1
// 
SLC_ULONG SLCAPI SlcSha1Init(
                        OUT     SLC_HASH_CTX            * pHashCtx
                        );
SLC_ULONG SLCAPI SlcSha1Update(
                        IN OUT  SLC_HASH_CTX            * pHashCtx,
                        IN      SLC_BYTE                * pbData,
                        IN      SLC_ULONG               ulDataLen
                        );
SLC_ULONG SLCAPI SlcSha1Final(
                        IN OUT  SLC_HASH_CTX            * pHashCtx,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcSha1( 
                        IN      SLC_BYTE                * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// SHA-256
// 
SLC_ULONG SLCAPI SlcSha256Init(
                        OUT     SLC_HASH_CTX            * pHashCtx
                        );
SLC_ULONG SLCAPI SlcSha256Update(  
                        IN OUT  SLC_HASH_CTX            * pHashCtx,
                        IN      SLC_BYTE                * pbData,
                        IN      SLC_ULONG               ulDataLen
                        );
SLC_ULONG SLCAPI SlcSha256Final( 
                        IN OUT  SLC_HASH_CTX            * pHashCtx,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcSha256( 
                        IN      SLC_BYTE                * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// MD5
// 
SLC_ULONG SLCAPI SlcMd5Init(
                        OUT     SLC_HASH_CTX            * pHashCtx
                        );
SLC_ULONG SLCAPI SlcMd5Update( 
                        IN OUT  SLC_HASH_CTX            * pHashCtx,
                        IN      SLC_BYTE                * pbData,
                        IN      SLC_ULONG               ulDataLen
                        );
SLC_ULONG SLCAPI SlcMd5Final(
                        IN OUT  SLC_HASH_CTX            * pHashCtx,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcMd5( 
                        IN      SLC_BYTE                * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// RSA encryption/decryption/key generation/sign/verify
// 
SLC_ULONG SLCAPI SlcRsaGenerateKey( 
                        IN      SLC_ULONG               ulKeyBitLen,
                        OUT     SLC_KEY                 * pSlcPubKey,
                        OUT     SLC_KEY                 * pSlcPriKey
                        );
SLC_ULONG SLCAPI SlcRsaPubEnc(
                        IN      SLC_BYTE                byPadMode,
                        IN      SLC_KEY                 SlcPubKey,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcRsaPriDec(
                        IN      SLC_BYTE                byPadMode,
                        IN      SLC_KEY                 SlcPriKey,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcRsaPriEnc(
                        IN      SLC_BYTE                byPadMode,
                        IN      SLC_KEY                 SlcPriKey,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcRsaPubDec(
                        IN      SLC_BYTE                byPadMode,
                        IN      SLC_KEY                 SlcPubKey,
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcRsaSign(
                        IN      SLC_BYTE                byPadMode,
                        IN      SLC_BYTE                byHashAlgo,
                        IN      SLC_KEY                 SlcPriKey,
                        IN      const SLC_BYTE          * pbMessage,
                        IN      SLC_ULONG               ulMessageLen,
                        OUT     SLC_BYTE                * pbSignatureBuf,
                        IN      SLC_ULONG               ulSignatureBufLen,
                        OUT     SLC_ULONG               * pulSignatureLen
                        );
SLC_ULONG SLCAPI SlcRsaVerify( 
                        IN      SLC_BYTE                byPadMode,
                        IN      SLC_BYTE                byHashAlgo,
                        IN      SLC_KEY                 SlcPubKey,
                        IN      const SLC_BYTE          * pbMessage,
                        IN      SLC_ULONG               ulMessageLen,
                        IN      const SLC_BYTE          * pbSignature,
                        IN      SLC_ULONG               ulSignatureLen
                        );
////////////////////////////////////////////////////////////////////////////////
// ECC key generation/sign/verify
//
SLC_ULONG SLCAPI SlcEccGenerateKey( 
                        IN      SLC_ULONG               ulKeyBitLen,
                        OUT     SLC_KEY                 * pSlcPubKey,
                        OUT     SLC_KEY                 * pSlcPriKey
                        );
SLC_ULONG SLCAPI SlcEccSign(
                        IN      SLC_BYTE                byHashAlgo,
                        IN      SLC_KEY                 SlcPriKey,
                        IN      const SLC_BYTE          * pbMessage,
                        IN      SLC_ULONG               ulMessageLen,
                        OUT     SLC_BYTE                * pbSignatureBuf,
                        IN      SLC_ULONG               ulSignatureBufLen,
                        OUT     SLC_ULONG               * pulSignatureLen
                        );
SLC_ULONG SLCAPI SlcEccVerify( 
                        IN      SLC_BYTE                byHashAlgo,
                        IN      SLC_KEY                 SlcPubKey,
                        IN      const SLC_BYTE          * pbMessage,
                        IN      SLC_ULONG               ulMessageLen,
                        IN      const SLC_BYTE          * pbSignature,
                        IN      SLC_ULONG               ulSignatureLen
                        );
////////////////////////////////////////////////////////////////////////////////
// Hmac GenKey/MD5/SHA1/SHA256
// 
SLC_ULONG SLCAPI SlcHmacGenerateKey(
                        IN      const SLC_BYTE          * pbInitData,
                        IN      SLC_ULONG               ulKeyBitLen,
						IN      SLC_ULONG               ulAlgorithm,
                        OUT     SLC_KEY                 * pSlcKey
                        );
SLC_ULONG SLCAPI SlcHmacRaw(
                        IN      SLC_BYTE                bAlgoHmac, 
                        IN      const SLC_BYTE          * pbKey, 
                    	IN      SLC_ULONG               ulKeyLen, 
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
SLC_ULONG SLCAPI SlcHmac(IN      SLC_KEY                 SlcKey, 
                        IN      const SLC_BYTE          * pbData,
                        IN      SLC_ULONG               ulDataLen,
                        OUT     SLC_BYTE                * pbOutBuf,
                        IN      SLC_ULONG               ulOutBufLen,
                        OUT     SLC_ULONG               * pulOutDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// format convertion
//
SLC_ULONG SLCAPI SlcKeyToAesKey( 
                        IN      SLC_KEY                 SlcKey, 
                        OUT     SLC_BYTE                * pbKeyBuf,
                        IN      SLC_ULONG               ulKeyBufLen,
                        OUT     SLC_ULONG               * pulKeyByteLen
                        );
SLC_ULONG SLCAPI SlcKeyToDesKey( 
                        IN      SLC_KEY                 SlcKey, 
                        OUT     SLC_BYTE                * pbKeyBuf,
                        IN      SLC_ULONG               ulKeyBufLen,
                        OUT     SLC_ULONG               * pulKeyByteLen
                        );
SLC_ULONG SLCAPI SlcKeyToTDesKey( 
                        IN      SLC_KEY                 SlcKey, 
                        OUT     SLC_BYTE                * pbKeyBuf,
                        IN      SLC_ULONG               ulKeyBufLen,
                        OUT     SLC_ULONG               * pulKeyByteLen
                        );
SLC_ULONG SLCAPI SlcKeyToHmacKey( 
                        IN      SLC_KEY                 SlcKey, 
                        OUT     SLC_BYTE                * pbKeyBuf,
                        IN      SLC_ULONG               ulKeyBufLen,
                        OUT     SLC_ULONG               * pulKeyByteLen,
                     		OUT     SLC_BYTE                * sbAlgorithm
                        );
////////////////////////////////////////////////////////////////////////////////
// check key type/size/export/import
//
SLC_ULONG SLCAPI SlcCheckKeyType(
                        IN      SLC_KEY                 SlcKey, 
                        IN      SLC_BYTE                byKeyType,
                        IN      SLC_BYTE                byAlgorithm
                        );
SLC_ULONG SLCAPI SlcGetKeySize(
                        IN      SLC_KEY                 SlcKey 
                        );
SLC_ULONG SLCAPI SlcGetKeyBitLength(
                        IN      SLC_KEY                 SlcKey 
                        );
SLC_ULONG SLCAPI SlcExportKey(
                        IN      SLC_KEY                 SlcKey, 
                        OUT     SLC_BYTE                * pbKeyBuf,
                        IN      SLC_ULONG               ulKeyBufLen,
                        OUT     SLC_ULONG               * pulKeyByteLen
                        );
SLC_ULONG SLCAPI SlcImportKey(
                        OUT     SLC_KEY                 * pSlcKey,
                        IN      const SLC_BYTE          * pbInputBuf,
                        IN      SLC_ULONG               ulInputDataLen
                        );
////////////////////////////////////////////////////////////////////////////////
// free key
// 
SLC_ULONG SLCAPI SlcFreeKey(
                        IN OUT  SLC_KEY                  * pSlcKey
                        );

#ifdef __cplusplus
}
#endif

#endif//#define _SLC_CRYPTO_H_
