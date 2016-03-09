// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "MapObject.h"

UMapObject* UMapObject::NewMapObject()
{
	return NewObject<UMapObject>();
}

void UMapObject::SetFloat(const FString& name, float value)
{
	FloatMapData.Add(name) = value;
}

void UMapObject::SetInt(const FString& name, int32 value)
{
	IntMapData.Add(name) = value;
}

void UMapObject::SetString(const FString& name, FString value)
{
	StrMapData.Add(name) = value;
}

void UMapObject::SetTexture2D(const FString& name, UTexture2D* value)
{
	Tex2DMapData.Add(name) = value;
}

bool UMapObject::GetFloat(const FString& name, float& value)
{
	float* ans = FloatMapData.Find(name);
	if (ans)
	{
		value = *ans;
		return true;
	}
	return false;
}

bool UMapObject::GetInt(const FString& name, int32& value)
{
	int32* ans = IntMapData.Find(name);
	if (ans)
	{
		value = *ans;
		return true;
	}
	return false;
}

bool UMapObject::GetString(const FString& name, FString& value)
{
	FString* ans = StrMapData.Find(name);
	if (ans)
	{
		value = *ans;
		return true;
	}
	return false;
}

bool UMapObject::GetTexture2D(const FString& name, UTexture2D*& value)
{
	UTexture2D** ans = Tex2DMapData.Find(name);
	if (ans)
	{
		value = *ans;
		return true;
	}
	return false;
}
