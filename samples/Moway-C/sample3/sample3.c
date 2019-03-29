#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/moway.h"

#define  BUFF_LEN_MAX  4096

unsigned char aes128keyfiledata[128];
unsigned short aes128keyfilelen;
unsigned char harddigest[2048];
unsigned int harddigestlen;
#define TEST_AES128_KEY_FILE_NAME   "aes_128.sls"
#define TEST_HMAC_SHA1_KEY_FILE_NAME     "hmac_sha1.sls"
unsigned char ucIV[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
	0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x1f, };

//3. 用户使用
int main(int argc, char *argv[])
{
	unsigned int     uiRet = MW_SUCCESS;
	unsigned int     uiMaxDevCount = 32; 
	MW_DEVICE_INFO_CTX   *pDevInfoList = (MW_DEVICE_INFO_CTX *)malloc (sizeof(MW_DEVICE_INFO_CTX)*uiMaxDevCount);
	unsigned int     puiDevCount = 0;
	MWHANDLE         hHandle = NULL;

	unsigned short maxTryCount = 0;
	char szOldUserPin[9]="12345678";
	char szNewUserPin[9]="12345678";
	unsigned char   szReadBuffer[4096]={0};

	unsigned char outdata[3008] = {0};
	unsigned int outlen = 3008;
	unsigned char testdata[3008] = {0};
	unsigned int testlen = 3008;
	unsigned int i = 0;
	

	//1.枚举锁
	uiRet = mw_enum(pDevInfoList,uiMaxDevCount,&puiDevCount);
	if (MW_SUCCESS !=uiRet || puiDevCount < 1) //确保最少枚举到1个锁，以便后续操作。
	{
		printf("error :: no moway device can be find !!!\n");
		getchar();
		free(pDevInfoList);
		return 1;
	}

	//2.打开锁
	uiRet = mw_open(&pDevInfoList[0],1,&hHandle); //打开枚举列表中的第一把锁，如果存在多把锁，则可以用 pDevInfoList[1] pDevInfoList[2] ....
	if (MW_SUCCESS !=uiRet )
	{
		printf("error :: open moway device error!! ,errorcode = %x\n",uiRet);
		getchar();
		free(pDevInfoList);
		return 1;
	}
	printf("mw_open success.\n");
	free(pDevInfoList);
	pDevInfoList = NULL;

	//3.验证用户PIN
	uiRet = mw_verify_pin(hHandle,MW_PIN_TYPE_USER,"12345678");
	if (MW_SUCCESS ==uiRet)
	{
		printf("mw_verify_pin success\n" );
	}
	else
	{
		printf("mw_verify_pin failed error = %x\n",uiRet);
		getchar();
		return 1;
	}
	//4.修改用户PIN
	uiRet = mw_change_pin(hHandle,MW_PIN_TYPE_USER,maxTryCount,szNewUserPin,szNewUserPin);
	if (MW_SUCCESS ==uiRet)
	{
		printf("mw_change_pin  success\n" );
	}
	else
	{
		printf("mw_change_pin  failed error = %d\n",uiRet);
		getchar();
		return 1;
	}
	//5.读只读文件
	uiRet = mw_read_file(hHandle,"userrd.evd",0,3,szReadBuffer);
	if (MW_SUCCESS !=uiRet )
	{
		printf("读用户只读文件测试失败 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("读用户只读文件测试成功 \n");
	}

	//6.修改可读写数据文件
	uiRet = mw_write_file(hHandle,"userwt.evd",0,3,"def");
	if (MW_SUCCESS !=uiRet )
	{
		printf("修改可读写数据文件失败 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("修改可读写数据文件成功 \n");
	}

	//7.使用密钥文件加解密（AES128）
	testlen = 3008;
	for(i=0;i<testlen;i++)
	{
		testdata[i] = (unsigned char)i%256;	
	}
	// ------------- 使用设备密钥文件aes128 ECB加密数据----------------------
	uiRet = mw_sym_encrypt(hHandle, TEST_AES128_KEY_FILE_NAME, MW_SYM_ALGO_MODE_ECB, NULL,testdata,testlen,outdata,&outlen);
	if (MW_SUCCESS !=uiRet )
	{
		printf("使用设备密钥文件aes128 ECB加密数据错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("使用设备密钥文件aes128 ECB加密数据成功 \n");
	}

	// ------------- 使用设备密钥文件aes128 ECB解密数据---------------------
	memset(testdata,0,3008);
	testlen = 3008;
	uiRet = mw_sym_decrypt(hHandle,TEST_AES128_KEY_FILE_NAME,MW_SYM_ALGO_MODE_ECB,NULL,outdata,outlen,testdata,&testlen);
	if (MW_SUCCESS !=uiRet )
	{
		printf("使用设备密钥文件aes128 ECB解密数据错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("使用设备密钥文件aes128 ECB解密数据成功 \n");
	}
	// ------------- 使用设备密钥文件aes128 CBC加密数据----------------------
	uiRet = mw_sym_encrypt(hHandle, TEST_AES128_KEY_FILE_NAME, MW_SYM_ALGO_MODE_CBC, ucIV,testdata,testlen,outdata,&outlen);
	if (MW_SUCCESS !=uiRet )
	{
		printf("使用设备密钥文件aes128 CBC加密数据错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("使用设备密钥文件aes128 CBC加密数据成功 \n");
	}

	// ------------- 使用设备密钥文件aes128 CBC解密数据---------------------
	memset(testdata,0,3008);
	testlen = 3008;
	uiRet = mw_sym_decrypt(hHandle,TEST_AES128_KEY_FILE_NAME,MW_SYM_ALGO_MODE_CBC,ucIV,outdata,outlen,testdata,&testlen);
	if (MW_SUCCESS !=uiRet )
	{
		printf("使用设备密钥文件aes128 CBC解密数据错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("使用设备密钥文件aes128 CBC解密数据成功 \n");
	}

	//8.使用密钥文件（HMAC）计算Hash值
	harddigestlen = 2048;
	uiRet = mw_hmac_calc(hHandle,TEST_HMAC_SHA1_KEY_FILE_NAME,testdata,testlen,harddigest,&harddigestlen);
	if (MW_SUCCESS !=uiRet )
	{
		printf("使用设备Hmac-sha1密钥计算消息摘要错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("使用设备Hmac-sha1密钥计算消息摘要成功 \n");
	}

	//9.关闭锁
	uiRet = mw_close(hHandle);
	if (MW_SUCCESS !=uiRet )
	{
		printf("close moway device error ,errorcode = %d\n",uiRet);
		return 1;
	}
	else
	{
		printf("close moway device success \n");
	}
	getchar();
	return 0;

}
