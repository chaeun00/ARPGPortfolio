// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/APGameInterface.h"
#include "APGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAPGameMode, Log, All)

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPGameMode : public AGameModeBase, public IAPGameInterface
{
	GENERATED_BODY()
	
public:
	AAPGameMode();

	virtual void OnPlayerDead() override;
};
