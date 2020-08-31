#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;

int main(int argc, char **argv)
{

    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;
    int i = 0;
    float wynik = 0;
    float avg = 0;
    int N = strtol(argv[1], NULL, 10);
    int *buffer = (int *)malloc(N * sizeof(int));
    char bufferChar[10000] = {0};
    HANDLE handleFile;

    while (1)
    {
        handleFile = CreateFile("liczby.txt", FILE_GENERIC_WRITE | FILE_GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (handleFile != INVALID_HANDLE_VALUE)
            break;
    }

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);

    cout << "Process 2" << endl;

    for (i = 0; i < N; i++)
    {
        ReadFile(handleFile, bufferChar, 4, NULL, NULL);
        string bufferString = bufferChar;
        *(buffer + i) = stoi(bufferString, nullptr, 10);
        //cout << *(buffer + i) << endl;
        wynik = wynik + *(buffer + i);
    }
    avg = wynik / i;

    int min = *(buffer + 0);
    int max = *(buffer + 0);
    for (i = 0; i < N; i++)
    {
        if (*(buffer + i) > max)
        {

            max = *(buffer + i);
        }
        else if (*(buffer + i) < min)
        {

            min = *(buffer + i);
        }
    }
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    
    cout << "Min: " << min << endl;
    cout << "Max: " << max << endl;
    cout << "Average: " << avg << endl;
    cout << "Time of operation: " << ElapsedMicroseconds.QuadPart << "μs" << endl;
    
    string bufferMin;
    string bufferMax;
    string bufferAvg;
    string toc;
    toc = to_string(ElapsedMicroseconds.QuadPart);
    bufferMin = to_string(min);
    bufferMax = to_string(max);
    bufferAvg = to_string(avg);
    unsigned int lngt = toc.length();
    
    HANDLE hWynik = CreateFile("wyniki.txt", FILE_GENERIC_WRITE | FILE_GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(hWynik, 0, NULL, FILE_END);
    WriteFile(hWynik,bufferMin.c_str(), 4, NULL, NULL);
    WriteFile(hWynik,"  ", 2, NULL, NULL);
    WriteFile(hWynik,bufferMax.c_str(), 4, NULL, NULL);
    WriteFile(hWynik,"  ", 2, NULL, NULL);
    WriteFile(hWynik,bufferAvg.c_str(), 6, NULL, NULL);
    WriteFile(hWynik,"  ", 2, NULL, NULL);
    WriteFile(hWynik,toc.c_str(), lngt, NULL, NULL);
    WriteFile(hWynik,"\r\n", 2, NULL, NULL);
    CloseHandle(hWynik);
    
    free(buffer);
    CloseHandle(handleFile);
    ExitProcess(0);
    return 0;
}