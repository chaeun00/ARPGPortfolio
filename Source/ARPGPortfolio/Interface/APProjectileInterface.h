// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APProjectileInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAPProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPGPORTFOLIO_API IAPProjectileInterface
{
	GENERATED_BODY()

public:
	virtual void OnReleased(AActor* InAttacker, FVector InStartLocation, FVector InForwardVector, int32 InDamage) = 0;

protected:
	virtual void CollisionTrace() = 0;
};
