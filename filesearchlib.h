#ifndef FILESEARCHLIB_H
#define FILESEARCHLIB_H

#include <string>

bool findFileInDirectory(const char* dirPath, const char* targetFileName);
void worker(const char* targetFileName);
void solver();

#endif // FILESEARCHLIB_H
