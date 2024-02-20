// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ChargeAttackCheck.h"
#include "Interface/APChargeAttackInterface.h"

void UAnimNotify_ChargeAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPChargeAttackInterface* Attacker = Cast<IAPChargeAttackInterface>(MeshComp->GetOwner());
		if (Attacker)
		{
			Attacker->ChargeStart();
		}
	}
}
