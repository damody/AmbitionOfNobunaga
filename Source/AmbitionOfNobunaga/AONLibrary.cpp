// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "AONLibrary.h"


float UAONLibrary::GetPropertyFloat(TSubclassOf<UObject> ObjectType, FString Properyname)
{
	for (TFieldIterator<UFloatProperty> Property(ObjectType); Property; ++Property)
	{
		if (Property->GetFName().GetPlainNameString() == Properyname)
		{
			return Property->GetPropertyValue_InContainer(ObjectType->GetDefaultObject());
// 			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta,
// 				Property->GetFName().GetPlainNameString() + FString::Printf(L":  %.1f", value));
		}
	}
	return 0;
}

int32 UAONLibrary::GetPropertyInt(TSubclassOf<UObject> ObjectType, FString Properyname)
{
	for (TFieldIterator<UIntProperty> Property(ObjectType); Property; ++Property)
	{
		if (Property->GetFName().GetPlainNameString() == Properyname)
		{
			return Property->GetPropertyValue_InContainer(ObjectType->GetDefaultObject());
		}
	}
	return 0;
}

FString UAONLibrary::GetPropertyString(TSubclassOf<UObject> ObjectType, FString Properyname)
{
	for (TFieldIterator<UStrProperty> Property(ObjectType); Property; ++Property)
	{
		if (Property->GetFName().GetPlainNameString() == Properyname)
		{
			return Property->GetPropertyValue_InContainer(ObjectType->GetDefaultObject());
		}
	}
	return FString();
}

UMapObject* UAONLibrary::GetAllProperty(TSubclassOf<UObject> ObjectType)
{
	UMapObject* res = UMapObject::NewMapObject();
	for (TFieldIterator<UFloatProperty> Property(ObjectType); Property; ++Property)
	{
		res->SetFloat(Property->GetFName().GetPlainNameString(),
		              Property->GetPropertyValue_InContainer(ObjectType->GetDefaultObject()));
	}
	for (TFieldIterator<UIntProperty> Property(ObjectType); Property; ++Property)
	{
		res->SetInt(Property->GetFName().GetPlainNameString(),
		            Property->GetPropertyValue_InContainer(ObjectType->GetDefaultObject()));
	}
	for (TFieldIterator<UStrProperty> Property(ObjectType); Property; ++Property)
	{
		res->SetString(Property->GetFName().GetPlainNameString(),
		               Property->GetPropertyValue_InContainer(ObjectType->GetDefaultObject()));
	}


	for (TFieldIterator<UProperty> TexPropertyit(ObjectType); TexPropertyit; ++TexPropertyit)
	{
		UProperty* TexProperty = *TexPropertyit;
		UTexture2D* tex = TexProperty->ContainerPtrToValuePtr<UTexture2D>(ObjectType->GetDefaultObject());
		if (tex)
		{
			res->SetTexture2D(TexProperty->GetFName().GetPlainNameString(), tex);
		}

	}
	return res;
}
