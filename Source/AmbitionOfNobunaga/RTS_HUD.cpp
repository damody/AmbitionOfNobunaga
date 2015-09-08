// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "RTS_HUD.h"
#include "HeroCharacter.h"
#include "EngineUtils.h"
#include <algorithm>

ARTS_HUD::ARTS_HUD()
{

}

void ARTS_HUD::BeginPlay()
{
    Super::BeginPlay();
    bMouseRButton = false;
    bMouseLButton = false;
	ClickedSelected = false;
    for(TActorIterator<AHeroCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        HeroCanSelection.Add(*ActorItr);
    }
}

void ARTS_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(RemoveSelection.Num() > 0)
    {
		for (AHeroCharacter* EachHero : RemoveSelection)
		{
			CurrentSelection.Remove(EachHero);
		}
		RemoveSelection.Empty();
    }
}

void ARTS_HUD::DrawHUD()
{
    Super::DrawHUD();
	
	if (bMouseLButton)
    {
		// selection box
		if (FVector2D::DistSquared(InitialMouseXY, CurrentMouseXY) > 25)
		{
			for (AHeroCharacter* EachHero : HeroCanSelection)
			{
				FVector pos = this->Project(EachHero->GetActorLocation());
				EachHero->ScreenPosition.X = pos.X;
				EachHero->ScreenPosition.Y = pos.Y;
				bool res = CheckInSelectionBox(EachHero->ScreenPosition);
				if (res && !EachHero->isSelection)
				{
					EachHero->SelectionOn();
				}
				else if (!res && EachHero->isSelection)
				{
					EachHero->SelectionOff();
				}
			}

			float maxX, maxY;
			float minX, minY;
			maxX = std::max(InitialMouseXY.X, CurrentMouseXY.X);
			maxY = std::max(InitialMouseXY.Y, CurrentMouseXY.Y);
			minX = std::min(InitialMouseXY.X, CurrentMouseXY.X);
			minY = std::min(InitialMouseXY.Y, CurrentMouseXY.Y);
			DrawLine(minX, minY, maxX, minY, SelectionBoxLineColor);
			DrawLine(maxX, minY, maxX, maxY, SelectionBoxLineColor);
			DrawLine(maxX, maxY, minX, maxY, SelectionBoxLineColor);
			DrawLine(minX, maxY, minX, minY, SelectionBoxLineColor);

			DrawRect(SelectionBoxFillColor, minX, minY, maxX - minX - 1, maxY - minY - 1);
		}
    }
}

bool ARTS_HUD::CheckInSelectionBox(FVector2D pos)
{
    float maxX, maxY;
    float minX, minY;
    maxX = std::max(InitialMouseXY.X, CurrentMouseXY.X);
    maxY = std::max(InitialMouseXY.Y, CurrentMouseXY.Y);
    minX = std::min(InitialMouseXY.X, CurrentMouseXY.X);
    minY = std::min(InitialMouseXY.Y, CurrentMouseXY.Y);

    if(minX < pos.X && pos.X < maxX &&
            minY < pos.Y && pos.Y < maxY)
    {
        return true;
    }
    return false;
}

void ARTS_HUD::ClearAllSelection()
{
    for(AHeroCharacter* EachHero : CurrentSelection)
    {
        EachHero->SelectionOff();
    }
	CurrentSelection.Empty();
}

void ARTS_HUD::OnMouseMove(FVector2D pos)
{
    CurrentMouseXY = pos;
}

void ARTS_HUD::OnRMousePressed(FVector2D pos)
{
    bMouseRButton = true;
}

void ARTS_HUD::OnRMouseReleased(FVector2D pos)
{
    bMouseRButton = false;
}

void ARTS_HUD::OnLMousePressed(FVector2D pos)
{
    bMouseLButton = true;
    InitialMouseXY = pos;
	if (!ClickedSelected)
	{
		ClearAllSelection();
	}
	ClickedSelected = false;
	UnSelectHero();
}

void ARTS_HUD::OnLMouseReleased(FVector2D pos)
{
    bMouseLButton = false;
	if (CurrentSelection.Num() > 0)
	{
		SelectedHero(CurrentSelection[0]);
	}
}

