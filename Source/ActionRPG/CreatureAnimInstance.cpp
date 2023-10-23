// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAnimInstance.h"

//캐릭터 클래스에 접근하기 위한 헤더 -> Character 클래스에서 공중에 떠 있는지 판별이 가능함.
#include "GameFramework/Character.h"

//PawnMovementComponent에 접근하기 위한 헤더 -> PawnMovementComponent에서 캐릭터가 정지해 있는지 이동하고 있는 지 알 수가 있음.
#include "GameFramework/PawnMovementComponent.h"

// 애니메이션을 업데이트 해주는 가상함수를 정의.
void UCreatureAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// 부모 클래스의 함수의 내용을 모두 가져옴.
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*폰 찾기*/
	APawn* Pawn = TryGetPawnOwner();

	//만약, 폰을 찼았다면?
	if (IsValid(Pawn)) {

		//멈춰 있는지? 이동중인지 변수에 저장함.
		Speed = Pawn->GetVelocity().Size();

		//찾은 폰을 캐릭터로 형변환.
		ACharacter* Character = Cast<ACharacter>(Pawn);

		//만약, 형변환이 되었다면?
		if (Character) {
			//캐릭터가 공중에 떠있는지 확인해서 변수에 저장
			bIsInAir = Character->GetMovementComponent()->IsFalling();
		}

	}
}
