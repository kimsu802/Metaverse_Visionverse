// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CreatureAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API ACreatureAIController : public AAIController
{
	GENERATED_BODY()

public:
	//������ ����
	ACreatureAIController();

	//��Ʈ�ѷ��� ���� �����ϴ� ���� �Լ��� ������ ����
	virtual void OnPossess(APawn* InPawn) override;

	//��Ʈ�ѷ��� ���� �������� �ϴ� �Լ� ���� ����
	virtual void OnUnPossess() override;

	//Ÿ���� ���� ȸ�������ִ� �����Լ� ���� ����
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;


private:


	/*Transient Ű����
	-> �𸮾� ������Ʈ���� ����ȭ ����� �־ ������Ʈ�� UPROPERTY �Ӽ��� �����ϰ� �ε��� �� ����.
	������Ʈ ���� �� CurrentHP (���� ������ �ִ� �����) ���� ���� ������ ������ ������ ����ǹǷ� �� ���� �����ϴ� ���� �ǹ̰� ����,
	������, ������Ʈ�� ������ �� �ʿ���� ��ũ ������ �����ϰ� ��.
	�̷��� �Ӽ����� Transient(�ֹ߼�) Ű���带 �߰��� �ش� �Ӽ��� ����ȭ���� ���ܽ�Ű�� ���� �ǹ���.
	*/
	UPROPERTY(Transient, meta =(AllowPrivateAccess = true))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	//BlackboardComponent�� ������ ������ ������.
	UPROPERTY(Transient, meta = (AllowPrivateAccess = true))
	class UBlackboardComponent* BlackboardComponent;

	//ũ���� ���忡�� ���� �÷��̾���. ���� ID�� ������ �� ������ ����.
	int32 TargetID;




};
