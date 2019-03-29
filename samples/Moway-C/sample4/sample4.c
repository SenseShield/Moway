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
//4. 开发商（升级包）
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

	// ---------------先读取文件----------------------
	if (readkeyfile())
	{
		printf("读取秘钥文件失败！\n");
		getchar();
		return 1;
	}
	else
	{
		printf("读取本地秘钥文件成功！\n");
	}

	//A、开发商制作升级包
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

	//4.制作升级包（升级二进制数据文件）
	// ------------- 获得PID----------------------
	uiInOutLen = sizeof(devPID);
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID,&devPID,&uiInOutLen);
	if (MW_SUCCESS ==uiRet)
	{
		printf("获取设备信息成功\n" );
		printf("设备PID号 ：0x%08x\n", devPID);
	}
	else
	{
		printf("获取设备信息错误！ error = %d\n",uiRet);
	}

	// --------------------制作升级块，升级已有数据文件databig.evd到目标设备--------------------
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
		printf("制作升级块成功\n" );
	}
	else
	{
		printf("制作升级块错误！ error = %d\n",uiRet);
		getchar();
		return 1;
	}

	//5.关闭锁
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

	//B、用户导入升级包
	pDevInfoList = (MW_DEVICE_INFO_CTX *)malloc (sizeof(MW_DEVICE_INFO_CTX)*uiMaxDevCount);
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

	//4.导入升级包
	uiRet = mw_update( hHandle,updatemod,updatemodlenth);    
	if (MW_SUCCESS ==uiRet)
	{
		printf("升级数据文件databig.evd数据文件到设备成功\n" );
	}
	else
	{
		printf("升级数据文件databig.evd数据文件到设备错误！ error = %d\n",uiRet);
	}

	//5.验证升级包内容（读取验证内容与升级内容一致）
	//---------------------验证开发商PIN----------------------------------------------
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

	//--------------------分包获取锁内升级的文件内容----------------------------------------
	uiRet = mw_read_file(hHandle,"databig.evd",0,databigfilelen,szReadBuffer2);
	if (MW_SUCCESS !=uiRet )
	{
		printf("读文件失败 错误码= %d\n",uiRet);
		getchar();
		return 1;
	}
	else
	{
		printf("读文件成功 \n");
	}
	
	//--------------------比较文件内容----------------------------------------
	for (i = 0; i< databigfilelen; i++)
	{
		if (databigfiledata[i] != szReadBuffer2[i])
		{
			printf("比较本地文件与锁内升级文件内容不相等！！！！！ \n");
			getchar();
			return 1;
		}
	}
	printf("比较本地文件与锁内升级文件 成功 \n");

	//6.关闭锁
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
		printf("读取DATA BIG失败！");
		return 1;
	}
	databigfilelen = fread(databigfiledata, 1, 5000, f);
	fclose(f);


	return 0;
}