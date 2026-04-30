#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#define MAIN_DIRNAME "remos"
#define LOGS_DIRNAME "logs"

#define CONFIG_FILENAME "config.cfg"
#define TEMP_FILENAME "temp.tmp"
#define CONFIG_LENGTH 4096
#define INPUT_LENGTH 1024

#define OPTIONS_COUNT 4
#define OPTIONS_LENGTH 16
#define OPTIONS {"keywords", "log", "url", "enabled"}

bool validateConfigFile(char *path);
void updateConfig(char *config_path, const char *key, const char *new_value);
char *readValueFromKey(char *config_path, const char *key);

#endif