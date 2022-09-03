// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "BasePlayerController.h"


// Sets default values
ABaseActor::ABaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseActor::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ABaseActor::OnOverlapEnd);

	MaterialInstance = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), this);

}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this) return;


	if (Cast<ABaseActor>(OtherActor)) {
		
		if(MaterialInstance)
			MaterialInstance->SetScalarParameterValue(FName("Enable"), 0.0f);

		bEnablePlace = false;
	}
	else {

		if (MaterialInstance)
			MaterialInstance->SetScalarParameterValue(FName("Enable"), 1.0f);

		bEnablePlace = true;
	}

	if (MaterialInstance)
		MeshComp->SetMaterial(0, MaterialInstance);
}

void ABaseActor::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;

	if (Cast<ABaseActor>(OtherActor)) {

		bEnablePlace = true;
	}

	
	if (MaterialInstance) {
		MaterialInstance->SetScalarParameterValue(FName("Enable"), 1.0f);
		MeshComp->SetMaterial(0, MaterialInstance);
	}


}

void ABaseActor::ActorPlaced()
{
	if (SmokeValue > 0.0f)
	{
		GetWorldTimerManager().SetTimer(SmokeTimer, this, &ABaseActor::ProduceSmoke, SmokeRate, true, SmokeRate);
	}
	if (AirValue > 0.0f)
	{
		GetWorldTimerManager().SetTimer(SmokeTimer, this, &ABaseActor::ProduceAir, AirRate, true, AirRate);
	}

	if (GetWorld()->GetFirstPlayerController()) {

		PC = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	}

}

void ABaseActor::ProduceSmoke()
{
	if (PC) {

		PC->SmokeAmount += SmokeValue;
		PC->EnergyAmount += EnergyValue;

		PC->UpdateHUD();
	}
}

void ABaseActor::ProduceAir()
{
	if (PC) {

		PC->AirAmount += AirValue;

		PC->UpdateHUD();
	}
}

