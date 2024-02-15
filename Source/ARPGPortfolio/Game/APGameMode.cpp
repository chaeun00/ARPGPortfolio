// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/APGameMode.h"

DEFINE_LOG_CATEGORY(LogAPGameMode)

AAPGameMode::AAPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ARPGPortfolio/Blueprints/BP_APPlayer.BP_APPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	else
	{
		UE_LOG(LogAPGameMode, Log, TEXT("Cannot find DefaultPawnClass"));
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Script/ARPGPortfolio.APPlayerController"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
	else
	{
		UE_LOG(LogAPGameMode, Log, TEXT("Cannot find PlayerControllerClass"));
	}
}
