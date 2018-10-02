#include "ColorCutQuantizer.h"
#include <algorithm>
#include <cstdint>
#include <cmath>

struct compare{
    bool operator()(Vbox a, Vbox b) const{
        return ((a.mMaxRed - a.mMinRed + 1) *
        (a.mMaxGreen - a.mMinGreen + 1) *
        (a.mMaxBlue - a.mMinBlue + 1))
        <
        ((b.mMaxRed - b.mMinRed + 1) *
        (b.mMaxGreen - b.mMinGreen + 1) *
        (b.mMaxBlue - b.mMinBlue + 1));
    }
} compare;

ColorCutQuantizer::ColorCutQuantizer(std::vector<uint32_t> pixels, int maxColors, v8::Local<v8::Value> filters, bool useDefault){
    std::vector<int> hist(1 << (QUANTIZE_WORD_WIDTH * 3), 0);
    
    useDefaultFilter = useDefault;
    
    /* if(!filters->IsNull()){
        //Nan::Callback callback();
        //mFilters(v8::Local<v8::Function>::Cast(filters));
        mFilters = filters;
    } */

    mFilters = filters;

    for(int i = 0; i < (int) pixels.size(); i++){
        uint32_t quantizedColor = quantizeFromRgb888(pixels[i]);
        
        pixels[i] = quantizedColor;

        hist[quantizedColor]++;
    }

    int distinctColorCount = 0;
    for(int color = 0; color < (int) hist.size(); color++){
        if(hist[color] > 0 && shouldIgnoreColor(color)){
            hist[color] = 0;
        }
        if(hist[color] > 0){
            distinctColorCount++;
        }
    }

    std::vector<uint32_t> colors(distinctColorCount, 0);

    int distinctColorIndex = 0;
    for(int color = 0; color < (int) hist.size(); color++){
        if(hist[color] > 0){
            colors[distinctColorIndex++] = color;
        }
    }

    if(distinctColorCount <= maxColors){
        for(int c : colors){
            Swatch *s = new Swatch(approximateToRgb888(c), hist[c]);
            mQuantizedColors.push_back(*s);
        }
    }
    else{
        std::vector<Swatch> temp = quantizePixels(maxColors);
        for(Swatch s : temp){
            mQuantizedColors.push_back(s);
        }
    }

    for(int i = 0; i < colors.size(); i++)
        mColors.push_back(colors[i]);
    
    for(int j = 0; j < hist.size(); j++)
        mHistogram.push_back(hist[j]);
    
    //std::copy(hist.begin(), hist.end(), mHistogram.begin());
    //std::copy(colors.begin(), colors.end(), mColors.begin());
};

bool ColorCutQuantizer::shouldIgnoreColor(uint32_t color565){
    uint32_t rgb = approximateToRgb888(color565);
    Color::RGBToHSL(Color::red(rgb), Color::green(rgb), Color::blue(rgb), mTempHsl);
    return shouldIgnoreColor(rgb, mTempHsl);
};

bool ColorCutQuantizer::shouldIgnoreColor(Swatch color){
    return shouldIgnoreColor(color.getRgb(), color.getHsl());
};

bool ColorCutQuantizer::shouldIgnoreColor(uint32_t rgb, float hsl[]){
    if(!mFilters->IsNull()){
        const int argc = 4;
        v8::Local<v8::Value> args[argc];
        v8::Local<v8::Function> tempFunc = v8::Local<v8::Function>::Cast(mFilters);
        Nan::Callback cb(tempFunc);

        args[0] = Nan::New(rgb);
        args[1] = Nan::New(hsl[0]);
        args[2] = Nan::New(hsl[1]);
        args[3] = Nan::New(hsl[2]);

        v8::Local<v8::Value> returnVal = cb.Call(argc, args);

        return !returnVal->BooleanValue();
    }
    else if(mFilters->IsNull() && useDefaultFilter){
        if(!isAllowed(rgb, hsl)) return true;
    }

    return false;
};

uint32_t ColorCutQuantizer::modifyWordWidth(uint32_t value, int currentWidth, int targetWidth){
    uint32_t newValue;

    if(targetWidth > currentWidth){
        newValue = value << (targetWidth - currentWidth);
    }
    else{
        newValue = value >> (currentWidth - targetWidth);
    }

    return newValue & ((1 << targetWidth) - 1);
};

