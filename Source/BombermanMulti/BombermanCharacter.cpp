// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanCharacter.h"
#include "Bomb.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "DamageComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABombermanCharacter::ABombermanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthThing = CreateDefaultSubobject<UDamageComponent>("DamageComponent");

}

void ABombermanCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombermanCharacter, HealthThing);
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
	
	if (!GetWorldTimerManager().IsTimerActive(BombCooldown)) {
		if (BombClass != nullptr  && BombPlacementAnim != nullptr && TileClass != nullptr) {
			GetCharacterMovement()->Deactivate();

			//Bomb placement Animation  
			// DELAY CLIENT ->SERVER 
			//CLIENT -> CLIENT ET SERVER -> CLIENT FONCTIONNE
			GetMesh()->GetAnimInstance()->Montage_Play(BombPlacementAnim, 1.0f);
			if (HasAuthority()) {
				Multi_BombAnimation();
			}
			else{
				Server_BombAnimation();
			}

			// Finding Nearest Tile
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

			FTimerDelegate TimerDel;

			FTimerHandle TimerHandle;

			TimerDel.BindUFunction(this, FName("SpawnBomb"), NearestActor);
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, .5f, false);

		}
		GetWorldTimerManager().SetTimer(BombCooldown,2.f,false);
	}
	else {
		if (OnCDSound != nullptr ) {
			UGameplayStatics::PlaySoundAtLocation(this, OnCDSound, GetActorLocation(), 1.0f, 1.f, 0.f);
		}
	}
}

int ABombermanCharacter::GetRemainingHealth()
{
	return HealthThing->GetHealth();
}

void ABombermanCharacter::SpawnBomb(AActor* NearestActor)
{
	if (NearestActor == this) {
		UE_LOG(LogTemp, Warning, TEXT("No Tiles near"));
		GetCharacterMovement()->Activate();
		return;
	}
	ATile* NearestTile = Cast<ATile>(NearestActor);
	if (NearestTile != nullptr) {
		if (NearestTile->AsBomb()) {
			GetCharacterMovement()->Activate();
			return;
		}


		NearestTile->SpawnBomb(GetController()); //Spawn for Localy Controlled

		if (!GetWorld()->IsServer()) {
			//On Client
			Server_SpawnBomb(NearestTile);
		}
		else {
			//ON Server
			Multi_SpawnBomb(NearestTile);
		}
	}
	GetCharacterMovement()->Activate();
}


//Replication 


//Bomb
bool ABombermanCharacter::Server_SpawnBomb_Validate(ATile* ClosestTile)
{
	//Any test ?
	return true;
}

void ABombermanCharacter::Server_SpawnBomb_Implementation(ATile* ClosestTile)
{

	Multi_SpawnBomb(ClosestTile); 
}

bool ABombermanCharacter::Multi_SpawnBomb_Validate(ATile* ClosestTile)
{
	return true;
}

void ABombermanCharacter::Multi_SpawnBomb_Implementation(ATile* ClosestTile)
{
	if(!IsLocallyControlled()) ClosestTile->SpawnBomb(GetController()); 
}

//Animation 
bool ABombermanCharacter::Server_BombAnimation_Validate()
{
	return true;
}

void ABombermanCharacter::Server_BombAnimation_Implementation()
{
	Multi_BombAnimation();
}

bool ABombermanCharacter::Multi_BombAnimation_Validate()
{
	return true;
}

void ABombermanCharacter::Multi_BombAnimation_Implementation()
{
	UAnimInstance* AnimeInstance = GetMesh()->GetAnimInstance();
	if(!IsLocallyControlled()) AnimeInstance->Montage_Play(BombPlacementAnim, 1.0f);
}


