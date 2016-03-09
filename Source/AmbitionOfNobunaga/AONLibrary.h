// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MapObject.h"
#include "AONLibrary.generated.h"


/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API UAONLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	static float GetPropertyFloat(TSubclassOf<UObject> EquipmentType, FString Properyname);
	
	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	static int32 GetPropertyInt(TSubclassOf<UObject> EquipmentType, FString Properyname);

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	static FString GetPropertyString(TSubclassOf<UObject> EquipmentType, FString Properyname);

	UFUNCTION(Category = "ObjectLibrary", BlueprintCallable)
	static UMapObject* GetAllProperty(TSubclassOf<UObject> ObjectType);
};