uint32_t ColorCutQuantizer::approximateToRgb888(uint32_t r, uint32_t g, uint32_t b){
    return Color::rgb(modifyWordWidth(r, QUANTIZE_WORD_WIDTH, 8),
                modifyWordWidth(g, QUANTIZE_WORD_WIDTH, 8),
                modifyWordWidth(b, QUANTIZE_WORD_WIDTH, 8));
};

uint32_t ColorCutQuantizer::approximateToRgb888(uint32_t color){
    return approximateToRgb888(quantizedRed(color), quantizedGreen(color), quantizedBlue(color));
};

uint32_t ColorCutQuantizer::quantizeFromRgb888(uint32_t color){
    uint32_t r = modifyWordWidth(Color::red(color), 8, QUANTIZE_WORD_WIDTH);
    uint32_t g = modifyWordWidth(Color::green(color), 8, QUANTIZE_WORD_WIDTH);
    uint32_t b = modifyWordWidth(Color::blue(color), 8, QUANTIZE_WORD_WIDTH);
    return r << (QUANTIZE_WORD_WIDTH + QUANTIZE_WORD_WIDTH) | g << QUANTIZE_WORD_WIDTH | b;
};

uint32_t ColorCutQuantizer::quantizedRed(uint32_t color){
    return (color >> (QUANTIZE_WORD_WIDTH + QUANTIZE_WORD_WIDTH)) & QUANTIZE_WORD_MASK;
};

uint32_t ColorCutQuantizer::quantizedBlue(uint32_t color){
    return color & QUANTIZE_WORD_MASK;
};

uint32_t ColorCutQuantizer::quantizedGreen(uint32_t color){
    return (color >> QUANTIZE_WORD_WIDTH) & QUANTIZE_WORD_MASK;
};

Swatch ColorCutQuantizer::getAverageColor(Vbox box){
    uint32_t redSum = 0;
    uint32_t greenSum = 0;
    uint32_t blueSum = 0;
    int totalPopulation = 0;

    for(int i = box.mLowerIndex; i <= box.mUpperIndex; i++){
        uint32_t color = mColors[i];
        int colorPopulation = mHistogram[i];

        totalPopulation += colorPopulation;
        redSum += colorPopulation * ColorCutQuantizer::quantizedRed(color);
        greenSum += colorPopulation * ColorCutQuantizer::quantizedGreen(color);
        blueSum += colorPopulation * ColorCutQuantizer::quantizedBlue(color);
    }

    uint32_t redMean = (uint32_t) std::round(redSum / (float) totalPopulation);
    uint32_t greenMean = (uint32_t) std::round(greenSum / (float) totalPopulation);
    uint32_t blueMean = (uint32_t) std::round(blueSum / (float) totalPopulation);

    Swatch s(ColorCutQuantizer::approximateToRgb888(redMean, greenMean, blueMean), totalPopulation);
    return s;
};

void ColorCutQuantizer::modifySignificantOctect(std::vector<uint32_t> *a, int dimension, int lower, int upper){
    switch (dimension)
    {
        case COMPONENT_RED:
            // Already in RGB, no need to do anything
            break;
    
        case COMPONENT_GREEN:
            for(int i = lower; i <= upper; i++){
                uint32_t color = (*a)[i];
                (*a)[i] = quantizedGreen(color) << (QUANTIZE_WORD_WIDTH + QUANTIZE_WORD_WIDTH) |
                quantizedRed(color) << QUANTIZE_WORD_WIDTH |
                quantizedBlue(color);
            }
            break;
        case COMPONENT_BLUE:
            for(int i = lower; i <= upper; i++){
                uint32_t color = (*a)[i];
                (*a)[i] = quantizedBlue(color) << (QUANTIZE_WORD_WIDTH + QUANTIZE_WORD_WIDTH) |
                quantizedGreen(color) << QUANTIZE_WORD_WIDTH |
                quantizedRed(color);
            }
            break;
    }
};

int ColorCutQuantizer::findSplitPoint(Vbox box){
    int longestDimension = getLongestColorDimension(box);
    std::vector<uint32_t> colors(mColors);
    std::vector<int> hist(mHistogram);

    modifySignificantOctect(&colors, longestDimension, box.mLowerIndex, box.mUpperIndex);

    std::sort(colors.begin() + box.mLowerIndex, colors.begin() + box.mUpperIndex + 1);

    modifySignificantOctect(&colors, longestDimension, box.mLowerIndex, box.mUpperIndex);

    int midPoint = box.mPopulation / 2;
    for(int i = box.mLowerIndex, count = 0; i <= box.mUpperIndex; i++){
        count += hist[colors[i]];
        if(count >= midPoint){
            return std::min(box.mUpperIndex - 1, i);
        }
    }

    return box.mLowerIndex;
}

