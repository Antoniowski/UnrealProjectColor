// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
class AFpsPlayer;
class UPlayerInputComponent;
class UPlayerAnimatorComponent;
UCLASS()
class PROJECT_COLOR_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main Component")
		AFpsPlayer* mainComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Var")
		float playerSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		float crouchAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		float dipAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		float walkAnimAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FVector walkingAnimPos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FRotator walkingAnimRot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FVector locationLagPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FVector pitchOffsetPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FVector cameraRotationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FRotator currentCameraRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alphas")
		FRotator cameraRotationRate;


private:
	UPlayerInputComponent* inputComponent;
	UPlayerAnimatorComponent* animatorComponent;
};
