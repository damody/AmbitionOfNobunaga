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
}

void AAmbitionOfNobunagaPlayerController::BeginPlay()
{
    Hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
    Hud->localController = this;
    bMouseRButton = false;
    bMouseLButton = false;
    for(TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if(*ActorItr != this)
        {
            ActorItr->SetOwner(this);
        }
    }
}

bool AAmbitionOfNobunagaPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
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

    if(PlayerInput)
    {
        bResult = PlayerInput->InputKey(Key, EventType, AmountDepressed, bGamepad);

        // TODO: Allow click key(s?) to be defined
        if(bEnableClickEvents && (Key == EKeys::LeftMouseButton || Key == EKeys::RightMouseButton))
        {
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
        FHitResult Hit;
        bool res = GetHitResultAtScreenPosition(CurrentMouseXY, ECC_Visibility, false, Hit);
//      if (Hit.Actor.IsValid())
//      {
//          Hit.Actor->AddActorWorldOffset(FVector(0, 0, 100));
//      }
        Hud->OnMouseMove(CurrentMouseXY, Hit.ImpactPoint);
    }
    ServerUpdateMove();
}

void AAmbitionOfNobunagaPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction("MouseRButton", IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed);
    InputComponent->BindAction("MouseRButton", IE_Released, this, &AAmbitionOfNobunagaPlayerController::OnMouseRButtonReleased);
    InputComponent->BindAction("MouseLButton", IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed);
    InputComponent->BindAction("MouseLButton", IE_Released, this, &AAmbitionOfNobunagaPlayerController::OnMouseLButtonReleased);

    // support touch devices
//     InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAmbitionOfNobunagaPlayerController::MoveToTouchLocation);
//     InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAmbitionOfNobunagaPlayerController::MoveToTouchLocation);
}
bool AAmbitionOfNobunagaPlayerController::ServerUpdateMove_Validate()
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::ServerUpdateMove_Implementation()
{
    if(HeroMoveQueue.Num() > 0)
    {
        TArray<FMoveData> TmpQueue;
        TmpQueue = HeroMoveQueue;
        HeroMoveQueue.Empty();
        for(FMoveData& EachMove : TmpQueue)
        {
            ServerMoveHeros(EachMove.DestLocation, EachMove.Heros);
        }
    }
    if(HeroMoveQueue2D.Num() > 0)
    {
        TArray<FMoveData2D> TmpQueue;
        TmpQueue = HeroMoveQueue2D;
        HeroMoveQueue2D.Empty();
        for(FMoveData2D& EachMove : TmpQueue)
        {
            FHitResult Hit;
            bool res = GetHitResultAtScreenPosition(EachMove.DestLocation, ECC_Visibility, false, Hit);
            ServerMoveHeros(Hit.ImpactPoint, EachMove.Heros);
        }
    }
    if(HeroStopMoveQueue.Num() > 0)
    {
        TArray<AHeroCharacter*> TmpQueue;
        TmpQueue = HeroStopMoveQueue;
        HeroStopMoveQueue.Empty();
        for(AHeroCharacter* EachHero : TmpQueue)
        {
            TArray<AHeroCharacter*> oneHero;
            oneHero.Add(EachHero);
            ServerStopMovement(oneHero);
        }
    }
    if(HeroThrowQueue.Num() > 0)
    {
        TArray<FHeroThrow> TmpQueue;
        TmpQueue = HeroThrowQueue;
        HeroThrowQueue.Empty();
        for(FHeroThrow& EachMove : TmpQueue)
        {
            AHeroCharacter* Selection = EachMove.Heros;
            Selection->WantThrow = Selection->Equipments[EachMove.EquIndex];
            Selection->ThrowDestination = EachMove.DestLocation;
            TArray<AHeroCharacter*> oneHero;
            oneHero.Add(EachMove.Heros);
            ServerMoveHeros(EachMove.DestLocation, oneHero);
        }
    }
    if(HeroPickupQueue.Num() > 0)
    {
        TArray<FHeroPickup> TmpQueue;
        TmpQueue = HeroPickupQueue;
        HeroPickupQueue.Empty();
        for(FHeroPickup& EachMove : TmpQueue)
        {
            AHeroCharacter* Selection = EachMove.Heros;
            Selection->WantPickup = EachMove.Equptr;
            Selection->ThrowDestination = EachMove.DestLocation;
            TArray<AHeroCharacter*> oneHero;
            oneHero.Add(EachMove.Heros);
            ServerMoveHeros(EachMove.DestLocation, oneHero);
        }
    }
}

