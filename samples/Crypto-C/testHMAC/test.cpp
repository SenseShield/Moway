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

	SLC_BYTE testdata[] = "this is test data！";

	unsigned short hmacmd5keydatalen = 16;
	SLC_BYTE hmacmd5keydata[16] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,};
	unsigned short hmacsha1keydatalen = 20;
	SLC_BYTE hmacsha1keydata[20] = {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,};
	unsigned short hmacsha256keydatalen = 32;
	SLC_BYTE hmacsha256keydata[32] = {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
									 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};


	time(&t);

	printf("#########################--HMAC TEST--#######################\n\n");

	////==============================软件生成秘钥文件==================================
	slcret = SlcHmacGenerateKey(hmacmd5keydata, 
										hmacmd5keydatalen*8,
										SLC_CIPHER_ALGO_HMAC_MD5,
										&skhmacmd5key
                        );
	if (slcret != SLC_SUCCESS)
	{
		printf("使用软件生成HMAC-MD5密钥slkey文件数据错误！\n");
		goto endtest;
	}
	printf("使用软件生成HMAC-MD5密钥slkey文件数据成功！\n");

	slcret = SlcHmacGenerateKey(hmacsha1keydata, 
										hmacsha1keydatalen*8,
										SLC_CIPHER_ALGO_HMAC_SHA1,
										&skhmacsha1key
                        );
	if (slcret != SLC_SUCCESS)
	{
		printf("使用软件生成HMAC-SHA1密钥slkey文件数据错误！\n");
		goto endtest;
	}
	printf("使用软件生成HMAC-SHA1密钥slkey文件数据成功！\n");

	slcret = SlcHmacGenerateKey(hmacsha256keydata, 
										hmacsha256keydatalen*8,
										SLC_CIPHER_ALGO_HMAC_SHA256,
										&skhmacsha256key
                        );
	if (slcret != SLC_SUCCESS)
	{
		printf("使用软件生成HMAC-SHA256密钥slkey文件数据错误！\n");
		goto endtest;
	}
	printf("使用软件生成HMAC-SHA256密钥slkey文件数据成功！\n");

	printf("--------------------HMAC生成密钥完成！--------------------\n");	

	////==============================HMAC-SHA1==================================
	slcret = SlcHmac(skhmacmd5key, testdata, sizeof(testdata), softdigestkey, sizeof(softdigestkey), &softdigestlenkey);
	if (slcret != SLC_SUCCESS)
	{
		printf("使用slkey软件Hmac-md5计算数据错误！\n");
		goto endtest;
	}
	printf("使用slkey软件Hmac-md5计算数据成功！\n");

	// ------------- 使用软件生成软件HMAC-MD5计算数据----------------------
	slcret = SlcHmacRaw(SLC_CIPHER_ALGO_HMAC_MD5, hmacmd5keydata, sizeof(hmacmd5keydata), testdata, sizeof(testdata), softdigestraw, sizeof(softdigestraw), &softdigestlenraw);
	if (slcret != SLC_SUCCESS)
	{
		printf("使用密钥数据软件Hmac-md5计算数据错误！\n");
		goto endtest;
	}
	printf("使用密钥数据软件Hmac-md5计算数据成功！\n");

	if (softdigestlenkey != softdigestlenraw
		|| memcmp(softdigestraw, softdigestkey, softdigestlenraw) )
	{
		printf("slkey与密钥数据计算不匹配！\n");
		goto endtest;
	}
	
	printf("--------------------HMAC-MD5计算测试完成！--------------------\n");

	////==============================HMAC-SHA1==================================
	// ------------- 使用软件HMAC-SHA1计算数据----------------------
	slcret = SlcHmac(skhmacsha1key, testdata, sizeof(testdata), softdigestkey, sizeof(softdigestkey), &softdigestlenkey);
	if (slcret != SLC_SUCCESS)
	{
		printf("使用slkey软件Hmac-SHA1计算数据错误！\n");
		goto endtest;
	}
	printf("使用slkey软件Hmac-SHA1计算数据成功！\n");


	// ------------- 使用软件生成软件HMAC-SHA1计算数据----------------------
	slcret = SlcHmacRaw(SLC_CIPHER_ALGO_HMAC_SHA1, hmacsha1keydata, sizeof(hmacsha1keydata), testdata, sizeof(testdata), softdigestraw, sizeof(softdigestraw), &softdigestlenraw);
	if (slcret != SLC_SUCCESS)
	{
		printf("使用密钥数据软件Hmac-SHA1计算数据错误！\n");
		goto endtest;
	}
	printf("使用密钥数据软件Hmac-SHA1计算数据成功！\n");

	if (softdigestlenkey != softdigestlenraw
		|| memcmp(softdigestraw, softdigestkey, softdigestlenraw) )
	{
		printf("slkey与密钥数据计算不匹配！\n");
		goto endtest;
	}
	
	printf("--------------------HMAC-SHA1计算测试完成！--------------------\n");	

	//==============================HMAC-SHA256==================================
	// ------------- 使用软件HMAC-SHA256计算数据----------------------
	slcret = SlcHmac(skhmacsha256key, testdata, sizeof(testdata), softdigestkey, sizeof(softdigestkey), &softdigestlenkey);
	if (slcret != SLC_SUCCESS)
	{
		printf("使用slkey软件Hmac-SHA256计算数据错误！\n");
		goto endtest;
	}
	printf("使用slkey软件Hmac-SHA256计算数据成功！\n");


	// ------------- 使用软件生成软件HMAC-SHA256计算数据----------------------
	slcret = SlcHmacRaw(SLC_CIPHER_ALGO_HMAC_SHA256, hmacsha256keydata, sizeof(hmacsha256keydata), testdata, sizeof(testdata), softdigestraw, sizeof(softdigestraw), &softdigestlenraw);
	if (slcret != SLC_SUCCESS)
	{
		printf("使用密钥数据软件Hmac-SHA256计算数据错误！\n");
		goto endtest;
	}
	printf("使用密钥数据软件Hmac-SHA256计算数据成功！\n");

	if (softdigestlenkey != softdigestlenraw
		|| memcmp(softdigestraw, softdigestkey, softdigestlenraw) )
	{
		printf("slkey与密钥数据计算不匹配！\n");
		goto endtest;
	}
	
	printf("--------------------HMAC-SHA256计算测试完成！--------------------\n");	

	//==============================END==================================

endtest:
	printf("\n测试完成，按任意键结束！！！！！！！！！！！！！！\n");
	getchar();

	return 0;
}
