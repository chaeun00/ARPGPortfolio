// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "APCharacterBase.generated.h"
#define	GRAVITYSCALE_DEFAULT		1.6f
#define MAX_SPEED_DASH				900.0f
#define MAX_SPEED_WALK				300.0f
#define MAX_ACCELERATION_DASH		3000
#define MAX_ACCELERATION_WALK		1500
#define MAX_CLIMBING_SPEED_DASH		200.0f
#define MAX_CLIMBING_SPEED_DEFAULT	100.0f

DECLARE_LOG_CATEGORY_EXTERN(LogAPCharacterBase, Log, All)

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Max
};

UCLASS()
class ARPGPORTFOLIO_API AAPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAPCharacterBase();

protected:
	virtual void SetCharacterControlData(const class UAPCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UAPCharacterControlData*> CharacterControlManager;
};
