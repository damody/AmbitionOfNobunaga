// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SceneObject.generated.h"

UCLASS()
class AMBITIONOFNOBUNAGA_API ASceneObject : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BodyBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent * StaticMesh;

	// ¦å¶q
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float DestoryCounting;

	bool NeedDestory;

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void PrepareBeDestory();
	


	// for UI
	UFUNCTION()
	void OnMouseClicked(UPrimitiveComponent* TouchComp);
};
