// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "HeroSkillAction.h"
#include "Components/SplineComponent.h"


// Sets default values

AHeroSkillAction::AHeroSkillAction(const FObjectInitializer& ObjectInitializer)
	: Super(FObjectInitializer::Get())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovePathSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("MovePath0"));

}

// Called when the game starts or when spawned
void AHeroSkillAction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeroSkillAction::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

