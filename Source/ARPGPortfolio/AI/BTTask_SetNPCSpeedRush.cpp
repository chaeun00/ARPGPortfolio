// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetNPCSpeedRush.h"
#include "AIController.h"
#include "Interface/APCharacterAIInterface.h"

UBTTask_SetNPCSpeedRush::UBTTask_SetNPCSpeedRush()
{
	NodeName = TEXT("SetRush");
}

EBTNodeResult::Type UBTTask_SetNPCSpeedRush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AIPawn->SetGroundSpeedRush();

	return EBTNodeResult::Succeeded;
}
