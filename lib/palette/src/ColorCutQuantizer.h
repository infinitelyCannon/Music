#include <vector>
#include <nan.h>
#include "Swatch.h"
#include "Color.h"
#include "Filter.h"
#ifndef _COLOR_QUANT_H
#define _COLOR_QUANT_H

struct Vbox{
    int mLowerIndex;
    int mUpperIndex;

    int mPopulation;

    int mMinRed;
    int mMaxRed;
    int mMinGreen;
    int mMaxGreen;
    int mMinBlue;
    int mMaxBlue;
};

class ColorCutQuantizer{
    public:
        ColorCutQuantizer(std::vector<uint32_t> pixels, int maxColors, v8::Local<v8::Function> *filters, bool useDefault);
        void getQuantizedColors(std::vector<Swatch> *s);
        static void modifySignificantOctect(std::vector<int> *a, int dimension, int lower, int upper);
        static int quantizedRed(int color);
        static int quantizedGreen(int color);
        static int quantizedBlue(int color);
        const static int COMPONENT_RED = -3;
        const static int COMPONENT_GREEN = -2;
        const static int COMPONENT_BLUE = -1;
    private:
        const static int QUANTIZE_WORD_WIDTH = 5;
        const static int QUANTIZE_WORD_MASK = (1 << QUANTIZE_WORD_WIDTH) - 1;
        bool useDefaultFilter;

        std::vector<int> mColors;
        std::vector<int> mHistogram;
        std::vector<Swatch> mQuantizedColors;
        Nan::Callback *mFilters = NULL;
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
        bool shouldIgnoreColor(int color565);
        bool shouldIgnoreColor(Swatch color);
        bool shouldIgnoreColor(int rgb, float *hsl);
        static int quantizeFromRgb888(int color);
        static int approximateToRgb888(int color);
        static int approximateToRgb888(int r, int g, int b);
        static int modifyWordWidth(int value, int currentWidth, int targetWidth);
        void splitBoxes(std::vector<Vbox> *boxes, int maxSize);
        std::vector<Swatch> *generateAverageColors(std::vector<Vbox> vboxes);

        bool isAllowed(int rgb, float *hsl){
            return !(hsl[2] >= 0.95f) && // Is considered white
            !(hsl[2] <= 0.05f) && // Is considered black
            !(hsl[0] >= 10.0f && hsl[0] <= 37.0f && hsl[1] <= 0.82f);// Is near red side of I line
        }
};

#endif