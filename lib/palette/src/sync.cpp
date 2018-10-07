#include "sync.h"
#include "Palette.h"

NAN_METHOD(GenerateSync){
    std::vector<int> bitmap;
    Bitmap bMap;
    int /*bWidth = 0, bHeight = 0,*/ maxColor = 16;
    bool useDefault;
    v8::Local<v8::Value> filterFunc;
    Rect *rect = new Rect();
    Palette *pal;
    std::vector<TargetMeta> targets;
    v8::Local<v8::Value> filter;

    if(!info[0]->IsObject()){
        return Nan::ThrowError(Nan::New("Palette: argument to be an object").ToLocalChecked());
    }
    if(info.Length() != 1){
        return Nan::ThrowError(Nan::New("Palette: expected only one argument").ToLocalChecked());
    }

    v8::Local<v8::Object> jsonObj = info[0]->ToObject();

    v8::Local<v8::String> bitmapProp = Nan::New("image").ToLocalChecked();
    v8::Local<v8::String> filterProp = Nan::New("filter").ToLocalChecked();
    v8::Local<v8::String> mColorProp = Nan::New("maxColors").ToLocalChecked();
    v8::Local<v8::String> regionProp = Nan::New("region").ToLocalChecked();
    v8::Local<v8::String> targetProp = Nan::New("targets").ToLocalChecked();
    v8::Local<v8::String> widthProp  = Nan::New("width").ToLocalChecked();
    v8::Local<v8::String> heightProp = Nan::New("height").ToLocalChecked();

    if(!Nan::HasOwnProperty(jsonObj, bitmapProp).FromJust() ||
       !Nan::HasOwnProperty(jsonObj, widthProp).FromJust() ||
       !Nan::HasOwnProperty(jsonObj, heightProp).FromJust()){
        return Nan::ThrowError(Nan::New("Missing required image properties").ToLocalChecked());
    }
    else{
        v8::Local<v8::Array> imgArr = v8::Local<v8::Array>::Cast(Nan::Get(jsonObj, bitmapProp).ToLocalChecked());
        v8::Local<v8::Value> width = Nan::Get(jsonObj, widthProp).ToLocalChecked();
        v8::Local<v8::Value> height = Nan::Get(jsonObj, heightProp).ToLocalChecked();
        for(int i = 0; i < (int) imgArr->Length(); i += 4){
            v8::Local<v8::Value> numR, numG, numB, numA;
            numR = imgArr->Get(i);
            numG = imgArr->Get(i + 1);
            numB = imgArr->Get(i + 2);
            numA = imgArr->Get(i + 3);
            int color = Color::argb(
                (int) numA->NumberValue(),
                (int) numR->NumberValue(),
                (int) numG->NumberValue(),
                (int) numB->NumberValue()
            );
            bitmap.push_back(color);
        }

        bMap.pixels = bitmap;
        bMap.width = (int) width->NumberValue();
        bMap.height = (int) height->NumberValue();
        pal = new Palette(&bMap);
    }

    if(Nan::HasOwnProperty(jsonObj, filterProp).FromJust()){
        filter = Nan::Get(jsonObj, filterProp).ToLocalChecked();
        if(filter->IsNull()){
            useDefault = false;
            filterFunc = Nan::Null();
        }
        else{
            useDefault = false;
            filterFunc = v8::Local<v8::Function>::Cast(filter);
        }
    }
    else{
        useDefault = true;
        filterFunc = Nan::Null();
    }

    if(Nan::HasOwnProperty(jsonObj, mColorProp).FromJust()){
        v8::Local<v8::Value> count = Nan::Get(jsonObj, mColorProp).ToLocalChecked();
        maxColor = (int) count->NumberValue();
        pal->maximumColorCount(maxColor);
    }

    if(Nan::HasOwnProperty(jsonObj, regionProp).FromJust()){
        v8::Local<v8::Object> box = Nan::Get(jsonObj, regionProp).ToLocalChecked()->ToObject();
        rect->set(
            (int) Nan::Get(box, Nan::New("left").ToLocalChecked()).ToLocalChecked()->NumberValue(),
            (int) Nan::Get(box, Nan::New("top").ToLocalChecked()).ToLocalChecked()->NumberValue(),
            (int) Nan::Get(box, Nan::New("right").ToLocalChecked()).ToLocalChecked()->NumberValue(),
            (int) Nan::Get(box, Nan::New("bottom").ToLocalChecked()).ToLocalChecked()->NumberValue()
        );

        if(!pal->setRegion(rect->left, rect->top, rect->right, rect->bottom))
            return Nan::ThrowError(Nan::New("Region argument is not with area of image").ToLocalChecked());
    }

    if(Nan::HasOwnProperty(jsonObj, targetProp).FromJust()){
        v8::Local<v8::Array> tArr = v8::Local<v8::Array>::Cast(Nan::Get(jsonObj, targetProp).ToLocalChecked());
        for(int i = 0; i < (int) tArr->Length(); i++){
            v8::Local<v8::Object> tempTar = tArr->Get(i)->ToObject();
            v8::Local<v8::Array> tempSat = v8::Local<v8::Array>::Cast(Nan::Get(tempTar, Nan::New("saturation").ToLocalChecked()).ToLocalChecked());
            v8::Local<v8::Array> tempLit = v8::Local<v8::Array>::Cast(Nan::Get(tempTar, Nan::New("lightness").ToLocalChecked()).ToLocalChecked());
            v8::Local<v8::Array> tempWei = v8::Local<v8::Array>::Cast(Nan::Get(tempTar, Nan::New("weights").ToLocalChecked()).ToLocalChecked());
            v8::Local<v8::Value> tempName = Nan::Get(tempTar, Nan::New("name").ToLocalChecked()).ToLocalChecked();
            std::string tarName = std::string(*Nan::Utf8String(tempName->ToString()));
            float s[3], l[3], w[3];
            TargetMeta target;

            s[0] = (float) tempSat->Get(0)->NumberValue();
            s[1] = (float) tempSat->Get(2)->NumberValue();
            s[2] = (float) tempSat->Get(1)->NumberValue();

            l[0] = (float) tempLit->Get(0)->NumberValue();
            l[1] = (float) tempLit->Get(1)->NumberValue();
            l[2] = (float) tempLit->Get(2)->NumberValue();

            w[0] = (float) tempWei->Get(0)->NumberValue();
            w[1] = (float) tempWei->Get(1)->NumberValue();
            w[2] = (float) tempWei->Get(2)->NumberValue();

            target.mSaturationTargets = s;
            target.mLightnessTargets = l;
            target.mWeights = w;
            target.name = tarName;

            targets.push_back(target);
        }

        for(TargetMeta t : targets){
            pal->addTarget(t);
        }
    }

    
    std::string swatches = pal->generate(filterFunc, useDefault);

    v8::Local<v8::String> jsonStr = Nan::New(swatches).ToLocalChecked();

    Nan::JSON NanJSON;
    Nan::MaybeLocal<v8::Value> result = NanJSON.Parse(jsonStr);
    if(!result.IsEmpty())
        info.GetReturnValue().Set(result.ToLocalChecked());
    else
        return Nan::ThrowError(Nan::New("Could not generate color swatches").ToLocalChecked());
}