// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class BOMBERMANMULTI_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class ATile> TileClass;
	
	TArray<TArray<class ATile*>> TilesArray;

	void GenerateMap();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TriggerExplosion(int X, int Y, int Puissance, char Dir);

private:
	const int TileSize = 100;
	const int32 BoardSizeL = 13;
	const int32 BoardSizeW = 11;
	const FString File  = FPaths::ProjectConfigDir().Append(TEXT("../Source/Map.txt"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString FileContent;
};
