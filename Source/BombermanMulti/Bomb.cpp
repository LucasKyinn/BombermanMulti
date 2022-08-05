// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "BombermanCharacter.h"
#include "DamageComponent.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;

	RootScene = CreateDefaultSubobject<USceneComponent>("RootComponent");
	if (!ensure(RootScene != nullptr)) return;
	RootComponent = RootScene;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	if (!ensure(MeshComp != nullptr)) return;
	MeshComp->SetupAttachment(RootComponent);

	HealthThing = CreateDefaultSubobject<UDamageComponent>("DamageComponent");
	Owner = nullptr;
}


// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthThing->IsDead()) {
		if (!HasAuthority()) Server_OnDeath();
		else Multi_OnDeath();

		Destroy();
	}
}

bool ABomb::Server_OnDeath_Validate()
{
	return true;
}

void ABomb::Server_OnDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Imple called"));
	Multi_OnDeath();
}

bool ABomb::Multi_OnDeath_Validate()
{
	return true;
}

void ABomb::Multi_OnDeath_Implementation()
{
	//Effects
	GEngine->AddOnScreenDebugMessage(0, 5.0, FColor::Red, TEXT("Bomb Died"));
	if (ExplosionSound != nullptr && ExplosionParticles != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), 1.0f, 1.f, 0.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
	}
}

