// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanCharacter.h"
#include "Bomb.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABombermanCharacter::ABombermanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABombermanCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 240.f;


}

// Called every frame
void ABombermanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABombermanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &ABombermanCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ABombermanCharacter::MoveRight);

	PlayerInputComponent->BindAction("PlaceBomb", IE_Pressed, this, &ABombermanCharacter::Bomb);

}

void ABombermanCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr && Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABombermanCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr && Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABombermanCharacter::Bomb()
{
	UAnimInstance* AnimeInstance = GetMesh()->GetAnimInstance();
	if (BombClass != nullptr && AnimeInstance != nullptr && BombPlacementAnim != nullptr && TileClass != nullptr) {
		AnimeInstance->Montage_Play(BombPlacementAnim,1.0f);
		
		// Nearest Tile
		TSet<AActor*> OverlappingActors;
		float NearsetDistance = 300.f;
		AActor* NearestActor = this; //Risqué mais dans le contexte on est perma en contacte avec une Tile

		GetOverlappingActors(OverlappingActors, TileClass);
		for (AActor* Actor : OverlappingActors) {
			float ActorDist = Actor->GetDistanceTo(this);
			if (NearsetDistance > ActorDist) {
				NearsetDistance = ActorDist;
				NearestActor = Actor;
			}
		}
		UWorld* World = GetWorld();
		if (World != nullptr) {
			FTransform BombSpawnTransform = NearestActor->GetTransform();
			BombSpawnTransform.SetLocation(NearestActor->GetActorLocation() + FVector(0.f,0.f,45.f));
			ABomb* SpawnedBomb = World->SpawnActorDeferred<ABomb>(BombClass, BombSpawnTransform, this);
			APlayerController* MyController = Cast<APlayerController>(GetController());
			if (MyController != nullptr) SpawnedBomb->Owner = this; //Inutile car dans SpawnACtorDeffered ?
			UGameplayStatics::FinishSpawningActor(SpawnedBomb, BombSpawnTransform);
		}
	}
}