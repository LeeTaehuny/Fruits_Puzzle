// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureManager.h"

UTextureManager::UTextureManager()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>AppleRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Textures/fruits/Apple.Apple'"));
	if (AppleRef.Object)
	{
		TextureMap.Add({ ETileType::TT_Apple, AppleRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>BananaRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Textures/fruits/Banana.Banana'"));
	if (BananaRef.Object)
	{
		TextureMap.Add({ ETileType::TT_Banana, BananaRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>PineAppleRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Textures/fruits/pineApple.pineApple'"));
	if (PineAppleRef.Object)
	{
		TextureMap.Add({ ETileType::TT_PineApple, PineAppleRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>CherryRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Textures/fruits/Cherry.Cherry'"));
	if (CherryRef.Object)
	{
		TextureMap.Add({ ETileType::TT_Cherry, CherryRef.Object });
	}
	
}

UTexture2D* UTextureManager::GetTileTexture(ETileType Type)
{
	if (TextureMap.Find(Type))
	{
		return TextureMap[Type];
	}
	else
	{
		return nullptr;
	}
}
