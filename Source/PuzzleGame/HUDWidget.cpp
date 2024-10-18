// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Components/TextBlock.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDWidget::Init(int32 Score, int32 Count)
{
	TXT_Score->SetText(FText::AsNumber(Score));
	TXT_Count->SetText(FText::AsNumber(Count));
}

void UHUDWidget::UpdateScore(int32 Num)
{
	TXT_Score->SetText(FText::AsNumber(Num));
}

void UHUDWidget::UpdateCount(int32 Num)
{
	TXT_Count->SetText(FText::AsNumber(Num));
}
