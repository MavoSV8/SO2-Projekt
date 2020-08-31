#include <iostream>
#include <windows.h>
#include <string>
/**
 * @file lab3
 * @author Łukasz Pasiński 
 * @date 28.06.2020
 * @brief EX 2 - Systemy operacyjne II, ćwiczenie 2 - LAB
 */

using namespace std;




int main(int argc, char** argv) {
STARTUPINFO si1;
PROCESS_INFORMATION pi1;


ZeroMemory(&si1, sizeof(si1));
si1.cb = sizeof(si1);
ZeroMemory(&pi1, sizeof(pi1));


if (argc == 1)
{

_SECURITY_ATTRIBUTES sa;
sa.nLength = sizeof(sa);
sa.bInheritHandle = true;
sa.lpSecurityDescriptor = NULL;
HANDLE handleWrite;
handleWrite = CreateFile("log.txt", FILE_GENERIC_WRITE|SYNCHRONIZE,FILE_SHARE_WRITE|FILE_SHARE_READ, &sa, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
CreateProcess(NULL,(LPSTR)"lab3.exe 1",NULL,NULL,true,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si1,&pi1); 
char b[10000] = {0};
cout << "Type something: " << endl;
cin.get(b,10000);
WaitForSingleObject(handleWrite,INFINITE);
char newLine[] = "\r\n";
int size = strlen(b);
SetFilePointer(handleWrite, 0, NULL, FILE_END);
WriteFile(handleWrite,b,size,NULL,NULL);
WriteFile(handleWrite,newLine,2,NULL,NULL);
WaitForSingleObject(pi1.hProcess,INFINITE);
CloseHandle(handleWrite);

}
else
{
PDWORD size2 = 0;
DWORD size3 = 0;
PDWORD newFileSize = 0;
DWORD newFileSize2 = 0;

HANDLE handleRead = CreateFile("log.txt", SYNCHRONIZE,FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
size3 = GetFileSize(handleRead,size2);
Sleep(10000);
DWORD fileAttr;
fileAttr = GetFileAttributes("log.txt");
    

if(0xFFFFFFFF == fileAttr)
{
        cout << "File doesn't exist."<< endl;
        return 0;
}

cout << "Size of the file is: "<< size3 << endl;

Sleep(10000);

newFileSize2 = GetFileSize(handleRead,newFileSize);
cout << newFileSize2 << endl;
cout << "Number of added character is: " << newFileSize2 - size3 << endl;
return 0;
CloseHandle(handleRead);
ExitProcess(0);
  
   
   
}


    return 0;
}