// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "RangeSkillActor.h"
#include "UnrealNetwork.h"
#include "AONGameState.h"
#include "HeroCharacter.h"
// for GEngine
#include "Engine.h"

ARangeSkillActor::ARangeSkillActor(const FObjectInitializer& ObjectInitializer)
    : Super(FObjectInitializer::Get())
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = true;
    Scene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("root0"));
    BulletParticle = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("BulletParticle0"));
    RootComponent = Scene;
    BulletParticle->AttachParent = RootComponent;
    CapsuleComponent = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("Capsule0"));
    CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
    CapsuleComponent->AttachParent = RootComponent;
    CapsuleComponent->CanCharacterStepUpOn = ECB_No;
    CapsuleComponent->bShouldUpdatePhysicsVolume = true;
    CapsuleComponent->bCheckAsyncSceneOnMove = false;
    CapsuleComponent->bCanEverAffectNavigation = false;
    CapsuleComponent->bDynamicObstacle = true;
    CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
    LifeTime = 2;
    DestroyDelay = 2;
    LifeCount = 0;
    HasDamaged = false;
	IsReadyToStart = false;
    DamageTime = 1;
}
#if WITH_EDITOR
void ARangeSkillActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
void ARangeSkillActor::OnBeginAttackOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
    if(hero)
    {
        AttackCollision.Add(hero);
    }
}

void ARangeSkillActor::OnEndAttackOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
	if (hero)
	{
		AttackCollision.Remove(hero);
	}
}

bool ARangeSkillActor::Injury_Validate()
{
	return true;
}

void ARangeSkillActor::Injury_Implementation()
{
	AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
	for (AHeroCharacter* hero : AttackCollision)
	{
		// 如果不同隊才造成傷害
		if (hero && hero->TeamId != TeamId)
		{
			// 物傷
			if (PhysicalDamage > 0)
			{
				float Injury = ags->ArmorConvertToInjuryPersent(hero->CurrentArmor);
				float Damage = PhysicalDamage * Injury;

				// 顯示傷害文字
				ADamageEffect* TempDamageText = GetWorld()->SpawnActor<ADamageEffect>(AHeroCharacter::ShowDamageEffect);
				if (TempDamageText)
				{
					FVector pos = hero->GetActorLocation();
					pos.X += 10;
					TempDamageText->OriginPosition = pos;
					TempDamageText->SetString(FString::FromInt((int32)PhysicalDamage));
					FVector scaleSize(TempDamageText->ScaleSize, TempDamageText->ScaleSize, TempDamageText->ScaleSize);
					TempDamageText->SetActorScale3D(scaleSize);
					FVector dir = hero->GetActorLocation() - GetActorLocation();
					dir.Normalize();
					TempDamageText->FlyDirection = dir;
				}

				hero->CurrentHP -= Damage;
			}
			// 法傷
			if (MagicDamage > 0)
			{
				float Damage = MagicDamage * (1 - hero->CurrentMagicInjured);
				hero->CurrentHP -= Damage;

				// 顯示傷害文字
				ADamageEffect* TempDamageText = GetWorld()->SpawnActor<ADamageEffect>(AHeroCharacter::ShowDamageEffect);
				if (TempDamageText)
				{
					FVector pos = hero->GetActorLocation();
					pos.X -= 10;
					TempDamageText->OriginPosition = pos;
					TempDamageText->SetString(FString::FromInt((int32)Damage));
					FVector scaleSize(TempDamageText->ScaleSize, TempDamageText->ScaleSize, TempDamageText->ScaleSize);
					TempDamageText->SetActorScale3D(scaleSize);
					FVector dir = hero->GetActorLocation() - GetActorLocation();
					dir.Normalize();
					TempDamageText->FlyDirection = dir;
				}
			}
			hero->BuffQueue.Append(UHeroBuff::CloneArray(Buffs));
		}
	}
}

// Called when the game starts or when spawned
void ARangeSkillActor::BeginPlay()
{
    Super::BeginPlay();
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ARangeSkillActor::OnBeginAttackOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ARangeSkillActor::OnEndAttackOverlap);
}

// Called every frame
void ARangeSkillActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	if (!IsReadyToStart)
	{
		return;
	}
    LifeCount += DeltaTime;
    if(!HasDamaged && LifeCount >= DamageTime)
    {
        HasDamaged = true;
		Injury();
    }
    if(!PrepareDestory && LifeCount >= LifeTime)
    {
        BulletParticle->SetActive(false);
        PrepareDestory = true;
        DestoryCount = 0;
    }
    if(PrepareDestory)
    {
        DestoryCount += DeltaTime;
        if(DestoryCount >= DestroyDelay)
        {
            this->Destroy();
        }
    }
}

void ARangeSkillActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARangeSkillActor, TeamId);
}