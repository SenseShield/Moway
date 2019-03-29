#include "SlcCrypt.h"
#include "helper.h"
#include <time.h>

SLC_ULONG   ecc_ulRetCode;

//公钥和私钥
SLC_KEY     g_ecc_SlcPriKey;
SLC_KEY     g_ecc_SlcPubKey;

SLC_ULONG   g_ecc_ulCipherLen;     //密文长度
SLC_ULONG   g_ecc_ulPlainLen;      //明文长度

SLC_BYTE g_ecc_data[32];
SLC_BYTE    g_ecc_sign[128] = {0};  //签名结果

int m;
char   ecc_loginfo[1024];
/*
生成SLC_KEY格式的ecc密钥
mode:验证生成的密钥时，签名验证所使用的hash模式
*/
void Test_SlcEccGenerateKey(SLC_ULONG ulBits, unsigned char mode)
{
	SLC_ULONG  ulKeyBits = ulBits;
	SLC_BYTE   byHashMode = (SLC_BYTE)mode;

	//随机生成32字节明文
	srand(time(NULL));
	g_ecc_data[0] = 0x00;
	for(m = 1; m < sizeof(g_ecc_data);m++)
	{
		g_ecc_data[m] = rand()%255; 
	}

	//生成密钥
	ecc_ulRetCode = SlcEccGenerateKey(ulKeyBits, &g_ecc_SlcPubKey, &g_ecc_SlcPriKey);
	if ( SLC_SUCCESS == ecc_ulRetCode)
	{

		//明文长度
		g_ecc_ulPlainLen = sizeof(g_ecc_data);

		//签名结果长度，此值不能小于key_bits/8
		g_ecc_ulCipherLen = sizeof(g_ecc_sign);

		//私钥签名
		memset( g_ecc_sign, 0x00, sizeof(g_ecc_sign));	
		ecc_ulRetCode = SlcEccSign(byHashMode,g_ecc_SlcPriKey, g_ecc_data, g_ecc_ulPlainLen, g_ecc_sign, g_ecc_ulCipherLen, &g_ecc_ulCipherLen);	
		if ( SLC_SUCCESS == ecc_ulRetCode)
		{
			//公钥认证
			g_ecc_ulPlainLen = sizeof(g_ecc_data);	
			g_ecc_ulCipherLen = ulKeyBits/8;    //此值不能小于key_bits/8
			ecc_ulRetCode = SlcEccVerify(byHashMode, g_ecc_SlcPubKey, g_ecc_data, g_ecc_ulPlainLen, g_ecc_sign, g_ecc_ulCipherLen);
			sprintf(ecc_loginfo, "Right1.SlcEccGenerateKey_%d", byHashMode);
			CheckResult(ecc_ulRetCode, SLC_SUCCESS, ecc_loginfo);
		}
		else
		{
			sprintf(ecc_loginfo, "Right2.SlcEccGenerateKey_%d", byHashMode);
			CheckResult(ecc_ulRetCode, SLC_SUCCESS, ecc_loginfo);
		}

	}
	SlcFreeKey(&g_ecc_SlcPubKey);
	SlcFreeKey(&g_ecc_SlcPriKey);
}

int main()
{
	int i;

	for(i=0;i<10000;i++)
	{
		printf("times:%d\n",i);
		//生成ECC公私钥对
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_192, SLC_HASH_ALGO_SHA1);
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_192, SLC_HASH_ALGO_SHA256);
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_192, SLC_HASH_ALGO_MD5);

		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_256, SLC_HASH_ALGO_SHA1);
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_256, SLC_HASH_ALGO_SHA256);
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_256, SLC_HASH_ALGO_MD5);

		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_384, SLC_HASH_ALGO_SHA1);
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_384, SLC_HASH_ALGO_SHA256);
		Test_SlcEccGenerateKey(SLC_ECC_KEY_BIT_384, SLC_HASH_ALGO_MD5);
	}
	return 0;
}