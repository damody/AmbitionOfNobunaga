// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Curves/CurveFloat.h"
#include "DamageEffect.generated.h"


UCLASS()
class AMBITIONOFNOBUNAGA_API ADamageEffect : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	void SetString(FString message);

	void SetColor(FColor color);

	void PostInitializeComponents();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* TextRender;

	UFUNCTION(Category = "Damage", BlueprintCallable)
	static void SetFaceDirection(FRotator face);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* DamageHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* DamageAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeCounting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Deadline;

	FVector FlyDirection;
	
	static FRotator FaceDirection;

	FVector OriginPosition;


	UPROPERTY()
	UMaterialInstanceDynamic* TextMaterial;
};
