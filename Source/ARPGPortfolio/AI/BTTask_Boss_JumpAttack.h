// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Boss_JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UBTTask_Boss_JumpAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Boss_JumpAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	void CooltimeProcess();

	APawn* BossPawn;
	UBlackboardComponent* Blackboard;
	FTimerHandle JumpAttackTimer;
	float Cooltime;
	float CurrentDuration;
};
