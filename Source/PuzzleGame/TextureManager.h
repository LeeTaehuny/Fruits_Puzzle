// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileType.h"
#include "TextureManager.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UTextureManager : public UObject
{
	GENERATED_BODY()
	
public:
	UTextureManager();

public:
	class UTexture2D* GetTileTexture(ETileType Type);

protected:
	UPROPERTY(EditAnywhere)
	TMap<ETileType, TObjectPtr<class UTexture2D>> TextureMap;
};
