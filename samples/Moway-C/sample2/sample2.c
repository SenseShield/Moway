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

//2. ������������
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
	
	// ---------------�ȶ�ȡ�����ļ�----------------------
	if (readkeyfile())
	{
		printf("��ȡ��Կ�ļ�ʧ�ܣ�\n");
		getchar();;
		return 1;
	}
	printf("��ȡ������Կ�ļ��ɹ���\n");

	//1.ö����
	uiRet = mw_enum(pDevInfoList,uiMaxDevCount,&puiDevCount);
	if (MW_SUCCESS !=uiRet || puiDevCount < 1) //ȷ������ö�ٵ�1�������Ա����������
	{
		printf("error :: no moway device can be find !!!\n");
		getchar();
		free(pDevInfoList);
		return 1;
	}

	//2.����
	uiRet = mw_open(&pDevInfoList[0],1,&hHandle); //��ö���б��еĵ�һ������������ڶ������������� pDevInfoList[1] pDevInfoList[2] ....
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

	//3.��֤������PIN
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

	//4.����PID
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
		printf("get PID ��0x%08x\n", iPid);
	}
	else
	{
		printf("get pid failed error = %x\n",uiRet);
	}

	//5.�޸Ŀ�����PIN
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

	//6.�����ļ��������ơ���д����Կ�ļ�-AES128����Կ�ļ�-HMAC��
	uiRet = mw_delete_file(hHandle,"userwt.evd"); //����ǰ��ɾ��
	uiRet = mw_delete_file(hHandle,"userrd.evd"); //����ǰ��ɾ��
	uiRet = mw_delete_file(hHandle,"userkey.sls"); //����ǰ��ɾ��
	uiRet = mw_delete_file(hHandle,TEST_AES128_KEY_FILE_NAME); //����ǰ��ɾ��
	uiRet = mw_delete_file(hHandle,TEST_HMAC_SHA1_KEY_FILE_NAME); //����ǰ��ɾ��
	
	// ------------- �����̴����������û��ɶ�д�ļ�----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_READ_WRITE;
	stFileProperty.uiSize = 0x100;
	strcpy(stFileProperty.acName,"userwt.evd");
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("�����ļ�ʧ�� ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("�����ļ��ɹ�\n");
	}
	// ------------- �����̴����������û�ֻ���ļ�----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_READ;
	stFileProperty.uiSize = 0x100;
	strcpy(stFileProperty.acName,"userrd.evd");
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("�����û�ֻ���ļ�ʧ�� ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("�����û�ֻ���ļ��ɹ�\n");
	}
	// ------------- �����̴�����Կ�ļ�----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize = 0x100;
	strcpy(stFileProperty.acName,"userkey.sls");
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("������Կ�ļ�ʧ�� ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("������Կ�ļ��ɹ�\n");
	}
	// ------------- �����̴�����Կ�ļ�----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	strcpy(stFileProperty.acName,TEST_AES128_KEY_FILE_NAME);
	stFileProperty.ucType = MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize = aes128keyfilelen;
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("������aes128Կ�ļ����� ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("������aes128Կ�ļ��ɹ� \n");
	}
	// ------------- ����hmac-sha1��Կ�ļ�----------------------
	memset(&stFileProperty,0,sizeof(stFileProperty));
	stFileProperty.ucType = MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize = hmacsha1filelen;
	strcpy(stFileProperty.acName,TEST_HMAC_SHA1_KEY_FILE_NAME);
	uiRet = mw_create_file(hHandle,&stFileProperty);
	if (MW_SUCCESS !=uiRet )
	{
		printf("����hmac-sha1��Կ�ļ����� ,errorcode = %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("����hmac-sha1��Կ�ļ��ɹ� \n");
	}

	//7.������Ȩ��д�ļ�
	// ------------- д�û���д�������ļ�----------------------
	uiRet = mw_write_file(hHandle,"userwt.evd",0,3,"abc");
	if (MW_SUCCESS !=uiRet )
	{
		printf("д�������û���д�ļ�����ʧ�� ������= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("д�������û���д�ļ����Գɹ� \n");
	}

	// ------------- д�û�ֻ���������ļ�--------------------
	uiRet = mw_write_file(hHandle,"userrd.evd",0,3,"abc");
	if (MW_SUCCESS !=uiRet )
	{
		printf("д�û�ֻ���������ļ�ʧ�� ������= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("д�û�ֻ���������ļ����Գɹ� \n");
	}
	// ------------- д�û���Կ�ļ�----------------------
	uiRet = mw_write_file(hHandle,"userkey.sls",0,3,"abc");
	if (MW_SUCCESS !=uiRet )
	{
		printf("������д�������û���д�ļ�����ʧ�� ������= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("������д�������û���д�ļ����Գɹ� \n");
	}
	// ------------- д�û���Կ�ļ�----------------------
	uiRet = mw_write_file(hHandle,TEST_AES128_KEY_FILE_NAME,0,aes128keyfilelen,aes128keyfiledata);
	if (MW_SUCCESS !=uiRet )
	{
		printf("дaes128�û���Կ�ļ����� ������= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("дaes128�û���Կ�ļ��ɹ� \n");
	}
	// ------------- д�û�hmac-sha1��Կ�ļ�----------------------
	uiRet = mw_write_file(hHandle,TEST_HMAC_SHA1_KEY_FILE_NAME,0,hmacsha1filelen,hmacsha1filedata);
	if (MW_SUCCESS !=uiRet )
	{
		printf("д�û�hmac-sha1��Կ�ļ����� ������= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("д�û�hmac-sha1��Կ�ļ��ɹ� \n");
	}

	//8.�ر���
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
		printf("��ȡAES128��Կʧ�ܣ�");
		return 1;
	}
	aes128keyfilelen = fread(aes128keyfiledata, 1, 128, f);
	fclose(f);

	// HMAC-SHA1
	f = fopen(TEST_HMAC_SHA1_KEY_FILE_NAME, "rb");
	if (!f)
	{
		printf("��ȡHMAC-SHA1��Կʧ�ܣ�");
		return 1;
	}
	hmacsha1filelen = fread(hmacsha1filedata, 1, 1024, f);
	fclose(f);
	
	return 0;
}
