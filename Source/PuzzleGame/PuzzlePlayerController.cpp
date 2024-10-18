// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlayerController.h"
#include "Tile.h"
#include "MainGameMode.h"

#include "Kismet/GameplayStatics.h"

APuzzlePlayerController::APuzzlePlayerController()
{
}

void APuzzlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);

	bShowMouseCursor = true;
}

void APuzzlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("LeftClick"), IE_Pressed, this, &APuzzlePlayerController::Select);
}

void APuzzlePlayerController::Select()
{
	FHitResult HitResult;
	bool bIsHit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, true, HitResult);
	if (bIsHit)
	{
		ATile* Tile = Cast<ATile>(HitResult.GetActor());
		if (Tile)
		{
			AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GameMode)
			{
				GameMode->ClickTile(Tile->GetIndex());
			}
		}

		
	}
}
