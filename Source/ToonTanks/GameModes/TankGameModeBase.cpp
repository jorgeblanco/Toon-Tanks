// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void ATankGameModeBase::HandleGameStart()
{
    // Call blueprint version of game start
    GameStart();
    
    if (PlayerController)
    {
        PlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        const FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(
            PlayerController,
            &APlayerControllerBase::SetPlayerEnabledState,
            true
        );
        GetWorld()->GetTimerManager().SetTimer(
            PlayerEnableHandle,
            PlayerEnableDelegate,
            StartDelay,
            false
        );
    }
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

    PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
    
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

        if (PlayerController)
        {
            PlayerController->SetPlayerEnabledState(false);
        }
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
