// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipMandible.generated.h"


class UBoxComponent;
class USkeletalMeshComponent;
UCLASS()
class BEETLE_PROJECT_API AEquipMandible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipMandible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
	float BiteLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
	float MaxSpeed;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
		float Bounce;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
		float FlightLength;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	USkeletalMeshComponent* MandibleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	UBoxComponent* Collider;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	void DestroyBullet();
};
