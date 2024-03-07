// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/APPhysicsBodyProp.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/APSpawnFXInterface.h"
#include "Interface/APGetAnimInstanceInterface.h"

AAPPhysicsBodyProp::AAPPhysicsBodyProp()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(0.1f);
	Mesh->SetAngularDamping(1);
	Mesh->SetEnableGravity(true);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->CanCharacterStepUpOn = ECB_No;
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FallSweepMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_FallSweep.AM_FallSweep'"));
	if (FallSweepMontageRef.Object)
	{
		FallSweepMontage = FallSweepMontageRef.Object;
	}
}

void AAPPhysicsBodyProp::CollisionTrace()
{
	double Dist = FVector::Distance(PrevLocation, GetActorLocation());
	UE_LOG(LogTemp, Log, TEXT("%f"), Dist);
	if (Dist < 0.1f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CollisionTraceTimerHandle);
	}

	PrevLocation = GetActorLocation();

	int32 Damage = Dist / 50;
	if (Damage < 1)
	{
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), 100, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
	{
		if (HitResult.GetActor()->Tags.Find(TEXT("Enemy")) != INDEX_NONE)
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, InstigatorController, this);

			UAnimInstance* AnimInsatnce = CastChecked<IAPGetAnimInstanceInterface>(HitResult.GetActor())->GetActorAnimInstance();
			AnimInsatnce->StopAllMontages(0);
			AnimInsatnce->Montage_Play(FallSweepMontage);

			CastChecked<ACharacter>(HitResult.GetActor())->LaunchCharacter((HitResult.GetActor()->GetActorLocation() - GetActorLocation() + FVector(0, 0, 100)) * Damage, false, false);
			
			CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::Melee_Boss_Critical, HitResult.GetActor()->GetActorLocation(), GetActorRotation());
		}
	}
}

void AAPPhysicsBodyProp::OnCollisionTrace(class AController* InController)
{
	InstigatorController = InController;
	PrevLocation = GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(CollisionTraceTimerHandle, this, &AAPPhysicsBodyProp::CollisionTrace, 0.05f, true);
}
