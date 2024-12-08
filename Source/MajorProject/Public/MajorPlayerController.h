// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MajorPlayerController.generated.h"


UCLASS()
class MAJORPROJECT_API AMajorPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetupInputComponent() override;

	void RequestMoveForward(float AxisValue);
	void RequestMoveRight(float AxisValue);
	void RequestLookUp(float AxisValue);
	void RequestLookRight(float AxisValue);

	void RequestJump();
	void RequestCrouch();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void RequestSprintStart();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void RequestSprintEnd();

	// Base lookup rate, in deg/sec. Other scaling may affect final lookup rate.
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;

	// Base lookup rate, in deg/sec. Other scaling may affect final lookup rate.
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;

	//used to determine flick of axis
	//float LastDelta = 0.0f;
	float LastAxis = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float FlickThreshold = 0.75f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.0f;

	float MaxWalkSpeed = 600.f;
	float SprintSpeed = 900.f;

	// Utility for checking if moving forward
	bool IsMovingForward() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsSprinting = false;

};
