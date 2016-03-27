// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "DamageEffect.h"
// for GEngine
#include "Engine.h"

FRotator ADamageEffect::FaceDirection;

void ADamageEffect::SetFaceDirection(FRotator face)
{
	FaceDirection = face;
}

// Sets default values
ADamageEffect::ADamageEffect(const FObjectInitializer& ObjectInitializer)
	: Super(FObjectInitializer::Get())
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TextRender = ObjectInitializer.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("TextRender0"));
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Scene0"));
	TextRender->AttachParent = RootComponent;
	Deadline = 3;
	ScaleSize = 5;
}

void ADamageEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TextMaterial = UMaterialInstanceDynamic::Create(TextRender->GetMaterial(0), this);
	TextRender->SetMaterial(0, TextMaterial);
	SetActorRotation(FaceDirection);
	FVector size = TextRender->GetTextLocalSize();
	size *= 0.5;
	size.Z = -size.Z;
	TextRender->SetRelativeLocation(size);
}

// Called when the game starts or when spawned
void ADamageEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	TimeCounting += DeltaTime;
	if (TimeCounting > Deadline)
	{
		this->ConditionalBeginDestroy();
	}
	if (TextMaterial->IsValidLowLevel() && DamageAlpha)
	{
		float alpha = DamageAlpha->GetFloatValue(TimeCounting);
		TextMaterial->SetScalarParameterValue(TEXT("Alpha"), alpha);
	}
	if (OriginPosition != FVector::ZeroVector && DamageHeight)
	{
		FVector pos = OriginPosition;
		pos += FlyDirection * TimeCounting * 100;
		pos.Z += DamageHeight->GetFloatValue(TimeCounting);
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
// 			FString::Printf(TEXT("ADamageEffect: pos.Z:%f"), pos.Z));
		SetActorLocation(pos);
	}
}

void ADamageEffect::SetString(FString message)
{
	TextRender->SetText(message);
	FVector size = TextRender->GetTextWorldSize();
	size *= 0.5;
	size.Z = -size.Z;
	TextRender->SetRelativeLocation(size);
}

void ADamageEffect::SetColor(FColor color)
{
	TextRender->SetTextRenderColor(color);
}


