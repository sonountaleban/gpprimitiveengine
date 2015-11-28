#ifndef _COLOR_H_
#define _COLOR_H_

class Color
{
public:
    int         m_red;
    int         m_green;
    int         m_blue;
    int         m_alpha;
    int         m_rgba;

    // ------------------------------------------------------------------

    Color();
    Color(const int red, const int green, const int blue);
    Color(const int red, const int green, const int blue, const int alpha);
    ~Color();

    void        Set(const int red, const int green, const int blue, const int alpha);
    void        CalcRGBA();
    void        CalcRedGreenBlueAlpha();
    void        Blend(Color *pColor);

    static void CopyColor(Color *pDst, const Color *pSrc);
    static void SubColors(Color *pDst, const Color *pSrc1, const Color *pSrc2);
    static void MultiplyColors(Color *pDst, const Color *pSrc1, const Color *pSrc2);
    static void InterpColors(Color *pDst, const Color *pSrc1, const Color *pSrc2, const float rate);
    static void SwapColors(Color *pColor1, Color *pColor2);
};

#endif
