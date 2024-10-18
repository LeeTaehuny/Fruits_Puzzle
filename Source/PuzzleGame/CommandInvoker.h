// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommandInvoker.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UCommandInvoker : public UObject
{
	GENERATED_BODY()
	
public:
	// ����� ���� �� �����ϱ� ���� �Լ�
	void ExecuteCommand(class IICommand* Command);
	// ������ ����� ����ϱ� ���� �Լ�
	void UndoCommand();
	// ������ ����� �����ϱ� ���� �Լ�
	void DestroyCommand();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class IICommand*> CommandHistory;
};
