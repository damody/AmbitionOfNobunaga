// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DA_RTSPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FDA_RTSModule"

void FDA_RTSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDA_RTSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
DEFINE_LOG_CATEGORY(DA_RTS_Log)

IMPLEMENT_MODULE(FDA_RTSModule, DA_RTS)