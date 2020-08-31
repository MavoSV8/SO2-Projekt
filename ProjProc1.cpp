#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;

int main(int argc, char **argv)
{
    string arg1;
    char version = 0;
    cout << "Process 1" << endl;
    //Wybór wariantu procesu drugiego
    cout << "Choose version of process 2 A/B: " << endl;
    while (version != 'A' && version != 'B')
    {
        cin >> version;
        if (version != 'A' && version != 'B')
        {
            cout << "Write A or B." << endl;
        }
    }
    if (version == 'A')
    {
        arg1 = "ProjProc2A.exe ";
    }
    else if (version == 'B')
    {
        arg1 = "ProjProc2B.exe ";
    }

    int pause = strtol(argv[2], NULL, 10);
    pause = pause * 1000;
    int repeats = strtol(argv[3], NULL, 10);
    int N = strtol(argv[1], NULL, 10);

    //Tworzenie pliku wynikowego i zapisywanie informacji o procesie 1 do niego
    HANDLE hWynik = CreateFile("wyniki.txt", FILE_GENERIC_WRITE | FILE_GENERIC_READ, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    SYSTEM_INFO sysinf;
    GetSystemInfo(&sysinf);
    WriteFile(hWynik, "Total Numbers: ", 16, NULL, NULL);
    WriteFile(hWynik, argv[1], strlen(argv[1]) + 1, NULL, NULL);
    WriteFile(hWynik, "Repeats: ", 10, NULL, NULL);
    WriteFile(hWynik, argv[3], strlen(argv[3]) + 1, NULL, NULL);
    WriteFile(hWynik, "Pause: ", 8, NULL, NULL);
    WriteFile(hWynik, argv[2], strlen(argv[3]) + 1, NULL, NULL);
    WriteFile(hWynik, "Version: ", 10, NULL, NULL);
    WriteFile(hWynik, &version, 1, NULL, NULL);
    WriteFile(hWynik, "\r\n", 2, NULL, NULL);
    WriteFile(hWynik, "System info: ", 14, NULL, NULL);
    WriteFile(hWynik, to_string(sysinf.dwProcessorType).c_str(), sizeof(sysinf.dwProcessorType), NULL, NULL);
    WriteFile(hWynik, to_string(sysinf.dwNumberOfProcessors).c_str(), sizeof(sysinf.dwNumberOfProcessors), NULL, NULL);
    WriteFile(hWynik, to_string(sysinf.wProcessorLevel).c_str(), sizeof(sysinf.wProcessorLevel), NULL, NULL);
    WriteFile(hWynik, to_string(sysinf.wProcessorArchitecture).c_str(), sizeof(sysinf.wProcessorArchitecture), NULL, NULL);
    WriteFile(hWynik, to_string(sysinf.dwPageSize).c_str(), sizeof(sysinf.dwPageSize), NULL, NULL);
    WriteFile(hWynik, "\r\n", 2, NULL, NULL);
    WriteFile(hWynik, "Min  ", 6, NULL, NULL);
    WriteFile(hWynik, "Max  ", 6, NULL, NULL);
    WriteFile(hWynik, "Avg  ", 6, NULL, NULL);
    WriteFile(hWynik,"  ",2,NULL,NULL);
    WriteFile(hWynik, "Time of completion in μs", 26, NULL, NULL);
    WriteFile(hWynik, "\r\n", 2, NULL, NULL);
    CloseHandle(hWynik);

    for (int r = 0; r < repeats; r++)
    {
        STARTUPINFO si1;
        PROCESS_INFORMATION pi1;

        //Tworzenie procesu
        ZeroMemory(&si1, sizeof(si1));
        si1.cb = sizeof(si1);
        ZeroMemory(&pi1, sizeof(pi1));
        string process = arg1 + argv[1];
        
        if(!CreateProcess(NULL, (LPSTR)process.c_str(), NULL, NULL, true, CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si1, &pi1))
        {
            cout << "Couldn't create process." << endl;
            return -1;
        }

        srand(time(0));

        cout << "Wartosc N: " << N << endl;
        int buffer = 0;
        string bufferchar;
        //Tworzenie pliku
        HANDLE handleFile = CreateFile("liczby.txt", FILE_GENERIC_WRITE | FILE_GENERIC_READ, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        //Losowanie i zapisywanie do pliku
        SetFilePointer(handleFile, 0, NULL, FILE_END);
        for (int i = 0; i < N; i++)
        {
            buffer = 1 + rand() % 100;
            bufferchar = to_string(buffer);
            WriteFile(handleFile, bufferchar.c_str(), 4, NULL, NULL);

            //cout << buffer << endl;
        }
        CloseHandle(handleFile);
        WaitForSingleObject(pi1.hProcess, INFINITE);
        Sleep(pause);
    }
    ExitProcess(0);
    return 0;
}