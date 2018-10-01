#include "Swatch.h"
#include "Target.h"
#include "ColorCutQuantizer.h"
#include <vector>
#include <map>
#ifndef _PALETTE_H
#define _PALETTE_H

struct Bitmap{
    std::vector<uint32_t> pixels;
    int width;
    int height;
};

class Rect{
    public:
        int top, bottom, left, right;
        Rect(){
            top = bottom = left = right = 0;
        }
        Rect(int l, int t, int r, int b){
            top = t;
            left = l;
            right = r;
            bottom = b;
        }
        void set(int l, int t, int r, int b){
            top = t;
            left = l;
            right = r;
            bottom = b;
        }
        bool intersect(int l, int t, int r, int b){
            if(l >= left && t <= top && r <= right && b >= bottom){
                set(l, t, r, b);
                return true;
            }
            else{
                return false;
            }
        }
        int width(){
            return right - left;
        }
        int height(){
            return bottom - top;
        }
};

class Palette{
    public:
        Palette(Bitmap *bipmap);
        Palette(std::vector<Swatch> swatches);
        Target DARK_MUTED;
        Target DARK_VIBRANT;
        Target LIGHT_MUTED;
        Target LIGHT_VIBRANT;
        Target MUTED;
        Target VIBRANT;
        void maximumColorCount(int colors);
        void resizeBitmapArea(int area);
        //void clearFilters();
        //void addFilter(Filter filter);
        bool setRegion(int l, int t, int r, int b);
        void clearRegion();
        void addTarget(Target target);
        void addTarget(TargetMeta meta);
        void clearTargets();
        std::string generate(v8::Local<v8::Value> filters, bool useDefault);
        void generate(std::vector<Swatch> swatches, std::vector<Target> targets);
        Swatch *generateScoredTarget(Target target);
        uint32_t getDominantColor();
    private:
        //static Filter DEFAULT_FILTER;
        constexpr static int DEFAULT_RESIZE_BITMAP_AREA = 112 * 112;
        constexpr static int DEFAULT_CALCULATE_NUMBER_COLORS = 16;

        constexpr static float MIN_CONTRAST_TITLE_TEXT = 3.0f;
        constexpr static float MIN_CONTRAST_BODY_TEXT = 4.5f;
        std::vector<Swatch> mSwatches;
        Bitmap *mBitmap = NULL;
        std::vector<Target> mTargets;
        std::map<Target, Swatch> mSelectedSwatches;
        std::map<uint32_t, bool> mUsedColors;
        Swatch *mDominantSwatch = NULL;

        int mMaxColors = DEFAULT_CALCULATE_NUMBER_COLORS;
        int mResizeArea = DEFAULT_RESIZE_BITMAP_AREA;
        int mResizeMaxDimension = -1;

        //std::vector<Filter> mFilters;
        Rect *mRegion = NULL;

        std::vector<uint32_t> getPixelsFromBitmap(Bitmap *bitmap);
        Swatch *getMaxScoredSwatchForTarget(Target target);
        bool shouldBeScoredForTarget(Swatch swatch, Target target);
        float generateScore(Swatch swatch, Target target);
        Swatch *findDominantSwatch();
        float *copyHslValues(Swatch color);
};

#endif