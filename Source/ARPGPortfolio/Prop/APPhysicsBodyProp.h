// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/APPhysicsBodyPropInterface.h"
#include "APPhysicsBodyProp.generated.h"

UCLASS()
class ARPGPORTFOLIO_API AAPPhysicsBodyProp : public AActor, public IAPPhysicsBodyPropInterface
{
	GENERATED_BODY()
	
public:	
	AAPPhysicsBodyProp();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FallSweepMontage;

	AController* InstigatorController;
	FVector PrevLocation;
	FTimerHandle CollisionTraceTimerHandle;

	virtual void CollisionTrace();

public:
	virtual void OnCollisionTrace(class AController* InController) override;
};
