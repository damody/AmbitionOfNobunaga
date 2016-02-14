// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "AONGameState.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "HeroCharacter.h"
// for GEngine
#include "Engine.h"

bool AAONGameState::SetHeroMove_Validate(AHeroCharacter* actor, const FVector& pos)
{
	return true;
}

void AAONGameState::SetHeroMove_Implementation(AHeroCharacter* actor, const FVector& pos)
{
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	float const Distance = FVector::Dist(pos, actor->GetActorLocation());
	// We need to issue move command only if far enough in order for walk animation to play correctly
	if (NavSys && (Distance > 120.0f))
	{
		NavSys->SimpleMoveToLocation(actor->GetController(), pos);
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta,
		FString::Printf(L"x:%.1f y:%.1f z:%.1f", pos.X,
			pos.Y, pos.Z)); 
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
