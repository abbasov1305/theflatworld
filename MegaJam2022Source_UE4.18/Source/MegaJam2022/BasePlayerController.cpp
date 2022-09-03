// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "BaseActor.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Components/ProgressBar.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Button.h"
#include "PortalActor.h"
#include "Kismet/GameplayStatics.h"


ABasePlayerController::ABasePlayerController() {

}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewport(false);
	SetInputMode(InputMode);


	PlayerCameraManager->ViewPitchMin = 0.0f;
	PlayerCameraManager->ViewPitchMax = 359.0f;
	
	if (MenuHUDClass) {
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuHUDClass);
		if (MenuWidget) {
			MenuWidget->AddToViewport();
			PlayButton = Cast<UButton>(MenuWidget->GetWidgetFromName(TEXT("PlayButton")));
			
			if (PlayButton)
				PlayButton->OnClicked.AddDynamic(this, &ABasePlayerController::PlayButtonClicked);
		}
	}



	if (WidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (CurrentWidget)
		{
			//CurrentWidget->AddToViewport();

			EnergyText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("EnergyText")));
			AirPercentText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("AirPercent")));
			TimerText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("TimerText")));

			if(EnergyText)
				EnergyText->SetColorAndOpacity(DefaultText);

			AirProgressBar = Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("AirProgressBar")));
		}
	}

	BC = Cast<ABaseCharacter>(GetPawn());
	if (BC)
	{
		SphereMID = UMaterialInstanceDynamic::Create(BC->MeshComp->GetMaterial(0), GetPawn());
		UE_LOG(LogTemp, Warning, TEXT("bc name : %s"), *BC->GetName());
	}



	UpdateHUD();
	bCountdownStarted = false;
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedActor) {

		FHitResult HitResult;
		if(GetHitResultUnderCursor(ECC_Visibility, true, HitResult)) {

			if (HitResult.ImpactPoint.Z < 0.0f) {
				SpawnedActor->SetActorRotation(FRotator(0.0f, 0.0f, 180.0f));
			}
			else {
				SpawnedActor->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
			}

			SpawnedActor->SetActorLocation(HitResult.ImpactPoint);
		}
	}

}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PlaceObject", IE_Pressed, this, &ABasePlayerController::PlaceObject);
	InputComponent->BindAction("RemoveObject", IE_Pressed, this, &ABasePlayerController::RemoveObject);

}

void ABasePlayerController::BuyItem(int32 index)
{
	if (PlaceableActors[index] == nullptr)return;

	FActorSpawnParameters SpawnParams;
	SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(PlaceableActors[index]);
}

void ABasePlayerController::PlaceObject()
{
	if (SpawnedActor && SpawnedActor->bEnablePlace) {

		if (SpawnedActor->Price > EnergyAmount) {


			if (EnergyText) {
				EnergyText->SetColorAndOpacity(AlarmText);
				//EnergyText->SetText(FText::FromString("not enough energy"));
				GetWorldTimerManager().ClearTimer(TextTimer);
				GetWorldTimerManager().SetTimer(TextTimer, this, &ABasePlayerController::RestoreText, 2.0f, false);
			}


			RemoveObject();
			
			return;
		}

		EnergyAmount -= SpawnedActor->Price;
		SpawnedActor->ActorPlaced();

		if (SpawnSound) UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, SpawnedActor->GetActorLocation());

		SpawnedActor = nullptr;
	}

	UpdateHUD();
}

void ABasePlayerController::RemoveObject()
{
	if (SpawnedActor) {

		SpawnedActor->Destroy();
	}
}

void ABasePlayerController::RestoreText()
{

	EnergyText->SetColorAndOpacity(DefaultText);

	UpdateHUD();

}


void ABasePlayerController::UpdateHUD()
{
	AirPercent = FMath::Clamp(SmokeAmount / AirAmount, 0.0f, 1.0f);
	if (EnergyAmount < 0) {
		EnergyAmount = 0; 
		TimeCountdown();
	}


	if (EnergyText)
		EnergyText->SetText(FText::FromString(FString::Printf(TEXT("%de"),EnergyAmount)));

	if (AirProgressBar)
		AirProgressBar->SetPercent(AirPercent);

	if (AirPercentText) {
		int32 Percent = AirPercent * 100.0f;
		AirPercentText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), Percent)));
	}

	if (AirPercent == 1.0f) {
		TimeCountdown();
	}

	if (SphereMID)
	{
		SphereMID->SetScalarParameterValue(FName("AirPercent"), AirPercent);
		BC->MeshComp->SetMaterial(0, SphereMID);
	}
}

void ABasePlayerController::TimeCountdown()
{
	if (!bCountdownStarted) {

		bCountdownStarted = true;

		GetWorldTimerManager().SetTimer(CountdownTimer, this, &ABasePlayerController::GameOver, 1.0f, true);
	}
}

void ABasePlayerController::GameOver()
{
	TimerAmount--;
	if (CountdownSound)
		UGameplayStatics::PlaySound2D(this, CountdownSound);
	

	if (TimerText) {
		TimerText->SetText(FText::FromString(FString::FromInt(TimerAmount)));
	}

	if (EnergyAmount > 0 && AirPercent < 1.0f) {

		GetWorldTimerManager().ClearTimer(CountdownTimer);
		TimerAmount = 30;
		TimerText->SetText(FText::FromString(""));
		bCountdownStarted = false;
	}

	if (TimerAmount <= 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Game Over"));
		if (GameOverHUDClass) {

			UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverHUDClass);

			if (GameOverWidget && !GameOverWidget->IsInViewport()) {

				if (CurrentWidget) CurrentWidget->RemoveFromViewport();

				GameOverWidget->AddToViewport();

			}
		}
	}
}

void ABasePlayerController::PlayButtonClicked()
{
	if (MenuWidget) MenuWidget->RemoveFromViewport();
	if(CurrentWidget) CurrentWidget->AddToViewport();

	if (PortalClass)
		SpawnedPortal = GetWorld()->SpawnActor<APortalActor>(PortalClass, FVector::ZeroVector, FRotator::ZeroRotator);
}
