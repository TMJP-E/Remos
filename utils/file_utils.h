#ifndef FILE_UTILS_H
#define FILE_UTILS_H

bool verifyDirExists(const char *path);
bool verifyFileExists(const char *path);
int createDir(const char *path, unsigned int mode);
int createFile(const char *path);
void logDirOrFile(int result, char *name, char *type);

#endif
