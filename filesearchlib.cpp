#include "filesearchlib.h"
#include <iostream>
#include <dirent.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>

std::mutex fileFoundMutex;
bool fileFound = false;

std::queue<std::string> directoriesToSearch;
std::mutex queueMutex;
std::condition_variable queueCondVar;

bool findFileInDirectory(const char* dirPath, const char* targetFileName) {
    DIR* dir = opendir(dirPath);
    if (dir == nullptr) {
        std::cerr << "Error opening directory: " << dirPath << std::endl;
        return false;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (fileFound)
            break;

        if (entry->d_type == DT_REG && std::strcmp(entry->d_name, targetFileName) == 0) {
            std::lock_guard<std::mutex> lock(fileFoundMutex);
            if (!fileFound) {
                std::cout << "Found the file: " << dirPath << "/" << entry->d_name << std::endl;
                fileFound = true;
            }
            break;
        }
    }

    closedir(dir);
    return fileFound;
}

void worker(const char* targetFileName) {
    while (true) {
        std::string currentDir;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondVar.wait(lock, [&] { return !directoriesToSearch.empty(); });

            currentDir = directoriesToSearch.front();
            directoriesToSearch.pop();
        }

        if (findFileInDirectory(currentDir.c_str(), targetFileName)) {
            queueCondVar.notify_all();
            return;
        }

        DIR* dir = opendir(currentDir.c_str());
        if (dir != nullptr) {
            dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_DIR && std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0) {
                    std::string newPath = currentDir + "/" + entry->d_name;

                    std::lock_guard<std::mutex> lock(queueMutex);
                    directoriesToSearch.push(newPath);
                }
            }
            closedir(dir);
        }

        queueCondVar.notify_all();
    }
}

void solver(){
    std::string targetFileName;
    std::cout << "Enter the name of the file to search for: ";
    std::cin >> targetFileName;

    // Start the search from the root directory
    directoriesToSearch.push("/");

    std::vector<std::thread> threads;
    const int numThreads = 8;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(worker, targetFileName.c_str());
    }

    for (auto& thread : threads) {
        thread.join();
    }

    if (!fileFound) {
        std::cout << "File not found." << std::endl;
    }
}