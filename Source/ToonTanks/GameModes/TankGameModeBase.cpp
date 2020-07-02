// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

void ATankGameModeBase::HandleGameStart()
{
    // initialise the start countdown, turret activation, pawn check, etc
    // Call blueprint version of game start
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    // See if the player has destroyed all turrets, show win result
    // if turret destroyed player, show lose result
    // Call blueprint version of game over
}

void ATankGameModeBase::BeginPlay()
{
    // Get refs and game win/lose conditions
    // Call HandleGameStart to initialise the start countdown, turret activation, pawn check, etc
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    // Check what type of actor died, if turret, tally, if player, go to lose condition
}
