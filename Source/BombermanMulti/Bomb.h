// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class BOMBERMANMULTI_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	class UDamageComponent* HealthThing; 

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = Particles)
	class UParticleSystem* ExplosionParticles;

	//Replication VFX + Sound Better with OnRep ?
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnDeath();
	bool Server_OnDeath_Validate();
	void Server_OnDeath_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Multi_OnDeath();
	bool Multi_OnDeath_Validate();
	void Multi_OnDeath_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AController* Owner;

	int PosX;
	int PosY;
	int Puissance;
};
