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
	//속도가 0이면 대기모션하고, 속도가 올라갈 수록 이동모션함. 속도를 저장해 둘 변수를 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pan", meta = (AllowPrivateAccess = true))
	float Speed;

	//캐릭터가 공중에 떠 있는지 판별해줄 불리언 변수를 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pan", meta = (AllowPrivateAccess = true))
	bool bIsInAir;




};
