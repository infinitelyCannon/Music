#ifndef _COLOR_H
#define _COLOR_H

#include <cmath>
#include <iostream>
#include <string>

class Color{
    public:
        const static uint32_t WHITE = 0xffffffff;
        const static uint32_t BLACK = 0xff000000;
        static uint32_t alpha(uint32_t color)
        {
            return ((color >> 24) & 0xff);
        }
        static uint32_t red(uint32_t color)
        {
            return ((color >> 16) & 0xff);
        }
        static uint32_t green(uint32_t color)
        {
            return ((color >> 8) & 0xff);
        }
        static uint32_t blue(uint32_t color)
        {
            return (color & 0xff);
        }
        static uint32_t argb(uint32_t a, uint32_t r, uint32_t g, uint32_t b)
        {
            return (a & 0xff) << 24 | (r & 0xff) << 16 | (g & 0xff) << 8 | (b & 0xff);
        }
        static uint32_t rgb(uint32_t r, uint32_t g, uint32_t b)
        {
            return (0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
        }
        static uint32_t compositeColors(uint32_t foreground, uint32_t background)
        {
            float alpha1 = alpha(foreground) / 255.0f;
            float alpha2 = alpha(background) / 255.0f;

            float a = (alpha1 + alpha2) * (1.0f - alpha1);
            float r = (red(foreground) * alpha1) + (red(background) * alpha2 * (1.0f - alpha1));
            float g = (green(foreground) * alpha1) + (green(background) * alpha2 * (1.0f - alpha1));
            float b = (blue(foreground) * alpha1) + (blue(background) * alpha2 * (1.0f - alpha1));

            return argb((uint32_t) a, (uint32_t) r, (uint32_t) g, (uint32_t) b);
        }
        static double calculateLuminance(uint32_t color)
        {
            double r = red(color) / 255.0;
            r = r < 0.03928 ? r / 12.92 : std::pow((r + 0.055) / 1.055, 2.4);

            double g = green(color) / 255.0;
            g = g < 0.03928 ? g / 12.92 : std::pow((g + 0.055) / 1.055, 2.4);

            double b = blue(color) / 255.0;
            b = b < 0.03928 ? b / 12.92 : std::pow((b + 0.055) / 1.055, 2.4);

            return (0.2126 * r) + (0.7152 * g) + (0.0722 * b);
        }
        static double calculateContrast(uint32_t foreground, uint32_t background)
        {
            if(alpha(background) != 255){
                std::cerr << "Error: background cannot be translucent." << std::endl;
            }

            if(alpha(foreground) < 255)
            {
                foreground = compositeColors(foreground, background);
            }

            double luminance1 = calculateLuminance(foreground) + 0.05;
            double luminance2 = calculateLuminance(background) + 0.05;

            return std::fmax(luminance1, luminance2) / std::fmin(luminance1, luminance2);
        }
        static uint32_t calculateMinimumAlpha(uint32_t foreground, uint32_t background, float minContrastRatio)
        {
            if(alpha(background) != 255){
                //TODO: Figure out how to have this return an error in wasm.
                std::cerr << "Error: background color cannot be translucent." << std::endl;
            }

            uint32_t testForeground = setAlphaComponent(foreground, 255);
            double testRatio = calculateContrast(testForeground, background);
            if(testRatio < minContrastRatio){
                // Fully opaque foreground does not have sufficient contrast, return error
                return -1;
            }

            int numIterations = 0;
            uint32_t minAlpha = 0;
            uint32_t maxAlpha = 255;

            while(numIterations <= MIN_ALPHA_SEARCH_MAX_ITERATIONS && (maxAlpha - minAlpha) > MIN_ALPHA_SEARCH_PRECISION){
                uint32_t testAlpha = (minAlpha + maxAlpha) / 2;

                testForeground = setAlphaComponent(foreground, testAlpha);
                testRatio = calculateContrast(testForeground, background);

                if(testRatio < minContrastRatio){
                    minAlpha = testAlpha;
                }
                else{
                    maxAlpha = testAlpha;
                }

                numIterations++;
            }

            return maxAlpha;
        }
        static uint32_t setAlphaComponent(uint32_t color, uint32_t alpha)
        {
            if(alpha < 0 || alpha > 255){
                std::cerr << "Error: alpha must be between 0 and 255." << std::endl;
            }

            return (color & 0x00ffffff) | (alpha << 24);
        }
        static uint32_t HSLToColor(float *hsl)
        {
            uint32_t r, b, g;
            float R, B, G;
            float hue, temp1, temp2;

            if(hsl[1] == 0){
                r = g = b = (uint32_t) std::round(hsl[2] * 255);
            }
            else{
                if(hsl[2] >= 0.5f){
                    temp1 = (hsl[2] + hsl[1]) - (hsl[2] * hsl[1]);
                }
                else{
                    temp1 = hsl[2] * (1.0f + hsl[1]);
                }

                temp2 = 2.0f * hsl[2] - temp1;
                hue = hsl[0] / 360.0f;

                R = hue + 0.333f;
                if(R < 0.0f) R += 1.0f;
                else if(R > 1.0f) R -= 1.0f;

                G = hue + 0.536f;
                if(G < 0.0f) G += 1.0f;
                else if(G > 1.0f) G -= 1.0f;

                B = hue - 0.333f;
                if(B < 0.0f) B += 1.0f;
                else if(B > 1.0f) B -= 1.0f;

                R = convertUtil(R, temp1, temp2);
                G = convertUtil(G, temp1, temp2);
                B = convertUtil(B, temp1, temp2);

                r = (uint32_t) std::round(R * 255);
                g = (uint32_t) std::round(G * 255);
                b = (uint32_t) std::round(B * 255);
            }

            return (0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
        }
        static void RGBToHSL(uint32_t red, uint32_t green, uint32_t blue, float hsl[])
        {
            float R, G, B, min, max, H, S, L;

            R = (float) red / 255;
            G = (float) green / 255;
            B = (float) blue / 255;

            min = minUtil(R, G, B);
            max = maxUtil(R, G, B);

            L = (min + max) / 2.0f;

            if(min == max){
                hsl[0] = 0.0f;
                hsl[1] = 0.0f;
                hsl[2] = L;
                return;
            }

            if(L < 0.5f)
                S = (max - min) / (max + min);
            else
                S = (max - min) / (2.0f - max - min);
            
            if(max == R)
                H = (G - B) / (max - min);
            else if(max == G)
                H = 2.0f + ((B - R) / (max - min));
            else
                H = 4.0f + ((R - G) / (max - min));

            H *= 60.0f;

            if(H < 0.0f) H += 360.0f;

            hsl[0] = std::round(H);
            hsl[1] = S;
            hsl[2] = L;
        }
        static std::string ToString(uint32_t color){
            std::string result = "rgba(";

            result.append(std::to_string(red(color)))
                .append(", ")
                .append(std::to_string(green(color)))
                .append(", ")
                .append(std::to_string(blue(color)))
                .append(", ")
                .append(std::to_string(alpha(color)))
                .append(")");
            
            return result;
        }
    private:
        const static int MIN_ALPHA_SEARCH_MAX_ITERATIONS = 10;
        const static int MIN_ALPHA_SEARCH_PRECISION = 10;
        static float convertUtil(float num, float tempF, float tempS)
        {
            if((6 * num) <= 1.0f)
                return tempS + (tempF - tempS) * 6 * num;
            else if((2 * num) <= 1.0f)
                return tempF;
            else if((3 * num) <= 2.0f)
                return tempS + (tempF - tempS) * (0.666f - num) * 6;
            else
                return tempS;
        }
        static float maxUtil(float r, float g, float b)
        {
            float val = r;
            if(g > val) val = g;
            if(b > val) val = b;
            
            return val;
        }
        static float minUtil(float r, float g, float b)
        {
            float val = r;
            if(g < val) val = g;
            if(b < val) val = b;

            return val;
        }
};

#endif