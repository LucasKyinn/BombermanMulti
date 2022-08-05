// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Brick.h"
#include "Net/UnrealNetwork.h"
#include "Bomb.h"
#include "DamageComponent.h"
#include "MapGenerator.h"
#include "BombermanCharacter.h"
#include <Components/BoxComponent.h>


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;


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

}

void ATile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATile, TileType);
	DOREPLIFETIME(ATile, MatType);
	DOREPLIFETIME(ATile, PosX);
	DOREPLIFETIME(ATile, PosY); 
	DOREPLIFETIME(ATile, bAsBomb);
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	if (TileType != 0) {
		if (UnbreakabkeBrick != nullptr && Brick !=nullptr) {
			UWorld* World = GetWorld();
			if (World != nullptr) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				FRotator Rotator;
				if (TileType == 1) {
					CollisionBox->Deactivate();
					FVector Location = GetActorLocation();

					World->SpawnActor<ABrick>(UnbreakabkeBrick, Location, Rotator, SpawnParams);
				}
				if (TileType == 2) {
					FVector Location = GetActorLocation() + FVector(0.f, 0.f, 20.f);
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

void ATile::DelegatedRemoveHealth(UDamageComponent* DamageComp, int Damage)
{
	bAsBomb = false;
	DamageComp->RemoveHealth(Damage);
	MapGenerator->TriggerExplosion(PosX, PosY, 2, 'A');
}



void ATile::SpawnBomb(AController* OwnerController, int Puissance)
{
	//	APlayerController* MyController = Cast<APlayerController>(GetController()); le cast pour plus tard smiley face

	UWorld* World = GetWorld();
	if (World != nullptr && BombClass != nullptr && bAsBomb == false) {
		FTransform BombSpawnTransform = GetTransform();
		BombSpawnTransform.SetLocation(GetActorLocation() + FVector(0.f, 0.f, 45.f));
		ABomb* SpawnedBomb = World->SpawnActorDeferred<ABomb>(BombClass, BombSpawnTransform, OwnerController);
		SpawnedBomb->Owner = OwnerController;
		SpawnedBomb->PosX = PosX;
		SpawnedBomb->PosY = PosY;
		SpawnedBomb->Puissance = Puissance;

		UGameplayStatics::FinishSpawningActor(SpawnedBomb, BombSpawnTransform);
		bAsBomb = true;
		Bomb = SpawnedBomb; 

		//Start Timer For bAsBomb To explode ;

		UActorComponent* Comp = SpawnedBomb->GetComponentByClass(UDamageComponent::StaticClass());
		if (Comp != nullptr) {
			UDamageComponent* BombDamageComp = Cast<UDamageComponent>(Comp);
			if (BombDamageComp != nullptr) {
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("DelegatedRemoveHealth"), BombDamageComp , 1);

				GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 2.5f, false);

			}
		}
	}
}

bool ATile::AsBomb()
{
	return bAsBomb;
}

void ATile::Explode()
{
	if (TileType != 1) { //Tiles Type 1 are indestructible
		if (ExplosionParticles != nullptr) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
		}
		if (TileType == 2) { // Have a Brick on it
			//Ugly as F
			TSet<AActor*> OverlappingActors;
			CollisionBox->GetOverlappingActors(OverlappingActors, ABrick::StaticClass());
			for (AActor* Actor : OverlappingActors) {
				ABrick* Char = Cast<ABrick>(Actor);
				if (Char != nullptr) {
					UDamageComponent* CharDamageComp = Cast<UDamageComponent>(Char->GetComponentByClass(UDamageComponent::StaticClass()));
					if (CharDamageComp != nullptr) {
						CharDamageComp->RemoveHealth(1);
					}
				}
			}
			TileType = 0;
		}
		//Deal damage to ppl on it
		TSet<AActor*> OverlappingActors;
		CollisionBox->GetOverlappingActors(OverlappingActors, ABombermanCharacter::StaticClass());
		for (AActor* Actor : OverlappingActors) {
			ABombermanCharacter* Char = Cast<ABombermanCharacter>(Actor);
			if (Char != nullptr) {
				UDamageComponent* CharDamageComp = Cast<UDamageComponent>(Char->GetComponentByClass(UDamageComponent::StaticClass()));
				if (CharDamageComp != nullptr) {
					CharDamageComp->RemoveHealth(1);
				}
			}
		}
	}

}

