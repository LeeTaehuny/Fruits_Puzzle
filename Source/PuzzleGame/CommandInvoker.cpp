// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandInvoker.h"
#include "ICommand.h"

void UCommandInvoker::ExecuteCommand(IICommand* Command)
{
	// ���� �� ����
	Command->Execute();
	CommandHistory.Push(Command);
}

void UCommandInvoker::UndoCommand()
{
	if (CommandHistory.Num() > 0)
	{
		// �ǵ����� �� ����
		IICommand* Command = CommandHistory.Top();
		Command->Undo();

		CommandHistory.Pop();
	}
}

void UCommandInvoker::DestroyCommand()
{
	if (CommandHistory.Num() > 0)
	{
		CommandHistory.Pop();
	}
}