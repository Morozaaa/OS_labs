#include<Windows.h>
#include<processthreadsapi.h>
#include<stdio.h>
void reverse(){
	while (true) {
		char str[100];
		scanf_s("%s", str, 100);
		printf("%s\n", _strrev(str));
	}
}
void random() {
	while (true) {
		printf("%f\n", (double)rand() / RAND_MAX);
		Sleep(2000);
	}
}
BOOL WINAPI CtrlHandler(_In_ DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_C_EVENT) {
        printf("Thread ended with CTRL_C_EVENT");
        exit(1);
    }
    else if (dwCtrlType == CTRL_CLOSE_EVENT) {
        printf("Thread ended with CTRL_CLOSE_EVENT");
        exit(2);
    }
    else {
        return false;
    }
}

int main()
{
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reverse, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)random, NULL, 0, NULL);
    while(true){}
    return 0;
}


