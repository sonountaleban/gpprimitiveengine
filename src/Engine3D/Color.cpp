#include "Engine3D/Color.h"

Color::Color()
{
    Set(0, 0, 0, 255);
}

Color::Color(const int red, const int green, const int blue)
{
    Set(red, green, blue, 255);
}

Color::Color(const int red, const int green, const int blue, const int alpha)
{
    Set(red, green, blue, alpha);
}

Color::~Color()
{
}

void Color::Set(const int red, const int green, const int blue, const int alpha)
{
    m_red = (unsigned int)red & 0xff;
    m_green = (unsigned int)green & 0xff;
    m_blue = (unsigned int)blue & 0xff;
    m_alpha = (unsigned int)alpha & 0xff;

    CalcRGBA();
}

void Color::CalcRGBA()
{
    m_rgba = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

void Color::CalcRedGreenBlueAlpha()
{
    m_red  = m_rgba & 0x000000ff;
    m_green = (m_rgba & 0x0000ff00) >> 8;
    m_blue = (m_rgba & 0x00ff0000) >> 16;
    m_alpha = (m_rgba & 0xff000000) >> 24;
}

void Color::Blend(Color *pColor)
{
    float r1;
    float g1;
    float b1;
    float r2;
    float g2;
    float b2;
    float rate;

    if (pColor)
    {
        r1 = (float)m_red;
        g1 = (float)m_green;
        b1 = (float)m_blue;
        r2 = (float)pColor->m_red;
        g2 = (float)pColor->m_green;
        b2 = (float)pColor->m_blue;
        rate = (float)pColor->m_alpha / 255.0f;

        m_red = (int)(r1 + (r2 - r1) * rate);
        m_green = (int)(g1 + (g2 - g1) * rate);
        m_blue = (int)(b1 + (b2 - b1) * rate);
        m_alpha = 255;

        CalcRGBA();
    }
}

void Color::CopyColor(Color *pDst, const Color *pSrc)
{
    if (pSrc && pDst)
    {
        pDst->m_red = pSrc->m_red;
        pDst->m_green = pSrc->m_green;
        pDst->m_blue = pSrc->m_blue;
        pDst->m_alpha = pSrc->m_alpha;
        pDst->m_rgba = pSrc->m_rgba;
    }
}

void Color::SubColors(Color *pDst, const Color *pSrc1, const Color *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_red = pSrc1->m_red - pSrc2->m_red;
        pDst->m_green = pSrc1->m_green - pSrc2->m_green;
        pDst->m_blue = pSrc1->m_blue - pSrc2->m_blue;
        pDst->m_alpha = pSrc1->m_alpha - pSrc2->m_alpha;
    }
}

void Color::MultiplyColors(Color *pDst, const Color *pSrc1, const Color *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_red = pSrc1->m_red * pSrc2->m_red / 255;
        pDst->m_green = pSrc1->m_green * pSrc2->m_green / 255;
        pDst->m_blue = pSrc1->m_blue * pSrc2->m_blue / 255;
        pDst->m_alpha = pSrc1->m_alpha * pSrc2->m_alpha / 255;
    }
}

void Color::InterpColors(Color *pDst, const Color *pSrc1, const Color *pSrc2, const float rate)
{
    float r1;
    float g1;
    float b1;
    float a1;
    float r2;
    float g2;
    float b2;
    float a2;

    if (!pDst || !pSrc1 || !pSrc2)
    {
        return;
    }

    r1 = (float)pSrc1->m_red;
    g1 = (float)pSrc1->m_green;
    b1 = (float)pSrc1->m_blue;
    a1 = (float)pSrc1->m_alpha;
    r2 = (float)pSrc2->m_red;
    g2 = (float)pSrc2->m_green;
    b2 = (float)pSrc2->m_blue;
    a2 = (float)pSrc2->m_alpha;

    pDst->m_red = (int)(r1 + (r2 - r1) * rate);
    pDst->m_green = (int)(g1 + (g2 - g1) * rate);
    pDst->m_blue = (int)(b1 + (b2 - b1) * rate);
    pDst->m_alpha = (int)(a1 + (a2 - a1) * rate);
}

void Color::SwapColors(Color *pColor1, Color *pColor2)
{
    int red;
    int green;
    int blue;
    int alpha;
    int rgba;

    if (pColor1 && pColor2)
    {
        red = pColor1->m_red;
        green = pColor1->m_green;
        blue = pColor1->m_blue;
        alpha = pColor1->m_alpha;
        rgba = pColor1->m_rgba;

        pColor1->m_red = pColor2->m_red;
        pColor1->m_green = pColor2->m_green;
        pColor1->m_blue = pColor2->m_blue;
        pColor1->m_alpha = pColor2->m_alpha;
        pColor1->m_rgba = pColor2->m_rgba;

        pColor2->m_red = red;
        pColor2->m_green = green;
        pColor2->m_blue = blue;
        pColor2->m_alpha = alpha;
        pColor2->m_rgba = rgba;
    }
}

