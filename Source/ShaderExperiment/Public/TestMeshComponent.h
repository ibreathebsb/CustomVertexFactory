// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "TestVertexFactory.h"
#include "TestMeshComponent.generated.h"

class UTestSceneProxy;




UCLASS()
class SHADEREXPERIMENT_API UTestMeshComponent : public UMeshComponent
{
	GENERATED_BODY()

public:

	UTestMeshComponent();
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	UMaterialInterface* CustomMaterial;
	// �Բ� �����ȻӰ����Ⱦ�ˣ�
	virtual int32 GetNumMaterials() const override { return 1; }
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
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
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
				MeshBatch.VertexFactory = VertexFactory; // ��������
				MeshBatch.bWireframe = false;
				MeshBatch.MaterialRenderProxy = Component->GetMaterial(0)->GetRenderProxy();
				MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative();
				MeshBatch.DepthPriorityGroup = SDPG_World;
				MeshBatch.bCanApplyViewModeOverrides = false;
				MeshBatch.CastShadow = false;
				MeshBatch.CastRayTracedShadow = false;
				BatchElement.IndexBuffer = &(VertexFactory->IndexBuffer); // ��������
				BatchElement.FirstIndex = 0;

				//�趨UniformBuffer
				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, false, false, false);
				BatchElement.PrimitiveIdMode = PrimID_FromPrimitiveSceneInfo;
				BatchElement.PrimitiveUniformBuffer = DynamicPrimitiveUniformBuffer.UniformBuffer.GetUniformBufferRef();
				//�趨�����ķ�Χ��
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexFactory->VertexBuffer.NumPoints - 1;
				BatchElement.NumPrimitives = VertexFactory->VertexBuffer.NumPoints / 3;
				// ʵ������Ⱦ
				BatchElement.NumInstances = 1;
				Collector.AddMesh(ViewIndex, MeshBatch);
			}
		}
	}
};

