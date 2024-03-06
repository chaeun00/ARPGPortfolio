// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/APArrow.h"
#include "Components/StaticMeshComponent.h"
#include "FX/APEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/APProjectileHitInterface.h"
#include "Interface/APSpawnFXInterface.h"

AAPArrow::AAPArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMeshMeshComponent"));
	ArrowMesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 90, 0)));
	ArrowMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ArrowMesh->SetSimulatePhysics(true);
	ArrowMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ArrowMesh->CanCharacterStepUpOn = ECB_No;
	ArrowMesh->SetGenerateOverlapEvents(true);
	RootComponent = ArrowMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HB_MedievalWeapPack3/Models/StaticMesh/HB_Arrow.HB_Arrow'"));
	if (ArrowMeshRef.Object)
	{
		ArrowMesh->SetStaticMesh(ArrowMeshRef.Object);
	}

	TrailFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ArrowTrailFX"));
	TrailFX->SetupAttachment(ArrowMesh);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailFXRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/ARPGPortfolio/WeaponTrail/FX_ArrowTrail.FX_ArrowTrail'"));
	if (TrailFXRef.Object)
	{
		TrailFX->SetAsset(TrailFXRef.Object);
	}
}

void AAPArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isPaused)
	{
		SetActorRotation(GetVelocity().ToOrientationQuat());

		CollisionTrace();
	}
}

void AAPArrow::CollisionTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetVelocity().ToOrientationQuat().Vector() * 10;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Attacker);
	FHitResult HitResult;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 5, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
	{
		if (HitResult.GetActor()->Tags.Find(TEXT("Player")) != INDEX_NONE)
		{
			CastChecked<IAPProjectileHitInterface>(HitResult.GetActor())->HitProjectile(false, DamageAmount);
			CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::ArrowHit, GetActorLocation(), GetActorRotation());
		}
		else
		{
			if (HitResult.GetActor()->Tags.Find(TEXT("Enemy")) != INDEX_NONE)
			{
				UE_LOG(LogTemp, Log, TEXT("Enemy!"));

				int32 CritRate = FMath::RandRange(1, 10);
				if (CritRate > 3)
				{
					CastChecked<IAPProjectileHitInterface>(HitResult.GetActor())->HitProjectile(false, DamageAmount);
					CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::ArrowHit, GetActorLocation(), GetActorRotation());
				}
				else
				{
					CastChecked<IAPProjectileHitInterface>(HitResult.GetActor())->HitProjectile(true, DamageAmount * 2);
					CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::ArrowHit_Critical, GetActorLocation(), GetActorRotation());
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Background!"));
				CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::Dust, GetActorLocation(), GetActorRotation());
				isPaused = true;
				return;
			}
		}

		Destroy();
	}
}

void AAPArrow::OnReleased(AActor* InAttacker, FVector InStartLocation, FVector InForwardVector, int32 InDamage)
{
	Attacker = InAttacker;
	DamageAmount = InDamage;

	SetActorLocation(InStartLocation);

	ArrowMesh->AddImpulse(InForwardVector * 3000);
	TrailFX->Activate();
}

