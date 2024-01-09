// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMeshComponent.h"

UTestMeshComponent::UTestMeshComponent()
{
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("Material'/Game/M_Test.M_Test'"));
	//if (Mat.Succeeded())
	//{
	//	CustomMaterial = Mat.Object;
	//	SetMaterial(0, CustomMaterial);
	//}
}

FPrimitiveSceneProxy* UTestMeshComponent::CreateSceneProxy()
{
	return new UTestSceneProxy(this);
}

FBoxSphereBounds UTestMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBox BoundingBox(ForceInit);
	TArray<FVector> Vertices = { {
		FVector(100, 0, 0),
		FVector(0, 100, 0),
		FVector(0, 0, 0),
	} };
	for (const FVector& Vertex : Vertices)
	{
		BoundingBox += Vertex;
	}
	BoundingBox = BoundingBox.TransformBy(LocalToWorld);
	return FBoxSphereBounds(BoundingBox);
}