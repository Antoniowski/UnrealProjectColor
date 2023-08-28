// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "PlayerAnimatorComponent.generated.h"


class AFpsPlayer;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_COLOR_API UPlayerAnimatorComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UPlayerAnimatorComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
		float walkAnimAlpha;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
		FVector walkingAnimPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
		FRotator walkingAnimRot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
		FVector locationLagPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Sway")
		FVector pitchOffsetPos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Sway")
		FRotator currentCameraRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Sway")
		FRotator cameraRotateRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Sway")
		FVector cameraRotationOffset;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movement | Walking")
		FTimeline walkingTL;
	UPROPERTY(EditAnywhere, Category = "Movement | Walking")
		UCurveFloat* upDownCurve;
	UPROPERTY(EditAnywhere, Category = "Movement | Walking")
		UCurveFloat* leftRightCurve;
	UPROPERTY(EditAnywhere, Category = "Movement | Walking")
		UCurveFloat* rollCurve;
	UPROPERTY(EditAnywhere, Category = "Movement | Walking")
		UCurveFloat* footstepCurve;

private:
	AFpsPlayer* mainActor;
	float leftRightAlpha;
	float upDownAlpha;
	float rollAlpha;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void UpdateHorizontalAlpha(float value);
	UFUNCTION()
		void UpdateVerticalAlpha(float value);
	UFUNCTION()
		void UpdateRollAlpha(float value);
	UFUNCTION()
		void GetVelocityVar(); //Lag position
	UFUNCTION()
		void GetLookInputVar(FRotator previousCameraRotation);

};
