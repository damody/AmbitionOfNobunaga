// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "BulletActor.h"
#include "HeroCharacter.h"

ABulletActor::ABulletActor(const FObjectInitializer& ObjectInitializer)
    : Super(FObjectInitializer::Get())
{
    PrimaryActorTick.bCanEverTick = true;
    BulletParticle = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("BulletParticle0"));
    RootComponent = BulletParticle;
	FlyParticle = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("FlyParticle0"));
	FlyParticle->AttachParent = BulletParticle;
    BreakDistance = 20;
    MoveSpeed = 100;
    DestroyDelay = 2;
    PrepareDestory = false;
	ActiveFlyParticleDied = false;
	ActiveBulletParticleDied = true;
    DestoryCount = 0;
    TargetActor = NULL;
}
// Sets default values
ABulletActor::ABulletActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    MoveSpeed = 100;
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(TargetActor && !PrepareDestory)
    {
        float move = DeltaTime * MoveSpeed;
        FVector ourpos = GetActorLocation();
        FVector dstpos = TargetActor->GetActorLocation();
        float dis = FVector::Dist(ourpos, dstpos);
        if(move >= dis)
        {
            SetActorLocation(dstpos);
        }
        else
        {
            FVector dir = dstpos - ourpos;
            dir.Normalize();
            dir *= move;
            SetActorLocation(ourpos + dir);
        }
        if(BreakDistance > dis)
        {
            AHeroCharacter* hero = Cast<AHeroCharacter>(TargetActor);
            hero->CurrentHP -= Damage;
            PrepareDestory = true;
			if (!ActiveFlyParticleDied)
			{
				FlyParticle->SetActive(false);
			}
			if (!ActiveBulletParticleDied)
			{
				BulletParticle->SetActive(false);
			}
        }
    }
    if(PrepareDestory)
    {
        if(DiedInHeroBody)
        {
            FVector dstpos = TargetActor->GetActorLocation();
            SetActorLocation(dstpos);
        }
        DestoryCount += DeltaTime;
        if(DestoryCount > DestroyDelay)
        {
            Destroy();
        }
    }
}

void ABulletActor::SetTartgetActor(AActor* TActor)
{
    TargetActor = TActor;
}

