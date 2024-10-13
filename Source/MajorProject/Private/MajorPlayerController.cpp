#include "MajorPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AMajorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//GameModeRef = Cast<ATantrumGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AMajorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UpdateCharacterRotationBasedOnMovement();
}

void AMajorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMajorPlayerController::RequestJump);
		InputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AMajorPlayerController::RequestCrouch);

		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMajorPlayerController::RequestSprintStart);
		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMajorPlayerController::RequestSprintEnd);

		//InputComponent->BindAxis(TEXT("MoveForward"), this, &AMajorPlayerController::RequestMoveForward);
		//InputComponent->BindAxis(TEXT("MoveRight"), this, &AMajorPlayerController::RequestMoveRight);
		InputComponent->BindAxis(TEXT("LookUp"), this, &AMajorPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("LookRight"), this, &AMajorPlayerController::RequestLookRight);
	}
}

//void AMajorPlayerController::UpdateCharacterRotationBasedOnMovement()
//{
//	// Check if movement input is applied
//	if (GetCharacter()->GetVelocity().Size() > 0.0f)
//	{
//		// Character is moving, enable "Orient Rotation to Movement"
//		GetCharacter()->GetCharacterMovement()->bOrientRotationToMovement = true;
//	}
//	else
//	{
//		// No movement input, disable rotation to movement
//		GetCharacter()->GetCharacterMovement()->bOrientRotationToMovement = false;
//	}
//}

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
	if (GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed += SprintSpeed;
	}
}

void AMajorPlayerController::RequestSprintEnd()
{
	if (GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed -= SprintSpeed;
	}
}

