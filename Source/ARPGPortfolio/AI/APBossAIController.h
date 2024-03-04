// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/APAIController.h"
#include "APBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPBossAIController : public AAPAIController
{
	GENERATED_BODY()
	
public:
	AAPBossAIController();

	virtual void RunAI() override;
};
