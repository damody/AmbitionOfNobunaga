// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AmbitionOfNobunaga.h"
#include "RTSHitBox.h"

FRTSHitBox::FRTSHitBox(FVector2D InCoords, FVector2D InSize, const FString& InName, bool bInConsumesInput, int32 InPriority)
    : Coords(InCoords)
    , Size(InSize)
    , Name(InName)
    , bConsumesInput(bInConsumesInput)
    , Priority(InPriority)
{
}

bool FRTSHitBox::Contains(FVector2D InCoords, float scale) const
{
    bool bResult = false;
    if((InCoords.X >= Coords.X * scale) && (InCoords.X <= (Coords.X + Size.X)*scale))
    {
        if((InCoords.Y >= Coords.Y * scale) && (InCoords.Y <= (Coords.Y + Size.Y)*scale))
        {
            bResult = true;
        }
    }
    return bResult;
}