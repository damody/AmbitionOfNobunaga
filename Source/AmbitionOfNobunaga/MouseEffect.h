// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MouseEffect.generated.h"

UCLASS()
class AMBITIONOFNOBUNAGA_API AMouseEffect : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMouseEffect();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Category = "Bullet", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* EffectMesh;
	
};
