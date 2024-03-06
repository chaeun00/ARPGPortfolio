// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/APGameInterface.h"
#include "Interface/APSpawnFXInterface.h"
#include "APGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAPGameMode, Log, All)

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPGameMode : public AGameModeBase, public IAPGameInterface, public IAPSpawnFXInterface
{
	GENERATED_BODY()
	
public:
	AAPGameMode();

	virtual void OnPlayerDead() override;

protected:
	virtual void BeginPlay() override;

// EffectPool Section
public:
	FORCEINLINE class AAPEffectPool* GetEffectPool() const { return EffectPool; }
	
	virtual void SpawnFX(EFXType InType, FVector InPosition, FRotator InRotation) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EffectPool)
	TObjectPtr<class AAPEffectPool> EffectPool;
};
