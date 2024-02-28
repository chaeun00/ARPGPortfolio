// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APNPCMarkWidget.h"
#include "Components/TextBlock.h"
#include "Interface/APCharacterWidgetInterface.h"

UAPNPCMarkWidget::UAPNPCMarkWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UAPNPCMarkWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuestionMark = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtQuestionMark")));
	ensure(QuestionMark);

	ExclamationMark = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtExclamationMark")));
	ensure(ExclamationMark);

	QuestionMark->SetVisibility(ESlateVisibility::Hidden);
	ExclamationMark->SetVisibility(ESlateVisibility::Hidden);

	IAPCharacterWidgetInterface* CharacterWidget = Cast<IAPCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UAPNPCMarkWidget::SetQuestionMarkVisible()
{
	QuestionMark->SetVisibility(ESlateVisibility::Visible);
}

void UAPNPCMarkWidget::SetQuestionMarkHidden()
{
	QuestionMark->SetVisibility(ESlateVisibility::Hidden);
}

void UAPNPCMarkWidget::SetExclamationMarkVisible()
{
	ExclamationMark->SetVisibility(ESlateVisibility::Visible);
}

void UAPNPCMarkWidget::SetExclamationMarkHidden()
{
	ExclamationMark->SetVisibility(ESlateVisibility::Hidden);
}