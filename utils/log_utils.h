#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include <stdbool.h>
#include "vector.h"

void startLog(char *command, StringVector *keywords, int *counters, char *log_name, char *url, bool url_enabled);
void initLogFile(const char *log_name);
void cleanLogLine(const char *input, char *output);
void saveToLog(const char *log_name, const char *linea);
bool checkKeywords(const char *linea, StringVector *keywords, int *counters);

#endif