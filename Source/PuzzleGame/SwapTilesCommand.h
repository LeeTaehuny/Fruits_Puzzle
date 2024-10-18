// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ICommand.h"
#include "SwapTilesCommand.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API USwapTilesCommand : public UObject, public IICommand
{
	GENERATED_BODY()

public:
	USwapTilesCommand();
	
	void Init(class ATileGrid* InGrid, int32 InFirstIndex, int32 InSecondIndex);

	virtual void Execute() override;
	virtual void Undo() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ATileGrid> Grid;

	int32 FirstIndex;
	int32 SecondIndex;
};
