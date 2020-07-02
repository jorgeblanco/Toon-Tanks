// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"

void ATankGameModeBase::HandleGameStart()
{
    // Call blueprint version of game start
    GameStart();
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    // Call blueprint version of game over
    GameOver(PlayerWon);
}

int32 ATankGameModeBase::GetCountOfTurrets() const
{
    const TSubclassOf<APawnTurret> ClassToFind = APawnTurret::StaticClass();
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, TurretActors);
    return TurretActors.Num();
}

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Get refs and game win/lose conditions
    TargetTurrets = GetCountOfTurrets();

    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    
    // Call HandleGameStart to initialise the start countdown, turret activation, pawn check, etc
    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    // Check what type of actor died, if turret, tally, if player, go to lose condition
    if (DeadActor == PlayerTank)
    {
        PlayerTank->PawnDestroyed();
        HandleGameOver(false);
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->PawnDestroyed();
        TargetTurrets--;
        if (TargetTurrets <= 0)
        {
            HandleGameOver(true);
        }
    }
}
