// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../FT_HUDProjectile.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "CannonCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ShootingState : uint8 {
	Power UMETA(DisplayName = "POWER"),
	Angle UMETA(DisplayName = "ANGLE"),
	Shot UMETA(DisplayName = "SHOT")
};

UCLASS()
class FT_HUD_API ACannonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFT_HUDProjectile> ProjectileClass;

	/** camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction;
public:
	// Sets default values for this character's properties
	ACannonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void UpdateValue(float* value, float DeltaTime);
	void Fire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// shooting values
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float power = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float angle = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float speed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ShootingState CurrentState = ShootingState::Power;

	// this hold the template we will create
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUD;

	UFUNCTION()
	void ProgressState();
};
