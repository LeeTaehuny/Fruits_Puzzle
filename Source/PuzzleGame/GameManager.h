// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameManager();

public:
	FORCEINLINE class UTextureManager* GetTextureManager() { return TextureManager; }
	FORCEINLINE class UCommandInvoker* GetCommandManager() { return CommandManager; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTextureManager> TextureManager;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCommandInvoker> CommandManager;
};
