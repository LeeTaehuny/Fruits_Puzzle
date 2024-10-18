// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileType.h"
#include "TileGrid.generated.h"

DECLARE_DELEGATE(FOnClickSuccessed);
DECLARE_DELEGATE_OneParam(FOnAddPoint, int32);

UENUM(BlueprintType)
enum class ETileGridState : uint8
{
	TS_Wait,
	TS_Move,
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

	ETileType TileType;
	bool bIsChecked;
	bool bIsEmpty;
};

UCLASS()
class PUZZLEGAME_API ATileGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ATileGrid();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// 타일맵 초기화
	void Init(int32 InHeight, int32 InWidth, float InTileSize);
	// 타일 스폰
	void SpawnTile();
	// 타일 선택
	void Select(int32 Index);
	// 타일 교체
	void SwapTile(int32 Index1, int32 Index2);
	// 타일 이동 종료
	void EndMove(int32 Index);
	// 무빙 세팅
	FORCEINLINE void SetMove(bool Value) { bIsSwap = Value; }

protected:
	// 선택 인덱스 체크
	bool CheckIndex(int32 Index1, int32 Index2);
	// 타일 매칭 체크
	void MatchTile(int32 Index);
	// 타일 움직임 체크
	bool CheckAllMoveStop();
	// 전체 타일 유효 체크
	bool CheckTile();
	// 전체 타일 내리기
	bool MoveDown();
	// 매칭 타일 체크
	void DFS(int32 Index, TArray<int32>& Indecies);
	// 타일 삭제
	void DestroyTile();
	// 전체 타일 매칭 가능 여부 체크
	bool TileMatchable();
	// 타일 업데이트
	void UpdateTile();
	
	// 상태 변경
	void ChangeState(ETileGridState State);
	// 점수 추가
	void AddPoint(int32 Num);

private:
	// 시간 누적
	float SumTime;

	// 추가할 임시 점수
	int32 TempPoints;

public:
	FOnClickSuccessed OnClickSuccessed;
	FOnAddPoint OnAddPoint;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	TArray<FTileData> TileMaps;

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	TArray<TObjectPtr<class ATile>> Tiles;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<class ATile> TileClass;

	int32 Height;
	int32 Width;
	float TileSize;
	bool bIsSwap;

private:
	UPROPERTY(VisibleAnywhere)
	ETileGridState TileGridState;

	int32 SelectIndex = -1;

	int32 dy[4] = { 1, 0, -1, 0 };
	int32 dx[4] = { 0, 1, 0, -1 };

	UPROPERTY(VisibleAnywhere)
	TArray<bool> MovingTiles;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> DestroyIndecies;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> ParticleEffect;
};
