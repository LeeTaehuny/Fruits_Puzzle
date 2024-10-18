// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTilesCommand.h"
#include "TileGrid.h"

USwapTilesCommand::USwapTilesCommand()
{
	Grid = nullptr;
	FirstIndex = -1;
	SecondIndex = -1;
}

void USwapTilesCommand::Init(ATileGrid* InGrid, int32 InFirstIndex, int32 InSecondIndex)
{
	Grid = InGrid;
	FirstIndex = InFirstIndex;
	SecondIndex = InSecondIndex;
}

void USwapTilesCommand::Execute()
{
	if (Grid)
	{
		Grid->SwapTile(FirstIndex, SecondIndex);
		Grid->SetMove(true);
	}
}

void USwapTilesCommand::Undo()
{
	if (Grid)
	{
		Grid->SwapTile(SecondIndex, FirstIndex);
		Grid->SetMove(false);
	}
}