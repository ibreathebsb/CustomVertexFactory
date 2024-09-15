// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShaderExperiment.h"
//#include "Interfaces/IPluginManager.h"
#include "Logging/LogMacros.h"
#include "Misc/Paths.h" // include this!

void FShaderExperimentModule::StartupModule()
{
	FString ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping("/Shaders", ShaderDirectory);
}

void FShaderExperimentModule::ShutdownModule() {}

IMPLEMENT_PRIMARY_GAME_MODULE(FShaderExperimentModule, ShaderExperiment, "ShaderExperiment");