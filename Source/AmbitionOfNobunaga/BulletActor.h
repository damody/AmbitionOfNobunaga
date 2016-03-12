// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class AMBITIONOFNOBUNAGA_API ABulletActor : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Category = "Bullet", VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* BulletParticle;
	
	UPROPERTY(Category = "Bullet", VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* FlyParticle;

	UFUNCTION(BlueprintCallable, Category = "Bullet")
	void SetTartgetActor(AActor* TActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float MoveSpeed;

	// 離Actor多近就爆炸 
	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	float BreakDistance;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	float DestroyDelay;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	float DestoryCount;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	uint32  ActiveFlyParticleDied: 1;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	uint32  ActiveBulletParticleDied: 1;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	uint32  DiedInHeroBody: 1;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	uint32  PrepareDestory: 1;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite)
	float Damage;

};
