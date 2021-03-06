// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"

#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		FireRateTimerHandle,
		this,
		&APawnTurret::CheckFireCondition,
		FireRate,
		true,
		false
	);

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}

void APawnTurret::CheckFireCondition()
{
	// If player == null, bail
	if (!PlayerPawn || !PlayerPawn->IsAlive())
	{
		return;
	}
	// If player is in range, fire
	if (GetDistanceToPlayer() <= FireRange)
	{
		// Fire
		Fire();
	}
}

float APawnTurret::GetDistanceToPlayer() const
{
	if (!PlayerPawn)
	{
		return 0.f;
	}

	const float Distance = (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
	return Distance;
}

APawnTurret::APawnTurret()
{
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || !PlayerPawn->IsAlive() || GetDistanceToPlayer() > FireRange)
	{
		return;
	}

	RotateTurret(PlayerPawn->GetActorLocation());
}
