// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	

	//////////////////////// INUTILE MAINTENANT
	if (HasAuthority()) {
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

	

}

void AMapGenerator::GenerateMap()
{
	FString TestMap = "00222222222000121212121210222222222222221212121212122222222222222212121212121222222222222222121212121212222222222222201212121212100022222222200";
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


			////////////////////////////////////// CRASH SI FILE MANIP QUAND PACKAGE
			if (TestMap[Poof] - 48 == 2) {
				int32 RandNum = FMath::RandRange(0, 100);
				if (RandNum <= 80) {
					SpawnedTile->TileType = TestMap[Poof] - 48; //Ugly but it works cant make TCString::Atoi work so whatever 
				}
				else {
					SpawnedTile->TileType = 0;
				}
			}
			else {
				SpawnedTile->TileType = TestMap[Poof] - 48;
			}
			//////////////////////////////////////



			//SpawnedTile->TileType = FileContent[Poof]-48; //Ugly but it works cant make TCString::Atoi work so whatever 

			SpawnedTile->MatType = Mat;
			SpawnedTile->MapGenerator = this;

			//UE_LOG(LogTemp, Warning, TEXT("(%d,%d) Res: %c"), PosX, PosY, FileContent[Poof]);

			UGameplayStatics::FinishSpawningActor(SpawnedTile, TileSpawnTransform);

			//Add to Array :
			Temp.Add(SpawnedTile);

			//Evolution :
			SpawnLocation.X += 100;
			PosY += 1;
			Mat = (Mat + 1) % 2;
			Poof++;

		}
		//Evolution
		PosY = 0;
		PosX += 1;
		SpawnLocation.X = -600;
		SpawnLocation.Y += 100;
		TilesArray.Add(Temp);
	}

}


void AMapGenerator::TriggerExplosion(int X, int Y, int Puissance, char Dir)
{
	//UE_LOG(LogTemp, Warning, TEXT("TriggerExplosion"));

	//l'explosion est suffisament puissante
	if (Puissance >= 0 ) {
		//UE_LOG(LogTemp, Warning, TEXT("Puissance sup 0"));

		//Explosion ne sort pas du tableau
		if (X >= 0 && X < BoardSizeW && Y >= 0 && Y < BoardSizeL && TilesArray[X][Y]->TileType != 1) {
			//UE_LOG(LogTemp, Warning, TEXT("InBound"));

			TilesArray[X][Y]->Explode();
			if (Dir == 'A') {
				//All Direction
				TriggerExplosion(X-1, Y, Puissance - 1, 'U');
				TriggerExplosion(X+1, Y, Puissance - 1, 'D');
				TriggerExplosion(X, Y-1, Puissance - 1, 'L');
				TriggerExplosion(X, Y+1, Puissance - 1, 'R');
			}
			if (Dir == 'U') TriggerExplosion(X - 1, Y, Puissance - 1, 'U'); //Up
			if (Dir == 'D')TriggerExplosion(X + 1, Y, Puissance - 1, 'D');	//Down
			if (Dir == 'L')TriggerExplosion(X, Y - 1, Puissance - 1, 'L');	//Left
			if (Dir == 'R')TriggerExplosion(X, Y + 1, Puissance - 1, 'R');	//Right

		}
	}
}





// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapGenerator::Explosion(int X, int Y, int Puissance, char Dir)
{
	//Called on server 
	//Server_TriggerExplosion(X, Y, Puissance, (uint8) Dir);
	if (HasAuthority()) {
		//just in case to be safe
		TriggerExplosion(X, Y, Puissance, Dir);
	}
}

