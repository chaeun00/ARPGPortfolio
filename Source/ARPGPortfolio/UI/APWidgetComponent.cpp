// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APWidgetComponent.h"
#include "APUserWidget.h"

void UAPWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UAPUserWidget* APUserWidget = Cast<UAPUserWidget>(GetWidget());
	if (APUserWidget)
	{
		APUserWidget->SetOwningActor(GetOwner());
	}
}