#ifndef LOG_H
#define LOG_H
#include <stdio.h>

#define LOG_ENABLED

#ifdef LOG_ENABLED
#define LOGI(...) fprintf(stderr, "I: " __VA_ARGS__)
#else
#define LOGI(...) do {} while (0) // 空操作，即忽略日志输出
#endif


#endif