#include "Swatch.h"
#include "Color.h"

Swatch::Swatch(){
    mRed = Color::red(0xffffffff);
    mGreen = Color::green(0xffffffff);
    mBlue = Color::blue(0xffffffff);
    mRgb = 0xffffffff;
    mPopulation = 0;
    mGeneratedTextColors = false;
    mHsl = NULL;
}
Swatch::Swatch(uint32_t color, int population)
{
    mRed = Color::red(color);
    mGreen = Color::green(color);
    mBlue = Color::blue(color);
    mRgb = color;
    mPopulation = population;
    mGeneratedTextColors = false;
    mHsl = NULL;
};

Swatch::Swatch(uint32_t red, uint32_t green, uint32_t blue, int population)
{
    mRed = red;
    mGreen = green;
    mBlue = blue;
    mRgb = Color::rgb(red, green, blue);
    mPopulation = population;
    mGeneratedTextColors = false;
};

Swatch::Swatch(float *hsl, int population)
{
    uint32_t color = Color::HSLToColor(hsl);
    
    mRed = Color::red(color);
    mGreen = Color::green(color);
    mBlue = Color::blue(color);
    mRgb = color;
    mPopulation = population;
    mHsl = hsl;
    mGeneratedTextColors = false;
};

uint32_t Swatch::getRgb()
{
    return mRgb;
};

float *Swatch::getHsl()
{
    if(mHsl == NULL){
        mHsl = new float[3];
    }

    Color::RGBToHSL(mRed, mGreen, mBlue, mHsl);
    return mHsl;
};

int Swatch::getPopulation()
{
    return mPopulation;
};

uint32_t Swatch::getTitleTextColor()
{
    ensureTextColorsGenerated();
    return mTitleTextColor;
};

uint32_t Swatch::getBodyTextColor()
{
    ensureTextColorsGenerated();
    return mBodyTextColor;
};

void Swatch::ensureTextColorsGenerated()
{
    if(!mGeneratedTextColors){
        const uint32_t lightBodyAlpha = Color::calculateMinimumAlpha(Color::WHITE, mRgb, MIN_CONTRAST_BODY_TEXT);
        const uint32_t lightTitleAlpha = Color::calculateMinimumAlpha(Color::WHITE, mRgb, MIN_CONTRAST_TITLE_TEXT);

        if(lightBodyAlpha != -1 && lightTitleAlpha != -1){
            mBodyTextColor = Color::setAlphaComponent(Color::WHITE, lightBodyAlpha);
            mTitleTextColor = Color::setAlphaComponent(Color::WHITE, lightTitleAlpha);
            mGeneratedTextColors = true;
            return;
        }

        const uint32_t darkBodyAlpha = Color::calculateMinimumAlpha(Color::BLACK, mRgb, MIN_CONTRAST_BODY_TEXT);
        const uint32_t darkTitleAlpha = Color::calculateMinimumAlpha(Color::BLACK, mRgb, MIN_CONTRAST_TITLE_TEXT);

        if(darkBodyAlpha != -1 && darkTitleAlpha != -1){
            mBodyTextColor = Color::setAlphaComponent(Color::BLACK, darkBodyAlpha);
            mTitleTextColor = Color::setAlphaComponent(Color::BLACK, darkTitleAlpha);
            mGeneratedTextColors = true;
            return;
        }

        // If we reach here then we can not find title and body values which use the same
        // lightness, we need to use mismatched values
        mBodyTextColor = lightBodyAlpha != -1
                ? Color::setAlphaComponent(Color::WHITE, lightBodyAlpha)
                : Color::setAlphaComponent(Color::BLACK, darkBodyAlpha);
        mTitleTextColor = lightTitleAlpha != -1
                ? Color::setAlphaComponent(Color::WHITE, lightTitleAlpha)
                : Color::setAlphaComponent(Color::BLACK, darkTitleAlpha);
        mGeneratedTextColors = true;
    }
};

bool Swatch::equals(Swatch *s)
{
    if(this == s){
        return true;
    }

    if(s == NULL){
        return false;
    }

    return mPopulation == s->mPopulation && mRgb == s->mRgb;
};

int Swatch::hashCode()
{
    return 31 + mRgb + mPopulation;
};

std::string Swatch::toString(){
    uint32_t tColor = getTitleTextColor();
    uint32_t bColor = getBodyTextColor();
    std::string color = Color::ToString(mRgb);
    std::string result = "{\"color\": ";
    std::string titleColor = Color::ToString(tColor);
    std::string bodyColor = Color::ToString(bColor);

    return result +
        color +
        ", \"population\": " +
        std::to_string(mPopulation) +
        ", \"hue\": " +
        std::to_string(mHsl[0]) +
        ", \"saturation\": " +
        std::to_string(mHsl[1]) +
        ", \"lightness\": " +
        std::to_string(mHsl[2]) +
        ", \"titleTextColor\": " +
        titleColor +
        ", \"bodyTextColor\": " +
        bodyColor + "}";
        
};