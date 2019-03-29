/*************************************************************
*CopyRight(c) Beijing Senselock Inc
*
* [当前版本] 1.0.0.0
*
* [作    者] lining
*
* [完成日期] 2013/04/02 
*
*------------------------------------------------------------
*[历史修订记录]
*
* [修改时间]      [版本]        [作者]         [修改记录说明]
*____________   ___________   __________   ___________________ 
*____________   ___________   __________   ___________________
*____________   ___________   __________   ___________________
*------------------------------------------------------------
*************************************************************/


#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#include <windows.h>
#include <stdio.h>
#include "helper.h"
#include <time.h>


void CheckResult( DWORD lRetCode, DWORD lCode, LPSTR lpszMsg )
{
	
	struct tm *now;
	time_t  sec = time(NULL);
	
	char    bucMessage[1024];
	FILE *f;

	now = localtime(&sec);
	printf("%d-%02d-%02d %02d:%02d:%02d    ", (now->tm_year+1900), (now->tm_mon + 1), now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	
	if ( lRetCode == (lCode) )
	{
		printf( "%s....[OK]\n", lpszMsg );
	}
	else
	{
		
		printf( "%s....[FAILED],tErrCode = 0x%08X.exceptcode= 0x%08X\r\n", lpszMsg,lRetCode ,lCode);
		f = fopen("log.txt", "a+");
		//sprintf( bucMessage, "%s....[FAILED]\t\t\tErrCode = 0x%08X.\r\n", lpszMsg,lRetCode  );
		sprintf( bucMessage, "%d-%02d-%02d %02d:%02d:%02d    %s....[FAILED]\t\t\treturnCode = 0x%08X. exceptcode = 0x%08X.\r\n", (now->tm_year+1900), (now->tm_mon + 1), now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec,lpszMsg,lRetCode  ,lCode);
		fwrite( bucMessage, 0x01, strlen(bucMessage)+1, f );
		fclose( f );
		getchar();
	}
}

//针对返回值不为0表示执行成功的函数
void CheckResult2( DWORD lRetCode, DWORD lCode, LPSTR lpszMsg )
{

	struct tm *now;
	time_t  sec = time(NULL);
	
	char    bucMessage[1024];
	FILE *f;

	now = localtime(&sec);

	printf("%d-%02d-%02d %02d:%02d:%02d    ", (now->tm_year+1900), (now->tm_mon + 1), now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);


	if ( lRetCode != (lCode) )
	{
		printf( "%s....[OK]\n", lpszMsg );
	}
	else
	{

		printf( "%s....[FAILED],tErrCode = 0x%08X.exceptcode= 0x%08X\r\n", lpszMsg,lRetCode ,lCode);
		f = fopen("log.txt", "a+");
		//sprintf( bucMessage, "%s....[FAILED]\t\t\tErrCode = 0x%08X.\r\n", lpszMsg,lRetCode  );
		sprintf( bucMessage, "%d-%02d-%02d %02d:%02d:%02d    %s....[FAILED]\t\t\treturnCode = 0x%08X. exceptcode = 0x%08X.\r\n", (now->tm_year+1900), (now->tm_mon + 1), now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec,lpszMsg,lRetCode  ,lCode);
		fwrite( bucMessage, 0x01, strlen(bucMessage)+1, f );
		fclose( f );
		getchar();
	}
}