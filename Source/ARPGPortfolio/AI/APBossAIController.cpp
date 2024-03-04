// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/APBossAIController.h"
#include "APAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AAPBossAIController::AAPBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/ARPGPortfolio/AI/BB_APBoss.BB_APBoss'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ARPGPortfolio/AI/BT_APBoss.BT_APBoss'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AAPBossAIController::RunAI()
{
	Super::RunAI();

	Blackboard.Get()->SetValueAsBool(BBKEY_LAUNCHABLE, true);
	Blackboard.Get()->SetValueAsBool(BBKEY_JUMPATTACKABLE, true);
}
