// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ShowExclamationMark.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UBTTask_ShowExclamationMark : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ShowExclamationMark();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
