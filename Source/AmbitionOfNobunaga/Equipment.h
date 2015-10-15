// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

UCLASS(BlueprintType)
class AMBITIONOFNOBUNAGA_API AEquipment : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UFUNCTION(Category = "Equipment", BlueprintCallable, Server, Reliable, WithValidation)
	void ServerSetLocation(FVector location);

	UFUNCTION()
	void OnMouseClicked(UPrimitiveComponent* TouchComp);


	UPROPERTY(Category = "Equipment", VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = "Equipment", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;
	
	// 大頭貼
	UPROPERTY(Category = "Equipment", EditAnywhere, BlueprintReadWrite)
	UTexture2D * Head;

	// 物品名
	UPROPERTY(Category = "Equipment", EditAnywhere, BlueprintReadOnly)
	FString Name;

	// 物品描述
	UPROPERTY(Category = "Equipment", EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_PosChange)
	FVector CurrentPosition;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RotChange)
	FRotator CurrentRotation;

	UFUNCTION()
	void OnRep_PosChange();

	UFUNCTION()
	void OnRep_RotChange();
};
