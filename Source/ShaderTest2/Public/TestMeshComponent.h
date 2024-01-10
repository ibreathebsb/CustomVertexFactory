// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "TestVertexFactory.h"
#include "TestMeshComponent.generated.h"

class UTestSceneProxy;




UCLASS()
class SHADERTEST2_API UTestMeshComponent : public UMeshComponent
{
	GENERATED_BODY()

public:

	UTestMeshComponent();
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	UMaterialInterface* CustomMaterial;


};

class UTestSceneProxy : public FPrimitiveSceneProxy
{
public:

	FTestVertexFactory* VertexFactory;

	FMaterialRelevance MaterialRelevance;

	UTestMeshComponent* Component;

	SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	virtual uint32 GetMemoryFootprint(void) const
	{
		return(sizeof(*this) + GetAllocatedSize());
	}

	uint32 GetAllocatedSize(void) const
	{
		return(FPrimitiveSceneProxy::GetAllocatedSize());
	}

	UTestSceneProxy(UTestMeshComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
		, Component(Component)
	{
		bVerifyUsedMaterials = false;

		VertexFactory = new FTestVertexFactory(GetScene().GetFeatureLevel());
		TArray<FTestData> Points = { {
			FTestData {
				FVector3f(0,0,0),
				FVector3f(0,0,1),
				FVector4f(1,0,0,1)
			},
			FTestData {
				FVector3f(0,100,0),
				FVector3f(0,0,1),
				FVector4f(0,1,0,1)
			},
			FTestData {
				FVector3f(100,0,0),
				FVector3f(0,0,1),
				FVector4f(0,0,1,1)
			},
		} };
		TArray<uint32> Indices = { 0, 1, 2 };
		VertexFactory->Initialize(Points, Indices);
		BeginInitResource(VertexFactory);
	}


	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = false;
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = false;
		Result.bRenderCustomDepth = false;
		Result.bTranslucentSelfShadow = false;

		// 透明渲染相关
		Result.bOpaque = false;
		Result.bNormalTranslucency = true;
		//Result.bSeparateTranslucency = true;

		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		Result.bVelocityRelevance = IsMovable() && Result.bOpaque && Result.bRenderInMainPass;
		return Result;
	}


	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex)) {
				FMeshBatch& MeshBatch = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = MeshBatch.Elements[0];
				MeshBatch.Type = PT_TriangleList;
				MeshBatch.LODIndex = 0;
				MeshBatch.VertexFactory = VertexFactory; // 顶点数据
				MeshBatch.bWireframe = false;
				MeshBatch.MaterialRenderProxy = Component->GetMaterial(0)->GetRenderProxy();
				MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative();
				MeshBatch.DepthPriorityGroup = SDPG_World;
				MeshBatch.bCanApplyViewModeOverrides = false;
				MeshBatch.CastShadow = false;
				MeshBatch.CastRayTracedShadow = false;
				BatchElement.IndexBuffer = &(VertexFactory->IndexBuffer); // 索引数据
				BatchElement.FirstIndex = 0;

				//设定UniformBuffer
				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, false, false, false);
				BatchElement.PrimitiveIdMode = PrimID_FromPrimitiveSceneInfo;
				BatchElement.PrimitiveUniformBuffer = DynamicPrimitiveUniformBuffer.UniformBuffer.GetUniformBufferRef();
				//设定索引的范围：
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexFactory->VertexBuffer.NumPoints - 1;
				BatchElement.NumPrimitives = VertexFactory->VertexBuffer.NumPoints / 3;
				// 实例化渲染
				BatchElement.NumInstances = 100;
				Collector.AddMesh(ViewIndex, MeshBatch);
			}
		}
	}
};

