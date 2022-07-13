// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "BombermanCharacter.h"
#include "DamageComponent.h"


// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	if (Owner != nullptr)
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, TEXT("Has Owner"));
	else 
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, TEXT("No Owner Bug !"));
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthThing->IsDead()) {
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("DEAD"));
		Destroy();
	}


}

