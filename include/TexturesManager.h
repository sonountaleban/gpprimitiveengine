#ifndef _TEXTURES_MANAGER_H_
#define _TEXTURES_MANAGER_H_

#include "utils.h"
#include "List.h"

#define NUM_MAX_TEXTURES 100

class TexturesManager
{
public:
    enum Textures
    {
        TEXTURE_NONE = -1,

        TEXTURE_FIRST = 0,
        TEXTURE_TEX00 = TEXTURE_FIRST,
        TEXTURE_TEX01,

        NUM_TEXTURES
    };

    enum TextureSheetTex00
    {
        TEXTURE_SHEET_TEX00_WHOLE,

        TEXTURE_SHEET_TEX00_NUM_FRAMES
    };

    enum TextureSheetTex01
    {
        TEXTURE_SHEET_TEX01_PICTURE0,
        TEXTURE_SHEET_TEX01_PICTURE0_ALPHA,

        TEXTURE_SHEET_TEX01_NUM_FRAMES
    };

    PACK(typedef struct)
    {
        char    name[256];
        int     x;
        int     y;
        int     w;
        int     h;
        float   u[4];
        float   v[4];
    } TextureSheetFrame;

    // ------------------------------------------------------------------

    TexturesManager();
    ~TexturesManager();

    bool                AddTexture(const char *pFileName);
    bool                AddTextureSheet(const char *pFileName);

    void                RemoveAllTextures();
    void                RemoveTexture(const Textures index);

    const char          *GetTextureFileName(const Textures index);
    int                 GetTextureIndex(const char *pFileName);

    int                 GetTextureWidth(const Textures index);
    int                 GetTextureHeight(const Textures index);

    int                 *GetTextureBuffer(const Textures index);

    TextureSheetFrame   *GetTextureSheetFrame(const Textures textureIndex, const int frameIndex);

private:
    char                m_fileNames[NUM_MAX_TEXTURES][256];

    int                 m_texturesWidths[NUM_MAX_TEXTURES];
    int                 m_texturesHeights[NUM_MAX_TEXTURES];

    int                 *m_texturesBuffers[NUM_MAX_TEXTURES];

    List                m_texturesFrames[NUM_MAX_TEXTURES];

    // ------------------------------------------------------------------

    void                RemoveTextureFrames(const Textures index);
};

extern TexturesManager *g_pTexturesManager;

#endif
