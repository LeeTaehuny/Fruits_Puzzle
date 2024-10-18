// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UEndingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetScore(int32 Num);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Score;
};
