// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ParasailEnd.h"
#include "Interface/APParasailInterface.h"

void UAnimNotify_ParasailEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPParasailInterface* ParasailingPawn = Cast<IAPParasailInterface>(MeshComp->GetOwner());
		if (ParasailingPawn)
		{
			ParasailingPawn->EndParasail();
		}
	}
}
