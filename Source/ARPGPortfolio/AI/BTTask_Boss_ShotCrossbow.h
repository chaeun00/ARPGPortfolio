// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Boss_ShotCrossbow.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UBTTask_Boss_ShotCrossbow : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Boss_ShotCrossbow();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	void CooltimeProcess();

	UBlackboardComponent* Blackboard;
	FTimerHandle LaunchTimer;
	float Cooltime;
	float CurrentDuration;

};
