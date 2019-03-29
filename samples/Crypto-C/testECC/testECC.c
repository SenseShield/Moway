#include "SlcCrypt.h"
#include "helper.h"
#include <time.h>

SLC_ULONG   ecc_ulRetCode;

//��Կ��˽Կ
SLC_KEY     g_ecc_SlcPriKey;
SLC_KEY     g_ecc_SlcPubKey;

SLC_ULONG   g_ecc_ulCipherLen;     //���ĳ���
SLC_ULONG   g_ecc_ulPlainLen;      //���ĳ���

SLC_BYTE g_ecc_data[32];
SLC_BYTE    g_ecc_sign[128] = {0};  //ǩ�����

int m;
char   ecc_loginfo[1024];
/*
����SLC_KEY��ʽ��ecc��Կ
mode:��֤���ɵ���Կʱ��ǩ����֤��ʹ�õ�hashģʽ
*/
void Test_SlcEccGenerateKey(SLC_ULONG ulBits, unsigned char mode)
{
	SLC_ULONG  ulKeyBits = ulBits;
	SLC_BYTE   byHashMode = (SLC_BYTE)mode;

	//�������32�ֽ�����
	srand(time(NULL));
	g_ecc_data[0] = 0x00;
	for(m = 1; m < sizeof(g_ecc_data);m++)
	{
		g_ecc_data[m] = rand()%255; 
	}

	//������Կ
	ecc_ulRetCode = SlcEccGenerateKey(ulKeyBits, &g_ecc_SlcPubKey, &g_ecc_SlcPriKey);
	if ( SLC_SUCCESS == ecc_ulRetCode)
	{

		//���ĳ���
		g_ecc_ulPlainLen = sizeof(g_ecc_data);

		//ǩ��������ȣ���ֵ����С��key_bits/8
		g_ecc_ulCipherLen = sizeof(g_ecc_sign);

		//˽Կǩ��
		memset( g_ecc_sign, 0x00, sizeof(g_ecc_sign));	
		ecc_ulRetCode = SlcEccSign(byHashMode,g_ecc_SlcPriKey, g_ecc_data, g_ecc_ulPlainLen, g_ecc_sign, g_ecc_ulCipherLen, &g_ecc_ulCipherLen);	
		if ( SLC_SUCCESS == ecc_ulRetCode)
		{
			//��Կ��֤
			g_ecc_ulPlainLen = sizeof(g_ecc_data);	
			g_ecc_ulCipherLen = ulKeyBits/8;    //��ֵ����С��key_bits/8
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
		//����ECC��˽Կ��
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