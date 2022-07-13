// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}




bool UDamageComponent::RemoveHealth(int Damage)
{
	Health -= Damage;
	if (Damagetaken != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, Damagetaken, GetOwner()->GetActorLocation(), 1.0f, 1.f, 0.f);
	}

	return IsDead();
}

bool UDamageComponent::IsDead()
{
	return Health <= 0;
}

int UDamageComponent::GetHealth()
{
	return Health;
}

