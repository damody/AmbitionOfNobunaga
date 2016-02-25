// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "SceneObject.h"
#include "RTS_HUD.h"
#include "UnrealNetwork.h"

// Sets default values
ASceneObject::ASceneObject(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh0"));
	RootComponent = StaticMesh;
	BodyBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BodyBox0"));
	if (StaticMesh)
	{
		StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		StaticMesh->OnClicked.AddDynamic(this, &ASceneObject::OnMouseClicked);
		StaticMesh->bGenerateOverlapEvents = true;
	}
	if (BodyBox)
	{
		BodyBox->AttachParent = RootComponent;
		BodyBox->SetBoxExtent(FVector(10, 10, 10));
		BodyBox->OnClicked.AddDynamic(this, &ASceneObject::OnMouseClicked);
		BodyBox->SetCollisionObjectType(ECC_Pawn);
		BodyBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BodyBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
		BodyBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		BodyBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		BodyBox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
		BodyBox->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
		BodyBox->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
		BodyBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		BodyBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		BodyBox->bGenerateOverlapEvents = true;
	}
	CurrentHP = 10;
	DestoryCounting = 10;
	NeedDestory = false;
}

// Called when the game starts or when spawned.
void ASceneObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASceneObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (CurrentHP < 0)
	{
		PrepareBeDestory();
	}
	if (NeedDestory)
	{
		DestoryCounting -= DeltaTime;
		this->SetActorRelativeScale3D(this->GetActorRelativeScale3D()*0.98);
		if (DestoryCounting <= 0)
		{
			this->ConditionalBeginDestroy();
		}
	}
}

void ASceneObject::PrepareBeDestory()
{
	NeedDestory = true;
}

void ASceneObject::OnMouseClicked(UPrimitiveComponent* TouchComp)
{
	ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		// «ö¤U¥kÁä
		if (hud->bMouseRButton)
		{
			if (hud->CurrentSelection.Num() > 0)
			{
				hud->HeroAttackSceneObject(this);
			}
		}
	}
}


void ASceneObject::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASceneObject, CurrentHP);
}

