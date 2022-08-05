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

	UPROPERTY(VisibleAnywhere,replicated, BluePrintReadOnly)
	class UDamageComponent* HealthThing; 

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = Particles)
	class UParticleSystem* ExplosionParticles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AController* Owner;
	UPROPERTY(replicated)
	int PosX;
	UPROPERTY(replicated)
	int PosY;
	UPROPERTY(replicated)
	int Puissance;
};
