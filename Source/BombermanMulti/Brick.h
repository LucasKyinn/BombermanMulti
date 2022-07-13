// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.generated.h"

UCLASS()
class BOMBERMANMULTI_API ABrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = BreakableBlock, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	class UDamageComponent* HealthThing;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
