#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/moway.h"

#define  BUFF_LEN_MAX  4096

#define  BUFF_LEN_BIG_MAX  10000

#define TEST_DATA_BIG_FILE_NAME     "databig.evd"

unsigned char databigfiledata[5000];
unsigned short databigfilelen;

unsigned char   szReadBuffer1[BUFF_LEN_BIG_MAX]={0};
unsigned char   szReadBuffer2[BUFF_LEN_BIG_MAX]={0};

unsigned int   uiReadBufferLen1 = BUFF_LEN_BIG_MAX; 
unsigned int   uiReadBufferLen2 = BUFF_LEN_BIG_MAX; 

int readkeyfile(void);
//4. �����̣���������
int main(int argc, char *argv[])
{
	unsigned int     uiRet = MW_SUCCESS;
	unsigned int     uiMaxDevCount = 32; 
	MW_DEVICE_INFO_CTX   *pDevInfoList = (MW_DEVICE_INFO_CTX *)malloc (sizeof(MW_DEVICE_INFO_CTX)*uiMaxDevCount);
	unsigned int     puiDevCount = 0;
	MWHANDLE         hHandle = NULL;

	unsigned int       devPID;
	unsigned int       uiInOutLen = 0;
	MW_UPDADE_FILE_CTX   fileCtxList[2];
	unsigned char updatemod[100000] = {0};
	unsigned int updatemodlenth = 100000;

	FILE *pBigFile = NULL;
	unsigned int   uiTemp = 0;
	unsigned int   uiCount = 0;
	unsigned int  uiAreadyCopy = 0;
	unsigned int  i = 0;
	unsigned char   szReadBufferTemp[BUFF_LEN_MAX]={0};

	// ---------------�ȶ�ȡ�ļ�----------------------
	if (readkeyfile())
	{
		printf("��ȡ��Կ�ļ�ʧ�ܣ�\n");
		getchar();
		return 1;
	}
	else
	{
		printf("��ȡ������Կ�ļ��ɹ���\n");
	}

	//A������������������
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

	//4.���������������������������ļ���
	// ------------- ���PID----------------------
	uiInOutLen = sizeof(devPID);
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID,&devPID,&uiInOutLen);
	if (MW_SUCCESS ==uiRet)
	{
		printf("��ȡ�豸��Ϣ�ɹ�\n" );
		printf("�豸PID�� ��0x%08x\n", devPID);
	}
	else
	{
		printf("��ȡ�豸��Ϣ���� error = %d\n",uiRet);
	}

	// --------------------���������飬�������������ļ�databig.evd��Ŀ���豸--------------------
	memset(fileCtxList,0,sizeof(fileCtxList));
	fileCtxList[0].stFileProperty.ucType = MW_FILE_TYPE_BINARY;
	fileCtxList[0].stFileProperty.usPrivilege = MW_FILE_PRIV_TYPE_READ;
	fileCtxList[0].stFileProperty.uiSize = databigfilelen;
	fileCtxList[0].pData = databigfiledata;
	fileCtxList[0].uiDateSize = databigfilelen;
	strcpy(fileCtxList[0].stFileProperty.acName,"databig.evd");//databig.evd

	uiRet = mw_make_update_pkg(hHandle, devPID,NULL,fileCtxList,1,updatemod,&updatemodlenth);
	if (MW_SUCCESS ==uiRet)
	{
		printf("����������ɹ�\n" );
	}
	else
	{
		printf("������������� error = %d\n",uiRet);
		getchar();
		return 1;
	}

	//5.�ر���
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

	//B���û�����������
	pDevInfoList = (MW_DEVICE_INFO_CTX *)malloc (sizeof(MW_DEVICE_INFO_CTX)*uiMaxDevCount);
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

	//3.��֤�û�PIN
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

	//4.����������
	uiRet = mw_update( hHandle,updatemod,updatemodlenth);    
	if (MW_SUCCESS ==uiRet)
	{
		printf("���������ļ�databig.evd�����ļ����豸�ɹ�\n" );
	}
	else
	{
		printf("���������ļ�databig.evd�����ļ����豸���� error = %d\n",uiRet);
	}

	//5.��֤���������ݣ���ȡ��֤��������������һ�£�
	//---------------------��֤������PIN----------------------------------------------
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

	//--------------------�ְ���ȡ�����������ļ�����----------------------------------------
	uiRet = mw_read_file(hHandle,"databig.evd",0,databigfilelen,szReadBuffer2);
	if (MW_SUCCESS !=uiRet )
	{
		printf("���ļ�ʧ�� ������= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("���ļ��ɹ� \n");
	}
	
	//--------------------�Ƚ��ļ�����----------------------------------------
	for (i = 0; i< databigfilelen; i++)
	{
		if (databigfiledata[i] != szReadBuffer2[i])
		{
			printf("�Ƚϱ����ļ������������ļ����ݲ���ȣ��������� \n");
			getchar();
			return 1;
		}
	}
	printf("�Ƚϱ����ļ������������ļ� �ɹ� \n");

	//6.�ر���
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

	f = fopen(TEST_DATA_BIG_FILE_NAME, "rb");
	if (!f)
	{
		printf("��ȡDATA BIGʧ�ܣ�");
		return 1;
	}
	databigfilelen = fread(databigfiledata, 1, 5000, f);
	fclose(f);


	return 0;
}