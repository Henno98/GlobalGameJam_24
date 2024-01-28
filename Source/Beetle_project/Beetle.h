// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Beetle.generated.h"


struct FInputActionValue;
class UInputAction;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class BEETLE_PROJECT_API ABeetle : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABeetle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class USphereComponent* Collider{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class UCameraComponent* Camera{ nullptr };

	//float setup
	float XInput;
	float YInput;
	float Yaw;
	float Roll;
	float Pitch;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
	float BiteLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
	float Bounce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Stats")
	float FlightLength;

	//Input Mapping setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	UInputAction* ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	UInputAction* UpInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	UInputAction* RightInput;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	UInputAction* MouseXInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	UInputAction* MouseYInput;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	UInputAction* SpecialInput;

	void Forward(const FInputActionValue& input);
	void Right(const FInputActionValue& input);
	void MouseX(const FInputActionValue& input);
	void MouseY(const FInputActionValue& input);
	void Special();
	void Movement();
	void GameStateChange();
	void Bite(const FInputActionValue& input);

	void AttachHeadMesh(USkeletalMeshComponent* NewHeadMesh);
	void SetGamePaused(bool bIsPaused);
	void OnHit();
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	bool GameOver = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	bool ShowSettings = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	bool GameWon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	bool GamePaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	bool IsPaused;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	bool bHasGameStarted;
};
