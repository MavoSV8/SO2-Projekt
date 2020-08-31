#include <iostream>
#include <windows.h>
#include <string>

/**
 * @file lab1i2
 * @author Łukasz Pasiński 
 * @date 24.06.2020
 * @brief EX 1 - Systemy operacyjne II, ćwiczenie 1 - LAB
 */

int f_argc;
char** f_argv;
int i;


using namespace std;

bool _writeLineToFile(HANDLE h) 
{
char newLine[] = "\r\n";
int size = strlen(f_argv[i]);
WriteFile(h,f_argv[i],size,NULL,NULL);
WriteFile(h,newLine,2,NULL,NULL);
return true;
}

bool _openFileInNotepad(string name)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    int size = name.length()+1;
    char *file = new char[size];
    strcpy(file, name.c_str()); 
    char notepad[] = "\"notepad.exe\" ";
    
    if(!CreateProcess(0,strcat(notepad,file),NULL,NULL,true,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi)) 
    {
        cout << "Couldn't create process" << endl;
        delete[] file;
        return false;
    }
    delete[] file;
    return true;
}

int main(int argc, char** argv) {
for(auto i = 0; i<argc; i++) 
		std::cout << argv[i] << std::endl;  
		
f_argc = argc;
f_argv = argv;
string name = "result.txt";
HANDLE handleFile = CreateFile(name.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


for (i = 0; i<argc; i++)
{
    _writeLineToFile(handleFile);

}



_openFileInNotepad(name);
CloseHandle(handleFile);


    return 0;
}
