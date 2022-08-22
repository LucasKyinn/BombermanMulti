// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermannGameInstance.h"
#include <SocketSubsystem.h>
#include <IPAddress.h>

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

void UBombermannGameInstance::IPAdress()
{
	bool canBind = false;
	TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);


	if (localIp->IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("IP : %s"), *localIp->ToString(false));
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Black, FString::Printf(TEXT("IP :  %s"), *localIp->ToString(false)));
	} 
	else {
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, FString::Printf(TEXT("IP :  ")));
		UE_LOG(LogTemp, Warning, TEXT("IP : None"));

	}
}

void UBombermannGameInstance::Lobby()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	if (World->IsServer()) World->ServerTravel("/Game/Maps/BM_Level_Loby");
}

