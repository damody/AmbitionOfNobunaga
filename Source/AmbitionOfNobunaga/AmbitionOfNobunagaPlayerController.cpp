// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AmbitionOfNobunaga.h"
#include "AmbitionOfNobunagaPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "RTS_HUD.h"
#include "HeroCharacter.h"

AAmbitionOfNobunagaPlayerController::AAmbitionOfNobunagaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAmbitionOfNobunagaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AAmbitionOfNobunagaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAmbitionOfNobunagaPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAmbitionOfNobunagaPlayerController::MoveToTouchLocation);
}

void AAmbitionOfNobunagaPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
		MouseCursorTouch3DLocation(Hit.ImpactPoint);
	}
}

void AAmbitionOfNobunagaPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
		MouseCursorTouch3DLocation(HitResult.ImpactPoint);
	}
}

void AAmbitionOfNobunagaPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		for (AHeroCharacter* EachHero : hud->CurrentSelection)
		{
			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			float const Distance = FVector::Dist(DestLocation, EachHero->GetActorLocation());

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if (NavSys && (Distance > 120.0f))
			{
				NavSys->SimpleMoveToLocation(EachHero->GetController(), DestLocation);
			}
		}
	}
}

void AAmbitionOfNobunagaPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AAmbitionOfNobunagaPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
