// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "AONGameState.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "HeroCharacter.h"
// for GEngine
#include "Engine.h"

bool AAONGameState::CharacterMove_Validate(ACharacter* actor, const FVector& pos)
{
	return true;
}

void AAONGameState::CharacterMove_Implementation(ACharacter* actor, const FVector& pos)
{
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	NavSys->SimpleMoveToLocation(actor->GetController(), pos);
}

bool AAONGameState::CharacterStopMove_Validate(ACharacter* actor)
{
	return true;
}

void AAONGameState::CharacterStopMove_Implementation(ACharacter* actor)
{
	actor->GetController()->StopMovement();
}

bool AAONGameState::SetObjectLocation_Validate(AActor* actor, const FVector& pos)
{
    return true;
}

void AAONGameState::SetObjectLocation_Implementation(AActor* actor, const FVector& pos)
{
	actor->SetActorLocation(pos);
}

float AAONGameState::ArmorConvertToInjuryPersent(float armor)
{
    return 1.f / (1.f + 0.06f * armor);
}

bool AAONGameState::SetHeroAction_Validate(AHeroCharacter* hero, const FHeroAction& action)
{
	return true;
}

void AAONGameState::SetHeroAction_Implementation(AHeroCharacter* hero, const FHeroAction& action)
{
	hero->ActionQueue.Empty();
	hero->ActionQueue.Add(action);
}

bool AAONGameState::AppendHeroAction_Validate(AHeroCharacter* hero, const FHeroAction& action)
{
	return true;
}

void AAONGameState::AppendHeroAction_Implementation(AHeroCharacter* hero, const FHeroAction& action)
{
	hero->ActionQueue.Add(action);
}


bool AAONGameState::ClearHeroAction_Validate(AHeroCharacter* hero, const FHeroAction& action)
{
	return true;
}

void AAONGameState::ClearHeroAction_Implementation(AHeroCharacter* hero, const FHeroAction& action)
{
	hero->ActionQueue.Empty();
}
