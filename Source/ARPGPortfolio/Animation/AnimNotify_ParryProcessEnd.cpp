// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ParryProcessEnd.h"
#include "Interface/APParryAttackEndInterface.h"

void UAnimNotify_ParryProcessEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPParryAttackEndInterface* Attacker = Cast<IAPParryAttackEndInterface>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->EndParry(MeshComp->GetOwner());
		}
	}
}
