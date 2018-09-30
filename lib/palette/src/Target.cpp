#include "Target.h"
#define ARRAY_LEN 3

Target::Target()
{
    setTargetDefaultValues(mSaturationTargets);
    setTargetDefaultValues(mLightnessTargets);
    setDefaultWeights();
    name = "default";
};

bool Target::isEqual(Target target1, Target target2){
    if(!(target1.isExclusive() ^ target2.isExclusive())) return false;
    if(target1.name == target2.name) return false;
    for(int i = 0; i < 3; i++){
        if(target1.mLightnessTargets[i] != target2.mLightnessTargets[i])
            return false;
        if(target1.mSaturationTargets[i] != target2.mSaturationTargets[i])
            return false;
        if(target1.mWeights[i] != target2.mWeights[i])
            return false;
    }

    return true;
};

Target::Target(TargetMeta *from)
{
    for(int s = 0; s < ARRAY_LEN; s++){
        mSaturationTargets[s] = from->mSaturationTargets[s];
    }

    for(int l = 0; l < ARRAY_LEN; l++){
        mLightnessTargets[l] = from->mLightnessTargets[l];
    }

    for(int w = 0; w < ARRAY_LEN; w++){
        mWeights[w] = from->mWeights[w];
    }

    name = from->name;
};

float Target::getMinimumSaturation()
{
    return mSaturationTargets[INDEX_MIN];
};

float Target::getTargetSaturation()
{
    return mSaturationTargets[INDEX_TARGET];
};

float Target::getMaximumSaturation()
{
    return mSaturationTargets[INDEX_MAX];
};

float Target::getMinimumLightness()
{
    return mLightnessTargets[INDEX_MIN];
};

float Target::getTargetLightness()
{
    return mLightnessTargets[INDEX_TARGET];
};

float Target::getMaximumLightness()
{
    return mLightnessTargets[INDEX_MAX];
};

float Target::getSaturationWeight()
{
    return mWeights[INDEX_WEIGHT_SAT];
};

float Target::getLightnessWeight()
{
    return mWeights[INDEX_WEIGHT_LUMA];
};

float Target::getPopulationWeight()
{
    return mWeights[INDEX_WEIGHT_POP];
};

bool Target::isExclusive()
{
    return mIsExclusive;
};

void Target::setTargetDefaultValues(float *values)
{
    values[INDEX_MIN] = 0.0f;
    values[INDEX_TARGET] = 0.5f;
    values[INDEX_MAX] = 1.0f;
};

void Target::setDefaultWeights()
{
    mWeights[INDEX_WEIGHT_SAT] = WEIGHT_SATURATION;
    mWeights[INDEX_WEIGHT_LUMA] = WEIGHT_LUMA;
    mWeights[INDEX_WEIGHT_POP] = WEIGHT_POPULATION;
};

void Target::normalizeWeights()
{
    float sum = 0;
    int i, z;
    for(i = 0, z = ARRAY_LEN; i < z; i++){
        float weight = mWeights[i];
        if(weight > 0){
            sum += weight;
        }
    }

    if(sum != 0){
        for(i = 0, z = ARRAY_LEN; i < z; i++){
            if(mWeights[i] > 0){
                mWeights[i] /= sum;
            }
        }
    }
};

void Target::setDefaultDarkLightnessValues(Target target)
{
    target.mLightnessTargets[INDEX_TARGET] = TARGET_DARK_LUMA;
    target.mLightnessTargets[INDEX_MAX] = MAX_DARK_LUMA;
};

void Target::setDefaultNormalLightnessValues(Target target)
{
    target.mLightnessTargets[INDEX_MIN] = MIN_NORMAL_LUMA;
    target.mLightnessTargets[INDEX_TARGET] = TARGET_NORMAL_LUMA;
    target.mLightnessTargets[INDEX_MAX] = MAX_NORMAL_LUMA;
};

void Target::setDefaultLightLightnessValues(Target target)
{
    target.mLightnessTargets[INDEX_MIN] = MIN_LIGHT_LUMA;
    target.mLightnessTargets[INDEX_TARGET] = TARGET_LIGHT_LUMA;
};

void Target::setDefaultVibrantSaturationValues(Target target)
{
    target.mSaturationTargets[INDEX_MIN] = MIN_VIBRANT_SATURATION;
    target.mSaturationTargets[INDEX_TARGET] = TARGET_VIBRANT_SATURATION;
};

void Target::setDefaultMutedSaturationValues(Target target)
{
    target.mSaturationTargets[INDEX_TARGET] = TARGET_MUTED_SATURATION;
    target.mSaturationTargets[INDEX_MAX] = MAX_MUTED_SATURATION;
};

void Target::setExclusive(bool exclusive)
{
    mIsExclusive = exclusive;
};

void Target::setLightnessWeight(float weight)
{
    mWeights[INDEX_WEIGHT_LUMA] = weight;
};

void Target::setMaximumLightness(float lightness)
{
    mLightnessTargets[INDEX_MAX] = lightness;
};

void Target::setMaximumSaturation(float saturation)
{
    mSaturationTargets[INDEX_MAX] = saturation;
};
void Target::setMinimumLightness(float lightness)
{
    mLightnessTargets[INDEX_MIN] = lightness;
};

void Target::setMinimumSaturation(float saturation)
{
    mSaturationTargets[INDEX_MIN] = saturation;
};

void Target::setPopulationWeight(float weight)
{
    mWeights[INDEX_WEIGHT_POP] = weight;
};

void Target::setSaturationWeight(float weight)
{
    mWeights[INDEX_WEIGHT_SAT] = weight;
};

void Target::setTargetLightness(float lightness)
{
    mLightnessTargets[INDEX_TARGET] = lightness;
};

void Target::setTargetSaturation(float saturation)
{
    mSaturationTargets[INDEX_TARGET] = saturation;
};