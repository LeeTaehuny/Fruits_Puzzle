// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "GameManager.h"
#include "TextureManager.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ATile::ATile()
{
 	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetCollisionProfileName(TEXT("Tile"));
	SetRootComponent(BoxComp);


	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TileMesh->SetupAttachment(BoxComp);

	bIsMove = false;
	MoveSpeed = 200.0f;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMove)
	{
		FVector MoveDir = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + MoveDir * MoveSpeed * DeltaTime;

		// 거리 체크
		if (FVector::Distance(NewLocation, TargetLocation) <= 10.0f)
		{
			SetActorLocation(TargetLocation);
			OnMoveEndDelegate.Execute(TileIndex);
			bIsMove = false;
		}
		else
		{
			SetActorLocation(NewLocation);
		}
	}
}

void ATile::Init(ETileType Type, int32 Index)
{
	// 타일의 타입 설정
	TileType = Type;
	// 타일의 인덱스 설정
	TileIndex = Index;

	// 타입에 따른 이미지 설정
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());
	if (GM && GM->GetTextureManager())
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(TileMesh->GetMaterial(0), this);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("Texture")), GM->GetTextureManager()->GetTileTexture(TileType));
		}

		TileMesh->SetMaterial(0, DynamicMaterial);
	}
}

void ATile::SetTargetLocation(FVector Location)
{
	TargetLocation = Location;
	bIsMove = true;
}

