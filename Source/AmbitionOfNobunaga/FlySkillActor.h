// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FlySkillActor.generated.h"

UCLASS()
class AMBITIONOFNOBUNAGA_API AFlySkillActor : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	void OnBeginAttackOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Category = "Equipment", VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* BulletParticle;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadWrite, Replicated)
	int32 TeamId;	

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Scene;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	float DestroyDelay;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	float DestoryCount;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	bool  PrepareDestory;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly)
	bool IsFixdLength;
	
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly)
	float PhysicalDamage;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly)
	float MagicDamage;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly)
	bool UseTargetLocation;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly)
	bool UseTargetActor;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadWrite, Replicated, meta = (EditCondition = "UseTargetLocation"))
	FVector TargetLocation;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadWrite, Replicated, meta = (EditCondition = "UseTargetActor"))
	AActor* TargetActor;

	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> AlreadyDamageActor;
};
