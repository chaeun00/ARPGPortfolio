// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/APPlayerController.h"

void AAPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// When game is started, cursor is hided
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
