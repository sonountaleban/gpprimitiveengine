#include <string.h>
#include "TexturesManager.h"
#include "utils.h"
#include "cJSON.h"

const char *TEXTURES_FILE_NAMES[TexturesManager::NUM_TEXTURES] = {
    "tex00.bmp",
    "tex01.png"
};

TexturesManager *g_pTexturesManager = NULL;

TexturesManager::TexturesManager()
{
    memset(m_fileNames, 0, sizeof(m_fileNames));

    memset(m_texturesWidths, 0, sizeof(m_texturesWidths));
    memset(m_texturesHeights, 0, sizeof(m_texturesHeights));

    memset(m_texturesBuffers, 0, sizeof(m_texturesBuffers));
}

TexturesManager::~TexturesManager()
{
    int a;

    for (a = TEXTURE_FIRST; a < NUM_MAX_TEXTURES; a ++)
    {
        RemoveTextureFrames((Textures)a);
    }

    RemoveAllTextures();
}

bool TexturesManager::AddTexture(const char *pFileName)
{
    bool        result;
    char        fileName[256];
    int         i;
    int         size;
    sf::Image   image;

    if (!pFileName || strlen(pFileName) <= 0)
    {
        return false;
    }

    DEBUGLOG("Add texture %s: ", pFileName);

    if (GetTextureIndex(pFileName) >= 0)
    {
        DEBUGLOG("Already exists!\n\n");

        return false;
    }

    for (i = 0; i < NUM_MAX_TEXTURES; i ++)
    {
        if (strlen(m_fileNames[i]) <= 0)
        {
            sprintf(fileName, "resources/%s", pFileName);

            result = image.loadFromFile(fileName);
            if (result)
            {
                m_texturesWidths[i] = image.getSize().x;
                m_texturesHeights[i] = image.getSize().y;

                size = m_texturesWidths[i] * m_texturesHeights[i];
                if (size > 0)
                {
                    m_texturesBuffers[i] = new int[size];                  
                    if (m_texturesBuffers[i])
                    {
                        strcpy(m_fileNames[i], pFileName);

                        memcpy(m_texturesBuffers[i], image.getPixelsPtr(), size * 4);
                    }

                    DEBUGLOG("OK.\n\n");

                    return true;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    DEBUGLOG("Error!\n\n");

    return false;
}

bool TexturesManager::AddTextureSheet(const char *pFileName)
{
    char                fileName[256];
    char                *pFileNameTemp;
    char                *pBuffer = NULL;
    char                *pFrameFilename;
    int                 a;
    int                 x;
    int                 y;
    int                 w;
    int                 h;
    int                 numBytes = 0;
    int                 numFrames;
    int                 textureIndex;
    int                 textureWidth;
    int                 textureHeight;
    FILE                *pFile;
    cJSON               *pRoot = NULL;
    cJSON               *pFrames;
    cJSON               *pFrame;
    TextureSheetFrame   *pTextureSheetFrame;
    Element             e;

    if (!AddTexture(pFileName))
    {
        return false;
    }

    textureIndex = GetTextureIndex(pFileName);
    textureWidth = GetTextureWidth((TexturesManager::Textures)textureIndex);
    textureHeight = GetTextureHeight((TexturesManager::Textures)textureIndex);

    RemoveTextureFrames((Textures)textureIndex);

    DEBUGLOG("Add texture sheet %s: ", pFileName);

    sprintf(fileName, "resources/%s", pFileName);

    pFileNameTemp = strrchr(fileName, '.');
    *pFileNameTemp = '\0';

    strcat(fileName, ".json");

    pFile = fopen(fileName, "r");
    if (pFile)
    {
        fseek(pFile, 0, SEEK_END);
        numBytes = ftell(pFile);
        if (numBytes > 0)
        {
            fseek(pFile, 0, SEEK_SET);

            pBuffer = (char *)malloc(sizeof(char) * (numBytes + 1));
            if (pBuffer)
            {
                pBuffer[numBytes] = '\0';

                if (!fread(pBuffer, numBytes, 1, pFile))
                {
                    free(pBuffer);
                    pBuffer = NULL;
                }
            }
        }

        fclose(pFile);
    }

    if (pBuffer)
    {
        pRoot = cJSON_Parse(pBuffer);
        if (pRoot)
        {
            pFrames = cJSON_GetObjectItem(pRoot, "frames");
            numFrames = cJSON_GetArraySize(pFrames);
            for (a = 0; a < numFrames; a ++)
            {
                pFrame = cJSON_GetArrayItem(pFrames, a);
                pFrameFilename = cJSON_GetObjectItem(pFrame, "filename")->valuestring;
                x = cJSON_GetObjectItem(cJSON_GetObjectItem(pFrame, "frame"), "x")->valueint;
                y = cJSON_GetObjectItem(cJSON_GetObjectItem(pFrame, "frame"), "y")->valueint;
                w = cJSON_GetObjectItem(cJSON_GetObjectItem(pFrame, "frame"), "w")->valueint;
                h = cJSON_GetObjectItem(cJSON_GetObjectItem(pFrame, "frame"), "h")->valueint;
                //DEBUGLOG("%d/%d %s - %d %d - %d %d\n", a, numFrames, pFrameFilename, x, y, w, h);

                pTextureSheetFrame = new TextureSheetFrame();

                strcpy(pTextureSheetFrame->name, pFrameFilename);
                pTextureSheetFrame->x = x;
                pTextureSheetFrame->y = y;
                pTextureSheetFrame->w = w;
                pTextureSheetFrame->h = h;

                if (textureWidth > 0)
                {
                    pTextureSheetFrame->u[0] = (float)x / textureWidth;
                    pTextureSheetFrame->u[1] = (float)(x + w) / textureWidth;
                    pTextureSheetFrame->u[2] = pTextureSheetFrame->u[1];
                    pTextureSheetFrame->u[3] = pTextureSheetFrame->u[0];
                }
                else
                {
                    pTextureSheetFrame->u[0] = 0.0f;
                    pTextureSheetFrame->u[1] = 1.0f;
                    pTextureSheetFrame->u[2] = 1.0f;
                    pTextureSheetFrame->u[3] = 0.0f;
                }

                if (textureHeight > 0)
                {
                    pTextureSheetFrame->v[0] = (float)y / textureHeight;
                    pTextureSheetFrame->v[1] = pTextureSheetFrame->v[0];
                    pTextureSheetFrame->v[2] = (float)(y + h) / textureHeight;
                    pTextureSheetFrame->v[3] = pTextureSheetFrame->v[2];
                }
                else
                {
                    if (textureHeight > 0)
                    {
                        pTextureSheetFrame->v[0] = 0.0f;
                        pTextureSheetFrame->v[1] = 0.0f;
                        pTextureSheetFrame->v[2] = 1.0f;
                        pTextureSheetFrame->v[3] = 1.0f;
                    }
                }

                e.p = pTextureSheetFrame;

                m_texturesFrames[textureIndex].AddElement(e);
            }

            cJSON_Delete(pRoot);

            DEBUGLOG("OK.\n\n");
        }
        else
        {
            DEBUGLOG("Error! Using whole texture...\n\n");
        }

        free(pBuffer);

        if (pRoot)
        {
            return true;
        }
    }
    else
    {
        DEBUGLOG("Error! Using whole texture...\n\n");
    }

    pTextureSheetFrame = new TextureSheetFrame();

    strcpy(pTextureSheetFrame->name, "whole");
    pTextureSheetFrame->x = 0;
    pTextureSheetFrame->y = 0;
    pTextureSheetFrame->w = textureWidth;
    pTextureSheetFrame->h = textureHeight;

    pTextureSheetFrame->u[0] = 0.0f;
    pTextureSheetFrame->u[1] = 1.0f;
    pTextureSheetFrame->u[2] = 1.0f;
    pTextureSheetFrame->u[3] = 0.0f;

    pTextureSheetFrame->v[0] = 0.0f;
    pTextureSheetFrame->v[1] = 0.0f;
    pTextureSheetFrame->v[2] = 1.0f;
    pTextureSheetFrame->v[3] = 1.0f;

    e.p = pTextureSheetFrame;

    m_texturesFrames[textureIndex].AddElement(e);

    return false;
}

void TexturesManager::RemoveAllTextures()
{
    int a;

    memset(m_fileNames, 0, sizeof(m_fileNames));

    for (a = 0; a < NUM_MAX_TEXTURES; a ++)
    {
        if (m_texturesBuffers[a])
        {
            delete[] m_texturesBuffers[a];
            m_texturesBuffers[a] = NULL;
        }
    }

    memset(m_texturesWidths, 0, sizeof(m_texturesWidths));
    memset(m_texturesHeights, 0, sizeof(m_texturesHeights));
}

void TexturesManager::RemoveTexture(const Textures index)
{
    if (index <= TEXTURE_NONE || index >= NUM_TEXTURES)
    {
        return;
    }

    memset(m_fileNames[index], 0, sizeof(m_fileNames[index]));

    if (m_texturesBuffers[index])
    {
        delete[] m_texturesBuffers[index];
        m_texturesBuffers[index] = NULL;
    }

    m_texturesWidths[index] = 0;
    m_texturesHeights[index] = 0;
}

const char *TexturesManager::GetTextureFileName(const Textures index)
{
    if (index <= TEXTURE_NONE || index >= NUM_TEXTURES)
    {
        return NULL;
    }

    return TEXTURES_FILE_NAMES[index];
}

int TexturesManager::GetTextureIndex(const char *pFileName)
{
    int i;

    if (!pFileName || strlen(pFileName) <= 0)
    {
        return -1;
    }

    for (i = 0; i < NUM_MAX_TEXTURES; i ++)
    {
        if (_strcasecmp(m_fileNames[i], pFileName))
        {
            return i;
        }
    }

    return -1;
}

int TexturesManager::GetTextureWidth(const Textures index)
{
    if (index <= TEXTURE_NONE || index >= NUM_TEXTURES)
    {
        return 0;
    }

    return m_texturesWidths[index];
}

int TexturesManager::GetTextureHeight(const Textures index)
{
    if (index <= TEXTURE_NONE || index >= NUM_TEXTURES)
    {
        return 0;
    }

    return m_texturesHeights[index];
}

int *TexturesManager::GetTextureBuffer(const Textures index)
{
    if (index <= TEXTURE_NONE || index >= NUM_TEXTURES)
    {
        return NULL;
    }

    return m_texturesBuffers[index];
}

TexturesManager::TextureSheetFrame *TexturesManager::GetTextureSheetFrame(const Textures textureIndex, int frameIndex)
{
    if (textureIndex <= TEXTURE_NONE || textureIndex >= NUM_TEXTURES ||
            frameIndex < 0 || frameIndex >= m_texturesFrames[textureIndex].GetNumElements())
    {
        return NULL;
    }

    return (TextureSheetFrame *)m_texturesFrames[textureIndex].GetElement(frameIndex).p;
}

void TexturesManager::RemoveTextureFrames(const Textures index)
{
    int                 a;
    TextureSheetFrame   *pTextureSheetFrame;

    if (index <= TEXTURE_NONE || index >= NUM_MAX_TEXTURES)
    {
        return;
    }

    for (a = 0; a < m_texturesFrames[index].GetNumElements(); a ++)
    {
        pTextureSheetFrame = (TextureSheetFrame *)m_texturesFrames[index].GetElement(a).p;
        if (pTextureSheetFrame)
        {
            delete pTextureSheetFrame;
        }
    }

    m_texturesFrames[index].ClearAll();
}
