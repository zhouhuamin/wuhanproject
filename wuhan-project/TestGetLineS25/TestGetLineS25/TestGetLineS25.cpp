// TestGetLineS25.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestGetLineS25.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

void UTF8ToGBK( char *&szOut );

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}


	//CString strSource = "";
	//CString strDest   = "xxxx";
	//char szData[3] = {0};
	//memcpy(szData, (LPCTSTR))




	FILE *fp=NULL;
	wchar_t test[2048];
	fp=fopen("E:\\�人������Ŀ\\TestGetS\\Debug\\CN_MT4101_1p0-4708000001-4708-20140318101010123508.XML","r");
	if (fp == NULL)
		return 1;

	FILE *fp2 = NULL;
	fp2 = fopen("E:\\�人������Ŀ\\TestGetS\\Debug\\CN_MT4101_1p0-4708000001-4708-20140318101010123508X.XML","w");
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
	//����
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, NULL, 0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, (LPWSTR)wszGBK, len);

	//����
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	szGBK = new char[len+1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	//szOut = szGBK; //�����õ���szOut����ȷ����Ϊ�˾������ǽ�szGBK���׵�ַ����szOut����delete []szGBKִ�к�szGBK����

	//��ռ佫���ͷţ���ʱ���ò���szOut������

	memset(szOut,'\0',strlen(szGBK)+1); //�Ľ�szGBK�����ݸ���szOut ��������ʹszGBK���ͷ�Ҳ�ܵõ���ȷ��ֵ
	memcpy(szOut,szGBK,strlen(szGBK));


	delete []szGBK;
	delete []wszGBK;
}

