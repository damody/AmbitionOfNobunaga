// Fill out your copyright notice in the Description page of Project Settings.

#include "DA_RTS_EditorPrivatePCH.h"
#include "HeroBuffFactory.h"


UHeroBuffFactory::UHeroBuffFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UHeroBuff::StaticClass();
}

UObject* UHeroBuffFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UHeroBuff* NewObjectAsset = NewObject<UHeroBuff>(InParent, Class, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}
