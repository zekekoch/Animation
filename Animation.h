#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"
#include "Segment.h"

enum AnimationType
{
    NoAnimation,
    SolidAnimation,
    FadeAnimation,
    ChaseAnimation,
    RainbowAnimation,
    BitFadeAnimation,
};
class Animation
{
private:

    struct Color
    {
        byte r;
        byte g;
        byte b;
    };
    
    Color color;
    
    AnimationType _animationType;
    int _internalFrame;
    int _startPixel;
    int _endPixel;
    int _currentPixel;
    Segment _pixelMask;
    byte _pace;
    int _localFrame;
    unsigned long _lastFrame = 0;
    unsigned long _timerDelta = 0;
    int _animationIndex;
    int _accelX;
    bool _chaseForward;
    byte fadeLevel = 0;
    byte direction = 0;
    
public:
    struct CRGB
    {
        unsigned char g;
        unsigned char r;
        unsigned char b;
    };
    CRGB* leds;
    int numLeds;
    
    // todo: why isn't clear working for bitfields
    void clear()
    {
        memset(leds, 0, numLeds * sizeof(struct CRGB));
    }
    
    //<<constructor>> setup the LED, make pin 13 an OUTPUT
    Animation(CRGB* lights, int numLeds)
    {
        _startPixel = 0;
        _endPixel = 0;
        _currentPixel = 0;
        _localFrame = 0;
        getIndex();
        leds = lights;
    }
    
    //<<destructor>>
    ~Animation()
    {
        /*nothing to destruct*/
    }
    
    //turn the LED on
    void setup()
    {
        // nothing
    }
    
    void getIndex()
    {
        static byte index = 0;
        _animationIndex = index++;
    }
    
    void setAccel(int accel)
    {
        _accelX = accel;
    }
    
    void setColor(byte r, byte g, byte b)
    {
        color.r = r;
        color.g = g;
        color.b = b;
    }
    
    //turn the LED off
    void play()
    {
        bool increment;
        
    	//Serial.print(_animationIndex);Serial.print(" ");Serial.print("play ");Serial.print(_animationType);
        //Serial.print(" now ");Serial.print(millis());Serial.print(" last ");Serial.print(_lastFrame);
        //Serial.print(" pace ");Serial.print(_pace);
        //Serial.println();
        
        if (millis() > _lastFrame + _pace)
        {
            increment = true;
            //Serial.println("Increment");
            _lastFrame = millis();
        }
        else
        {
            //Serial.println("Don't Increment");
            increment = false;
        }
        
        switch(_animationType)
        {
            case (SolidAnimation):
                runSolid(increment);
                break;
            case (ChaseAnimation):
                runChase(increment);
                break;
            case (FadeAnimation):
                runFade(increment);
                break;
            case (RainbowAnimation):
                runRainbow(increment);
                break;
            case (BitFadeAnimation):
                runBitFade(increment);
                break;
        }
    }
    
    void chase(int start, int end, int color, byte pace)
    {
        //	Serial.print(getIndex(false));Serial.print(" ");Serial.println("start chase");
        
        if (start < end)
        {
            _startPixel = start;
            _currentPixel = start;
            _endPixel = end;
            _chaseForward = true;
        }
        else
        {
            _startPixel = end;
            _currentPixel = end;
            _endPixel = start;
            _chaseForward = false;
        }
        _animationType = ChaseAnimation;
        _pace = pace;
    }
    
    void runChase(bool increment)
    {
        //	Serial.print(getIndex(false));Serial.print(" ");Serial.println("runChase");
        
        if (increment)
        {
            if (_chaseForward)
            {
                if (_currentPixel < _endPixel)
                {
                    _currentPixel++;
                }
                else
                {
                    _currentPixel = _startPixel;
                }
            }
            else
            {
                if (_currentPixel > _startPixel)
                {
                    _currentPixel--;
                }
                else
                {
                    _currentPixel = _endPixel;
                }
            }
        }
        
        int iFadePos = _currentPixel;
        int traceLength;
        
        if (_accelX > 64)
        {
            traceLength = _accelX/32;
        }
        else if (_accelX < -64)
        {
            traceLength = -_accelX/32;
        }
        
        traceLength=2;
        
        for(int brightness = 255;brightness>1;brightness/=traceLength)
        {
            leds[iFadePos].r = brightness;
            leds[iFadePos].g = 0;
            leds[iFadePos].b = 0;
            if (_chaseForward)
            {
                if (--iFadePos < _startPixel)
                {
                    break;
                }
            }
            else // !chaseForward
            {
                if (++iFadePos > _endPixel)
                    break;
            }
        }
        
        leds[_currentPixel].r = color.r;
        leds[_currentPixel].g = color.g;
        leds[_currentPixel].b = color.b;
        // nothing
    }
    
    
    void solid(int start, int end, int color, byte pace)
    {
        //	Serial.print(getIndex(false));Serial.print(" ");Serial.println("start solid");
        
        _startPixel = start;
        _currentPixel = start;
        _endPixel = end;
        _pace = pace;
        _animationType = SolidAnimation;
    }
    
