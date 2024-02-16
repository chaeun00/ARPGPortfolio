// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APSteminaInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAPSteminaInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPGPORTFOLIO_API IAPSteminaInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RecoveryStemina() = 0;
	virtual void SetExhausted() = 0;
	virtual void StartExhausted() = 0;
	virtual void ExhaustedProcess() = 0;
	virtual void EndExhausted() = 0;
};
