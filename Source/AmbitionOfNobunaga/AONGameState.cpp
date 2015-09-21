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
