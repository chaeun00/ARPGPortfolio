// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheckEnd.h"
#include "Interface/APAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheckEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPAnimationAttackInterface* Attacker = Cast<IAPAnimationAttackInterface>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->WeaponCollisionOff();
		}
	}
}
