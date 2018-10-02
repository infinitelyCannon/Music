#include <string>
#ifndef _SWATCH_H
#define _SWATCH_H

class Swatch{
    public:
        Swatch();
        Swatch(uint32_t color, int population);
        Swatch(uint32_t red, uint32_t green, uint32_t blue, int population);
        Swatch(float *hsl, int population);
        uint32_t getRgb();
        float *getHsl();
        int getPopulation();
        uint32_t getTitleTextColor();
        uint32_t getBodyTextColor();
        std::string toString();
        bool equals(Swatch *s);
        int hashCode();
        void copyValues(Swatch s);
    private:
        uint32_t mRed;
        uint32_t mGreen;
        uint32_t mBlue;
        uint32_t mRgb;
        int mPopulation;
        uint32_t mTitleTextColor;
        uint32_t mBodyTextColor;

        bool mGeneratedTextColors;

        float mHsl[3] = {0.0f, 0.0f, 0.0f};
        float MIN_CONTRAST_BODY_TEXT = 4.5f;
        float MIN_CONTRAST_TITLE_TEXT = 3.0f;

        void ensureTextColorsGenerated();
};

#endif