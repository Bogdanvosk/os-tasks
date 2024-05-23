#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <mutex>

#define MAX_FILES 1000
#define MAX_FILE_NAME 32
#define MAX_DATA_SIZE 1024

using namespace std;

typedef struct File
{
    char name[MAX_FILE_NAME];
    char data[MAX_DATA_SIZE];
    int size;
    bool isModifying;
    mutex mtx;
} File;

File files[MAX_FILES];
int cur_file_idx = 0;

void createFile(const char* name_)
{
    if (cur_file_idx < MAX_FILES) {
        strcpy_s(files[cur_file_idx].name, name_);
        cout << "File '" << name_ << "' created successfully." << endl;
        cur_file_idx++;
    }
    else {
        cout << "Cannot create file '" << name_ << "'. Maximum number of files reached." << endl;
    }
}

int readFile(const char* name_)
{
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, name_) == 0) {
            lock_guard<mutex> lock(files[i].mtx);

            cout << "File content for '" << name_ << "': " << files[i].data << endl;

            return 0;
        }
    }

    cout << "File with the name '" << name_ << "' not found." << endl;
    return -1;
}

int writeFile(const char* name_, const char* text_)
{
    for (int i = 0; i < cur_file_idx; i++)
    {
        if (strcmp(files[i].name, name_) == 0)
        {
            lock_guard<mutex> lock(files[i].mtx);

            memset(files[i].data, '\0', sizeof(files[i].data));

            strcpy_s(files[i].data, text_);

            files[i].size = strlen(text_);

            cout << "File '" << name_ << "' has been updated successfully." << endl;
            return 0;
        }
    }
    return 0;
}

int deleteFile(const char* name_) {
    int fileIndex = -1;

    for (int i = 0; i < cur_file_idx; i++) {
        if (strcmp(files[i].name, name_) == 0) {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1) {
        cout << "No file with the name '" << name_ << "' found.\n";
        return -1;
    }

    if (files[fileIndex].isModifying) {
        cout << "File is being modified and cannot be deleted.\n";
        return -1;
    }

    for (int i = fileIndex; i < cur_file_idx; i++) {
        strcpy_s(files[i].name, files[i + 1].name);
        strcpy_s(files[i].data, files[i + 1].data);
        files[i].size = files[i + 1].size;
    }

    cur_file_idx--;

    std::cout << "File '" << name_ << "' has been deleted successfully.\n";

    return 0;
}

void fsInfo()
{
    int totalFiles = 0;
    int totalSize = 0;
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].name[0] != '\0')
        {
            totalFiles++;
            totalSize += files[i].size;
        }
    }
    cout << "Total files: " << totalFiles << endl << "Total size bytes: " << totalSize << endl;
}

int main()
{
    return 0;
}
