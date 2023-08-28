// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "InputActionValue.h"
#include "PlayerInputComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class AFpsPlayer;
class UTimelineComponent;
class UCurveFloat;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_COLOR_API UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInputComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isRunning;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float baseWalkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "Movement | Crouch")
		float crouchAlpha;
	UPROPERTY(EditAnywhere, Category = "Movement | Dip")
		float dipStrenght;
	UPROPERTY(EditAnywhere, Category = "Movement | Dip")
		float dipAlpha;
	UPROPERTY(EditAnywhere, Category = "Movement | Jump")
		float coyoteMaxTime;





	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInput(UInputComponent* playerInput);
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void CustomJump(const FInputActionValue& value);
	void Run(const FInputActionValue& value);
	void Crouch(const FInputActionValue& value);
	void Uncrouch(const FInputActionValue& value);

	UPROPERTY(VisibleAnywhere, Category = "Movement | Jump")
		FTimerHandle coyoteTimer;

	UFUNCTION()
		void StartCoyoteTimer();
	UFUNCTION()
		void StopCoyoteTimer();

	UFUNCTION()
		void Dip(float speed, float strenght);





protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* mapping;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* moveAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* lookAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* jumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* runAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* crouchAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* uncrouchAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main Component")
		AFpsPlayer* mainPlayerComponent;
	UPROPERTY(VisibleAnywhere, Category = "Movement | Crouch")
		FTimerHandle uncrouchTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Crouch")
		UCurveFloat* crouchCurve = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Movement | Crouch")
		float crouchHeight;
	UPROPERTY(VisibleAnywhere, Category = "Movement | Crouch")
		float standHeight;
	UPROPERTY(VisibleAnywhere, Category = "Movement | Crouch")
		float crouchedMovementSpeedMultiplier = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Dip")
		UCurveFloat* dipCurve = nullptr;





private:
	FTimeline crouchTimeline;

	FTimeline dipTL;

	UFUNCTION()
		void OnUpdateCrouchAlpha(float value);
	UFUNCTION()
		void OnFinishCrouch();

	UFUNCTION()
		void CheckIfCanStandUp();


	UFUNCTION()
		void OnUpdateDipAlpha(float value);

	UFUNCTION()
		void OnLandDip();
};
