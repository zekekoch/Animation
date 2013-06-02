//
//  constants.h
//  
//
//  Created by Zeke Koch on 5/30/13.
//
//

#ifndef _constants_h
#define _constants_h

enum StrandEnum
{
    AllStrands = -1,
    RingTop = 0,
    RingMiddle = 1,
    RingBottom = 2,
    VTop = 3,
    VMiddle = 4,
    VBottom = 5,
    Vertical = 6,
};

enum AnimationType
{
    NoAnimation,
    SolidAnimation,
    FadeAnimation,
    ChaseAnimation,
    RainbowAnimation,
    BitFadeAnimation,
    BitHSVFadeAnimation,
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
    
    VStraight,
    
    AllOff,
    AllOn,
};

#endif
