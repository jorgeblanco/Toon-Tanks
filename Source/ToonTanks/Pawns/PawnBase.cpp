// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToonTanks/Actors/ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void APawnBase::PawnDestroyed()
{
	HandleDestruction();
}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// Update the rotation of the turret mesh to face towards the look at target

	const FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(
		StartLocation,
		FVector(LookAtTarget.X, LookAtTarget.Y, StartLocation.Z)
	);

	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
	// Get projectile spawn point location and rotation, then spawn projectile class
	if (!Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("No projectile class defined for %s"), *GetOwner()->GetName());
		return;
	}

	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
		Projectile,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation()
	);
	TempProjectile->SetOwner(this);
}

void APawnBase::HandleDestruction()
{
	// Play death FX
	// Do unique child overrides:
	// PawnTurret - inform gamemode turret died, destroy self
	// PawnTank - inform gamemode player died, hide all components
}
