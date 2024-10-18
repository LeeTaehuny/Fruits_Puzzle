// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "TextureManager.h"
#include "CommandInvoker.h"

UGameManager::UGameManager()
{
	// �Ŵ��� Ŭ���� ����
	TextureManager = CreateDefaultSubobject<UTextureManager>(TEXT("TextManager"));
	CommandManager = CreateDefaultSubobject<UCommandInvoker>(TEXT("CommandManager"));
}
