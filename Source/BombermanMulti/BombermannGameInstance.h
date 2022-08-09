// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BombermannGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANMULTI_API UBombermannGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& Address);
	

	UFUNCTION(Exec)
	void StartGame();

	UFUNCTION(Exec)
	void IPAdress();
};
