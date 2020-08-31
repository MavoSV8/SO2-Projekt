#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h> 

using namespace std;




int main(int argc, char** argv) {


int i = 0;
int buffer = 0;
int wynik = 0;
int avg = 0;
int N = strtol(argv[1],NULL,10);
char bufferChar[10000] = {0};
HANDLE handleFile;
while(1) {
handleFile = CreateFile("liczby.txt",FILE_GENERIC_WRITE|FILE_GENERIC_READ,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
if (handleFile != INVALID_HANDLE_VALUE)
break;
Sleep(3000);
}
cout << "Process 2"<< endl;

for(i = 0;i<N;i++)
{
ReadFile(handleFile,bufferChar,4,NULL,NULL);
string bufferString = bufferChar;
buffer = stoi(bufferString,nullptr,10);
cout << buffer << endl;
wynik = wynik + buffer;
}

avg = wynik/i;
cout << "Average: " << avg << endl;
CloseHandle(handleFile);
ExitProcess(0);
    return 0;
}