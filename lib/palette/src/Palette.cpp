#include "Palette.h"
#include "ColorCutQuantizer.h"
#include "Color.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <memory>

#define MIN_ALPHA_CONTRAST 4.5

/*
    Todo
    -Switch mSelectedSwatches to an unordered_map and establish all the
    necessary components (Hash key, KeyEqualm etc.) to fix the error of
    no comparator existing for Target < Target.
    -Add public get functions
*/

struct DistanceCompare{
    int color;

    bool operator()(Swatch a, Swatch b) const {
        return Color::calculateDistance(b.getRgb(), color) > 
        Color::calculateDistance(a.getRgb(), color);
    }
};

Palette::Palette(Bitmap *bitmap){
    if(bitmap == NULL){
        std::cout << "Error: bitmap is nulled" << std::endl;
    }

    //mFilters.push_back(DEFAULT_FILTER);
    mBitmap = bitmap;
    mSwatches.clear();

    TargetMeta temp;
    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 1.0f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.name = "lightVibrant";

    LIGHT_VIBRANT = Target(&temp);

    mTargets.push_back(LIGHT_VIBRANT);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.name = "vibrant";

    VIBRANT = Target(&temp);

    mTargets.push_back(VIBRANT);

    temp.mLightnessTargets[0] = 0.0f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.name = "darkVibrant";

    DARK_VIBRANT = Target(&temp);

    mTargets.push_back(DARK_VIBRANT);

    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 1.0f;

    temp.mSaturationTargets[0] = 0.0f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.name = "lightMuted";

    LIGHT_MUTED = Target(&temp);

    mTargets.push_back(LIGHT_MUTED);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.0f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.name = "muted";

    MUTED = Target(&temp);

    mTargets.push_back(MUTED);

    temp.mLightnessTargets[0] = 0.0f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.0f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.name = "darkMuted";

    DARK_MUTED = Target(&temp);

    mTargets.push_back(DARK_MUTED);
};

Palette::Palette(std::vector<Swatch> swatches){
    //mFilters.push_back(DEFAULT_FILTER);
    std::copy(swatches.begin(), swatches.end(), mSwatches.begin());
    mBitmap = NULL;

    TargetMeta temp;
    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 1.0f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.name = "lightVibrant";

    LIGHT_VIBRANT = Target(&temp);

    mTargets.push_back(LIGHT_VIBRANT);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.name = "vibrant";

    VIBRANT = Target(&temp);

    mTargets.push_back(VIBRANT);

    temp.mLightnessTargets[0] = 0.0f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.name = "darkVibrant";

    DARK_VIBRANT = Target(&temp);

    mTargets.push_back(DARK_VIBRANT);

    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 1.0f;

    temp.mSaturationTargets[0] = 0.0f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.name = "lightMuted";

    LIGHT_MUTED = Target(&temp);

    mTargets.push_back(LIGHT_MUTED);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.0f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.name = "muted";

    MUTED = Target(&temp);

    mTargets.push_back(MUTED);

    temp.mLightnessTargets[0] = 0.0f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.0f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.name = "darkMuted";

    DARK_MUTED = Target(&temp);

    mTargets.push_back(DARK_MUTED);
};

void Palette::maximumColorCount(int colors){
    mMaxColors = colors;
};

void Palette::resizeBitmapArea(int area){
    mResizeArea = area;
    mResizeMaxDimension = -1;
};

/* void Palette::clearFilters(){
    mFilters.clear();
};

void Palette::addFilter(Filter filter){
    if(&filter != NULL)
        mFilters.push_back(filter);
}; */

bool Palette::setRegion(int l, int t, int r, int b){
    if(mBitmap != NULL){
        if(mRegion == NULL) mRegion = new Rect();

        mRegion->set(0, 0, mBitmap->width, mBitmap->height);

        if(!mRegion->intersect(l, t, r, b)){
            mRegion = NULL;
            return false;
        }
    }

    return true;
};

void Palette::clearRegion(){
    if(mRegion != NULL){
        delete mRegion;
        mRegion = NULL;
    }
};

