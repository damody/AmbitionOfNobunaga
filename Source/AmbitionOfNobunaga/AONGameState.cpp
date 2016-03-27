// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "AONGameState.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "HeroCharacter.h"
// for GEngine
#include "Engine.h"
#include "AIController.h"


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

bool AAONGameState::CharacterMove_Validate(AHeroCharacter* actor, const FVector& pos)
{
	return true;
}

void AAONGameState::CharacterMove_Implementation(AHeroCharacter* actor, const FVector& pos)
{
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
// 	if (NavSys && actor->GetController())
// 	{
// 		NavSys->SimpleMoveToLocation(actor->GetController(), pos);
// 	}
	if (actor->WalkAI)
	{
		//actor->WalkAI->MoveToLocation(pos);
	}
	else
	{
		UE_LOG(LogAmbitionOfNobunaga, Log, TEXT("%s WalkAI->MoveToLocation FAIL"), *(actor->GetFullName()));
	}
}

bool AAONGameState::CharacterStopMove_Validate(AHeroCharacter* actor)
{
	return true;
}

void AAONGameState::CharacterStopMove_Implementation(AHeroCharacter* actor)
{
	if (actor->WalkAI)
	{
		actor->WalkAI->StopMovement();
	}
	else
	{
		UE_LOG(LogAmbitionOfNobunaga, Log, TEXT("%s WalkAI->StopMovement FAIL"), *(actor->GetFullName()));
	}
}

bool AAONGameState::HeroUseSkill_Validate(AHeroCharacter* hero, int32 index, const FVector& VFaceTo, const FVector& Pos)
{
	return true;
}

void AAONGameState::HeroUseSkill_Implementation(AHeroCharacter* hero, int32 index, const FVector& VFaceTo,
        const FVector& Pos)
{
	hero->UseSkill(index, VFaceTo, Pos);
}
