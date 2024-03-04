// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Boss_ShotCrossbow.h"
#include "APAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/APCharacterAIInterface.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Boss_ShotCrossbow::UBTTask_Boss_ShotCrossbow()
{
	NodeName = TEXT("CrossbowShot");

	CurrentDuration = 0;
	Cooltime = 10;
}

EBTNodeResult::Type UBTTask_Boss_ShotCrossbow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IAPCharacterAIInterface* AIPawn = Cast<IAPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);

	FVector NextPatrolPos = UKismetMathLibrary::GetDirectionUnitVector(FVector::ZeroVector, ControllingPawn->GetActorLocation()) * 5000;
	Blackboard = OwnerComp.GetBlackboardComponent(); 
	Blackboard->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos);

	Blackboard->SetValueAsBool(BBKEY_LAUNCHABLE, false);
	GetWorld()->GetTimerManager().SetTimer(LaunchTimer, this, &UBTTask_Boss_ShotCrossbow::CooltimeProcess, FApp::GetDeltaTime(), true);

	AIPawn->FireByAI();
	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_ShotCrossbow::CooltimeProcess()
{
	CurrentDuration += FApp::GetDeltaTime();

	if (CurrentDuration >= Cooltime)
	{
		Blackboard->SetValueAsBool(BBKEY_LAUNCHABLE, true);
		CurrentDuration = 0;
		GetWorld()->GetTimerManager().ClearTimer(LaunchTimer);
	}
}
