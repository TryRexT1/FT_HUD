// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACannonCharacter::ACannonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a capsule component and set it as the root component
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(1.f, 1.f);
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-50.f, 0.f, 0.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false; // we will control the rotation of the camera ourselves
}

// Called when the game starts or when spawned
void ACannonCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Hello from cannon"));
	// do we have a valid hud 
	if (HUDType)
	{
		// create our hud
		HUD = CreateWidget<UUserWidget>(GetWorld(), HUDType);
		// if we successfuly created the widget
		if (HUD)
		{
			HUD->AddToViewport();
		}
	}
}

// Called every frame
void ACannonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (CurrentState)
	{
	case ShootingState::Power:
		UpdateValue(&power, DeltaTime);
		break; 
	case ShootingState::Angle:
		UpdateValue(&angle, DeltaTime);
		break;
	case ShootingState::Shot:
		// pew pew
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("WHY YOU MESSING WITH THE ENUMS!"));
		break;
	}
}

// Called to bind functionality to input
void ACannonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// readied 
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ACannonCharacter::ProgressState);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to setup player"));
	}
}

void ACannonCharacter::ProgressState()
{
	switch (CurrentState)
	{
	case ShootingState::Power:
		CurrentState = ShootingState::Angle;
		UE_LOG(LogTemp, Warning, TEXT("Angle select"));
		break;
	case ShootingState::Angle:
		CurrentState = ShootingState::Angle;
		UE_LOG(LogTemp, Warning, TEXT("FIRE THE WEAPON"));
		CurrentState = ShootingState::Shot;
		Fire();
		break;
	case ShootingState::Shot:
		// pew pew
		UE_LOG(LogTemp, Warning, TEXT("Wait!"));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("WHY YOU MESSING WITH THE ENUMS!"));
		break;
	}
}

// notice now i use a pointer and update the values in one function
// this is a form of passing by reference, as i know i do this to muliple values
// i also want to avoid copying as that is expensive
void ACannonCharacter::UpdateValue(float* value, float DeltaTime)
{
	// the *value means dereference 
	if (*value < 0 || *value > 90)
	{
		// flip speed from positive to negative
		speed = -speed;
	}
	*value += speed * DeltaTime;
}

void ACannonCharacter::Fire()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileClass not set"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) { return; }

	// Use actor’s current yaw so the cannon shoots where you face.
	const FRotator ActorRot = GetActorRotation();
	const float Yaw = ActorRot.Yaw;

	const FRotator SpawnRotation(angle, Yaw, 0.f);
	const FVector SpawnLocation = GetActorLocation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	Params.Owner = this;
	Params.Instigator = this;

	AActor* Projectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
	if (!Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile"));
		return;
	}

	// Compute initial velocity from angle + power
	const FVector FireDir = SpawnRotation.Vector();  // get the forward vector
	const FVector InitialVelocity = FireDir * power; // Power is initial speed (units/sec)
	if (UProjectileMovementComponent* Move = Projectile->FindComponentByClass<UProjectileMovementComponent>())
	{
		Move->Velocity = InitialVelocity;
	}
}