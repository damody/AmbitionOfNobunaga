// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HeroSkillAction.generated.h"

class USplineComponent;

UCLASS()
class AMBITIONOFNOBUNAGA_API AHeroSkillAction : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeroSkillAction();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* MovePathSpline;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	UCurveFloat* MovePathCurve;
	
};
