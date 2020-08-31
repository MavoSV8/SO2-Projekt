#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h> 
#include <iomanip>

using namespace std;
HANDLE mutexHandle;


int N = 0;
//Funkcja wątku licząca średnią
DWORD WINAPI averageThread(LPVOID lpParameter)
{
int *buffer = (int*)lpParameter;
int i;
float wynik = 0;
for(i = 0;i<N;i++)
{
WaitForSingleObject(mutexHandle,INFINITE);
wynik = wynik + *(buffer + i);
ReleaseMutex(mutexHandle);

}

float avg = wynik/i;
cout << "Average: " << fixed << setprecision(3) <<  avg << endl;
return 0;
}

//Funkcja wątku znajdująca wartość największą.
DWORD WINAPI maxThread(LPVOID lpParameter)
{
int *buffer = (int*)lpParameter;
int max = *(buffer + 0);
for(int i = 0;i<N;i++)
{
    WaitForSingleObject(mutexHandle,INFINITE);
    if (*(buffer + i) > max)
    {

        max = *(buffer + i);

    }
ReleaseMutex(mutexHandle);

}
cout << "Max: " << max << endl;
return 0;
}

//Funkcja wątku znajdująca wartość najmniejszą
DWORD WINAPI minThread(LPVOID lpParameter)
{
int *buffer = (int*)lpParameter;
int min = *(buffer + 0);
for(int i = 0;i<N;i++)
{
WaitForSingleObject(mutexHandle,INFINITE);
    if(*(buffer + i) < min)
    {
        min = *(buffer + i);
    }
ReleaseMutex(mutexHandle);
}
cout << "Min: " << min << endl;
return 0;
}


int main(int argc, char** argv) {

int i = 0;
HANDLE threadsArray[3];
PDWORD threadsIDs[3] = {0};
//Tworzenie mutexa dla wątków

mutexHandle = CreateMutex(NULL,FALSE,NULL);

N = strtol(argv[1],NULL,10);
int *buffer = (int*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,N * sizeof(buffer));
char bufferChar[10000] = {0};
HANDLE handleFile;
//Spinlock
while(1) {
handleFile = CreateFile("liczby.txt",FILE_GENERIC_WRITE|FILE_GENERIC_READ,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
if (handleFile != INVALID_HANDLE_VALUE)
break;
Sleep(3000);
}
cout << "Process 2"<< endl;
//Wczytywanie z pliku
for(i = 0;i<N;i++)
{
ReadFile(handleFile,bufferChar,4,NULL,NULL);
string bufferString = bufferChar;
*(buffer + i) = stoi(bufferString,nullptr,10);
cout << *(buffer + i) << endl;

}
WaitForSingleObject(handleFile,INFINITE);

threadsArray[1] = CreateThread(NULL,0,minThread,buffer,0,threadsIDs[1]);
threadsArray[2] = CreateThread(NULL,0,maxThread,buffer,0,threadsIDs[2]);   
threadsArray[0] = CreateThread(NULL,0,averageThread,buffer,0,threadsIDs[0]); 

WaitForMultipleObjects(3, threadsArray, TRUE, INFINITE);
HeapFree(GetProcessHeap(),0,buffer);
CloseHandle(handleFile);
CloseHandle(mutexHandle);
ExitProcess(0);
    return 0;
}