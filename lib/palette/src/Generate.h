#ifndef _GENERATE_H
#define _GENERATE_H
#include <nan.h>
#include <string>

struct Bitmap{
    std::vector<int> pixels;
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

v8::Local<v8::Object> generateColors(Bitmap *bitmap, Rect *rect);

#endif