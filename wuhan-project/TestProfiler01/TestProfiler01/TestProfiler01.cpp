// TestProfiler01.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestProfiler01.h"

#include <cstdio>
#include <ctime>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

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

	Foo foo;
	foo.Bar("FooBar");
	const int value = GetValue();
	const int factorial = CalculateFactorial(value);
	printf("Factorial of %d is %d.\n", value, factorial);

	return nRetCode;
}
