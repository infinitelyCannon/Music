#include "Palette.h"
#include "ColorCutQuantizer.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <memory>

/*
    Todo
    -Switch mSelectedSwatches to an unordered_map and establish all the
    necessary components (Hash key, KeyEqualm etc.) to fix the error of
    no comparator existing for Target < Target.
    -Add public get functions
*/

Palette::Palette(Bitmap *bitmap){
    if(bitmap == NULL){
        std::cout << "Error: bitmap is nulled" << std::endl;
    }

    //mFilters.push_back(DEFAULT_FILTER);
    mBitmap = bitmap;
    mSwatches.clear();

    TargetMeta temp;
    temp.mLightnessTargets = new float[3];
    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 0.74f;

    temp.mSaturationTargets = new float[3];
    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.mWeights = new float[3];
    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;
    temp.name = "lightVibrant";

    LIGHT_VIBRANT = Target(&temp);

    mTargets.push_back(LIGHT_VIBRANT);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;
    temp.name = "vibrant";

    VIBRANT = Target(&temp);

    mTargets.push_back(VIBRANT);

    temp.mLightnessTargets[0] = 0.26f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;
    temp.name = "darkVibrant";

    DARK_VIBRANT = Target(&temp);

    mTargets.push_back(DARK_VIBRANT);

    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 0.74f;

    temp.mSaturationTargets[0] = 0.3f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;
    temp.name = "lightMuted";

    LIGHT_MUTED = Target(&temp);

    mTargets.push_back(LIGHT_MUTED);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.3f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;
    temp.name = "muted";

    MUTED = Target(&temp);

    mTargets.push_back(MUTED);

    temp.mLightnessTargets[0] = 0.26f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.3f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;
    temp.name = "darkMuted";

    DARK_MUTED = Target(&temp);

    mTargets.push_back(DARK_MUTED);
};

Palette::Palette(std::vector<Swatch> swatches){
    //mFilters.push_back(DEFAULT_FILTER);
    std::copy(swatches.begin(), swatches.end(), mSwatches.begin());
    mBitmap = NULL;

    TargetMeta temp;
    temp.mLightnessTargets = new float[3];
    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 0.74f;

    temp.mSaturationTargets = new float[3];
    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.mWeights = new float[3];
    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;

    LIGHT_VIBRANT = Target(&temp);

    mTargets.push_back(LIGHT_VIBRANT);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;

    VIBRANT = Target(&temp);

    mTargets.push_back(VIBRANT);

    temp.mLightnessTargets[0] = 0.26f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.35f;
    temp.mSaturationTargets[1] = 1.0f;
    temp.mSaturationTargets[2] = 1.0f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;

    DARK_VIBRANT = Target(&temp);

    mTargets.push_back(DARK_VIBRANT);

    temp.mLightnessTargets[0] = 0.55f;
    temp.mLightnessTargets[1] = 0.74f;
    temp.mLightnessTargets[2] = 0.74f;

    temp.mSaturationTargets[0] = 0.3f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;

    LIGHT_MUTED = Target(&temp);

    mTargets.push_back(LIGHT_MUTED);

    temp.mLightnessTargets[0] = 0.3f;
    temp.mLightnessTargets[1] = 0.5f;
    temp.mLightnessTargets[2] = 0.7f;

    temp.mSaturationTargets[0] = 0.3f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;

    MUTED = Target(&temp);

    mTargets.push_back(MUTED);

    temp.mLightnessTargets[0] = 0.26f;
    temp.mLightnessTargets[1] = 0.26f;
    temp.mLightnessTargets[2] = 0.45f;

    temp.mSaturationTargets[0] = 0.3f;
    temp.mSaturationTargets[1] = 0.3f;
    temp.mSaturationTargets[2] = 0.4f;

    temp.mWeights[0] = 0.24f;
    temp.mWeights[1] = 0.52f;
    temp.mWeights[2] = 0.24f;

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

std::vector<uint32_t> Palette::getPixelsFromBitmap(Bitmap *bitmap){
    int bWidth = bitmap->width;
    //int bHeight = bitmap->height;
    int regionWidth, regionHeight;
    
    std::vector<uint32_t> pixels(bitmap->pixels.begin(), bitmap->pixels.end());

    if(mRegion == NULL){
        return pixels;
    }

    regionWidth = mRegion->width();
    regionHeight = mRegion->height();

    std::vector<uint32_t> subsetPixels(regionWidth * regionHeight);

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
    uint32_t sColor = color.getRgb();

    Color::RGBToHSL(Color::red(sColor), Color::green(sColor), Color::blue(sColor), newHsl);

    return newHsl;
};

Swatch *Palette::findDominantSwatch(){
    int maxPop = INT32_MIN;
    Swatch *maxSwatch = NULL;

    for(int i = 0, count = mSwatches.size(); i < count; i++){
        Swatch swatch = mSwatches[i];
        if(swatch.getPopulation() > maxPop){
            maxSwatch = &swatch;
            maxPop = swatch.getPopulation();
        }
    }

    return maxSwatch;
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
        mUsedColors.insert(std::pair <uint32_t, bool> (maxScoreSwatch.getRgb(), true));
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

uint32_t Palette::getDominantColor(){
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

    for(int i = 0, count = mTargets.size(); i < count; i++){
        Target target = mTargets[i];
        target.normalizeWeights();
        mSelectedSwatches.insert(std::pair <Target, Swatch> (target, generateScoredTarget(target)));
    }

    mUsedColors.clear();

    for(auto t : mSelectedSwatches){
        result.append("\"")
            .append(t.first.name)
            .append("\": ")
            .append(t.second.toString())
            .append(",");
    }

    result.append("\"dominantColor\": ")
        .append(Color::ToString(getDominantColor()))
        .append("}");
    
    return result;
};

/*
    There's a function within Palette called:
    private Bitmap scaleBitmapDown(final Bitmap bitmap)
    But I'm ingnoring it for now under the assumption that I won't need it.
*/