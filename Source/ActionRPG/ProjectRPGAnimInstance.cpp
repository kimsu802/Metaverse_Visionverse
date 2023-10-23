// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectRPGAnimInstance.h"


//캐릭터 클래스에 접근하기 위하여
#include "GameFramework/Character.h"

//캐릭터의 이동관련 라이브러리
#include "GameFramework/CharacterMovementComponent.h"


void UProjectRPGAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	//액터 찾기.
	AActor* Actor = GetOwningActor();

	//만약, 액터를 찾았다면
	if (IsValid(Actor)) {
		//Character로 형변환해서 멤버 변수에 저장
		Character = Cast<ACharacter>(Actor);

		//만약, Charater로 형변환이 되었다면,
		if (IsValid(Character)) {

			//CharacterMovementCharacter를 찾아서 MovementComponent 멤버 변수에 저장함.
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


	//만일, Character 멤버 변수가 유효하다면
	if (IsValid(Character)) {
		//속도의 사이즈값을 GroundSpeed 멤버 변수에 저장해줌.
		GroundSpeed = MovementComponent->Velocity.Size2D();

		//현재 속도가 3보다 크고(&&), 가속도가 존재하는지를 판별해서(현재 이동중인지 판별)
		//bShouldMove 불리언 변수에 저장.

		bShouldMove = (GroundSpeed > 3) && (MovementComponent->GetCurrentAcceleration() != FVector(0.0f, 0.0f, 0.0f));

		//공중에 떠있는지 판별해서 bIsFalling 불리언 변수에 저장.
		bIsFalling = MovementComponent->IsFalling();

	}

}
