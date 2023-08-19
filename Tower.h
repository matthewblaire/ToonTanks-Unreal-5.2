// Copyright (c) 2023 Matthew Blaire All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	ATower();
	void HandleDestruction();
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "True"))
		USceneComponent* LineTraceStartPoint;
	class ATank* Tank;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting", meta=(AllowPrivateAccess="True"));
	float FireRange = 200.f;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.f;
	void CheckFireCondition();

	bool TankIsInRange();

};
