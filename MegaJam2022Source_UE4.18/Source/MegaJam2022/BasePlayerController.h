// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MEGAJAM2022_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ABasePlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnergyAmount = 200;
	UPROPERTY(EditAnywhere, Category = "HUD")
	class UTextBlock* EnergyText;
	UPROPERTY()
	FTimerHandle TextTimer;

	UPROPERTY(EditAnywhere, Category = "HUD")
	FSlateColor DefaultText;
	UPROPERTY(EditAnywhere, Category = "HUD")
	FSlateColor AlarmText;

	UPROPERTY(EditAnywhere)
	float AirAmount = 100.0f;
	UPROPERTY(EditAnywhere)
	float SmokeAmount;
	UPROPERTY(EditAnywhere)
	float AirPercent;
	UPROPERTY()
	class UProgressBar* AirProgressBar;
	UPROPERTY()
	class UTextBlock* AirPercentText;

	UPROPERTY(EditAnywhere, Category = "HUD")
	class UTextBlock* TimerText;
	UPROPERTY()
	int32 TimerAmount = 30;
	UFUNCTION()
	void TimeCountdown();
	FTimerHandle CountdownTimer;
	bool bCountdownStarted = false;


	UPROPERTY()
	class UMaterialInstanceDynamic* SphereMID;


	UFUNCTION()
	void RestoreText();
	UFUNCTION()
	void UpdateHUD();

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> WidgetClass;
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf< class ABaseActor>> PlaceableActors;
	UPROPERTY(EditAnywhere)
	class ABaseActor* SpawnedActor;
	UPROPERTY()
	class ABaseCharacter* BC;



	UFUNCTION(BlueprintCallable)
	void BuyItem(int32 index);

	UFUNCTION()
	void PlaceObject();
	UFUNCTION()
	void RemoveObject();

private:

	UFUNCTION()
	void GameOver();

	UPROPERTY(EditAnywhere, Category= "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MenuHUDClass;
	UPROPERTY(EditAnywhere, Category= "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> GameOverHUDClass;
	UPROPERTY()
	class UUserWidget* MenuWidget;
	UPROPERTY()
	class UButton* PlayButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APortalActor> PortalClass;
	UPROPERTY()
	class APortalActor* SpawnedPortal;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SpawnSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* CountdownSound;

	UFUNCTION()
	void PlayButtonClicked();

};
