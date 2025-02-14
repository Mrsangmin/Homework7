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
    // �浹 ĸ��
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCapsuleComponent* CapsuleComponent;

    // ���̷�Ż �޽�
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USkeletalMeshComponent* SkeletalMeshComponent;

    // ī�޶� ��
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USpringArmComponent* SpringArmComponent;

    // ī�޶�
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCameraComponent* CameraComponent;

    //�̵��ӵ� ����
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 300.0f;
    //�� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Movement")
    float TurnSpeed = 50.0f;

    // �̵� �� ȸ�� �Լ�
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    // Enhanced Input ���� ����
    UPROPERTY(EditAnywhere, Category = "Enhanced Input")
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, Category = "Enhanced Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Enhanced Input")
    class UInputAction* LookAction;


};

