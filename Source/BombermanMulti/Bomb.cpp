// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "BombermanCharacter.h"
#include "DamageComponent.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
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

void ABomb::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABomb, PosX);
	DOREPLIFETIME(ABomb, PosY);
	DOREPLIFETIME(ABomb, Puissance);
	DOREPLIFETIME(ABomb, HealthThing);
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
		//GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("DEAD"));

		//Effects
		if (ExplosionSound != nullptr && ExplosionParticles!=nullptr) {
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), 1.0f, 1.f, 0.f);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
		}
		Destroy();
	}


}

