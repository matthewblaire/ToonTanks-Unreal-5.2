// Copyright (c) 2023 Matthew Blaire All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ToonTanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetPlayerEnabledState(bool bPlayerEnabled);
	
};
