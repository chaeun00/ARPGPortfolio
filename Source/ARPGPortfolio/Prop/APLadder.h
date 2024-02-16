// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APLadder.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAPLadder, Log, All)

UCLASS()
class ARPGPORTFOLIO_API AAPLadder : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPLadder();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	TObjectPtr<class UBoxComponent> BoxComponent;

};
