// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_QuestionMarkEnd.h"
#include "Interface/APCharacterAIInterface.h"

void UAnimNotify_QuestionMarkEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAPCharacterAIInterface* APAI = Cast<IAPCharacterAIInterface>(MeshComp->GetOwner());
		if (APAI)
		{
			APAI->HideQuestionMark();
		}
	}
}