void Palette::addTarget(TargetMeta meta){
    Target tar(&meta);

    for(int i = 0; i < (int) mTargets.size(); i++){
        if(Target::isEqual(mTargets[i], tar))
            return;
    }

    mTargets.push_back(tar);
};

void Palette::addTarget(Target target){
    for(int i = 0; i < (int) mTargets.size(); i++){
        if(Target::isEqual(mTargets[i], target))
            return;
    }

    mTargets.push_back(target);
};

void Palette::clearTargets(){
    mTargets.clear();
};

std::vector<int> Palette::getPixelsFromBitmap(Bitmap *bitmap){
    int bWidth = bitmap->width;
    //int bHeight = bitmap->height;
    int regionWidth, regionHeight;
    
    std::vector<int> pixels(bitmap->pixels.begin(), bitmap->pixels.end());

    if(mRegion == NULL){
        return pixels;
    }

    regionWidth = mRegion->width();
    regionHeight = mRegion->height();

    std::vector<int> subsetPixels(regionWidth * regionHeight);

    for(int row = 0; row < regionHeight; row++){
        std::copy(
            pixels.begin() + ((row + mRegion->top) * bWidth) + mRegion->left,
            pixels.begin() + regionWidth + ((row + mRegion->top) * bWidth) + mRegion->left,
            subsetPixels.begin() + (row * regionWidth)
        );
    }

    return subsetPixels;
};

float *Palette::copyHslValues(Swatch color){
    std::allocator<float> af;
    float *newHsl = af.allocate(3);
    int sColor = color.getRgb();

    Color::RGBToHSL(Color::red(sColor), Color::green(sColor), Color::blue(sColor), newHsl);

    return newHsl;
};

Swatch *Palette::findDominantSwatch(){
    int maxPop = INT32_MIN;
    Swatch *maxSwatch = NULL;
    int resultIndex = -1;

    for(int i = 0; i < (int) mSwatches.size(); i++){
        Swatch swatch = mSwatches[i];
        if(swatch.getPopulation() > maxPop){
            resultIndex = i;
            maxPop = swatch.getPopulation();
        }
    }

    if(resultIndex == -1){
        return maxSwatch;
    }
    else{
        maxSwatch = &mSwatches[resultIndex];
        return maxSwatch;
    }
};

float Palette::generateScore(Swatch swatch, Target target){
    float *hsl = swatch.getHsl();

    float saturationScore = 0;
    float luminanceScore = 0;
    float populationScore = 0;

    int maxPopulation = mDominantSwatch != NULL ? mDominantSwatch->getPopulation() : 1;

    if(target.getSaturationWeight() > 0){
        saturationScore = target.getSaturationWeight() *
        (1.0f - std::abs(hsl[1] - target.getTargetSaturation()));
    }
    if(target.getLightnessWeight() > 0){
        luminanceScore = target.getLightnessWeight() *
        (1.0f - std::abs(hsl[2] - target.getTargetLightness()));
    }
    if(target.getPopulationWeight() > 0){
        populationScore = target.getPopulationWeight() *
        (swatch.getPopulation() / (float) maxPopulation);
    }

    return saturationScore + luminanceScore + populationScore;
};

bool Palette::shouldBeScoredForTarget(Swatch swatch, Target target){
    float *hsl = swatch.getHsl();
    auto used = mUsedColors.find(swatch.getRgb());

    return hsl[1] >= target.getMinimumSaturation() &&
    hsl[1] <= target.getMaximumSaturation() &&
    hsl[2] >= target.getMinimumLightness() &&
    hsl[2] <= target.getMaximumLightness() &&
    !(used == mUsedColors.end() ? false : used->second);
};

