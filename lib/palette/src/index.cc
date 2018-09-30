#include <nan.h>
#include "sync.h"

NAN_MODULE_INIT(InitAll){
    Nan::Set(target, Nan::New<v8::String>("GenerateSync").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(GenerateSync)).ToLocalChecked());
}

NODE_MODULE(palette, InitAll);