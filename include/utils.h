#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

#define PACK( __Declaration__ )     __Declaration__ __attribute__((__packed__))

#define DEBUGLOG(x, ...)            OpenLogFile(true); fprintf(g_pLog, x, ##__VA_ARGS__); CloseLogFile();
#define DEBUGLOG2(x, ...)           fprintf(stdout, x, ##__VA_ARGS__); fflush(stdout);

#define TITLE_APPLICATION           "GPPrimitiveEngine"
#define VERSION_MAJ_APPLICATION     0
#define VERSION_MIN_APPLICATION     1

#define TOLLERANCE                  0.0001f
#define TO_RADIANS(x)               ((x) / 180.0f * (float)M_PI)

extern FILE *g_pLog;

// ------------------------------------------------------------------

extern unsigned int ReadChar(FILE *pFile);
extern unsigned int ReadShort(FILE *pFile);
extern unsigned int ReadInt(FILE *pFile);

extern bool         _strcasecmp(const char *pString1, const char *pString2);
extern int          ExtractValueFromKey(const char *pStringLine, const char *pKey);
extern bool         ExtractLineFromString(const char **ppString, char *pLine, int *pSize, int numMaxCharsLine);

extern void         OpenLogFile(bool append);
extern void         CloseLogFile();

#endif // _UTILS_H_
