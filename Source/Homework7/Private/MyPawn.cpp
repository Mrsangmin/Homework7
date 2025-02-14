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

	// 캡슐 컴포넌트 
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->InitCapsuleSize(40.f, 92.f);
	CapsuleComponent->SetSimulatePhysics(false); // 물리 비활성화

	// 스켈레탈 메시 
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	SkeletalMeshComponent->SetSimulatePhysics(false);

	// SpringArm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);
	SpringArmComponent->TargetArmLength = 300.0f;  // 카메라 거리 설정
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bDoCollisionTest = false;  // 충돌 테스트를 비활성화해서 카메라가 벽에 막히지 않도록


	// 카메라
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMyPawn::BeginPlay()
{
	// Enhanced Input 활성화
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
		// 카메라의 Yaw(좌우 회전)만 반영하여 이동 방향을 계산
		FRotator ControlRotation = GetController()->GetControlRotation();
		ControlRotation.Pitch = 0.f;  // 상하 회전은 무시

		FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y); // 전방
		FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);   // 오른쪽

		FVector MoveOffset = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X)
			* MoveSpeed * GetWorld()->GetDeltaSeconds();

		AddActorLocalOffset(MoveOffset, true);
	}
}

//Look
void AMyPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	// 상하 회전 (Pitch)은 SpringArm에만 적용
	FRotator SpringArmRotation = SpringArmComponent->GetComponentRotation();
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch + LookVector.Y * TurnSpeed * GetWorld()->GetDeltaSeconds(), -80.f, 80.f);
	SpringArmComponent->SetWorldRotation(SpringArmRotation);

	// 좌우 회전 (Yaw)은 캐릭터의 회전만 변경
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += LookVector.X * TurnSpeed * GetWorld()->GetDeltaSeconds();
	SetActorRotation(NewRotation);
}
