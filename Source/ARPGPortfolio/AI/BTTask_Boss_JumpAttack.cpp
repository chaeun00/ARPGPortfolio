// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Boss_JumpAttack.h"
#include "APAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/APCharacterAIInterface.h"

UBTTask_Boss_JumpAttack::UBTTask_Boss_JumpAttack()
{
	NodeName = TEXT("JumpAttack");

	CurrentDuration = 0;
	Cooltime = 10;
}

EBTNodeResult::Type UBTTask_Boss_JumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	AIPawn->SetGroundSpeedRush();
	
	Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->SetValueAsBool(BBKEY_JUMPATTACKABLE, false);
	GetWorld()->GetTimerManager().SetTimer(JumpAttackTimer, this, &UBTTask_Boss_JumpAttack::CooltimeProcess, FApp::GetDeltaTime(), true);

	AIPawn->JumpAttackByAI();
	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_JumpAttack::CooltimeProcess()
{
	CurrentDuration += FApp::GetDeltaTime();

	if (CurrentDuration <= 1.5f)
	{
		BossPawn->SetActorLocation(FMath::Lerp<FVector>(BossPawn->GetActorLocation(), CastChecked<APawn>(Blackboard->GetValueAsObject(BBKEY_TARGET))->GetActorLocation(), CurrentDuration / 1.5f));
	}

	if (CurrentDuration >= Cooltime)
	{
		Blackboard->SetValueAsBool(BBKEY_JUMPATTACKABLE, true);
		CurrentDuration = 0;
		GetWorld()->GetTimerManager().ClearTimer(JumpAttackTimer);
	}
}
