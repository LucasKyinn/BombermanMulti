// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include "Net/UnrealNetwork.h"
#include "DamageComponent.h"


// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!ensure(RootScene != nullptr)) return;
	RootComponent = RootScene;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	if (!ensure(MeshComp != nullptr)) return;
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetupAttachment(RootComponent);

	HealthThing = CreateDefaultSubobject<UDamageComponent>("DamageComponent");

}

void ABrick::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABrick, HealthThing);
}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HealthThing->IsDead()) {
		Destroy();
	}
}