int ColorCutQuantizer::getLongestColorDimension(Vbox box){
    int redLength = box.mMaxRed - box.mMinRed;
    int greenLength = box.mMaxGreen - box.mMinGreen;
    int blueLength = box.mMaxBlue - box.mMinBlue;

    if (redLength >= greenLength && redLength >= blueLength) {
        return COMPONENT_RED;
    } else if (greenLength >= redLength && greenLength >= blueLength) {
        return COMPONENT_GREEN;
    } else {
        return COMPONENT_BLUE;
    }
};

Vbox ColorCutQuantizer::splitBox(Vbox& box){
    if(!canSplit(box)){
        //Should catch this beforehand. Error to be declaired later.
    }

    int splitPoint = findSplitPoint(box);

    Vbox newBox = newVbox(splitPoint + 1, box.mUpperIndex);

    box.mUpperIndex = splitPoint;
    fitBox(box);

    return newBox;
};

void ColorCutQuantizer::fitBox(Vbox& box){
    std::vector<uint32_t> colors(mColors);
    std::vector<int> hist(mHistogram);

    uint32_t minRed, minGreen, minBlue;
    minRed = minGreen = minBlue = INT32_MAX;
    uint32_t maxRed, maxGreen, maxBlue;
    maxRed = maxGreen = maxBlue = INT32_MIN;
    int count = 0;

    for(int i = box.mLowerIndex; i <= box.mUpperIndex; i++){
        int color = colors[i];
        count += hist[color];

        uint32_t r = quantizedRed(color);
        uint32_t g = quantizedGreen(color);
        uint32_t b = quantizedBlue(color);

        if (r > maxRed) {
            maxRed = r;
        }
        if (r < minRed) {
            minRed = r;
        }
        if (g > maxGreen) {
            maxGreen = g;
        }
        if (g < minGreen) {
            minGreen = g;
        }
        if (b > maxBlue) {
            maxBlue = b;
        }
        if (b < minBlue) {
            minBlue = b;
        }
    }

    box.mMinRed = minRed;
    box.mMaxRed = maxRed;
    box.mMinGreen = minGreen;
    box.mMaxGreen = maxGreen;
    box.mMinBlue = minBlue;
    box.mMaxBlue = maxBlue;
    box.mPopulation = count;
};

Vbox ColorCutQuantizer::newVbox(int lowerIndex, int upperIndex){
    Vbox box;

    box.mLowerIndex = lowerIndex;
    box.mUpperIndex = upperIndex;

    fitBox(box);

    return box;
};

void ColorCutQuantizer::splitBoxes(std::vector<Vbox> *boxes, int maxSize){
    while((int) boxes->size() < maxSize){
        Vbox vbox = boxes->back();

        if(/*&vbox != NULL && */canSplit(vbox)){
            boxes->push_back(splitBox(vbox));
            std::sort(boxes->begin(), boxes->end(), compare);

            boxes->push_back(vbox);
            std::sort(boxes->begin(), boxes->end(), compare);
        }
        else{
            return;
        }
    }
};

std::vector<Swatch> ColorCutQuantizer::generateAverageColors(std::vector<Vbox> vboxes){
    std::vector<Swatch> colors;
    for(Vbox box : vboxes){
        Swatch s = getAverageColor(box);
        if(!shouldIgnoreColor(s)){
            colors.push_back(s);
        }
    }

    return colors;
};

std::vector<Swatch> ColorCutQuantizer::quantizePixels(int maxColors){
    std::vector<Vbox> pq;

    pq.push_back(newVbox(0, mColors.size() - 1));
    std::sort(pq.begin(), pq.end(), compare);

    splitBoxes(&pq, maxColors);

    return generateAverageColors(pq);
};

void ColorCutQuantizer::getQuantizedColors(std::vector<Swatch>& s){
    std::copy(mQuantizedColors.begin(), mQuantizedColors.end(), s.begin());
};

int ColorCutQuantizer::getVolume(Vbox box){
    return (box.mMaxRed - box.mMinRed + 1) *
    (box.mMaxGreen - box.mMinGreen + 1) *
    (box.mMaxBlue - box.mMinBlue + 1);
};

int ColorCutQuantizer::getColorCount(Vbox box){
    return 1 + box.mUpperIndex - box.mLowerIndex;
};

bool ColorCutQuantizer::canSplit(Vbox box){
    return getColorCount(box) > 1;
};