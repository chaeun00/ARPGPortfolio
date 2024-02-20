// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheckStart.h"
#include "Interface/APAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheckStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPAnimationAttackInterface* Attacker = Cast<IAPAnimationAttackInterface>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->WeaponCollisionOn();
		}
	}
}
