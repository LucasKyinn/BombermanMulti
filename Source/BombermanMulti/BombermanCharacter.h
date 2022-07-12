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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anime, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BombPlacementAnim;

private:
	UFUNCTION()
	void SpawnBomb(AActor* NearestActor);

};
