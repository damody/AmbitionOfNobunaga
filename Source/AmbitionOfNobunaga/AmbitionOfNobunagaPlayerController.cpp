// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AmbitionOfNobunaga.h"
#include "AmbitionOfNobunagaPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "RTS_HUD.h"
#include "HeroCharacter.h"
#include "Engine/LocalPlayer.h"
// for GEngine
#include "IHeadMountedDisplay.h"
#include "Engine.h"
#include "Equipment.h"


AAmbitionOfNobunagaPlayerController::AAmbitionOfNobunagaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	FloorActorName = TEXT("Floor");
}

void AAmbitionOfNobunagaPlayerController::BeginPlay()
{
	Hud = Cast<ARTS_HUD>(this->GetHUD());
	if (Hud)
	{
		Hud->localController = this;
	}
	bMouseRButton = false;
	bMouseLButton = false;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (*ActorItr != this)
		{
			ActorItr->SetOwner(this);
		}
	}
}

bool AAmbitionOfNobunagaPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed,
        bool bGamepad)
{
	bool bResult = false;

	if(GEngine->HMDDevice.IsValid())
	{
		bResult = GEngine->HMDDevice->HandleInputKey(PlayerInput, Key, EventType, AmountDepressed, bGamepad);
		if(bResult)
		{
			return bResult;
		}
	}
	if (Key == EKeys::LeftShift)
	{
		if (Hud)
		{
			if (EventType == IE_Pressed)
			{
				Hud->bLeftShiftDown = true;
			}
			else if (EventType == IE_Released)
			{
				Hud->bLeftShiftDown = false;
			}
		}
	}
	else if (Key == EKeys::RightShift)
	{
		if (Hud)
		{
			if (EventType == IE_Pressed)
			{
				Hud->bRightShiftDown = true;
			}
			else if (EventType == IE_Released)
			{
				Hud->bRightShiftDown = false;
			}
		}
	}
	if(PlayerInput)
	{
		bResult = PlayerInput->InputKey(Key, EventType, AmountDepressed, bGamepad);

		// TODO: Allow click key(s?) to be defined
		if(bEnableClickEvents && (Key == EKeys::LeftMouseButton || Key == EKeys::RightMouseButton))
		{
			if (Key == EKeys::LeftMouseButton)
			{
				AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed1();
			}
			else if (Key == EKeys::RightMouseButton)
			{
				AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed1();
			}
			FVector2D MousePosition;
			if(CastChecked<ULocalPlayer>(Player)->ViewportClient->GetMousePosition(MousePosition))
			{
				ClickedPrimitive = NULL;
				if(bEnableMouseOverEvents)
				{
					ClickedPrimitive = CurrentClickablePrimitive.Get();
				}
				else
				{
					FHitResult HitResult;
					const bool bHit = GetHitResultAtScreenPosition(MousePosition, CurrentClickTraceChannel, true, HitResult);
					if(bHit)
					{
						ClickedPrimitive = HitResult.Component.Get();
					}
				}
				if(GetHUD())
				{
					if(GetHUD()->UpdateAndDispatchHitBoxClickEvents(MousePosition, EventType))
					{
						ClickedPrimitive = NULL;
					}
				}

				if(ClickedPrimitive)
				{
					switch(EventType)
					{
					case IE_Pressed:
					case IE_DoubleClick:
						ClickedPrimitive->DispatchOnClicked();
						break;

					case IE_Released:
						ClickedPrimitive->DispatchOnReleased();
						break;

					case IE_Axis:
					case IE_Repeat:
						break;
					}
				}

				bResult = true;
			}
			if (Key == EKeys::LeftMouseButton)
			{
				AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed2();
			}
			else if (Key == EKeys::RightMouseButton)
			{
				AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed2();
			}
		}
	}

	return bResult;
}

void AAmbitionOfNobunagaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(Hud)
	{
		CurrentMouseXY = GetMouseScreenPosition();
		TArray<FHitResult> Hits;
		bool res;
		FVector WorldOrigin;
		FVector WorldDirection;
		FCollisionObjectQueryParams CollisionQuery;
		CollisionQuery.AddObjectTypesToQuery(ECC_WorldStatic);
		if (UGameplayStatics::DeprojectScreenToWorld(this, CurrentMouseXY, WorldOrigin, WorldDirection) == true)
		{
			res = GetWorld()->LineTraceMultiByObjectType(Hits, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance,
			        CollisionQuery);
		}
		// 只trace 地板的actor
		// 地板名可以自定義
		FVector HitPoint(0, 0, 0);
		if (Hits.Num() > 0)
		{
			for (FHitResult Hit : Hits)
			{
				if (Hit.Actor.IsValid() && Hit.Actor->GetFName().GetPlainNameString() == FloorActorName)
				{
					HitPoint = Hit.ImpactPoint;
				}
				// all landscape can click
				if (Hit.Actor.IsValid())
				{
					FString ResStr;
					Hit.Actor->GetClass()->GetName(ResStr);
					if (ResStr == "Landscape")
					{
						HitPoint = Hit.ImpactPoint;
					}
				}
			}
		}
		Hud->OnMouseMove(CurrentMouseXY, HitPoint);
	}
}

void AAmbitionOfNobunagaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	//InputComponent->BindAction("MouseRButton", IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed);
	InputComponent->BindAction("MouseRButton", IE_Released, this,
	                           &AAmbitionOfNobunagaPlayerController::OnMouseRButtonReleased);
	//InputComponent->BindAction("MouseLButton", IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed);
	InputComponent->BindAction("MouseLButton", IE_Released, this,
	                           &AAmbitionOfNobunagaPlayerController::OnMouseLButtonReleased);

	// support touch devices
//     InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::MoveToTouchLocation);
//     InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAmbitionOfNobunagaPlayerController::MoveToTouchLocation);
}
FVector2D AAmbitionOfNobunagaPlayerController::GetMouseScreenPosition()
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if(LocalPlayer && LocalPlayer->ViewportClient)
	{
		return LocalPlayer->ViewportClient->GetMousePosition();
	}
	return FVector2D(-1, -1);
}

void AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed1()
{
	bMouseRButton = true;
	if (Hud)
	{
		Hud->OnRMousePressed1(GetMouseScreenPosition());
	}
}

void AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed2()
{
	bMouseRButton = true;
	if(Hud)
	{
		Hud->OnRMousePressed2(GetMouseScreenPosition());
	}
}

void AAmbitionOfNobunagaPlayerController::OnMouseRButtonReleased()
{
	bMouseRButton = false;
	if(Hud)
	{
		Hud->OnRMouseReleased(GetMouseScreenPosition());
	}
}

void AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed1()
{
	if (Hud)
	{
		Hud->OnLMousePressed1(GetMouseScreenPosition());
	}
}

void AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed2()
{
	bMouseLButton = true;
	if(Hud)
	{
		Hud->OnLMousePressed2(GetMouseScreenPosition());
	}
}

void AAmbitionOfNobunagaPlayerController::OnMouseLButtonReleased()
{
	bMouseLButton = false;
	if(Hud)
	{
		Hud->OnLMouseReleased(GetMouseScreenPosition());
	}
}

// network function


bool AAmbitionOfNobunagaPlayerController::SetHeroAction_Validate(AHeroCharacter* hero, const FHeroAction& action)
{
	return true;
}

void AAmbitionOfNobunagaPlayerController::SetHeroAction_Implementation(AHeroCharacter* hero, const FHeroAction& action)
{
	hero->ActionQueue.Empty();
	hero->ActionQueue.Add(action);
}

bool AAmbitionOfNobunagaPlayerController::AppendHeroAction_Validate(AHeroCharacter* hero, const FHeroAction& action)
{
	return true;
}

void AAmbitionOfNobunagaPlayerController::AppendHeroAction_Implementation(AHeroCharacter* hero, const FHeroAction& action)
{
	hero->ActionQueue.Add(action);
}


bool AAmbitionOfNobunagaPlayerController::ClearHeroAction_Validate(AHeroCharacter* hero, const FHeroAction& action)
{
	return true;
}

void AAmbitionOfNobunagaPlayerController::ClearHeroAction_Implementation(AHeroCharacter* hero, const FHeroAction& action)
{
	hero->ActionQueue.Empty();
}



bool AAmbitionOfNobunagaPlayerController::CharacterMove_Validate(AHeroCharacter* actor, const FVector& pos)
{
	return true;
}

void AAmbitionOfNobunagaPlayerController::CharacterMove_Implementation(AHeroCharacter* actor, const FVector& pos)
{
	//UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	//NavSys->SimpleMoveToLocation(actor->GetController(), pos);
	if (actor->WalkAI)
	{
		actor->WalkAI->MoveToLocation(pos);
	}
}

bool AAmbitionOfNobunagaPlayerController::CharacterStopMove_Validate(AHeroCharacter* actor)
{
	return true;
}

void AAmbitionOfNobunagaPlayerController::CharacterStopMove_Implementation(AHeroCharacter* actor)
{
	actor->GetController()->StopMovement();
}

bool AAmbitionOfNobunagaPlayerController::HeroUseSkill_Validate(AHeroCharacter* hero, int32 index, const FVector& VFaceTo, const FVector& Pos)
{
	return true;
}

void AAmbitionOfNobunagaPlayerController::HeroUseSkill_Implementation(AHeroCharacter* hero, int32 index, const FVector& VFaceTo,
	const FVector& Pos)
{
	hero->UseSkill(index, VFaceTo, Pos);
}

