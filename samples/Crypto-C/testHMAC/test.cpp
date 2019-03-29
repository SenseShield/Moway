// restore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WINDOWS.H>
#include <STDIO.H>
#include <string.h>
#include "SlcCrypt.h"
#include <TIME.H>

//------------------------test function-------------------------
int main(int argc, char* argv[])
{
	SLC_BYTE indata[3000];
	unsigned short inlen;
	SLC_BYTE outdata[3000];
	unsigned short outlen;
	time_t t;

	SLC_ULONG slcret;

	SLC_BYTE softdigestraw[128];
	SLC_ULONG softdigestlenraw;
	SLC_BYTE softdigestkey[128];
	SLC_ULONG softdigestlenkey;

	SLC_KEY skhmacmd5key;
	SLC_KEY skhmacsha1key;
	SLC_KEY skhmacsha256key;

	SLC_BYTE testdata[] = "this is test data��";

	unsigned short hmacmd5keydatalen = 16;
	SLC_BYTE hmacmd5keydata[16] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,};
	unsigned short hmacsha1keydatalen = 20;
	SLC_BYTE hmacsha1keydata[20] = {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,};
	unsigned short hmacsha256keydatalen = 32;
	SLC_BYTE hmacsha256keydata[32] = {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
									 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};


	time(&t);

	printf("#########################--HMAC TEST--#######################\n\n");

	////==============================���������Կ�ļ�==================================
	slcret = SlcHmacGenerateKey(hmacmd5keydata, 
										hmacmd5keydatalen*8,
										SLC_CIPHER_ALGO_HMAC_MD5,
										&skhmacmd5key
                        );
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ���������HMAC-MD5��Կslkey�ļ����ݴ���\n");
		goto endtest;
	}
	printf("ʹ���������HMAC-MD5��Կslkey�ļ����ݳɹ���\n");

	slcret = SlcHmacGenerateKey(hmacsha1keydata, 
										hmacsha1keydatalen*8,
										SLC_CIPHER_ALGO_HMAC_SHA1,
										&skhmacsha1key
                        );
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ���������HMAC-SHA1��Կslkey�ļ����ݴ���\n");
		goto endtest;
	}
	printf("ʹ���������HMAC-SHA1��Կslkey�ļ����ݳɹ���\n");

	slcret = SlcHmacGenerateKey(hmacsha256keydata, 
										hmacsha256keydatalen*8,
										SLC_CIPHER_ALGO_HMAC_SHA256,
										&skhmacsha256key
                        );
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ���������HMAC-SHA256��Կslkey�ļ����ݴ���\n");
		goto endtest;
	}
	printf("ʹ���������HMAC-SHA256��Կslkey�ļ����ݳɹ���\n");

	printf("--------------------HMAC������Կ��ɣ�--------------------\n");	

	////==============================HMAC-SHA1==================================
	slcret = SlcHmac(skhmacmd5key, testdata, sizeof(testdata), softdigestkey, sizeof(softdigestkey), &softdigestlenkey);
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ��slkey���Hmac-md5�������ݴ���\n");
		goto endtest;
	}
	printf("ʹ��slkey���Hmac-md5�������ݳɹ���\n");

	// ------------- ʹ������������HMAC-MD5��������----------------------
	slcret = SlcHmacRaw(SLC_CIPHER_ALGO_HMAC_MD5, hmacmd5keydata, sizeof(hmacmd5keydata), testdata, sizeof(testdata), softdigestraw, sizeof(softdigestraw), &softdigestlenraw);
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ����Կ�������Hmac-md5�������ݴ���\n");
		goto endtest;
	}
	printf("ʹ����Կ�������Hmac-md5�������ݳɹ���\n");

	if (softdigestlenkey != softdigestlenraw
		|| memcmp(softdigestraw, softdigestkey, softdigestlenraw) )
	{
		printf("slkey����Կ���ݼ��㲻ƥ�䣡\n");
		goto endtest;
	}
	
	printf("--------------------HMAC-MD5���������ɣ�--------------------\n");

	////==============================HMAC-SHA1==================================
	// ------------- ʹ�����HMAC-SHA1��������----------------------
	slcret = SlcHmac(skhmacsha1key, testdata, sizeof(testdata), softdigestkey, sizeof(softdigestkey), &softdigestlenkey);
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ��slkey���Hmac-SHA1�������ݴ���\n");
		goto endtest;
	}
	printf("ʹ��slkey���Hmac-SHA1�������ݳɹ���\n");


	// ------------- ʹ������������HMAC-SHA1��������----------------------
	slcret = SlcHmacRaw(SLC_CIPHER_ALGO_HMAC_SHA1, hmacsha1keydata, sizeof(hmacsha1keydata), testdata, sizeof(testdata), softdigestraw, sizeof(softdigestraw), &softdigestlenraw);
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ����Կ�������Hmac-SHA1�������ݴ���\n");
		goto endtest;
	}
	printf("ʹ����Կ�������Hmac-SHA1�������ݳɹ���\n");

	if (softdigestlenkey != softdigestlenraw
		|| memcmp(softdigestraw, softdigestkey, softdigestlenraw) )
	{
		printf("slkey����Կ���ݼ��㲻ƥ�䣡\n");
		goto endtest;
	}
	
	printf("--------------------HMAC-SHA1���������ɣ�--------------------\n");	

	//==============================HMAC-SHA256==================================
	// ------------- ʹ�����HMAC-SHA256��������----------------------
	slcret = SlcHmac(skhmacsha256key, testdata, sizeof(testdata), softdigestkey, sizeof(softdigestkey), &softdigestlenkey);
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ��slkey���Hmac-SHA256�������ݴ���\n");
		goto endtest;
	}
	printf("ʹ��slkey���Hmac-SHA256�������ݳɹ���\n");


	// ------------- ʹ������������HMAC-SHA256��������----------------------
	slcret = SlcHmacRaw(SLC_CIPHER_ALGO_HMAC_SHA256, hmacsha256keydata, sizeof(hmacsha256keydata), testdata, sizeof(testdata), softdigestraw, sizeof(softdigestraw), &softdigestlenraw);
	if (slcret != SLC_SUCCESS)
	{
		printf("ʹ����Կ�������Hmac-SHA256�������ݴ���\n");
		goto endtest;
	}
	printf("ʹ����Կ�������Hmac-SHA256�������ݳɹ���\n");

	if (softdigestlenkey != softdigestlenraw
		|| memcmp(softdigestraw, softdigestkey, softdigestlenraw) )
	{
		printf("slkey����Կ���ݼ��㲻ƥ�䣡\n");
		goto endtest;
	}
	
	printf("--------------------HMAC-SHA256���������ɣ�--------------------\n");	

	//==============================END==================================

endtest:
	printf("\n������ɣ����������������������������������������\n");
	getchar();

	return 0;
}
