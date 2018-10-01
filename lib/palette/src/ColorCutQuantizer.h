#include <vector>
#include <nan.h>
#include "Swatch.h"
#include "Color.h"
#ifndef _COLOR_QUANT_H
#define _COLOR_QUANT_H

struct Vbox{
    int mLowerIndex;
    int mUpperIndex;

    int mPopulation;

    uint32_t mMinRed;
    uint32_t mMaxRed;
    uint32_t mMinGreen;
    uint32_t mMaxGreen;
    uint32_t mMinBlue;
    uint32_t mMaxBlue;
};

class ColorCutQuantizer{
    public:
        ColorCutQuantizer(std::vector<uint32_t> pixels, int maxColors, v8::Local<v8::Value> filters, bool useDefault);
        void getQuantizedColors(std::vector<Swatch>& s);
        static void modifySignificantOctect(std::vector<uint32_t> *a, int dimension, int lower, int upper);
        static uint32_t quantizedRed(uint32_t color);
        static uint32_t quantizedGreen(uint32_t color);
        static uint32_t quantizedBlue(uint32_t color);
        const static int COMPONENT_RED = -3;
        const static int COMPONENT_GREEN = -2;
        const static int COMPONENT_BLUE = -1;
    private:
        const static int QUANTIZE_WORD_WIDTH = 5;
        const static int QUANTIZE_WORD_MASK = (1 << QUANTIZE_WORD_WIDTH) - 1;
        bool useDefaultFilter;

        std::vector<uint32_t> mColors;
        std::vector<int> mHistogram;
        std::vector<Swatch> mQuantizedColors;
        v8::Local<v8::Value> mFilters;
        float mTempHsl[3] = {0.0f, 0.0f, 0.0f};

        template<typename T> bool compareBox(T left, T right){
            return getVolume(left) < getVolume(right);
        }

        int getVolume(Vbox box);
        int getColorCount(Vbox box);
        bool canSplit(Vbox box);
        void fitBox(Vbox *box);
        Vbox *splitBox(Vbox *box);
        int getLongestColorDimension(Vbox box);
        int findSplitPoint(Vbox *box);
        Swatch *getAverageColor(Vbox box);
        Vbox *newVbox(int lowerIndex, int upperIndex);

        std::vector<Swatch> *quantizePixels(int maxColors);
        bool shouldIgnoreColor(uint32_t color565);
        bool shouldIgnoreColor(Swatch color);
        bool shouldIgnoreColor(uint32_t rgb, float hsl[]);
        static uint32_t quantizeFromRgb888(uint32_t color);
        static uint32_t approximateToRgb888(uint32_t color);
        static uint32_t approximateToRgb888(uint32_t r, uint32_t g, uint32_t b);
        static uint32_t modifyWordWidth(uint32_t value, int currentWidth, int targetWidth);
        void splitBoxes(std::vector<Vbox> *boxes, int maxSize);
        std::vector<Swatch> *generateAverageColors(std::vector<Vbox> vboxes);

        bool isAllowed(uint32_t rgb, float hsl[]){
            return !(hsl[2] >= 0.95f) && // Is considered white
            !(hsl[2] <= 0.05f) && // Is considered black
            !(hsl[0] >= 10.0f && hsl[0] <= 37.0f && hsl[1] <= 0.82f);// Is near red side of I line
        }
};

#endif