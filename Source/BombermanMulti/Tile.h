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

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	int MatType=0 ;

	UPROPERTY(EditAnywhere, Category = "Components")
	int TileType = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
