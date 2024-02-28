// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ShowQuestionMark.h"
#include "APAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/APCharacterAIInterface.h"

UBTTask_ShowQuestionMark::UBTTask_ShowQuestionMark()
{
	NodeName = TEXT("ShowQuestionMark");
}

EBTNodeResult::Type UBTTask_ShowQuestionMark::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_DOUBTTARGET));
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

	AIPawn->SetAIQuestionMarkEndDelegate(OnMarkFinished);
	AIPawn->ShowQuestionMark();

	return EBTNodeResult::InProgress;
}
