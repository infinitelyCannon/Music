#include <string>
#ifndef _SWATCH_H
#define _SWATCH_H

class Swatch{
    public:
        Swatch();
        Swatch(int color, int population);
        Swatch(int red, int green, int blue, int population);
        Swatch(float *hsl, int population);
        int getRgb();
        float *getHsl();
        int getPopulation();
        int getTitleTextColor();
        int getBodyTextColor();
        std::string toString();
        bool equals(Swatch *s);
        int hashCode();
        void copyValues(Swatch s);
    private:
        int mRed;
        int mGreen;
        int mBlue;
        int mRgb;
        int mPopulation;
        int mTitleTextColor;
        int mBodyTextColor;

        bool mGeneratedTextColors;

        float mHsl[3] = {0.0f, 0.0f, 0.0f};
        float MIN_CONTRAST_BODY_TEXT = 4.5f;
        float MIN_CONTRAST_TITLE_TEXT = 3.0f;

        void ensureTextColorsGenerated();
};

#endif