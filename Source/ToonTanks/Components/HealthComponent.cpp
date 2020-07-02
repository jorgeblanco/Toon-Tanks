// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = DefaultHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode not found in %s"), *GetName());
		return;
	}
	Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner not found in %s"), *GetName());
		return;
	}
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser
)
{
	if (Damage == 0 || Health == 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);

	if (Health <= 0.f)
	{
		GameMode->ActorDied(Owner);
	}
}

