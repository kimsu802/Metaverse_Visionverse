// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectRPGAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONRPG_API UProjectRPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	//UObject 자식 클래스들이나 AActor 자식클래스 변수에 UPROPERTY 매크로가 붙으면 엔진이 자체적으로 메모리 관리함.
	//EditAnywhere : 클래스의 디테일 패널이나 블루프린트의 디테일 패널에서 읽고 쓰기가 가능함.
	//EditInstanceOnly : 클래스의 디테일 패널에서 읽고 쓰기가 가능함.
	//EditDefaultsOnly : 블루프린트의 디테일패널에서 읽고 쓰기가 가능함.
	//VisibleAnywhere : 클래스의 디테일 패널이나 블루프린트의 디테일 패널에서 읽기가 가능함.
	//VisibleInstanceOnly : 클래스의 디테일 패널에서 읽기가 가능함.
	//VisiblaDefaultsOnly : 블루프린트의 디테일 패널에서 읽기가 가능함
	// Category = "Pawn" : "Pawn" 항목에서 변수가 노출됨. 새로운 항목임.
	// Category = Pawn : "Pawn" 항목에서 변수가 노출됨. 이미 있는 항목.
	// BlueRrintReadWrite : 블루프린트에서 노드를 읽거나 쓸 수 있음.
	// BluePrintReadOnly : 블루프린트에서 노드를 읽을 수 있음.
	// meta = (AllowPrivateAceess = "true")
	// meta = (AllowPrivateAceess = true) : 멤버 변수가 private 접근지정자 안에 있다면 
	//이 클래스를 상속받은 자식 클래스에서 조차도 접근이 불가능함. 하지만 이 클래스를 부모 클래스로 상속 받은 블루프린트 자식 클래스에서
	// 접근이 가능하도록 해줌.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn",meta = (AllowPrivateAccess = true))
	class ACharacter* Character;

	//class 지정어를 두어서 전방선언을 해줌. 전방선언을 하게 되면 헤더를 include 하지 않아도 됨.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	
};
