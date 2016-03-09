// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MapObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class AMBITIONOFNOBUNAGA_API UMapObject : public UObject
{
	GENERATED_BODY()

public:
	// Set value function

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	static UMapObject* NewMapObject();

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	void SetFloat(const FString& name, float value);

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	void SetInt(const FString& name, int32 value);
	
	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	void SetString(const FString& name, FString value);

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	void SetTexture2D(const FString& name, UTexture2D* value);

	// Get value function

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	bool GetFloat(const FString& name, float& value);
	
	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	bool GetInt(const FString& name, int32& value);

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	bool GetString(const FString& name, FString& value);

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	bool GetTexture2D(const FString& name, UTexture2D*& value);

	TMap<FString, float> FloatMapData;
	TMap<FString, int> IntMapData;
	TMap<FString, FString> StrMapData;
	TMap<FString, UTexture2D*> Tex2DMapData;
};
