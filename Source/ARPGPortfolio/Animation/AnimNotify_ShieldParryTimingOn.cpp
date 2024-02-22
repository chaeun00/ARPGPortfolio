// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShieldParryTimingOn.h"
#include "Interface/APShieldParryInterface.h"

void UAnimNotify_ShieldParryTimingOn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPShieldParryInterface* Pawn = Cast<IAPShieldParryInterface>(MeshComp->GetOwner());
		if (Pawn)
		{
			Pawn->ShieldParryTimingOn();
		}
	}
}
