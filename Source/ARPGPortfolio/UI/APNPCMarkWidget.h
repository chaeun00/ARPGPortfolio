// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/APUserWidget.h"
#include "APNPCMarkWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API UAPNPCMarkWidget : public UAPUserWidget
{
	GENERATED_BODY()

public:
	UAPNPCMarkWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void SetQuestionMarkVisible();
	void SetQuestionMarkHidden();
	void SetExclamationMarkVisible();
	void SetExclamationMarkHidden();

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> QuestionMark;

	UPROPERTY()
	TObjectPtr<class UTextBlock> ExclamationMark;
};