// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/APJavelin.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/APProjectileHitInterface.h"
#include "Interface/APSpawnFXInterface.h"

AAPJavelin::AAPJavelin()
{
	PrimaryActorTick.bCanEverTick = true;

	JavelinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JavelinMeshComponent"));
	JavelinMesh->SetRelativeScale3D(FVector(1, 1, 2));
	JavelinMesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(90, 90, 0)));
	JavelinMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	JavelinMesh->SetSimulatePhysics(true);
	JavelinMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	JavelinMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	JavelinMesh->CanCharacterStepUpOn = ECB_No;
	JavelinMesh->SetGenerateOverlapEvents(true);
	RootComponent = JavelinMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> JavelinMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Baguette/StaticMesh.StaticMesh'"));
	if (JavelinMeshRef.Object)
	{
		JavelinMesh->SetStaticMesh(JavelinMeshRef.Object);
	}

	TrailFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("JavelintTrailFX"));
	TrailFX->SetupAttachment(JavelinMesh);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailFXRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/ARPGPortfolio/WeaponTrail/FX_SpearTrail.FX_SpearTrail'"));
	if (TrailFXRef.Object)
	{
		TrailFX->SetAsset(TrailFXRef.Object);
	}
}

void AAPJavelin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetVelocity().ToOrientationQuat() * FVector(0, 0, 90).ToOrientationQuat());

	CollisionTrace();
}

void AAPJavelin::CollisionTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetVelocity().ToOrientationQuat().Vector() * 50;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 25, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
	{
		if (HitResult.GetActor()->Tags.Find(TEXT("Player")) != INDEX_NONE)
		{
			return;
		}
		else
		{
			if (HitResult.GetActor()->Tags.Find(TEXT("Enemy")) != INDEX_NONE)
			{
				//UE_LOG(LogTemp, Log, TEXT("Enemy!"));
				CastChecked<IAPProjectileHitInterface>(HitResult.GetActor())->HitProjectile(true, 15);
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("Background!"));
			}

			CastChecked<IAPSpawnFXInterface>(GetWorld()->GetAuthGameMode())->SpawnFX(EFXType::Melee_Spear_Critical, GetActorLocation(), GetActorRotation());
			Destroy();
		}
	}
}

void AAPJavelin::OnReleased(AActor* InAttacker, FVector InStartLocation, FVector InForwardVector, int32 Damage)
{
	SetActorLocation(InStartLocation);
	
	JavelinMesh->AddImpulse(InForwardVector * 2500);
	TrailFX->Activate();
}