#include "async.h"
#include "Palette.h"

class GeneratorAlpha : public Nan::AsyncWorker{
    public:
        GeneratorAlpha(Nan::Callback *callback, Bitmap *bMap, Rect *rect);
};