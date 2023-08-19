// Copyright (c) 2023 Matthew Blaire All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ToonTanksGameMode.generated.h"
/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	void ActorDied(AActor* DeadActor);
	void RegisterTower(class ATower* NewTower);

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:
	class ATank* Tank;
	void RemoveTower(class ATower* Tower);
	TArray<class ATower*> TowerList;
	class AToonTanksPlayerController* ToonTanksPlayerController;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "True"))
	TSubclassOf<AActor> TowerClass;
	float StartDelay = 4.f;

	void HandleGameStart();

	int32 GetNumTowersInWorld();
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="True"))
	int32 TargetTowers = 0;

	

};
