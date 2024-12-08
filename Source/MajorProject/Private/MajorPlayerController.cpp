#include "MajorPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

class ACharacter;

void AMajorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Set input mode to Game Only
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// Ensure mouse cursor is hidden during gameplay
	bShowMouseCursor = false;
	//GameModeRef = Cast<AMajorGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AMajorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSprinting)
	{
		if (!IsMovingForward())
		{
			RequestSprintEnd();
		}
	}
}

void AMajorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMajorPlayerController::RequestJump);
		InputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AMajorPlayerController::RequestCrouch);

		//FInputActionBinding sprintPressed = InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMajorPlayerController::RequestSprintStart);
		//InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMajorPlayerController::RequestSprintEnd);
		//sprintPressed.bConsumeInput = false;

		InputComponent->BindAxis(TEXT("LookUp"), this, &AMajorPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("LookRight"), this, &AMajorPlayerController::RequestLookRight);
	}
}


void AMajorPlayerController::RequestMoveForward(float AxisValue)
{

	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), AxisValue);
	}
}

void AMajorPlayerController::RequestMoveRight(float AxisValue)
{

	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), AxisValue);
	}
}

void AMajorPlayerController::RequestLookUp(float AxisValue)
{
	AddPitchInput(AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMajorPlayerController::RequestLookRight(float AxisValue)
{
	AddYawInput(AxisValue * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}

void AMajorPlayerController::RequestJump()
{

	if (GetCharacter())
	{
		GetCharacter()->Jump();
	}
}

void AMajorPlayerController::RequestCrouch()
{
	if (!GetCharacter()->GetCharacterMovement()->IsMovingOnGround()) { return; }
	if (GetCharacter())
	{
		GetCharacter()->Crouch();
	}

	if (GetCharacter()->bIsCrouched)
	{
		GetCharacter()->UnCrouch();
	}
}

void AMajorPlayerController::RequestSprintStart()
{
	//if (IsMovingForward())
	ACharacter* ThisCharacter = GetCharacter();
	if (ThisCharacter)
	{
		// Only start sprinting if the player is moving forward
		if (IsMovingForward())
		{
			UCharacterMovementComponent* MovementComponent = GetCharacter()->GetCharacterMovement();
			if (MovementComponent)
			{
				MovementComponent->MaxWalkSpeed = SprintSpeed;
				UE_LOG(LogTemp, Warning, TEXT("Sprint started"));

				IsSprinting = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint restricted: Not moving forward"));
	}
}

void AMajorPlayerController::RequestSprintEnd()
{
	
	if (GetCharacter())
	{
		UCharacterMovementComponent* MovementComponent = GetCharacter()->GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->MaxWalkSpeed = WalkSpeed; // Reset to normal walking speed
			IsSprinting = false;
		}
	}
}


bool AMajorPlayerController::IsMovingForward() const
{
	ACharacter* LocalCharacter = GetCharacter();
	if (LocalCharacter)
	{
		FVector Velocity = LocalCharacter->GetVelocity();
		FVector ForwardVector = LocalCharacter->GetActorForwardVector();

		FVector NormalizedVelocity = Velocity.GetSafeNormal();
		FVector NormalizedForward = ForwardVector.GetSafeNormal();

		float DotProduct = FVector::DotProduct(NormalizedVelocity, NormalizedForward);

		return Velocity.SizeSquared() > 0.0f && DotProduct > 0.7f; // Adjusted threshold
	}

	return false;
}

