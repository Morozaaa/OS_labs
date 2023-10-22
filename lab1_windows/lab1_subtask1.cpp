#include <windows.h>
#include <stdio.h>


int main()
{
	while(SetConsoleCtrlHandler(NULL, false)){
		continue;

	}
	return 0;
}
