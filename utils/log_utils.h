#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include <stdbool.h>
#include "vector.h"

void startLog(char *command, StringVector *keywords, int *counters, char *log_name, char *url, bool url_enabled);

#endif