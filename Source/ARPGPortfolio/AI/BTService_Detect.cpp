#include "AI/BTService_Detect.h"
#include "APAI.h"
#include "AIController.h"
#include "Interface/APCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/APCollision.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IAPCharacterAIInterface* AIPawn = Cast<IAPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	// Detect Section
	float DetectRadius = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET) == nullptr ? AIPawn->GetAIDetectRange() : AIPawn->GetAIChaseRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_APACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	// Doubt Section
	DetectRadius = AIPawn->GetAIDoubtRange();
	TArray<FOverlapResult> DoubtOverlapResults;
	bool bDoubtResult = World->OverlapMultiByChannel(
		DoubtOverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_APACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bDoubtResult)
	{
		for (auto const& DoubtOverlapResult : DoubtOverlapResults)
		{
			APawn* Pawn = Cast<APawn>(DoubtOverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_DOUBTTARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_DOUBTTARGET, nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_SHOWNEXCLAMATIONMARK, false);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}