// TestGetS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "afx.h"

int main(int argc, char* argv[])
{

	DWORD dwLen = 0;
	dwLen -= 1;

	if (dwLen > 0)
	{
		printf("xxxxx World!\n");
	}



	printf("Hello World!\n");
	FILE *fp=NULL;
	wchar_t test[2048];
	fp=fopen("E:\\武汉物流项目\\TestGetS\\Debug\\CN_MT4101_1p0-4708000001-4708-20140318101010123508.XML","r");

	FILE *fp2 = NULL;
	fp2 = fopen("E:\\武汉物流项目\\TestGetS\\Debug\\CN_MT4101_1p0-4708000001-4708-20140318101010123508X.XML","w");
	while(!feof(fp))
	{
		//if(fgets(test,100,fp)!=NULL)fgetws
		if(fgetws(test,100,fp)!=NULL)
		{
			printf("%s",test);

			CString strLeft;
			CString strMid;
			CString strRight;
			CString strLine = test;

			//sscanf(test, "<%s>%s</%s>\r\n", strLeft, strMid, strRight);

			//strLine.Format("<%s>%s</%s>\r\n", strLeft, strMid, strRight);
			//int nPos = strLine.Find("<");
			//memset(test, 0x00, sizeof(test));

			//fputws(test, )
		}
	}
	fclose(fp);

	return 0;
}

