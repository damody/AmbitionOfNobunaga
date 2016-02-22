// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "MouseEffect.h"

AMouseEffect::AMouseEffect(const FObjectInitializer& ObjectInitializer)
	: Super(FObjectInitializer::Get())
{
	PrimaryActorTick.bCanEverTick = true;
	EffectMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh0"));
	RootComponent = EffectMesh;
}

// Sets default values
AMouseEffect::AMouseEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMouseEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMouseEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

