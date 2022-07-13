// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	//TilesArray.Init(TArray<TSubclassOf<class ATile>>, BoardSizeW); nonono
	//File exist (Copy pasted from the internet)
	if (FileManager.FileExists(*File))
	{
		if (FFileHelper::LoadFileToString(FileContent, *File, FFileHelper::EHashOptions::None))
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Text From File: %s"), *FileContent);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *File);
	}
	if (!ensure(TileClass != nullptr)) return;

	GenerateMap();
}

void AMapGenerator::GenerateMap()
{
	int PosX = 0, PosY = 0 , Mat = 0 , Poof = 0;
	FVector SpawnLocation(-600.f,-500.f,0.f);
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TArray<ATile*> Temp; 

	for (int i = 0; i < BoardSizeW; i++) 
	{
		Temp.Empty();
		for (int j = 0; j < BoardSizeL; j++) 
		{
			//Spawn Transform
			FTransform TileSpawnTransform ;
			TileSpawnTransform.SetLocation(SpawnLocation);

			//SetUp Tile
			ATile* SpawnedTile = GetWorld()->SpawnActorDeferred<ATile>(TileClass, TileSpawnTransform, this);
			SpawnedTile->PosX = PosX; 
			SpawnedTile->PosY = PosY;
			SpawnedTile->TileType = FileContent[Poof]-48; //Ugly but it works cant make TCString::Atoi work so whatever 
			SpawnedTile->MatType = Mat;

			UE_LOG(LogTemp, Warning, TEXT("(%d,%d) Res: %c"), PosX, PosY, FileContent[Poof]);

			UGameplayStatics::FinishSpawningActor(SpawnedTile, TileSpawnTransform);

			//Add to Array :
			Temp.Add(SpawnedTile);

			//Evolution :
			SpawnLocation.X += 100;
			PosX += 1;
			Mat = (Mat + 1) % 2;
			Poof++;

		}
		//Evolution
		PosX = 0;
		PosY += 1;
		SpawnLocation.X = -600;
		SpawnLocation.Y += 100;
		TilesArray.Add(Temp);
	}

}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

