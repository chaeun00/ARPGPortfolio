// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_FireArrow.h"
#include "Interface/APBowInterface.h"

void UAnimNotify_FireArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPBowInterface* NPC = Cast<IAPBowInterface>(MeshComp->GetOwner());
		if (NPC)
		{
			NPC->FireArrow();
		}
	}
}
