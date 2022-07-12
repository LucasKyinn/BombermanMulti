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

	UPROPERTY(VisibleAnywhere , Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = BreakableBlock, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	int Timer;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AController* Owner;
	int PosX;
	int PosY;
	int Puissance;
};
