// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

UCLASS()
class MEGAJAM2022_API ABaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 Price = 100;



	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	bool bEnablePlace = false;

	UPROPERTY(VisibleAnywhere)
	class UMaterialInstanceDynamic* MaterialInstance;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ActorPlaced();


private:

	UPROPERTY()
	class ABasePlayerController* PC;

	UPROPERTY(EditAnywhere)
	float SmokeRate;
	UPROPERTY(EditAnywhere)
	float SmokeValue;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float EnergyValue;
	UPROPERTY(EditAnywhere)
	float AirRate;
	UPROPERTY(EditAnywhere)
	float AirValue;

	FTimerHandle SmokeTimer;
	FTimerHandle AirTimer;

	UFUNCTION()
	void ProduceSmoke();
	UFUNCTION()
	void ProduceAir();

};
