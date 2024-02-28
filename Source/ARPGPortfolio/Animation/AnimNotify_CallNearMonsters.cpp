// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_CallNearMonsters.h"
#include "Interface/APCharacterAIInterface.h"

void UAnimNotify_CallNearMonsters::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPCharacterAIInterface* NPC = Cast<IAPCharacterAIInterface>(MeshComp->GetOwner());
		if (NPC)
		{
			NPC->CallNearMonsters();
		}
	}
}
