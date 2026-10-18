#include <stdio.h>
#include <windows.h>
#include "EKey.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Input: <Input file path to ekey format>\n");
		return 1;
	}
	HANDLE file = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, nullptr,
							  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	HANDLE map = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
	void *mem = MapViewOfFile(map, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(file);
	CloseHandle(map);
	EKey_Start(mem);
	return 0;
}
