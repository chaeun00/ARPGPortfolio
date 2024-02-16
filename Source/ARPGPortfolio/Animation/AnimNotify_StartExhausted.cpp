// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_StartExhausted.h"
#include "Interface/APSteminaInterface.h"

void UAnimNotify_StartExhausted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPSteminaInterface* Pawn = Cast<IAPSteminaInterface>(MeshComp->GetOwner());
		if (Pawn)
		{
			Pawn->StartExhausted();
		}
	}
}
