// dlltester.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "pck_handle.h"
#include <iostream>

#define TEST_BASE_DIR	"d:\\pcktest\\"
#define TEST_SRC_DIR	"gfx"
int nVersionCount = 0;


//版本列表
int list_version()
{
	nVersionCount = pck_getVersionCount();
	printf("nVersionCount = %d\r\n", nVersionCount);

	for (int i = 0; i < nVersionCount; i++) {
		wprintf(L"id=%d, name=%s\r\n", i, pck_getVersionNameById(i));
	}
	return nVersionCount;
}

//新建pck
int create_pck(const wchar_t *_src, const wchar_t *_dst)
{
	return do_CreatePckFile(_src, _dst, 0);
}

//解压全部文件
int unpack_all(const wchar_t *_dst, const wchar_t *_src)
{
	return do_ExtractAllFiles(_src, _dst);
}


int main()
{
	setlocale(LC_CTYPE, ".936");

    std::cout << "Hello World!\n"; 

	//版本列表
	list_version();
	//unpack_all(L"D:\\pcktest\\extractAllTest", L"H:\\SourceCode\\VC\\WinPCK\\testpck\\gfx.pck");

	printf("unpack_all finish\r\n");

	create_pck(L"D:\\pcktest\\extractAllTest", L"D:\\pcktest\\createed.pck");

	printf("unpack_all finish\r\n");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
