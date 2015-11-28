#include "utils.h"

FILE *g_pLog = NULL;

// ------------------------------------------------------------------

unsigned int ReadChar(FILE *pFile)
{
    return fgetc(pFile);
}

unsigned int ReadShort(FILE *pFile)
{
    unsigned int temp = ReadChar(pFile);

    return (temp | (ReadChar(pFile) << 8));
}

unsigned int ReadInt(FILE *pFile)
{
    unsigned int temp1;
    unsigned int temp2;

    temp1 = ReadShort(pFile);
    temp2 = ReadShort(pFile);

    return (temp1 | (temp2 << 16));
}

bool _strcasecmp(const char *pString1, const char *pString2)
{
    int a;
    int len1;
    int len2;

    if (!pString1 || !pString2)
    {
        return false;
    }

    if (pString1 == pString2)
    {
        return true;
    }

    len1 = strlen(pString1);
    len2 = strlen(pString2);

    if (len1 != len2)
    {
        return false;
    }

    for (a = 0; a < len1; a ++)
    {
        if (tolower(pString1[a]) != tolower(pString2[a]))
        {
            return false;
        }
    }

    return true;
}

int ExtractValueFromKey(const char *pStringLine, const char *pKey)
{
    char        value[20];
    const char  *pStart;
    const char  *pEnd;
    int         length;

    if (!pStringLine || strlen(pStringLine) <= 0 || !pKey || strlen(pKey) <= 0)
    {
        return 0;
    }

    pStart = strstr(pStringLine, pKey);
    if (!pStart)
    {
        return 0;
    }

    pStart += strlen(pKey);

    pEnd = strstr(pStart, " ");
    if (!pEnd)
    {
        return 0;
    }

    length = pEnd - pStart;

    strncpy(value, pStart, length);
    value[length] = '\0';

    return atoi(value);
}

bool ExtractLineFromString(const char **ppString, char *pLine, int *pSize, int numMaxCharsLine)
{
    bool        getLine = true;
    int         length;
    const char  *pStart;
    const char  *pCurr;

    if (!ppString || !*ppString || strlen(*ppString) <= 0 || !pLine ||
        !pSize || *pSize <= 0 || numMaxCharsLine <= 0)
    {
        return false;
    }

    pStart = pCurr = *ppString;

    while (true)
    {
        if (*pCurr == 13 || *pCurr == 10 || *pSize <= 0)
        {
            if (getLine)
            {
                getLine = false;

                length = pCurr - pStart;
                if (length < numMaxCharsLine)
                {
                    strncpy(pLine, pStart, length);
                    pLine[length] = '\0';
                }
                else
                {
                    strncpy(pLine, pStart, numMaxCharsLine - 1);
                    pLine[numMaxCharsLine - 1] = '\0';
                }
            }

            if (*pSize <= 0)
            {
                break;
            }
        }
        else if (!getLine)
        {
            break;
        }

        pCurr ++;
        (*pSize)--;
    }

    *ppString = pCurr;

    return true;
}

void OpenLogFile(bool append)
{
    if (!g_pLog)
    {
        if (append)
        {
            g_pLog = fopen("log.txt", "a");
        }
        else
        {
            g_pLog = fopen("log.txt", "w");
        }
    }
}

void CloseLogFile()
{
    if (g_pLog)
    {
        fclose(g_pLog);
        g_pLog = NULL;
    }
}