Swatch Palette::getMaxScoredSwatchForTarget(Target target){
    float maxScore = 0.0f;
    Swatch maxScoreSwatch(0x00ffffff, -1);
    bool changed = false;

    for(int i = 0, count = mSwatches.size(); i < count; i++){
        Swatch swatch = mSwatches[i];
        if(shouldBeScoredForTarget(swatch, target)){
            float score = generateScore(swatch, target);
            if(!changed || score > maxScore){
                maxScoreSwatch.copyValues(swatch);
                maxScore = score;
                changed = true;
            }
        }
    }

    return maxScoreSwatch;
};

Swatch Palette::generateScoredTarget(Target target){
    Swatch maxScoreSwatch = getMaxScoredSwatchForTarget(target);

    if(maxScoreSwatch.getRgb() != 0x00ffffff && maxScoreSwatch.getPopulation() != -1 && target.isExclusive()){
        mUsedColors.insert(std::pair <int, bool> (maxScoreSwatch.getRgb(), true));
    }

    return maxScoreSwatch;
};

void Palette::generate(std::vector<Swatch> swatches, std::vector<Target> targets){
    std::copy(swatches.begin(), swatches.end(), mSwatches.begin());
    std::copy(targets.begin(), targets.end(), mTargets.begin());

    mDominantSwatch = findDominantSwatch();

    for(int i = 0, count = mTargets.size(); i < count; i++){
        Target target = targets[i];
        target.normalizeWeights();
        mSelectedSwatches.insert(std::pair <Target, Swatch> (target, generateScoredTarget(target)));
    }

    mUsedColors.clear();
};

int Palette::getDominantColor(){
    return mDominantSwatch->getRgb();
};

std::string Palette::generate(v8::Local<v8::Value> filters, bool useDefault){
    std::string result = "{";

    if(mBitmap != NULL){
        ColorCutQuantizer quantizer(getPixelsFromBitmap(mBitmap),
        mMaxColors,
        filters, useDefault);

        quantizer.getQuantizedColors(mSwatches);
    }
    
    mDominantSwatch = findDominantSwatch();
    unsigned fallbackColors[] = {0xffffffff, 0xff000000};
    int textColor, backgroundColor;

    for(int i = 0, count = mTargets.size(); i < count; i++){
        Target target = mTargets[i];
        target.normalizeWeights();
        mSelectedSwatches.insert(std::pair <Target, Swatch> (target, generateScoredTarget(target)));
    }

    mUsedColors.clear();

    getMatchingColors(fallbackColors, false, backgroundColor, textColor);

    for(auto t : mSelectedSwatches){
        result.append("\"")
            .append(t.first.name)
            .append("\": ")
            .append(t.second.getPopulation() == -1 ? "null" : t.second.toString())
            .append(",");
    }

    result.append("\"dominantColor\": ")
        .append(Color::ToString(getDominantColor()))
        .append(",\"textColor\": ")
        .append(Color::ToString(textColor))
        .append(",\"backgroundColor\": ")
        .append(Color::ToString(backgroundColor))
        .append("}");
    
    return result;
};

void Palette::getMatchingColors(unsigned fallback[], bool invert, int &matchingColor, int &swatchColor){
    if(mDominantSwatch != NULL){
        std::vector<Swatch> swatches(mSwatches);
        int swatchC = mDominantSwatch->getRgb();
        int matchC = -1;
        DistanceCompare compare;
        compare.color = swatchC;
        std::sort(swatches.begin(), swatches.end(), compare);

        for(int i = 0; i < (int) swatches.size(); i++){
            if(Color::calculateContrast(swatchC, swatches[i].getRgb()) > MIN_ALPHA_CONTRAST){
                matchC = swatches[0].getRgb();
                break;
            }
        }

        if(matchC == -1){
            matchC = mDominantSwatch->getBodyTextColor();
        }

        if(invert){
            matchingColor = swatchC;
            swatchColor = matchC;
            return;
        }

        matchingColor = matchC;
        swatchColor = swatchC;
        return;
    }

    matchingColor = fallback[1];
    swatchColor = fallback[0];
};

/*
    There's a function within Palette called:
    private Bitmap scaleBitmapDown(final Bitmap bitmap)
    But I'm ingnoring it for now under the assumption that I won't need it.
*/