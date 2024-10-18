// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void Init(int32 Score, int32 Count);
	void UpdateScore(int32 Num);
	void UpdateCount(int32 Num);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Count;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Score;
};
