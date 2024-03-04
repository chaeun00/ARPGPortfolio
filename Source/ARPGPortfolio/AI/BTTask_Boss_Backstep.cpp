// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Boss_Backstep.h"
#include "APAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/APCharacterAIInterface.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Boss_Backstep::UBTTask_Boss_Backstep()
{
	NodeName = TEXT("Backstep");

	CurrentDuration = 0;
	Cooltime = 10;
}

EBTNodeResult::Type UBTTask_Boss_Backstep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	BossPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossPawn)
	{
		return EBTNodeResult::Failed;
	}

	IAPCharacterAIInterface* AIPawn = Cast<IAPCharacterAIInterface>(BossPawn);
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

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMPATTACKABLE, false);
	GetWorld()->GetTimerManager().SetTimer(BackstepTimer, this, &UBTTask_Boss_Backstep::CooltimeProcess, FApp::GetDeltaTime(), true);
	AIPawn->BackstepByAI();

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Backstep::CooltimeProcess()
{
	CurrentDuration += FApp::GetDeltaTime();

	if (CurrentDuration <= 1.9f)
	{
		FVector NextDir = UKismetMathLibrary::GetDirectionUnitVector(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), BossPawn->GetActorLocation());
		NextDir.Z = 0;

		FVector NextPos = BossPawn->GetActorLocation() + NextDir * 15;
		BossPawn->SetActorLocation(NextPos);
	}

	if (CurrentDuration >= Cooltime)
	{
		Blackboard->SetValueAsBool(BBKEY_JUMPATTACKABLE, true);
		CurrentDuration = 0;
		GetWorld()->GetTimerManager().ClearTimer(BackstepTimer);
	}
}
