// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "Equipment.h"
#include "RTS_HUD.h"
#include "UnrealNetwork.h"


// Sets default values
AEquipment::AEquipment(const FObjectInitializer& ObjectInitializer)
    : Super(FObjectInitializer::Get())
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ACharacter::CapsuleComponentName);
    CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
    CapsuleComponent->CanCharacterStepUpOn = ECB_No;
    CapsuleComponent->bShouldUpdatePhysicsVolume = true;
    CapsuleComponent->bCheckAsyncSceneOnMove = false;
    CapsuleComponent->bCanEverAffectNavigation = false;
    CapsuleComponent->bDynamicObstacle = true;

    StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh0"));
    if(StaticMesh)
    {
        StaticMesh->AlwaysLoadOnClient = true;
        StaticMesh->AlwaysLoadOnServer = true;
        StaticMesh->bOwnerNoSee = false;
        StaticMesh->bCastDynamicShadow = true;
        StaticMesh->bAffectDynamicIndirectLighting = true;
        StaticMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        StaticMesh->AttachParent = CapsuleComponent;
		StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
    }
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
    RootComponent = CapsuleComponent;
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
    Super::BeginPlay();
    CapsuleComponent->OnClicked.AddDynamic(this, &AEquipment::OnMouseClicked);
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AEquipment::OnRep_PosChange()
{
    SetActorLocation(CurrentPosition);
}

void AEquipment::OnRep_RotChange()
{
    SetActorRotation(CurrentRotation);
}

void AEquipment::OnMouseClicked(UPrimitiveComponent* TouchComp)
{
    ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
    if(hud && hud->bMouseRButton)
    {
        hud->AssignSelectionHeroPickup(this);
    }
}

// if (GEngine->GetNetMode(GetWorld()) == ENetMode::NM_Client)
// if (Role == ROLE_Authority)

bool AEquipment::ServerSetLocation_Validate(FVector location)
{
    return true;
}

void AEquipment::ServerSetLocation_Implementation(FVector location)
{
	CurrentPosition = location;
	SetActorLocation(location);
}

void AEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AEquipment, CurrentPosition);
    DOREPLIFETIME(AEquipment, CurrentRotation);
}
