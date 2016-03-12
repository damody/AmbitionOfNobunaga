// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HeroBuff.h"
#include "RangeSkillActor.generated.h"


class AHeroCharacter;

UCLASS()
class AMBITIONOFNOBUNAGA_API ARangeSkillActor : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UFUNCTION()
	void OnBeginAttackOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndAttackOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, WithValidation, Reliable)
	void Injury();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Category = "Equipment", VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = "RangeSkill", VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* BulletParticle;
	
	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite, Replicated)
	int32 TeamId;	

	UPROPERTY(Category = "RangeSkill", VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Scene;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	float LifeTime;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	float LifeCount;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadOnly)
	float DamageTime;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	uint32  HasDamaged: 1;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	float DestroyDelay;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	float DestoryCount;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	uint32  PrepareDestory: 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FightRole")
	TArray<AHeroCharacter*> AttackCollision;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadOnly)
	uint32 IsFixdLength: 1;
	
	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadOnly)
	float PhysicalDamage;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadOnly)
	float MagicDamage;
	
	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	TArray<AHeroCharacter*> AlreadyDamageActor;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	TArray<UHeroBuff*> Buffs;

	UPROPERTY(Category = "RangeSkill", EditAnywhere, BlueprintReadWrite)
	uint32 IsReadyToStart: 1;
};
