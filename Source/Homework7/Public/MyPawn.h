#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class HOMEWORK7_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;


private:
    // 충돌 캡슐
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCapsuleComponent* CapsuleComponent;

    // 스켈레탈 메시
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USkeletalMeshComponent* SkeletalMeshComponent;

    // 카메라 암
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USpringArmComponent* SpringArmComponent;

    // 카메라
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCameraComponent* CameraComponent;

    //이동속도 조절
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 300.0f;
    //턴 속도
    UPROPERTY(EditAnywhere, Category = "Movement")
    float TurnSpeed = 50.0f;

    // 이동 및 회전 함수
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    // Enhanced Input 관련 변수
    UPROPERTY(EditAnywhere, Category = "Enhanced Input")
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, Category = "Enhanced Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Enhanced Input")
    class UInputAction* LookAction;


};

