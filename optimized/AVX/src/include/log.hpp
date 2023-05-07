#ifndef LOG_H
#define LOG_H

#include "ReadFileUtils.hpp"


FILE* logFile = openFile("log.txt", "w");

//#define NO_LOG
#ifndef NO_LOG
    #define log(...) {                     \
        fprintf(logFile, __VA_ARGS__);     \
        fflush(logFile);                   \
    }
#else
    #define log(...) ;
#endif


#endif