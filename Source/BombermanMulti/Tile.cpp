// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Kismet/GameplayStatics.h"

#include "Brick.h"
#include <Components/BoxComponent.h>


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!ensure(RootScene != nullptr)) return;
	RootComponent = RootScene;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(false);
	if (!ensure(MeshComp != nullptr)) return;
	MeshComp->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
	if (!ensure(CollisionBox != nullptr)) return;
	CollisionBox->SetupAttachment(MeshComp);

	TileType = 0;
	MatType = 0;
}

ATile::ATile(int OTileType,int OMatType) : ATile()
{
	MatType = OMatType;
	TileType = OTileType;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	if (TileType != 0) {
		CollisionBox->Deactivate();

		if (UnbreakabkeBrick != nullptr && Brick !=nullptr) {
			UWorld* World = GetWorld();
			if (World != nullptr) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				FRotator Rotator;
				if (TileType == 1) {
					FVector Location = GetActorLocation();

					World->SpawnActor<ABrick>(UnbreakabkeBrick, Location, Rotator, SpawnParams);
				}
				if (TileType == 2) {
					FVector Location = GetActorLocation()+FVector(0.f,0.f,20.f);

					World->SpawnActor<ABrick>(Brick, Location, Rotator, SpawnParams);
				}
			}
		}
	}
	if (FirstMat != nullptr && SecondMat != nullptr) {
		if (MatType == 0) MeshComp->SetMaterial(0, FirstMat);
		if (MatType == 1) MeshComp->SetMaterial(0, SecondMat);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