bool AAmbitionOfNobunagaPlayerController::ServerMoveHeros_Validate(const FVector DestLocation, const TArray<AHeroCharacter*>& heros)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::ServerMoveHeros_Implementation(const FVector DestLocation, const TArray<AHeroCharacter*>& heros)
{
    for(AHeroCharacter* EachHero : heros)
    {
        UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
        float const Distance = FVector::Dist(DestLocation, EachHero->GetActorLocation());

        // We need to issue move command only if far enough in order for walk animation to play correctly
        if(NavSys && (Distance > 120.0f))
        {
            NavSys->SimpleMoveToLocation(EachHero->GetController(), DestLocation);
        }
    }
}

bool AAmbitionOfNobunagaPlayerController::ServerStopMovement_Validate(const TArray<AHeroCharacter*>& heros)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::ServerStopMovement_Implementation(const TArray<AHeroCharacter*>& heros)
{
    for(AHeroCharacter* EachHero : heros)
    {
        EachHero->GetController()->StopMovement();
    }
}

bool AAmbitionOfNobunagaPlayerController::AddHeroToMoveQueue_Validate(const FVector DestLocation, const TArray<AHeroCharacter*>& heros)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::AddHeroToMoveQueue_Implementation(const FVector DestLocation, const TArray<AHeroCharacter*>& heros)
{
    HeroMoveQueue.Add(FMoveData(heros, DestLocation));
}

bool AAmbitionOfNobunagaPlayerController::AddHeroToThrowQueue_Validate(const FVector DestLocation, AHeroCharacter* heros, int32 EquIndex)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::AddHeroToThrowQueue_Implementation(const FVector DestLocation, AHeroCharacter* heros, int32 EquIndex)
{
    HeroThrowQueue.Add(FHeroThrow(heros, DestLocation, EquIndex));
}

bool AAmbitionOfNobunagaPlayerController::AddHeroToMoveQueue2D_Validate(const FVector2D DestLocation, const TArray<AHeroCharacter*>& heros)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::AddHeroToMoveQueue2D_Implementation(const FVector2D DestLocation, const TArray<AHeroCharacter*>& heros)
{
    HeroMoveQueue2D.Add(FMoveData2D(heros, DestLocation));
}

bool AAmbitionOfNobunagaPlayerController::AddHeroToStopMoveQueue_Validate(AHeroCharacter* hero)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::AddHeroToStopMoveQueue_Implementation(AHeroCharacter* hero)
{
    HeroStopMoveQueue.Add(hero);
}

bool AAmbitionOfNobunagaPlayerController::AddHeroToPickupQueue_Validate(const FVector DestLocation, AHeroCharacter* heros, AEquipment* equ)
{
    return true;
}

void AAmbitionOfNobunagaPlayerController::AddHeroToPickupQueue_Implementation(const FVector DestLocation, AHeroCharacter* heros, AEquipment* equ)
{
    HeroPickupQueue.Add(FHeroPickup(heros, DestLocation, equ));
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

void AAmbitionOfNobunagaPlayerController::OnMouseRButtonPressed()
{
    bMouseRButton = true;
    if(Hud)
    {
        Hud->OnRMousePressed(GetMouseScreenPosition());
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

void AAmbitionOfNobunagaPlayerController::OnMouseLButtonPressed()
{
    bMouseLButton = true;
    if(Hud)
    {
        Hud->OnLMousePressed(GetMouseScreenPosition());
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
