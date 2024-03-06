#pragma once
#include "CoreMinimal.h"
#include "APEffect.generated.h"

UENUM(BlueprintType)
enum class EFXType : uint8
{
	Melee_Blade = 0,
	Melee_Blade_Critical,
	Melee_Spear,
	Melee_Spear_Critical,
	ArrowHit,
	ArrowHit_Critical,
	ShieldParryHit,
	Melee_Boss,
	Melee_Boss_Critical,
	Dust
};