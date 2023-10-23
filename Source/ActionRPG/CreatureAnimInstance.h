// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CreatureAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UCreatureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

private:
	//�ӵ��� 0�̸� ������ϰ�, �ӵ��� �ö� ���� �̵������. �ӵ��� ������ �� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pan", meta = (AllowPrivateAccess = true))
	float Speed;

	//ĳ���Ͱ� ���߿� �� �ִ��� �Ǻ����� �Ҹ��� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pan", meta = (AllowPrivateAccess = true))
	bool bIsInAir;




};
