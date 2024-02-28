// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_HitAnimEnd.h"
#include "Interface/APAnimationAttackInterface.h"
#include "Interface/APCharacterAIInterface.h"

void UAnimNotify_HitAnimEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPAnimationAttackInterface* Player = MeshComp->GetOwner()->Tags.Find(TEXT("Player")) != INDEX_NONE ? Cast<IAPAnimationAttackInterface>(MeshComp->GetOwner()) : nullptr;
		if (nullptr != Player)
		{
			Player->OffPlayerHit();
		}

		IAPCharacterAIInterface* Enemy = MeshComp->GetOwner()->Tags.Find(TEXT("Enemy")) != INDEX_NONE ? Cast<IAPCharacterAIInterface>(MeshComp->GetOwner()) : nullptr;
		if (nullptr != Enemy)
		{
			Enemy->RunNPCAI();
		}
	}
}
