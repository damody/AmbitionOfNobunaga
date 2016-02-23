// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DA_RTS_EditorPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FDA_RTS_EditorModule"

void FDA_RTS_EditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDA_RTS_EditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDA_RTS_EditorModule, DA_RTS_Editor)