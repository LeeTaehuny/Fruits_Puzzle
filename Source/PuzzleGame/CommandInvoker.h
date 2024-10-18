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
	// 명령을 실행 및 저장하기 위한 함수
	void ExecuteCommand(class IICommand* Command);
	// 마지막 명령을 취소하기 위한 함수
	void UndoCommand();
	// 마지막 명령을 삭제하기 위한 함수
	void DestroyCommand();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class IICommand*> CommandHistory;
};
