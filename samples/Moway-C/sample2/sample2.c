#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/moway.h"

#define  BUFF_LEN_MAX  4096
unsigned char aes128keyfiledata[128];
unsigned short aes128keyfilelen;
unsigned char hmacsha1filedata[128];
unsigned short hmacsha1filelen;
#define TEST_AES128_KEY_FILE_NAME   "aes_128.sls"
#define TEST_HMAC_SHA1_KEY_FILE_NAME     "hmac_sha1.sls"
int readkeyfile(void);

//2. 开发商设置锁
int main(int argc, char *argv[])
{
	unsigned int     uiRet = MW_SUCCESS;
	unsigned int     uiMaxDevCount = 32; 
	MW_DEVICE_INFO_CTX   *pDevInfoList = (MW_DEVICE_INFO_CTX *)malloc (sizeof(MW_DEVICE_INFO_CTX)*uiMaxDevCount);
	unsigned int     puiDevCount = 0;
	MWHANDLE         hHandle = NULL;

	unsigned short maxTryCount = 0;
	char szOldPin[25]="000000000000000000000000";
	char szNewPin[25]="123456781234567812345678";

	char szPIDSeed[33]="00000000000000000000000000000000";
	unsigned int iPid = 0;
	unsigned int uiInOutLen = 0;

	MW_FILE_PROPERTY   stFileProperty;
	
	// ---------------先读取本地文件----------------------
	if (readkeyfile())
	{
		printf("读取秘钥文件失败！\n");
		getchar();;
		return 1;
	}
	printf("读取本地秘钥文件成功！\n");

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

	//3.验证开发商PIN
	uiRet = mw_verify_pin(hHandle,MW_PIN_TYPE_DEVELOPER,"123456781234567812345678");
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

	//4.设置PID
	uiRet = mw_set_pid(hHandle,szPIDSeed,32);
	if (MW_SUCCESS ==uiRet)
	{
		printf("set PID success\n" );
	}
	else
	{
		printf("set PID failed error = %x\n",uiRet);
		getchar();
		return 1;
	}
	uiInOutLen = sizeof(unsigned int);
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID,&iPid,&uiInOutLen);
	if (MW_SUCCESS ==uiRet)
	{
		printf("get PID ：0x%08x\n", iPid);
	}
	else
	{
		printf("get pid failed error = %x\n",uiRet);
	}

	//5.修改开发商PIN
	uiRet = mw_change_pin(hHandle,MW_PIN_TYPE_DEVELOPER,maxTryCount,szNewPin,szNewPin);
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

	//6.创建文件（二进制、读写、密钥文件-AES128、密钥文件-HMAC）
	uiRet = mw_delete_file(hHandle,"userwt.evd"); //创建前先删除
	uiRet = mw_delete_file(hHandle,"userrd.evd"); //创建前先删除
	uiRet = mw_delete_file(hHandle,"userkey.sls"); //创建前先删除
	uiRet = mw_delete_file(hHandle,TEST_AES128_KEY_FILE_NAME); //创建前先删除
	uiRet = mw_delete_file(hHandle,TEST_HMAC_SHA1_KEY_FILE_NAME); //创建前先删除
	
	// ------------- 开发商创建二进制用户可读写文件----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_READ_WRITE;
	stFileProperty.uiSize = 0x100;
	strcpy(stFileProperty.acName,"userwt.evd");
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("创建文件失败 ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("创建文件成功\n");
	}
	// ------------- 开发商创建二进制用户只读文件----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_READ;
	stFileProperty.uiSize = 0x100;
	strcpy(stFileProperty.acName,"userrd.evd");
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("创建用户只读文件失败 ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("创建用户只读文件成功\n");
	}
	// ------------- 开发商创建密钥文件----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize = 0x100;
	strcpy(stFileProperty.acName,"userkey.sls");
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("创建密钥文件失败 ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("创建密钥文件成功\n");
	}
	// ------------- 开发商创建密钥文件----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	strcpy(stFileProperty.acName,TEST_AES128_KEY_FILE_NAME);
	stFileProperty.ucType = MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize = aes128keyfilelen;
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("创建密aes128钥文件错误 ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("创建密aes128钥文件成功 \n");
	}
	// ------------- 创建hmac-sha1密钥文件----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize = hmacsha1filelen;
	strcpy(stFileProperty.acName,TEST_HMAC_SHA1_KEY_FILE_NAME);
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("创建hmac-sha1密钥文件错误 ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("创建hmac-sha1密钥文件成功 \n");
	}

	//7.开发商权限写文件
	// ------------- 写用户可写二进制文件----------------------
	uiRet = mw_write_file(hHandle,"userwt.evd",0,3,"abc");
	if (MW_SUCCESS !=uiRet )
	{
		printf("写二进制用户读写文件测试失败 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("写二进制用户读写文件测试成功 \n");
	}

	// ------------- 写用户只读二进制文件--------------------
	uiRet = mw_write_file(hHandle,"userrd.evd",0,3,"abc");
	if (MW_SUCCESS !=uiRet )
	{
		printf("写用户只读二进制文件失败 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("写用户只读二进制文件测试成功 \n");
	}
	// ------------- 写用户密钥文件----------------------
	uiRet = mw_write_file(hHandle,"userkey.sls",0,3,"abc");
	if (MW_SUCCESS !=uiRet )
	{
		printf("开发商写二进制用户读写文件测试失败 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("开发商写二进制用户读写文件测试成功 \n");
	}
	// ------------- 写用户密钥文件----------------------
	uiRet = mw_write_file(hHandle,TEST_AES128_KEY_FILE_NAME,0,aes128keyfilelen,aes128keyfiledata);
	if (MW_SUCCESS !=uiRet )
	{
		printf("写aes128用户密钥文件错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("写aes128用户密钥文件成功 \n");
	}
	// ------------- 写用户hmac-sha1密钥文件----------------------
	uiRet = mw_write_file(hHandle,TEST_HMAC_SHA1_KEY_FILE_NAME,0,hmacsha1filelen,hmacsha1filedata);
	if (MW_SUCCESS !=uiRet )
	{
		printf("写用户hmac-sha1密钥文件错误 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("写用户hmac-sha1密钥文件成功 \n");
	}

	//8.关闭锁
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


int readkeyfile(void)
{	
	FILE *f;

	
	f = fopen(TEST_AES128_KEY_FILE_NAME, "rb");
	if (!f)
	{
		printf("读取AES128秘钥失败！");
		return 1;
	}
	aes128keyfilelen = fread(aes128keyfiledata, 1, 128, f);
	fclose(f);

	// HMAC-SHA1
	f = fopen(TEST_HMAC_SHA1_KEY_FILE_NAME, "rb");
	if (!f)
	{
		printf("读取HMAC-SHA1秘钥失败！");
		return 1;
	}
	hmacsha1filelen = fread(hmacsha1filedata, 1, 1024, f);
	fclose(f);
	
	return 0;
}
