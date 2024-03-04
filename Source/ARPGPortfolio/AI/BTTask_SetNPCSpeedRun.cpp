// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetNPCSpeedRun.h"
#include "AIController.h"
#include "Interface/APCharacterAIInterface.h"

UBTTask_SetNPCSpeedRun::UBTTask_SetNPCSpeedRun()
{
	NodeName = TEXT("SetRun");
}

EBTNodeResult::Type UBTTask_SetNPCSpeedRun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IAPCharacterAIInterface* AIPawn = Cast<IAPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->SetGroundSpeedRun();

	return EBTNodeResult::Succeeded;
}
