// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAnimInstance.h"

//ĳ���� Ŭ������ �����ϱ� ���� ��� -> Character Ŭ�������� ���߿� �� �ִ��� �Ǻ��� ������.
#include "GameFramework/Character.h"

//PawnMovementComponent�� �����ϱ� ���� ��� -> PawnMovementComponent���� ĳ���Ͱ� ������ �ִ��� �̵��ϰ� �ִ� �� �� ���� ����.
#include "GameFramework/PawnMovementComponent.h"

// �ִϸ��̼��� ������Ʈ ���ִ� �����Լ��� ����.
void UCreatureAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// �θ� Ŭ������ �Լ��� ������ ��� ������.
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*�� ã��*/
	APawn* Pawn = TryGetPawnOwner();

	//����, ���� á�Ҵٸ�?
	if (IsValid(Pawn)) {

		//���� �ִ���? �̵������� ������ ������.
		Speed = Pawn->GetVelocity().Size();

		//ã�� ���� ĳ���ͷ� ����ȯ.
		ACharacter* Character = Cast<ACharacter>(Pawn);

		//����, ����ȯ�� �Ǿ��ٸ�?
		if (Character) {
			//ĳ���Ͱ� ���߿� ���ִ��� Ȯ���ؼ� ������ ����
			bIsInAir = Character->GetMovementComponent()->IsFalling();
		}

	}
}
