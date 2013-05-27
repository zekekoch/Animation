//
//  Segment.h
//  DressTester
//
//  Created by Zeke Koch on 5/16/13.
//  Copyright (c) 2013 Zeke Koch. All rights reserved.
//

#ifndef DressTester_Segment_h
#define DressTester_Segment_h

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"

enum Strand
{
    Error = -1,
    RingTop = 0,
    RingMiddle = 1,
    RingBottom = 2,
    VTop = 3,
    VMiddle = 4,
    VBottom = 5,
    Vertical = 6,
};

enum SegmentEnum
{
    RingTopBackCenter,
    RingTopBackRightInner,
    RingTopBackRightOuter,
    RingTopLeft,
    RingTopFrontLeftOuter,
    RingTopFrontLeftInner,
    RingTopFrontRightInner,
    RingTopFrontRightOuter,
    RingTopRight,
    RingTopBackLeftOuter,
    RingTopBackLeftInner,
    
    RingMiddleBackCenter,
    RingMiddleBackRightInner,
    RingMiddleBackRightOuter,
    RingMiddleLeft,
    RingMiddleFrontLeftInner,
    RingMiddleFrontRightInner,
    RingMiddleRight,
    RingMiddleBackLeftOuter,
    RingMiddleBackLeftInner,
    
    RingBottomBackCenter,
    RingBottomBackRightInner,
    RingBottomLeft,
    RingBottomRight,
    RingBottomBackLeftInner,
    
    VTopBackCenter,
    VTopRight,
    VTopLeft,
    
    VMiddleBackCenter,
    VMiddleBackLeftLower,
    VMiddleBackLeftMiddle,
    VMiddleBackLeftUpper,
    VMiddleFrontRightMiddle,
    VMiddleFrontLeftMiddle,
    VMiddleBackRightUpper,
    VMiddleBackRightMiddle,
    VMiddleBackRightLower,
    
    VBottomBackCenter,
    VBottomBackRightLower,
    VBottomBackRightMiddle,
    VBottomBackRightTop,
    VBottomFrontLeftMiddle,
    VBottomFrontLeftLower,
    VBottomFrontRightLower,
    VBottomFrontRightMiddle,
    VBottomBackLeftTop,
    VBottomBackLeftMiddle,
    VBottomBackLeftLower,
};

// this maps form the segmentenum to a strand
// I make the size of the array the index of the last
// item in the enum.  there might be a better way to do this
static const Strand segmentToStrand[VBottomBackLeftLower+1] =
{
    RingTop,RingTop,RingTop,RingTop,RingTop,RingTop,RingTop,RingTop,RingTop,RingTop,RingTop,
    RingMiddle,RingMiddle,RingMiddle,RingMiddle,RingMiddle,RingMiddle,RingMiddle,RingMiddle,RingMiddle,
    RingBottom,RingBottom,RingBottom,RingBottom,RingBottom,
    VTop,VTop,VTop,
    VMiddle,VMiddle,VMiddle,VMiddle,VMiddle,VMiddle,VMiddle,VMiddle,VMiddle,
    VBottom,VBottom,VBottom,VBottom,VBottom,VBottom,VBottom,VBottom,VBottom,VBottom,VBottom,
};


class Segment
{
    
private:
    static const int LightMapSize = 4;

public:

    int lightMap[LightMapSize];
    int length;
    SegmentEnum segment;
    Strand strand;

    Segment()
    {
        
    }
    
    Segment(SegmentEnum segmentEnum, String bitField)
    {
        length = bitField.length();
        segment = segmentEnum;
        strand = segmentToStrand[segmentEnum];
//        strand = RingTop;
        int chunk = 0;
        int bit = 0;
        
        // i'm packing a string of one and zeros into four ints
        for(int light = 0;light<length;light++)
        {
            // crudely convert the current character to a digit (should be 1 or 0)
            // then set that bit in the current segment
            if (1 == bitField[light] - '0')
            {
                lightMap[chunk] |= 1 << bit;
            }
            
            if (bit < 31)
            {
                bit++;
            }
            else
            {
                chunk++;
                bit = 0;
            }
        }
    }
    
    Segment(SegmentEnum segmentEnum, int s0, int s1, int s2, int s3, int segmentLength)
    {
        segment = segmentEnum;
        length = segmentLength;
        strand = segmentToStrand[segmentEnum];
        lightMap[0] = s0;
        lightMap[1] = s1;
        lightMap[2] = s2;
        lightMap[3] = s3;
    }
    
    ~Segment()
    {
        
    }
    
    Segment operator& (Segment param) {
        Segment temp = Segment();
        temp.strand = (strand == param.strand) ? strand : Error;
        temp.length = length;

        for (int i = 0;i<LightMapSize;i++)
        {
            temp.lightMap[i] = lightMap[i] & param.lightMap[i];
        }
        return (temp);
    }

    Segment operator| (Segment param) {
        Segment temp = Segment();
        
        temp.strand = (strand == param.strand) ? strand : Error;
        temp.length = length;        
        
        for (int i = 0;i<LightMapSize;i++)
        {
            temp.lightMap[i] = lightMap[i] | param.lightMap[i];
        }
        return (temp);
    }
    
    int operator[] (const int index)
    {
        // i'm pulling the bits from an array of 32 bit ints
        // the int to pull is just the index / the size of an int
        // and the bit is then the remainder
        //
        // &'ing the int with a mask with just the appropriate bit set
        // to the appropriate checks to see if that's set
        
        if (lightMap[index/32] & (1 << index % 32))
            return 1;
        else
            return 0;
    }

    
    String toString ()
    {
        String s = "";
        for (int i = 0;i<LightMapSize;i++)
        {
            s+=("i");s+=(i);s+=(":");
            for (int bit = 0;bit<32;bit++)
            {
                s+= ((lightMap[i] >> bit) & 1);
            }
            s+= "\n";
        }
        return s;
    }
    
};


#endif
