// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectRPGAnimInstance.h"


//ĳ���� Ŭ������ �����ϱ� ���Ͽ�
#include "GameFramework/Character.h"

//ĳ������ �̵����� ���̺귯��
#include "GameFramework/CharacterMovementComponent.h"


void UProjectRPGAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	//���� ã��.
	AActor* Actor = GetOwningActor();

	//����, ���͸� ã�Ҵٸ�
	if (IsValid(Actor)) {
		//Character�� ����ȯ�ؼ� ��� ������ ����
		Character = Cast<ACharacter>(Actor);

		//����, Charater�� ����ȯ�� �Ǿ��ٸ�,
		if (IsValid(Character)) {

			//CharacterMovementCharacter�� ã�Ƽ� MovementComponent ��� ������ ������.
			MovementComponent = Character->GetCharacterMovement();
			UE_LOG(LogTemp, Log, TEXT("MovementComponent is Set"));
			
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("MovementComponent is not Set"));
		}
	}

}

void UProjectRPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	//����, Character ��� ������ ��ȿ�ϴٸ�
	if (IsValid(Character)) {
		//�ӵ��� ������� GroundSpeed ��� ������ ��������.
		GroundSpeed = MovementComponent->Velocity.Size2D();

		//���� �ӵ��� 3���� ũ��(&&), ���ӵ��� �����ϴ����� �Ǻ��ؼ�(���� �̵������� �Ǻ�)
		//bShouldMove �Ҹ��� ������ ����.

		bShouldMove = (GroundSpeed > 3) && (MovementComponent->GetCurrentAcceleration() != FVector(0.0f, 0.0f, 0.0f));

		//���߿� ���ִ��� �Ǻ��ؼ� bIsFalling �Ҹ��� ������ ����.
		bIsFalling = MovementComponent->IsFalling();

	}

}
