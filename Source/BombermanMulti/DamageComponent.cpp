// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"

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

