// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class BOMBERMANMULTI_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = BreakableBlock, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;


	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UMaterialInterface* FirstMat;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UMaterialInterface* SecondMat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere , Category = "Components")
	TSubclassOf<class ABrick> UnbreakabkeBrick;

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class ABrick> Brick;

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class ABomb> BombClass;

	UPROPERTY(replicated)
	bool bAsBomb = false;

	class ABomb* Bomb;

	UFUNCTION()
	void DelegatedRemoveHealth(class UDamageComponent* DamageComp, int Damage = 1);

	UPROPERTY(EditAnywhere, Category = Particles)
	class UParticleSystem* ExplosionParticles;


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ExplosinCall();
	bool Server_ExplosinCall_Validate();
	void Server_ExplosinCall_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Explode();
	bool Multi_Explode_Validate();
	void Multi_Explode_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, replicated , Category = "Components")
	int MatType = 0;

	UPROPERTY(VisibleAnywhere, replicated , Category = "Components")
	int32 TileType = 0;

	UPROPERTY(VisibleAnywhere, replicated, Category = "Components")
	int32 PosX;

	UPROPERTY(VisibleAnywhere, replicated, Category = "Components")
	int32 PosY;

	void SpawnBomb(AController* OwnerController, int Puissance = 1);

	bool AsBomb();

	class AMapGenerator* MapGenerator;

	void Explode();

};
