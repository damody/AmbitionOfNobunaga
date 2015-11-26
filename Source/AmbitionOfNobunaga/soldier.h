// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "soldier.generated.h"

UCLASS()
class AMBITIONOFNOBUNAGA_API Asoldier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Asoldier();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
