// TestGetLineS25.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestGetLineS25.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

void UTF8ToGBK( char *&szOut );

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}


	//CString strSource = "";
	//CString strDest   = "xxxx";
	//char szData[3] = {0};
	//memcpy(szData, (LPCTSTR))




	FILE *fp=NULL;
	wchar_t test[2048];
	fp=fopen("E:\\武汉物流项目\\TestGetS\\Debug\\CN_MT4101_1p0-4708000001-4708-20140318101010123508.XML","r");
	if (fp == NULL)
		return 1;

	FILE *fp2 = NULL;
	fp2 = fopen("E:\\武汉物流项目\\TestGetS\\Debug\\CN_MT4101_1p0-4708000001-4708-20140318101010123508X.XML","w");
	if (fp2 == NULL)
		return 1;
	while(!feof(fp))
	{
		//if(fgets(test,100,fp)!=NULL)fgetws
		if(fgetws(test,2048,fp)!=NULL)
		{
			wprintf(_T("%s"),test);

			//CString strLeft;
			//CString strMid;
			//CString strRight;

			//char szNote[2048] = {0};
			//char *pTmp = szNote;
			//memcpy(szNote, test, sizeof(szNote) - 1);
			//UTF8ToGBK(pTmp);
		
			//USES_CONVERSION;

			CString strLine = test;
			//strLine.Format("%s", szNote);

			DWORD nLeft = strLine.Find(_T(">"));
			if (nLeft >= 0)
			{
				CString strLeft = strLine.Left(nLeft + 1);
				DWORD nRight = strLine.ReverseFind(_T('<'));
				if (nLeft < nRight)
				{
					CString strRight = strLine.Mid(nRight);
					CString strMid = strLine.Mid(nLeft + 1, nRight - nLeft - 1);
					CString strReplace = _T("");

					int nFlag = 0;
					int nRet = strMid.Find(_T('<'));
					if (nRet >= 0)
					{
						strReplace.Format(_T("<![CDATA[%s]]>"), strMid);

						//strMid.Replace(strMid, strReplace);

						CString strDest = strLeft + strReplace + strRight;
						int nLen = strDest.GetLength();
						memset(test, 0x00, sizeof(wchar_t) * 2048);
						memcpy(test, strDest.GetBuffer(), strDest.GetLength() * sizeof(TCHAR));
						strDest.ReleaseBuffer();
						nFlag++;
					}

					nRet = strMid.Find(_T('>'));
					if (nRet >= 0 && nFlag == 0)
					{
						strReplace.Format(_T("<![CDATA[%s]]>"), strMid);

						//strMid.Replace(strMid, strReplace);

						CString strDest = strLeft + strReplace + strRight;
						int nLen = strDest.GetLength();
						memset(test, 0x00, sizeof(wchar_t) * 2048);
						memcpy(test, strDest.GetBuffer(), strDest.GetLength() * sizeof(TCHAR));
						strDest.ReleaseBuffer();	
						nFlag++;
					}

					nRet = strMid.Find(_T('&'));
					if (nRet >= 0 && nFlag == 0)
					{
						strReplace.Format(_T("<![CDATA[%s]]>"), strMid);

						//strMid.Replace(strMid, strReplace);

						CString strDest = strLeft + strReplace + strRight;
						int nLen = strDest.GetLength();
						memset(test, 0x00, sizeof(wchar_t) * 2048);
						memcpy(test, strDest.GetBuffer(), strDest.GetLength() * sizeof(TCHAR));
						strDest.ReleaseBuffer();
						nFlag++;
					}

					nRet = strMid.Find(_T('\''));
					if (nRet >= 0 && nFlag == 0)
					{
						strReplace.Format(_T("<![CDATA[%s]]>"), strMid);

						//strMid.Replace(strMid, strReplace);

						CString strDest = strLeft + strReplace + strRight;
						int nLen = strDest.GetLength();
						memset(test, 0x00, sizeof(wchar_t) * 2048);
						memcpy(test, strDest.GetBuffer(), strDest.GetLength() * sizeof(TCHAR));
						strDest.ReleaseBuffer();
						nFlag++;
					}

					nRet = strMid.Find(_T('\"'));
					if (nRet >= 0 && nFlag == 0)
					{
						strReplace.Format(_T("<![CDATA[%s]]>"), strMid);

						//strMid.Replace(strMid, strReplace);

						CString strDest = strLeft + strReplace + strRight;
						int nLen = strDest.GetLength();
						wmemset(test, (wchar_t)0x00, 2048);
						wmemcpy(test, strDest.GetBuffer(), strDest.GetLength());
						strDest.ReleaseBuffer();
						nFlag++;
					}

				}
			}

			fputws(test, fp2);
			//sscanf(test, "<%s>%s</%s>\r\n", strLeft, strMid, strRight);

			//strLine.Format("<%s>%s</%s>\r\n", strLeft, strMid, strRight);
			//int nPos = strLine.Find("<");
			//memset(test, 0x00, sizeof(test));
		}
	}
	fclose(fp);fclose(fp2);







	return nRetCode;
}

void UTF8ToGBK( char *&szOut )
{
	unsigned short *wszGBK;
	char *szGBK;
	//长度
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, NULL, 0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, (LPWSTR)wszGBK, len);

	//长度
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	szGBK = new char[len+1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	//szOut = szGBK; //这样得到的szOut不正确，因为此句意义是将szGBK的首地址赋给szOut，当delete []szGBK执行后szGBK的内

	//存空间将被释放，此时将得不到szOut的内容

	memset(szOut,'\0',strlen(szGBK)+1); //改将szGBK的内容赋给szOut ，这样即使szGBK被释放也能得到正确的值
	memcpy(szOut,szGBK,strlen(szGBK));


	delete []szGBK;
	delete []wszGBK;
}

