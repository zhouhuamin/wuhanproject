// TestProfiler01.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestProfiler01.h"

#include <cstdio>
#include <ctime>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;


class Foo
{
public:
	Foo();
	~Foo();

	void Bar(const char* p_message);
};

Foo::Foo()
{
	printf("Foo initialization.\n");
}

Foo::~Foo()
{
	printf("Foo deinitilization.\n");
}

void Foo::Bar(const char* p_message)
{
	printf("%s\n", p_message);
}

int GetValue()
{
	return time(NULL) % 15;
}

int Multiply(int value1, int value2)
{
	int result = 0;
	for (int i = 0; i < value2; ++i)
		result += value1;

	return result;
}

int CalculateFactorial(int value)
{
	int result = 1;
	for (int i = 1; i <= value; ++i)
	{
		result = Multiply(result, i);
	}

	return result;
}


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

	Foo foo;
	foo.Bar("FooBar");
	const int value = GetValue();
	const int factorial = CalculateFactorial(value);
	printf("Factorial of %d is %d.\n", value, factorial);

	return nRetCode;
}
