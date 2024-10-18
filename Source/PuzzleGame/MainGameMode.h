// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangedScore, int32);
DECLARE_DELEGATE_OneParam(FOnChangedCount, int32);

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMainGameMode();

public:
	void ClickTile(int32 Index);
	void EndGame();

protected:
	virtual void BeginPlay() override;

protected:
	void DecreaseClickNum();
	void AddPoints(int32 Point);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATileGrid> TileGridClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ATileGrid> TileGrid;

	UPROPERTY(EditAnywhere)
	int32 ClickNum;

	UPROPERTY(VisibleAnywhere)
	int32 Points;

private:
	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 Height;

	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 Width;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float TileSize;

private:
	UPROPERTY(EditAnywhere, Category = "Setting")
	TSubclassOf<class UHUDWidget> HudWidgetClass;

	UPROPERTY()
	TObjectPtr<class UHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, Category = "Setting")
	TSubclassOf<class UEndingWidget> EndingWidgetClass;

	UPROPERTY()
	TObjectPtr<class UEndingWidget> EndingWidget;

	FOnChangedScore OnChangedScore;
	FOnChangedCount OnChangedCount;
};
