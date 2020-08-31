#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <iomanip>

using namespace std;
CRITICAL_SECTION critSect;
float avg = 0;
int Min = 0;
int Max = 0;

int N = 0;
//Funkcja wątku licząca średnią
DWORD WINAPI averageThread(LPVOID lpParameter)
{
    int *buffer = (int *)lpParameter;
    int i;
    float wynik = 0;
    for (i = 0; i < N; i++)
    {
        EnterCriticalSection(&critSect);
        wynik = wynik + *(buffer + i);
        LeaveCriticalSection(&critSect);
    }

    avg = wynik / i;

    return 0;
}

//Funkcja wątku znajdująca wartość największą.
DWORD WINAPI maxThread(LPVOID lpParameter)
{
    int *buffer = (int *)lpParameter;
    Max = *(buffer + 0);
    for (int i = 0; i < N; i++)
    {
        EnterCriticalSection(&critSect);
        if (*(buffer + i) > Max)
        {

            Max = *(buffer + i);
        }
        LeaveCriticalSection(&critSect);
    }

    return 0;
}

//Funkcja wątku znajdująca wartość najmniejszą
DWORD WINAPI minThread(LPVOID lpParameter)
{
    int *buffer = (int *)lpParameter;
    Min = *(buffer + 0);
    for (int i = 0; i < N; i++)
    {
        EnterCriticalSection(&critSect);
        if (*(buffer + i) < Min)
        {
            Min = *(buffer + i);
        }
        LeaveCriticalSection(&critSect);
    }

    return 0;
}

//Funkcja wątku wypisującego wyniki
DWORD WINAPI outputThread(LPVOID lpParameter)
{

    cout << "Min: " << Min << endl;
    cout << "Max: " << Max << endl;
    cout << "Average: " << avg << endl;

    return 0;
}

int main(int argc, char **argv)
{

    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;
    int i = 0;
    PDWORD outputID = 0;
    HANDLE threadsArray[4];
    PDWORD threadsIDs[4] = {0};

    //Tworzenie sekcji krytycznej dla wątków
    InitializeCriticalSectionAndSpinCount(&critSect, 0x00000400);

    N = strtol(argv[1], NULL, 10);
    int *buffer = (int *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, N * sizeof(buffer));
    char bufferChar[10000] = {0};
    HANDLE handleFile;
    
    //Spinlock
    while (1)
    {
        handleFile = CreateFile("liczby.txt", FILE_GENERIC_WRITE | FILE_GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (handleFile != INVALID_HANDLE_VALUE)
            break;
    }

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);

    cout << "Process 2" << endl;

    //Wczytywanie z pliku
    for (i = 0; i < N; i++)
    {
        ReadFile(handleFile, bufferChar, 4, NULL, NULL);
        string bufferString = bufferChar;
        *(buffer + i) = stoi(bufferString, nullptr, 10);
        //cout << *(buffer + i) << endl;
    }

    WaitForSingleObject(handleFile, INFINITE);
    CloseHandle(handleFile);

    //Tworzenie wątków
    threadsArray[0] = CreateThread(NULL, 0, minThread, buffer, 0, threadsIDs[0]);
    threadsArray[1] = CreateThread(NULL, 0, maxThread, buffer, 0, threadsIDs[1]);
    threadsArray[2] = CreateThread(NULL, 0, averageThread, buffer, 0, threadsIDs[2]);
    WaitForMultipleObjects(3, threadsArray, TRUE, INFINITE);

    HANDLE handleOutput = CreateThread(NULL, 0, outputThread, NULL, 0, outputID);
    WaitForSingleObject(handleOutput, INFINITE);

    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    cout << "Time of operation: " << ElapsedMicroseconds.QuadPart << "μs" << endl;
    
    
    string bufferMin;
    string bufferMax;
    string bufferAvg;
    string toc;
    toc = to_string(ElapsedMicroseconds.QuadPart);
    bufferMin = to_string(Min);
    bufferMax = to_string(Max);
    bufferAvg = to_string(avg);
    unsigned int lngt = toc.length();

    //Otwieranie pliku wynikowego i zapis wyników do niego.
    HANDLE hWynik = CreateFile("wyniki.txt", FILE_GENERIC_WRITE | FILE_GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(hWynik, 0, NULL, FILE_END);
    WriteFile(hWynik, bufferMin.c_str(), 4, NULL, NULL);
    WriteFile(hWynik, "  ", 2, NULL, NULL);
    WriteFile(hWynik, bufferMax.c_str(), 4, NULL, NULL);
    WriteFile(hWynik, "  ", 2, NULL, NULL);
    WriteFile(hWynik, bufferAvg.c_str(), 6, NULL, NULL);
    WriteFile(hWynik, "  ", 2, NULL, NULL);
    WriteFile(hWynik, toc.c_str(), lngt, NULL, NULL);
    WriteFile(hWynik, "\r\n", 2, NULL, NULL);
    CloseHandle(hWynik);

    HeapFree(GetProcessHeap(), 0, buffer);
    for (int i = 0; i < 3; i++)
    {
        CloseHandle(threadsArray[i]);
    }
    CloseHandle(handleOutput);
    DeleteCriticalSection(&critSect);
    ExitProcess(0);
    return 0;
}