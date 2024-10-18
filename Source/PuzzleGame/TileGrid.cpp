// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"
#include "Tile.h"
#include "GameManager.h"
#include "MainGameMode.h"
#include "SwapTilesCommand.h"
#include "CommandInvoker.h"

#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileGrid::ATileGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	Height = 6;
	Width = 6;
	TileSize = 100.0f;
	bIsSwap = false;

	ConstructorHelpers::FObjectFinder<UParticleSystem> FXRef(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (FXRef.Succeeded())
	{
		ParticleEffect = FXRef.Object;
	}
}

void ATileGrid::BeginPlay()
{
	Super::BeginPlay();
}

void ATileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 이동 중인 타일이 존재하는 경우
	if (TileGridState == ETileGridState::TS_Move)
	{
		SumTime += DeltaTime;

		if (SumTime >= 0.5f)
		{
			SumTime = 0.0f;

			// 타일 움직임 체크
			if (CheckAllMoveStop())
			{
				// 움직이지 않는 경우 업데이트 진행
				UpdateTile();
			}
		}
	}
}

void ATileGrid::Init(int32 InHeight, int32 InWidth, float InTileSize)
{
	Height = InHeight;
	Width = InWidth;
	TileSize = InTileSize;

	Tiles.Init(nullptr, Height * Width);
	MovingTiles.Init(false, Height * Width);

	// 타일맵 초기화
	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			// 랜덤 인덱스 추출
			uint8 RandIdx = FMath::RandRange(0, static_cast<uint8>(ETileType::TT_MaxNum) - 1);

			FTileData Data;
			Data.TileType = static_cast<ETileType>(RandIdx);
			Data.bIsChecked = false;
			Data.bIsEmpty = false;

			// 해당 인덱스의 과일로 초기화
			TileMaps.Add(Data);
		}
	}

	SpawnTile();
	ChangeState(ETileGridState::TS_Move);
}

void ATileGrid::SpawnTile()
{
	// 생성되어있는 모든 타일 순회
	for (int32 i = 0; i < Tiles.Num(); i++)
	{
		// 유효하지 않은 타일인 경우
		if (!IsValid(Tiles[i]))
		{
			if (TileMaps[i].bIsEmpty == true)
			{
				uint8 RandIdx = FMath::RandRange(0, static_cast<uint8>(ETileType::TT_MaxNum) - 1);

				FTileData Data;
				Data.TileType = static_cast<ETileType>(RandIdx);
				Data.bIsChecked = false;
				Data.bIsEmpty = false;

				// 해당 인덱스의 과일로 초기화
				TileMaps[i] = Data;
			}

			// 스폰
			float SpawnX = (i % Width) * TileSize;
			float SpawnY = (Height - i / Height) * TileSize * - 1.0f;
			float LocationY = (i / Height * TileSize);

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(SpawnX - (TileSize * Width / 2), SpawnY, 0.0f));

			class ATile* Tile = GetWorld()->SpawnActorDeferred<ATile>(TileClass, SpawnTransform);
			if (Tile)
			{
				// 타일 초기화
				Tile->SetOwner(this);
				Tile->Init(TileMaps[i].TileType, i);
				// 타일 이동 설정
				Tile->SetTargetLocation(FVector(SpawnX - (TileSize * Width / 2), LocationY, 0.0f));
				Tile->OnMoveEndDelegate.BindUObject(this, &ATileGrid::EndMove);
				MovingTiles[i] = true;
				// 타일 저장
				Tiles[i] = Tile;

				Tile->FinishSpawning(SpawnTransform);
			}
		}
	}
}

