// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Boss_Backstep.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UBTTask_Boss_Backstep : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Boss_Backstep();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	void CooltimeProcess();
	
	APawn* BossPawn;
	UBlackboardComponent* Blackboard;
	FTimerHandle BackstepTimer;
	float Cooltime;
	float CurrentDuration;
};
