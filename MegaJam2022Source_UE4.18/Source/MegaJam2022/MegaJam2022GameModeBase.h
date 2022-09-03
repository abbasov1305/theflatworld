// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MegaJam2022GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MEGAJAM2022_API AMegaJam2022GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	public:

	AMegaJam2022GameModeBase();

	virtual void BeginPlay() override;
	
	
};
