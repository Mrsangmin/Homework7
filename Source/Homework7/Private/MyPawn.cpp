#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// ĸ�� ������Ʈ 
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->InitCapsuleSize(40.f, 92.f);
	CapsuleComponent->SetSimulatePhysics(false); // ���� ��Ȱ��ȭ

	// ���̷�Ż �޽� 
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	SkeletalMeshComponent->SetSimulatePhysics(false);

	// SpringArm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);
	SpringArmComponent->TargetArmLength = 300.0f;  // ī�޶� �Ÿ� ����
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bDoCollisionTest = false;  // �浹 �׽�Ʈ�� ��Ȱ��ȭ�ؼ� ī�޶� ���� ������ �ʵ���


	// ī�޶�
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMyPawn::BeginPlay()
{
	// Enhanced Input Ȱ��ȭ
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}


void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPawn::Look);
	}
}

//Move
void AMyPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!MovementVector.IsNearlyZero())
	{
		// ī�޶��� Yaw(�¿� ȸ��)�� �ݿ��Ͽ� �̵� ������ ���
		FRotator ControlRotation = GetController()->GetControlRotation();
		ControlRotation.Pitch = 0.f;  // ���� ȸ���� ����

		FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y); // ����
		FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);   // ������

		FVector MoveOffset = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X)
			* MoveSpeed * GetWorld()->GetDeltaSeconds();

		AddActorLocalOffset(MoveOffset, true);
	}
}

//Look
void AMyPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	// ���� ȸ�� (Pitch)�� SpringArm���� ����
	FRotator SpringArmRotation = SpringArmComponent->GetComponentRotation();
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch + LookVector.Y * TurnSpeed * GetWorld()->GetDeltaSeconds(), -80.f, 80.f);
	SpringArmComponent->SetWorldRotation(SpringArmRotation);

	// �¿� ȸ�� (Yaw)�� ĳ������ ȸ���� ����
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += LookVector.X * TurnSpeed * GetWorld()->GetDeltaSeconds();
	SetActorRotation(NewRotation);
}