    void runSolid(bool increment)
    {
        //	Serial.print(getIndex(false));Serial.print(" ");Serial.println("runSolid");
        for(int i = _startPixel;i<=_endPixel;i++)
        {
            leds[i].r = color.r;
            leds[i].g = color.g;
            leds[i].b = color.b;
        }
        
    }
    
    void rainbow(int start, int end, int color, byte pace)
    {
        _startPixel = start;
        _currentPixel = start;
        _endPixel = end;
        _animationType = RainbowAnimation;
        _pace = pace;
    }
    
    void runRainbow(bool increment)
    {
        Color c;
        if (increment)
        {
            for(int i = _startPixel;i<=_endPixel;i++)
            {
                c = Wheel(i);
                leds[i].r = c.r;
                leds[i].g = c.g;
                leds[i].b = c.b;
            }
        }
    }
    
    void bitFade(Segment segment, int color, byte pace)
    {
        _pixelMask = segment;
        _animationType = BitFadeAnimation;
        _pace = pace;
    }
    
    void runBitFade(bool increment)
    {
        if (increment)
        {
            if (direction == 0)
            {
                fadeLevel++;
                if (fadeLevel == 128)
                    direction = 1;
            }
            else if (direction == 1)
            {
                fadeLevel--;
                if (fadeLevel == 0)
                    direction = 0;
            }
        }

        // loop over the bits in the pixel mask and if it's set to 1 then change the fade level
        //Serial.print(_pixelMask);Serial.print(" ");
        for (int i = 0; i < _pixelMask.length; i++) {
            if (1 == _pixelMask[i] && fadeLevel > 0)
            {
                //Serial.print(1);
                leds[i].r = color.r * 128/fadeLevel;
                leds[i].g = color.g * 128/fadeLevel;
                leds[i].b = color.b * 128/fadeLevel;
            }
            else
            {
                //Serial.print(1);
                leds[i].r = 0;
                leds[i].g = 0;
                leds[i].b = 0;
                //Serial.print(0);
            }
        }
        //Serial.println();
        
    }

    void fade(int start, int end, int color, byte pace)
    {
        //	Serial.print(getIndex(false));Serial.print(" ");Serial.println("start solid");
        
        _startPixel = start;
        _currentPixel = start;
        _endPixel = end;
        _animationType = FadeAnimation;
        _pace = pace;
    }
    
    void runFade(bool increment)
    {
        //	Serial.print("fade ");Serial.print(fadeLevel);Serial.print(" dir ");Serial.print(direction);Serial.println();
        
        if (increment)
        {
            if (direction == 0)
            {
                fadeLevel++;
                if (fadeLevel == 128)
                    direction = 1;
            }
            else if (direction == 1)
            {
                fadeLevel--;
                if (fadeLevel == 0)
                    direction = 0;
            }
        }
        
        for(int i = _startPixel;i<=_endPixel;i++)
        {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = fadeLevel;
        }
    }
    


    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    Color Wheel(byte WheelPos)
    {
        Animation::Color c;
        if(WheelPos < 85)
        {
            c.r = WheelPos * 3;
            c.g = 255 - WheelPos * 3;
            c.b = 0;
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            c.r = 255 - WheelPos * 3;
            c.g = 0;
            c.b = WheelPos * 3;
        }
        else
        {
            WheelPos -= 170;
            c.r = 0;
            c.g = WheelPos * 3;
            c.b = 255 - WheelPos * 3;
        }
        return c;
    }

};

#endif