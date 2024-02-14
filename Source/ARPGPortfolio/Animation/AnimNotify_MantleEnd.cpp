// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MantleEnd.h"
#include "Interface/APMantleInterface.h"

void UAnimNotify_MantleEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPMantleInterface* MantlePawn = Cast<IAPMantleInterface>(MeshComp->GetOwner());
		if (MantlePawn)
		{
			MantlePawn->MantleSystem(EMantleType::NoMantle);
		}
	}
}