void ATileGrid::Select(int32 Index)
{
	if (TileGridState == ETileGridState::TS_Move) return;

	// 인덱스 체크
	if (Tiles.IsValidIndex(Index))
	{
		// 처음 선택된 경우
		if (SelectIndex == -1)
		{
			// 인덱스를 저장합니다.
			SelectIndex = Index;
		}
		// 같은 타일이 선택된 경우
		else if (SelectIndex == Index)
		{
			// 인덱스를 초기화합니다.
			SelectIndex = -1;
		}
		// 다른 타일이 선택된 경우
		else if (CheckIndex(SelectIndex, Index))
		{
			//SwapTile(SelectIndex, Index);

			UGameManager* GM = Cast<UGameManager>(GetGameInstance());
			if (GM)
			{
				USwapTilesCommand* Command = NewObject<USwapTilesCommand>();
				if (Command)
				{
					Command->Init(this, SelectIndex, Index);
					GM->GetCommandManager()->ExecuteCommand(Command);
				}
			}

			// 인덱스 초기화
			SelectIndex = -1;
		}
		else
		{
			// 인덱스를 초기화합니다.
			SelectIndex = -1;
		}
	}
}

void ATileGrid::SwapTile(int32 Index1, int32 Index2)
{
	// 둘 중 하나라도 유효하지 않은 경우 초기화하고 반환
	if (!IsValid(Tiles[Index1]) || !IsValid(Tiles[Index2]))
	{
		return;// false;
	}

	// 이동 위치 체크
	FVector NextLocation = Tiles[Index2]->GetActorLocation();
	FVector CurrentLocation = Tiles[Index1]->GetActorLocation();

	// 이동 목표 설정
	Tiles[Index1]->SetTargetLocation(NextLocation);
	Tiles[Index2]->SetTargetLocation(CurrentLocation);
	Tiles[Index2]->SetIndex(Index1);
	Tiles[Index1]->SetIndex(Index2);

	Tiles.Swap(Index1, Index2);
	TileMaps.Swap(Index1, Index2);

	// 이동 변수 체크
	MovingTiles[Index1] = true;
	MovingTiles[Index2] = true;

	ChangeState(ETileGridState::TS_Move);
}

bool ATileGrid::CheckIndex(int32 Index1, int32 Index2)
{
	// 체크
	if (Index2 < 0 || !TileMaps.IsValidIndex(Index2) || Index1 == -1) return false;

	int32 CurrentY = Index1 / Width;
	int32 NextY = Index2 / Width;

	int32 DisY = FMath::Abs(CurrentY - NextY);

	int32 CurrentX = Index1 % Width;
	int32 NextX = Index2 % Width;

	int32 DisX = FMath::Abs(CurrentX - NextX);

	// y, x값의 차가 1 이상인 경우 false
	if (DisY > 1 || DisX > 1) return false;

	// x, y값의 차의 합이 2 이상인 경우 false
	if (DisY + DisX >= 2) return false;

	return true;
}

void ATileGrid::MatchTile(int32 Index)
{
	// 첫 칸 기준 맵 탐색
	TArray<int32> Indecies;

	// 가로 탐색
	DFS(Index, Indecies);

	// 탐색 종료 시 배열 정렬
	Indecies.Sort();

	TArray<int32> TempHorizontal;
	TArray<int32> TempVertical;
	for (int32 i = 0; i < Indecies.Num(); i++)
	{
		// 현재 값 추가
		TempHorizontal.Add(Indecies[i]);
		int32 TempIndex = 1;
		while (true)
		{
			// 오른쪽 인덱스가 체크되어 있고, y차이가 동일하다면?
			if (Indecies.Find(Indecies[i] + TempIndex) != INDEX_NONE && (Indecies[i] / Width) == ((Indecies[i] + TempIndex) / Width))
			{
				TempHorizontal.Add(Indecies[i] + TempIndex);
				TempIndex++;
			}
			// 오른쪽 인덱스가 체크되어 있지 않다면
			else
			{
				// 만약 이미 추가된 값들이 3개 이상이라면?
				if (TempHorizontal.Num() >= 3)
				{
					// 모두 넣기
					for (int32 temp : TempHorizontal)
					{
						DestroyIndecies.AddUnique(temp);
					}

					AddPoint(TempHorizontal.Num());
				}

				// 임시 저장값 삭제
				TempHorizontal.Reset();
				break;
			}
		}

		// 현재 값 추가
		TempVertical.Add(Indecies[i]);
		TempIndex = 1;
		while (true)
		{
			// 아래 인덱스가 체크되어 있다면?
			if (Indecies.Find(Indecies[i] + (TempIndex * Width)) != INDEX_NONE)
			{
				TempVertical.Add(Indecies[i] + (TempIndex * Width));
				TempIndex++;
			}
			// 아래쪽 인덱스가 체크되어 있지 않다면
			else
			{
				// 만약 이미 추가된 값들이 3개 이상이라면?
				if (TempVertical.Num() >= 3)
				{
					// 모두 넣기
					for (int32 temp : TempVertical)
					{
						DestroyIndecies.AddUnique(temp);
					}

					AddPoint(TempVertical.Num());
				}

				// 임시 저장값 삭제
				TempVertical.Reset();
				break;
			}
		}
	}
}

