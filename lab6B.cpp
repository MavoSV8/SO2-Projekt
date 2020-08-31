#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h> 
#include <iomanip>

using namespace std;
HANDLE mutexHandle;
float avg = 0;
int Min = 0;
int Max = 0;

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

avg = wynik/i;

return 0;
}

//Funkcja wątku znajdująca wartość największą.
DWORD WINAPI maxThread(LPVOID lpParameter)
{
int *buffer = (int*)lpParameter;
Max = *(buffer + 0);
for(int i = 0;i<N;i++)
{
    WaitForSingleObject(mutexHandle,INFINITE);
    if (*(buffer + i) > Max)
    {

        Max = *(buffer + i);

    }
ReleaseMutex(mutexHandle);

}

return 0;
}

//Funkcja wątku znajdująca wartość najmniejszą
DWORD WINAPI minThread(LPVOID lpParameter)
{
int *buffer = (int*)lpParameter;
Min = *(buffer + 0);
for(int i = 0;i<N;i++)
{
WaitForSingleObject(mutexHandle,INFINITE);
    if(*(buffer + i) < Min)
    {
        Min = *(buffer + i);
    }
ReleaseMutex(mutexHandle);
}

return 0;
}

//Funkcja wątku wypisującego wyniki
DWORD WINAPI outputThread(LPVOID lpParameter)
{

    cout << "Min: " << Min << endl;
    cout << "Max: " << Max << endl;
    cout << "Average: "<< avg <<endl;

return 0;
}



int main(int argc, char** argv) {

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

QueryPerformanceFrequency(&Frequency); 
QueryPerformanceCounter(&StartingTime);



int i = 0;
PDWORD outputID = 0;
HANDLE threadsArray[4];
PDWORD threadsIDs[4] = {0};
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

}
cout << "Process 2"<< endl;
//Wczytywanie z pliku
for(i = 0;i<N;i++)
{
ReadFile(handleFile,bufferChar,4,NULL,NULL);
string bufferString = bufferChar;
*(buffer + i) = stoi(bufferString,nullptr,10);
//cout << *(buffer + i) << endl;

}
WaitForSingleObject(handleFile,INFINITE);

threadsArray[0] = CreateThread(NULL,0,minThread,buffer,0,threadsIDs[0]);
threadsArray[1] = CreateThread(NULL,0,maxThread,buffer,0,threadsIDs[1]);   
threadsArray[2] = CreateThread(NULL,0,averageThread,buffer,0,threadsIDs[2]); 

WaitForMultipleObjects(3, threadsArray, TRUE, INFINITE);

HANDLE handleOutput = CreateThread(NULL,0,outputThread,NULL,0,outputID); 
WaitForSingleObject(handleOutput,INFINITE);

QueryPerformanceCounter(&EndingTime);
ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;


ElapsedMicroseconds.QuadPart *= 1000000;
ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
cout << "Time of operation: " << ElapsedMicroseconds.QuadPart <<"μs" << endl;


HeapFree(GetProcessHeap(),0,buffer);
for (int i = 0;i<3;i++)
{
    
    CloseHandle(threadsArray[i]);

}
CloseHandle(handleOutput);
CloseHandle(handleFile);
CloseHandle(mutexHandle);
ExitProcess(0);
    return 0;
}