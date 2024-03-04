// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterNonPlayer.h"
#include "Interface/APCharacterHUDInterface.h"
#include "APCharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPORTFOLIO_API AAPCharacterBoss : public AAPCharacterNonPlayer, public IAPCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AAPCharacterBoss();

protected:
	virtual void BeginPlay() override;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UAPHUDWidget> BossHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UAPHUDWidget> BossHUDWidget;

	virtual void SetupHpBar() override;
	virtual void SetupHUDWidget(class UAPHUDWidget* InHUDWidget) override;

public:
	virtual void ShowHpBar() override;
	virtual void HideHpBar() override;
};
