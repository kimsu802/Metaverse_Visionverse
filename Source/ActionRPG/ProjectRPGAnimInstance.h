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

	//UObject �ڽ� Ŭ�������̳� AActor �ڽ�Ŭ���� ������ UPROPERTY ��ũ�ΰ� ������ ������ ��ü������ �޸� ������.
	//EditAnywhere : Ŭ������ ������ �г��̳� �������Ʈ�� ������ �гο��� �а� ���Ⱑ ������.
	//EditInstanceOnly : Ŭ������ ������ �гο��� �а� ���Ⱑ ������.
	//EditDefaultsOnly : �������Ʈ�� �������гο��� �а� ���Ⱑ ������.
	//VisibleAnywhere : Ŭ������ ������ �г��̳� �������Ʈ�� ������ �гο��� �бⰡ ������.
	//VisibleInstanceOnly : Ŭ������ ������ �гο��� �бⰡ ������.
	//VisiblaDefaultsOnly : �������Ʈ�� ������ �гο��� �бⰡ ������
	// Category = "Pawn" : "Pawn" �׸񿡼� ������ �����. ���ο� �׸���.
	// Category = Pawn : "Pawn" �׸񿡼� ������ �����. �̹� �ִ� �׸�.
	// BlueRrintReadWrite : �������Ʈ���� ��带 �аų� �� �� ����.
	// BluePrintReadOnly : �������Ʈ���� ��带 ���� �� ����.
	// meta = (AllowPrivateAceess = "true")
	// meta = (AllowPrivateAceess = true) : ��� ������ private ���������� �ȿ� �ִٸ� 
	//�� Ŭ������ ��ӹ��� �ڽ� Ŭ�������� ������ ������ �Ұ�����. ������ �� Ŭ������ �θ� Ŭ������ ��� ���� �������Ʈ �ڽ� Ŭ��������
	// ������ �����ϵ��� ����.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn",meta = (AllowPrivateAccess = true))
	class ACharacter* Character;

	//class ����� �ξ ���漱���� ����. ���漱���� �ϰ� �Ǹ� ����� include ���� �ʾƵ� ��.
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
