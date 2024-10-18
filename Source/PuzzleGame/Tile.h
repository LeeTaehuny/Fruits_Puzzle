// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileType.h"
#include "Tile.generated.h"

DECLARE_DELEGATE_OneParam(FOnMoveEndDelegate, int32);

UCLASS()
class PUZZLEGAME_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FOnMoveEndDelegate OnMoveEndDelegate;

public:
	void Init(ETileType Type, int32 Index);
	void SetTargetLocation(FVector Location);

public:
	FORCEINLINE int32 GetIndex() { return TileIndex; }
	FORCEINLINE void SetIndex(int32 Index) { TileIndex = Index; }
	FORCEINLINE bool GetIsMoving() { return bIsMove; }
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	TObjectPtr<class UStaticMeshComponent> TileMesh;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TObjectPtr<class UTexture2D> TileTest;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float MoveSpeed;

	ETileType TileType;
	FVector TargetLocation;
	bool bIsMove;
	int32 TileIndex;
};