bool ATileGrid::CheckAllMoveStop()
{
	for (int32 i = 0; i < MovingTiles.Num(); i++)
	{
		// 움직이는 타일이 하나라도 있는 경우 종료
		if (MovingTiles[i])
		{
			return false;
		}
	}

	// 하나도 없는 경우
	return true;
}

void ATileGrid::DestroyTile()
{
	if (DestroyIndecies.Num() == 0) return;

	for (int32 num : DestroyIndecies)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, Tiles[num]->GetActorLocation(), FRotator::ZeroRotator, true);
		MovingTiles[num] = false;
		// 타일 삭제
		Tiles[num]->Destroy();

		TileMaps[num].bIsEmpty = true;
	}

	OnAddPoint.Execute(TempPoints);
	TempPoints = 0;

	// 삭제 리스트 초기화
	DestroyIndecies.Empty();

	return;
}

bool ATileGrid::TileMatchable()
{
	// 상하좌우 체크용 배열
	TArray<int32> Directions = { -Width, 1, Width, -1 };

	for (int32 i = 0; i < Height * Width; i++)
	{
		for (int32 Direction : Directions)
		{
			// 이동 가능한 경우
			if (CheckIndex(i, i + Direction))
			{
				// 체크 초기화
				for (int32 j = 0; j < TileMaps.Num(); j++)
				{
					TileMaps[j].bIsChecked = false;
				}

				// 교체
				TileMaps.Swap(i, i + Direction);
				// 판별
				MatchTile(i + Direction);
				// 원복
				TileMaps.Swap(i + Direction, i);

				if (DestroyIndecies.Num() >= 3)
				{
					DestroyIndecies.Reset();
					return true;
				}
				else
				{
					DestroyIndecies.Reset();
				}
			}
		}
	}

	return false;
}

void ATileGrid::UpdateTile()
{
	// 전체 타일 초기화
	for (int32 i = 0; i < Tiles.Num(); i++)
	{
		TileMaps[i].bIsChecked = false;
	}

	// 전체 타일 매칭
	for (int32 i = 0; i < Tiles.Num(); i++)
	{
		if (IsValid(Tiles[i]) && !TileMaps[i].bIsChecked)
		{
			MatchTile(i);
		}
	}

	// 전체 타일 초기화
	for (int32 i = 0; i < Tiles.Num(); i++)
	{
		TileMaps[i].bIsChecked = false;
	}

	// 제거 목록의 수가 3개 이상인 경우
	if (DestroyIndecies.Num() >= 3)
	{
		if (bIsSwap)
		{
			UGameManager* GameManager = Cast<UGameManager>(GetGameInstance());
			if (GameManager)
			{
				USwapTilesCommand* Command = NewObject<USwapTilesCommand>();
				if (Command)
				{
					GameManager->GetCommandManager()->DestroyCommand();
					bIsSwap = false;
					OnClickSuccessed.Execute();
				}
			}
		}

		// 타일 제거
		DestroyTile();
		// 아래로 타일 내리기
		MoveDown();
	}
	// 제거 목록이 없는 경우
	else
	{
		if (bIsSwap)
		{
			UGameManager* GameManager = Cast<UGameManager>(GetGameInstance());
			if (GameManager)
			{
				USwapTilesCommand* Command = NewObject<USwapTilesCommand>();
				if (Command)
				{
					GameManager->GetCommandManager()->UndoCommand();
					return;
				}
			}
		}

		// 전체 타일 체크
		if (CheckTile())
		{
			// 전체가 가능한지 체크
			if (TileMatchable())
			{
				ChangeState(ETileGridState::TS_Wait);
			}
			// 가능하지 않은 경우 게임 종료
			else
			{
				// 게임 종료
				AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GM)
				{
					GM->EndGame();
				}
			}
		}
		else
		{
			SpawnTile();
		}
	}
}

