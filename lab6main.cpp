#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h> 

using namespace std;




int main(int argc, char** argv) {
cout << "Process 1" << endl;
STARTUPINFO si1;
PROCESS_INFORMATION pi1;

//Tworzenie procesu
ZeroMemory(&si1, sizeof(si1));
si1.cb = sizeof(si1);
ZeroMemory(&pi1, sizeof(pi1));
char arg1[] = "lab6B.exe ";
strcat(arg1,argv[1]);
CreateProcess(NULL,(LPSTR)arg1,NULL,NULL,true,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si1,&pi1);
srand(time(0));

int N = strtol(argv[1],NULL,10);

cout << "Wartosc N: " << N << endl;
int buffer = 0;
string bufferchar;
//Tworzenie pliku
HANDLE handleFile = CreateFile("liczby.txt",FILE_GENERIC_WRITE|FILE_GENERIC_READ,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
//Losowanie i zapisywanie do pliku
SetFilePointer(handleFile, 0, NULL, FILE_END);
for(int i = 0;i<N;i++)
{
buffer = 1 + rand()%100;
bufferchar = to_string(buffer);
WriteFile(handleFile,bufferchar.c_str(),4,NULL,NULL);

//cout << buffer << endl;
}
CloseHandle(handleFile);
WaitForSingleObject(pi1.hProcess,INFINITE);

ExitProcess(0);
    return 0;
}