// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermannGameInstance.h"

void UBombermannGameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Hosting on BM_Level_1"));
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	World->ServerTravel("/Game/Maps/BM_Level_Loby?listen");
}

void UBombermannGameInstance::Join(const FString& Address)
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, FString::Printf(TEXT("Joining %s"), *Address));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UBombermannGameInstance::StartGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	if(World->IsServer()) World->ServerTravel("/Game/Maps/BM_Level_1");
}

