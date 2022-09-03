// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"

UCLASS()
class MEGAJAM2022_API APortalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* TopBox;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BottomBox;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* TopMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BottomMesh;
	UPROPERTY(EditAnywhere)
	class USceneComponent* TopPoint;
	UPROPERTY(EditAnywhere)
	class USceneComponent* BottomPoint;

	UPROPERTY()
	class ABasePlayerController* PC;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABasePawn> PawnClass;
	UPROPERTY(EditAnywhere)
	float SpawnRate = 10.0f;
	FTimerHandle SpawnTimer;
	UFUNCTION()
	void SpawnPawn();


	UFUNCTION()
	void OnOverlapBeginTop(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	

};
