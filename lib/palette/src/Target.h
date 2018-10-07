#ifndef _TARGET_H
#define _TARGET_H
#include <string>

struct TargetMeta
{
    float mSaturationTargets[3] = {0.0f, 0.5f, 1.0f};
    float mLightnessTargets[3] = {0.0f, 0.5f, 1.0f};
    float mWeights[3] = {0.24f, 0.52f, 0.24f};
    std::string name;
};


class Target{
    public:
        Target();
        Target(TargetMeta *from);
        float getLightnessWeight();
        float getMaximumLightness();
        float getMaximumSaturation();
        float getMinimumLightness();
        float getMinimumSaturation();
        float getPopulationWeight();
        float getSaturationWeight();
        float getTargetLightness();
        float getTargetSaturation();

        bool isExclusive();

        void setExclusive(bool exclusive);
        void setLightnessWeight(float weight);
        void setMaximumLightness(float lightness);
        void setMaximumSaturation(float saturation);
        void setMinimumLightness(float lightness);
        void setMinimumSaturation(float saturation);
        void setPopulationWeight(float weight);
        void setSaturationWeight(float weight);
        void setTargetLightness(float lightness);
        void setTargetSaturation(float saturation);
        void normalizeWeights();
        static bool isEqual(Target target1, Target target2);
        std::string name;

        bool operator< (const Target& tar) const
        {
            return tar.name < this->name;
        }
    private:
        void setTargetDefaultValues(float *values);
        void setDefaultWeights();
        static void setDefaultDarkLightnessValues(Target target);
        static void setDefaultNormalLightnessValues(Target target);
        static void setDefaultLightLightnessValues(Target target);
        static void setDefaultVibrantSaturationValues(Target target);
        static void setDefaultMutedSaturationValues(Target target);
        constexpr static float TARGET_DARK_LUMA = 0.26f;
        constexpr static float MAX_DARK_LUMA = 0.45f;
        
        constexpr static float MIN_LIGHT_LUMA = 0.55f;
        constexpr static float TARGET_LIGHT_LUMA = 0.74f;

        constexpr static float MIN_NORMAL_LUMA = 0.3f;
        constexpr static float TARGET_NORMAL_LUMA = 0.5f;
        constexpr static float MAX_NORMAL_LUMA = 0.7f;

        constexpr static float TARGET_MUTED_SATURATION = 0.3f;
        constexpr static float MAX_MUTED_SATURATION = 0.4f;

        constexpr static float TARGET_VIBRANT_SATURATION = 1.0f;
        constexpr static float MIN_VIBRANT_SATURATION = 0.35f;

        constexpr static float WEIGHT_SATURATION = 0.24f;
        constexpr static float WEIGHT_LUMA = 0.52f;
        constexpr static float WEIGHT_POPULATION = 0.24f;

        const static int INDEX_WEIGHT_SAT = 0;
        const static int INDEX_WEIGHT_LUMA = 1;
        const static int INDEX_WEIGHT_POP = 2;

        const static int INDEX_MIN = 0;
        const static int INDEX_TARGET = 1;
        const static int INDEX_MAX = 2;

        float mSaturationTargets[3];
        float mLightnessTargets[3];
        float mWeights[3];
        bool mIsExclusive = true;
};

#endif