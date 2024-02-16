#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "APCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FAPCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAPCharacterStat() : MaxHp(0.0f), MaxStemina(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxStemina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	FAPCharacterStat operator+(const FAPCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FAPCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FAPCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};
