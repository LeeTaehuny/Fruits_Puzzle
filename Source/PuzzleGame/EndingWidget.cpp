// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingWidget.h"

#include "Components/TextBlock.h"

void UEndingWidget::SetScore(int32 Num)
{
	TXT_Score->SetText(FText::AsNumber(Num));
}
