// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "RTS_HUD.h"
#include "HeroCharacter.h"


void ARTS_HUD::Tick(float DeltaSeconds)
{
	for (AHeroCharacter* EachHero : HeroCanSelection)
	{
		FVector pos = this->Project(EachHero->GetActorLocation());
		EachHero->ScreenPosition.X = pos.X;
		EachHero->ScreenPosition.Y = pos.Y;
	}
}

void ARTS_HUD::ClearAllSelection()
{
	for (AHeroCharacter* EachHero : CurrentSelection)
	{
		EachHero->SelectionOff();
	}
}
