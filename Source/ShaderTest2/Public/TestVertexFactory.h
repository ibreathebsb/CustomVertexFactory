#pragma once
#pragma once

#include "CoreMinimal.h"
#include "VertexFactory.h"
#include "Components.h"
#include "PrimitiveViewRelevance.h"
#include "RenderResource.h"
#include "RenderingThread.h"
#include "PrimitiveSceneProxy.h"
#include "Containers/ResourceArray.h"
#include "EngineGlobals.h"
#include "VertexFactory.h"
#include "MaterialShared.h"
#include "Materials/Material.h"
#include "Engine/Engine.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "StaticMeshResources.h"
#include "MeshMaterialShader.h"
#include "ShaderParameters.h"
#include "RHIUtilities.h"

struct FTestData
{
	FVector3f Position;
	FVector3f Normal;
	FVector4f Color;
};



class FTestVertexBuffer : public FVertexBuffer
{
public:
	TArray<FTestData> Data;
	int32 NumPoints;
	virtual void InitRHI() override;
	virtual FString GetFriendlyName() const override { return TEXT("FTestVertexBuffer"); }
};

class FTestIndexBuffer : public FIndexBuffer
{
public:
	TArray<uint32> Indices;
	virtual void InitRHI() override;
	virtual FString GetFriendlyName() const override { return TEXT("FTestIndexBuffer"); }
};

/**
 *
 */
class FTestVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FTestVertexFactory);
public:
	static bool ShouldCache(const FVertexFactoryShaderPermutationParameters& Parameters) { return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5); }
	static bool ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters);
	static void ModifyCompilationEnvironment(const FVertexFactoryShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment);

	FTestVertexFactory(ERHIFeatureLevel::Type InFeatureLevel)
		: FVertexFactory(InFeatureLevel)
	{
	}
	FTestVertexBuffer VertexBuffer;
	FTestIndexBuffer IndexBuffer;
	virtual void InitRHI() override;
	virtual void ReleaseRHI() override;
	void Initialize(TArray<FTestData> Data, TArray<uint32> Indices);

	float MyValue = 1.0;
};
