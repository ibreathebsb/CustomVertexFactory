#include "TestVertexFactory.h"
#include "EngineGlobals.h"
#include "RenderResource.h"
#include "VertexFactory.h"
#include "LocalVertexFactory.h"
#include "MeshBatch.h"
#include "RenderCommandFence.h"
#include "MaterialDomain.h"
#include "MeshDrawShaderBindings.h"
#include "MeshMaterialShader.h"

bool FTestVertexFactory::ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters)
{

	//if (Parameters.MaterialParameters.MaterialDomain == MD_UI)
	//{
	//	return !!WITH_EDITOR;
	//}

	return true;
}

void FTestVertexFactory::ModifyCompilationEnvironment(const FVertexFactoryShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
{
	// OutEnvironment.SetDefine(TEXT("TEST_VERTEX"), 1);

}

void FTestVertexFactory::InitRHI()
{
	FVertexDeclarationElementList Elements;
	Elements.Add(AccessStreamComponent(FVertexStreamComponent(&VertexBuffer, STRUCT_OFFSET(FTestData, Position), sizeof(FTestData), VET_Float3), 0));
	Elements.Add(AccessStreamComponent(FVertexStreamComponent(&VertexBuffer, STRUCT_OFFSET(FTestData, Normal), sizeof(FTestData), VET_Float3), 1));
	Elements.Add(AccessStreamComponent(FVertexStreamComponent(&VertexBuffer, STRUCT_OFFSET(FTestData, Color), sizeof(FTestData), VET_Float4), 2));
	InitDeclaration(Elements);
	VertexBuffer.InitResource();
	IndexBuffer.InitResource();
}

void FTestVertexFactory::Initialize(TArray<FTestData> D, TArray<uint32> Indices)
{
	VertexBuffer.Data = D;
	VertexBuffer.NumPoints = D.Num();
	IndexBuffer.Indices = Indices;
}

void FTestVertexFactory::ReleaseRHI()
{
	VertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();

}



class FTestVertexFactoryShaderParameters : public FVertexFactoryShaderParameters
{
	DECLARE_TYPE_LAYOUT(FTestVertexFactoryShaderParameters, NonVirtual);
public:
	void Bind(const FShaderParameterMap& ParameterMap)
	{
		TArray<FString> Out;
		ParameterMap.GetAllParameterNames(Out);
		MyValue.Bind(ParameterMap, TEXT("MyValue"), SPF_Optional);
	};

	void GetElementShaderBindings(
		const class FSceneInterface* Scene,
		const FSceneView* View,
		const class FMeshMaterialShader* Shader,
		const EVertexInputStreamType InputStreamType,
		ERHIFeatureLevel::Type FeatureLevel,
		const FVertexFactory* VertexFactory,
		const FMeshBatchElement& BatchElement,
		class FMeshDrawSingleShaderBindings& ShaderBindings,
		FVertexInputStreamArray& VertexStreams
	) const
	{
		const FTestVertexFactory* Factory = ((FTestVertexFactory*)VertexFactory);
		float Value = Factory->MyValue;

		//UE_LOG(LogTemp, Display, TEXT("Bind ??? %d"),MyValue.IsBound());
		ShaderBindings.Add(MyValue, 0.3);
	};
	LAYOUT_FIELD(FShaderParameter, MyValue);

};


IMPLEMENT_TYPE_LAYOUT(FTestVertexFactoryShaderParameters);
IMPLEMENT_VERTEX_FACTORY_PARAMETER_TYPE(FTestVertexFactory, SF_Vertex, FTestVertexFactoryShaderParameters);



IMPLEMENT_VERTEX_FACTORY_TYPE(FTestVertexFactory, "/CustomShaders/ShaderTest.ush",
	EVertexFactoryFlags::UsedWithMaterials
	//| EVertexFactoryFlags::SupportsStaticLighting
	//| EVertexFactoryFlags::SupportsDynamicLighting
	//| EVertexFactoryFlags::SupportsPrecisePrevWorldPos
	| EVertexFactoryFlags::SupportsPositionOnly
	//| EVertexFactoryFlags::SupportsCachingMeshDrawCommands
	//| EVertexFactoryFlags::SupportsPrimitiveIdStream
	//| EVertexFactoryFlags::SupportsRayTracing
	//| EVertexFactoryFlags::SupportsRayTracingDynamicGeometry
	//| EVertexFactoryFlags::SupportsLightmapBaking
	//| EVertexFactoryFlags::SupportsManualVertexFetch
	//| EVertexFactoryFlags::SupportsPSOPrecaching
	//| EVertexFactoryFlags::SupportsGPUSkinPassThrough
	//| EVertexFactoryFlags::SupportsLumenMeshCards
);


void FTestVertexBuffer::InitRHI()
{
	const uint32 BufferSize = NumPoints * sizeof(FTestData);
	FRHIResourceCreateInfo CreateInfo(TEXT("FTestVertexBuffer"));
	VertexBufferRHI = RHICreateBuffer(BufferSize, BUF_Static | BUF_VertexBuffer, sizeof(FTestData), ERHIAccess::VertexOrIndexBuffer, CreateInfo);
	void* Buffer = RHILockBuffer(VertexBufferRHI, 0, BufferSize, RLM_WriteOnly);
	FMemory::Memcpy(Buffer, Data.GetData(), BufferSize);
	RHIUnlockBuffer(VertexBufferRHI);
}

void FTestIndexBuffer::InitRHI()
{
	const uint32 BufferSize = Indices.Num() * sizeof(uint32);
	FRHIResourceCreateInfo CreateInfo(TEXT("FTestIndexBuffer"));
	IndexBufferRHI = RHICreateBuffer(BufferSize, BUF_Static | BUF_IndexBuffer, sizeof(uint32), ERHIAccess::VertexOrIndexBuffer, CreateInfo);
	void* Buffer = RHILockBuffer(IndexBufferRHI, 0, BufferSize, RLM_WriteOnly);
	FMemory::Memcpy(Buffer, Indices.GetData(), BufferSize);
	RHIUnlockBuffer(IndexBufferRHI);
}