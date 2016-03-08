// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ObjectPlacer.generated.h"


UCLASS()
class AMBITIONOFNOBUNAGA_API AObjectPlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectPlacer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TSubclassOf<AActor> WantPlacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool ClickToUpdate;
	
};
