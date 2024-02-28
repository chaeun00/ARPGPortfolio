// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ShowExclamationMark.h"
#include "APAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/APCharacterAIInterface.h"

UBTTask_ShowExclamationMark::UBTTask_ShowExclamationMark()
{
	NodeName = TEXT("ShowExclamationMark");
}

EBTNodeResult::Type UBTTask_ShowExclamationMark::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IAPCharacterAIInterface* AIPawn = Cast<IAPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAIMarkFinished OnMarkFinished;
	OnMarkFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIExclamationMarkEndDelegate(OnMarkFinished);
	AIPawn->ShowExclamationMark();

	return EBTNodeResult::InProgress;
}