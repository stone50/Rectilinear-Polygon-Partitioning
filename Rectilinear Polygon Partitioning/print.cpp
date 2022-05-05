#include <sstream>

#include "print.h"

void print(float f) {
	std::wstringstream wss;
	wss << f;
	OutputDebugStringW(wss.str().c_str());
}
void print(LPCWSTR s) {
	OutputDebugStringW(s);
}
void println(float f) {
	print(f);
	OutputDebugStringW(L"\n");
}
void println(LPCWSTR s) {
	print(s);
	OutputDebugStringW(L"\n");
}