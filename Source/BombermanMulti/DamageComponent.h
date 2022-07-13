// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBERMANMULTI_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	int Health = 1; 

	bool bIsDead = false;


	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* Damagetaken;

public:	

	//Remove X HP and return if dead
	bool RemoveHealth(int Damage = 1); 

	bool IsDead();

	int GetHealth();
		
};
