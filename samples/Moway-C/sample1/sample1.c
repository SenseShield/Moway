#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/moway.h"

#define  BUFF_LEN_MAX  4096

//1. 设备信息获取-无权限要求
int main(int argc, char *argv[])
{
	unsigned int     uiRet = MW_SUCCESS;
	unsigned int     uiMaxDevCount = 32; 
	MW_DEVICE_INFO_CTX   *pDevInfoList = (MW_DEVICE_INFO_CTX *)malloc (sizeof(MW_DEVICE_INFO_CTX)*uiMaxDevCount);
	unsigned int     puiDevCount = 0;
	MWHANDLE         hHandle = NULL;
	
	MW_DEVICE_ALL_ITEM_INFO deviceAllItemInfo;
	unsigned int i = 0;
	unsigned char szBuffer[BUFF_LEN_MAX] = {0};
	unsigned int  uiBufferLen = BUFF_LEN_MAX;
	unsigned int uiPid = 0;
	unsigned int uiTimestamp = 0;
	unsigned int uiAllCapacity = 0; 
	unsigned int uiFreeCapacity = 0;
	unsigned int uiVersion = 0;
	unsigned char ucStatus = 0;
	unsigned int uiUpdateCode = 0;
	unsigned char ucUpdateCtrlCode = 0;

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

	//3.获取所有信息（结构体）
	uiBufferLen = sizeof(deviceAllItemInfo);
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_ALL,&deviceAllItemInfo,&uiBufferLen);
	printf("mw_get_device_info  MW_DEVICE_ALL_ITEM_INFO success :: \n");
	printf("PID = 0x%08x\n", deviceAllItemInfo.uiPID);
	printf("uiProduceDate = 0x%08x\n", deviceAllItemInfo.uiProduceDate);
	printf("uiAllCapacity = 0x%08x\n", deviceAllItemInfo.uiAllCapacity);
	printf("uiFreeCapacity = 0x%08x\n", deviceAllItemInfo.uiFreeCapacity);
	printf("uiVersion = 0x%08x\n", deviceAllItemInfo.uiVersion);
	printf("ucStatus = 0x%08x\n", deviceAllItemInfo.ucStatus);
	printf("uiUpdateCode = 0x%08x\n", deviceAllItemInfo.uiUpdateCode);
	printf("ucUpdateCtrlCode = 0x%08x\n", deviceAllItemInfo.ucUpdateCtrlCode);
	printf("mw_get_device_info SN success :");
	for (i=0;i<MW_SN_LENGTH;i++)
	{
		printf("%02x", deviceAllItemInfo.acSN[i]);
	}
	printf("\n\n");


	//4.获取指定信息（Item）
	//获取PID
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		uiPid = *((unsigned int*)szBuffer);
		printf("mw_get_device_info  PID success :: PID = 0x%08x\n", uiPid);
	}
	else
	{
		printf("mw_get_device_info PID failed !! error = %d\n",uiRet);
	}
	//获取SN
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_SN,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		printf("mw_get_device_info SN success :");
		for (i=0;i<MW_SN_LENGTH;i++)
		{
			printf("%02x", szBuffer[i]);
		}
		printf("\n");
	}
	else
	{
		printf("mw_get_device_info SN failed !! error = %d\n",uiRet);
	}

	//获取生产时间截
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PRODUCE_DATE,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		uiTimestamp = *((unsigned int*)szBuffer);
		printf("mw_get_device_info produce date success :: timestamp = 0x%08x\n", uiTimestamp);
	}
	else
	{
		printf("mw_get_device_info  produce date failed !! error = %d\n",uiRet);
	}

	//获取总容量
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_ALL_CAPACITY,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		uiAllCapacity = *((unsigned int*)szBuffer);
		printf("mw_get_device_info success :: uiAllCapacity = 0x%08x\n", uiAllCapacity);
	}
	else
	{
		printf("mw_get_device_info uiAllCapacity failed !! error = %d\n",uiRet);
	}

	//获取可用容量
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_FREE_CAPACITY,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		uiFreeCapacity = *((unsigned int*)szBuffer);
		printf("mw_get_device_info success :: uiFreeCapacity = 0x%08x\n", uiFreeCapacity);
	}
	else
	{
		printf("mw_get_device_info  uiFreeCapacity failed !! error = %d\n",uiRet);
	}

	//获取版本
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_VERSION,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		uiVersion = *((unsigned int*)szBuffer);
		printf("mw_get_device_info success :: uiVersion = 0x%08x\n", uiVersion);
	}
	else
	{
		printf("mw_get_device_info uiVersion failed !! error = %d\n",uiRet);
	}

	//获取外壳号
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_SHELL_SN,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		printf("mw_get_device_info success :: shell SN = ");
		for (i = 0; i< szBuffer[0]; i++)
		{
			printf("%c", szBuffer[i+1]);
		}
		printf("\n");
	}
	else
	{
		printf("mw_get_device_info shell SN failed !! error = %d\n",uiRet);
	}

	//获取设置状态
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_STATUS,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		ucStatus = szBuffer[0];
		printf("mw_get_device_info success :: ucStatus = 0x%08x\n", ucStatus);
	}
	else
	{
		printf("mw_get_device_info ucStatus failed !! error = %d\n",uiRet);
	}

	//获取设备远程升级码
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_UPDATE_CODE,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		uiUpdateCode = szBuffer[0];
		printf("mw_get_device_info success :: uiUpdateCode = 0x%08x\n", uiUpdateCode);
	}
	else
	{
		printf("mw_get_device_info uiUpdateCode failed !! error = %d\n",uiRet);
	}

	//获取远程升级控制锁标识
	memset(szBuffer,0,BUFF_LEN_MAX);
	uiBufferLen = BUFF_LEN_MAX;
	uiRet = mw_get_device_info(hHandle,MW_GET_INFO_ITEM_UPDATE_CTRL_CODE,szBuffer,&uiBufferLen);
	if (MW_SUCCESS ==uiRet)
	{
		ucUpdateCtrlCode = szBuffer[0];
		printf("mw_get_device_info success :: ucUpdateCtrlCode = 0x%08x\n", ucUpdateCtrlCode);
	}
	else
	{
		printf("mw_get_device_info ucUpdateCtrlCode failed !! error = %d\n",uiRet);
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
	getchar();
	return 0;

}
