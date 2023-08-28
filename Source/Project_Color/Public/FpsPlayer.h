// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FpsPlayer.generated.h"

class UPlayerInputComponent;
class ACustomWeapon;
class UPlayerAnimatorComponent;

UCLASS()
class PROJECT_COLOR_API AFpsPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFpsPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Camera)
		UCameraComponent* mainCamera;

	UPROPERTY(EditAnywhere, Category = Camera)
		USpringArmComponent* springArmComponent;

	UPROPERTY(EditAnywhere, Category = "1P_Mesh")
		USceneComponent* firstPersonRoot;

	UPROPERTY(EditAnywhere, Category = "1P_Mesh")
		USpringArmComponent* meshSpringArm;

	UPROPERTY(EditAnywhere, Category = "1P_Mesh")
		USceneComponent* meshRoot;

	UPROPERTY(EditAnywhere, Category = "1P_Mesh")
		USkeletalMeshComponent* firstPersonMesh;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Components")
		UPlayerInputComponent* playerInputComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Components")
		UPlayerAnimatorComponent* playerAnimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Utility)
		UArrowComponent* rightWeaponPosition;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Utility)
		UArrowComponent* leftWeaponPosition;


	//Functions


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	//Custon CanJump
	bool CanJump();

	//Funzione con override da modificare per fgestire le condizioni di salto
	virtual bool CanJumpInternal_Implementation() const override;

	//Getter
	USceneComponent* GetFPSRoot();

	UCameraComponent* GetFPSCamera();

};
