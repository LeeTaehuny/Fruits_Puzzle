// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "TileGrid.h"
#include "PuzzlePlayerController.h"
#include "HUDWidget.h"
#include "EndingWidget.h"

AMainGameMode::AMainGameMode()
{
	PlayerControllerClass = APuzzlePlayerController::StaticClass();

	ClickNum = 30;
}

void AMainGameMode::ClickTile(int32 Index)
{
	if (TileGrid)
	{
		TileGrid->Select(Index);
	}
}

void AMainGameMode::EndGame()
{
	if (EndingWidget)
	{
		EndingWidget->SetScore(Points);
		EndingWidget->AddToViewport();
	}
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	ATileGrid* Grid = GetWorld()->SpawnActor<ATileGrid>(TileGridClass);
	if (Grid)
	{
		TileGrid = Grid;
		TileGrid->Init(Height, Width, TileSize);
		TileGrid->OnClickSuccessed.BindUObject(this, &AMainGameMode::DecreaseClickNum);
		TileGrid->OnAddPoint.BindUObject(this, &AMainGameMode::AddPoints);
	}

	if (HudWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HudWidgetClass);
		if (HUDWidget)
		{
			OnChangedCount.BindUObject(HUDWidget, &UHUDWidget::UpdateCount);
			OnChangedScore.BindUObject(HUDWidget, &UHUDWidget::UpdateScore);

			HUDWidget->AddToViewport();
			HUDWidget->Init(Points, ClickNum);
		}
	}

	if (EndingWidgetClass)
	{
		EndingWidget = CreateWidget<UEndingWidget>(GetWorld(), EndingWidgetClass);
	}
}

void AMainGameMode::DecreaseClickNum()
{
	ClickNum--;
	OnChangedCount.Execute(ClickNum);

	if (ClickNum <= 0)
	{
		EndGame();
		return;
	}
}

void AMainGameMode::AddPoints(int32 Point)
{
	Points += Point;
	OnChangedScore.Execute(Points);
}
