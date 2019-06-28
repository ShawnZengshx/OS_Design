// OS课设2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "C:\\Codes\\cpp_codes\\kernel_lib\\kernel_lib\\\kernel_lib.h"

#pragma comment(lib, "C:\\Codes\\cpp_codes\\kernel_lib\\Debug\\kernel_lib.lib")
int main()
{
	cout << "启动系统..." << endl;

	return start_kernel();
}
