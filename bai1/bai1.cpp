#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include <Windows.h>

using namespace std;
namespace fs = std::filesystem;

bool acquireLock(const string& lockFilePath) {
    HANDLE lockFile = CreateFile(
        lockFilePath.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (lockFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    CloseHandle(lockFile);
    return true;
}

void releaseLock(const string& lockFilePath) {
    DeleteFile(lockFilePath.c_str());
}

void traverseAndSave(const fs::path& dirPath, int depth, set<string>& pathsSet) {
    if (depth > 10) {
        return;
    }

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        pathsSet.insert(entry.path().string());

        if (fs::is_directory(entry.path())) {
            traverseAndSave(entry.path(), depth + 1, pathsSet);
        }
    }
}

void processDirectory(const string& inputPath, const string& outputPath) {
    set<string> pathsSet;

    traverseAndSave(inputPath, 0, pathsSet);

    {
        ofstream outFile(outputPath + "\\output.txt", ios::app);
        if (!outFile) {
            cout << "Error opening output file.\n";
            return;
        }

        for (const auto& path : pathsSet) {
            outFile << path << "\n";
        }
    }
}

int main() {
    const string lockFilePath = "lockfile.lock";

    if (!acquireLock(lockFilePath)) {
        cout << "Error: Another instance is already running.\n";
        return 1;
    }


    cout << "Nhap duong dan thu muc can duyet: ";
    string inputPath;
    getline(cin, inputPath);

    cout << "Nhap duong dan thu muc luu ket qua: ";
    string outputPath;
    getline(cin, outputPath);

    processDirectory(inputPath, outputPath);

    cout << "Da luu duong dan vao file output.txt.\n";

    releaseLock(lockFilePath);

    return 0;
}
