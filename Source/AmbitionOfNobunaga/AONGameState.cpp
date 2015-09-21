// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "AONGameState.h"


bool AAONGameState::SetObjectLocation_Validate(AActor* actor, FVector pos)
{
    return true;
}


void AAONGameState::SetObjectLocation_Implementation(AActor* actor, FVector pos)
{
    actor->SetActorLocation(pos);
}

float AAONGameState::ArmorConvertToInjuryPersent(float armor)
{
    return 1.f / (1.f + 0.06f * armor);
}
