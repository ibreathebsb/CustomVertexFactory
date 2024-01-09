// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShaderTest2.h"
//#include "Interfaces/IPluginManager.h"
#include "Logging/LogMacros.h"
#include "Misc/Paths.h" // include this!

void FShaderTest2Module::StartupModule()
{
	FString ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("CustomShaders"));
	AddShaderSourceDirectoryMapping("/CustomShaders", ShaderDirectory);
}

void FShaderTest2Module::ShutdownModule() {}

IMPLEMENT_PRIMARY_GAME_MODULE(FShaderTest2Module, ShaderTest2, "ShaderTest2");