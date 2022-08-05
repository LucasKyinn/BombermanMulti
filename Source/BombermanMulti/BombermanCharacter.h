// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BombermanCharacter.generated.h"

UCLASS()
class BOMBERMANMULTI_API ABombermanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABombermanCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Bomb();

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class ABomb> BombClass;

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* OnCDSound;

	UPROPERTY(VisibleAnywhere,replicated, BluePrintReadOnly)
	class UDamageComponent* HealthThing;

	//Bomb Replication
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnBomb(class ATile* ClosestTile);
	bool Server_SpawnBomb_Validate(class ATile* ClosestTile);
	void Server_SpawnBomb_Implementation(class ATile* ClosestTile);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_SpawnBomb(class ATile* ClosestTile);
	bool Multi_SpawnBomb_Validate(class ATile* ClosestTile);
	void Multi_SpawnBomb_Implementation(class ATile* ClosestTile);

	//Anim Replication rien de mieux ??
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_BombAnimation();
	bool Server_BombAnimation_Validate();
	void Server_BombAnimation_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_BombAnimation();
	bool Multi_BombAnimation_Validate();
	void Multi_BombAnimation_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anime, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BombPlacementAnim;

	UFUNCTION(BlueprintCallable)
	int GetRemainingHealth();

private:
	UFUNCTION()
	void SpawnBomb(AActor* NearestActor);

	bool bIsInCD= false; 
	FTimerHandle BombCooldown;

};