bool ATileGrid::MoveDown()
{
	bool Result = false;

	// 뒤에서 부터 탐색하며 빈 자리로 이동
	for (int32 i = Height - 2; i >= 0; i--)
	{
		for (int32 j = 0; j < Width; j++)
		{
			// 현재 타일이 없으면 건너뛰기
			if (TileMaps[i * Width + j].bIsEmpty) continue;

			// 아래 칸이 비어있는지 체크
			if (TileMaps[(i + 1) * Width + j].bIsEmpty)
			{
				int32 NextIdx = (i + 1) * Width + j;

				// 가장 아래 칸까지 체크
				while (true)
				{
					// 다음 칸이 불가능하면 종료
					if ((NextIdx / Width) + 1 >= Height) break;

					// 다음 칸도 비어있는지 체크
					if (TileMaps[NextIdx + Width].bIsEmpty)
					{
						// 다음칸 업데이트
						NextIdx += Width;
					}
					else
					{
						break;
					}
				}

				// 현재 칸 데이터 아래 칸으로 이동
				TileMaps.Swap(i * Width + j, NextIdx);
				// 타일도 이동
				// * 이동할 위치
				FVector NewLocation = FVector(Tiles[i * Width + j]->GetActorLocation().X, (NextIdx / Height) * TileSize, 0.0f);
				Tiles[i * Width + j]->SetTargetLocation(NewLocation);
				MovingTiles[i * Width + j] = false;

				// 타일 이동
				Tiles[i * Width + j]->SetIndex(NextIdx);
				Tiles.Swap(i * Width + j, NextIdx);
				MovingTiles[NextIdx] = true;

				Result = true;
			}
		}
	}

	if (Result)
	{
		ChangeState(ETileGridState::TS_Move);
	}

	return Result;
}

void ATileGrid::ChangeState(ETileGridState State)
{
	if (TileGridState != State)
	{
		TileGridState = State;
	}
}

void ATileGrid::AddPoint(int32 Num)
{
	if (Num == 3)
	{
		TempPoints += 10;
	}
	else if (Num == 4)
	{
		TempPoints += 20;
	}
	else
	{
		TempPoints += 10 * Num;
	}
}

void ATileGrid::DFS(int32 Index, TArray<int32>& Indecies)
{
	// 방문 처리
	TileMaps[Index].bIsChecked = true;
	// 배열에 추가
	Indecies.Add(Index);

	for (int32 i = 0; i < 4; i++)
	{
		int32 Ny = (Index / Width) + dy[i];
		int32 Nx = (Index % Width) + dx[i];

		if (Ny < 0 || Ny >= Height) continue;
		if (Nx < 0 || Nx >= Width) continue;

		int32 NewIndex = (Ny * Width) + Nx;

		if (!TileMaps.IsValidIndex(NewIndex)) continue;
		if (TileMaps[NewIndex].bIsChecked) continue;
		if (TileMaps[NewIndex].TileType != TileMaps[Index].TileType) continue;
		if (IsValid(Tiles[NewIndex]))
		{
			DFS(NewIndex, Indecies);
		}
	}
}

void ATileGrid::EndMove(int32 Index)
{
	MovingTiles[Index] = false;
}

bool ATileGrid::CheckTile()
{
	for (int32 i = 0; i < Tiles.Num(); i++)
	{
		if (!IsValid(Tiles[i]))
		{
			return false;
		}
	}

	return true;
}