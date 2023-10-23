// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	/*�����ڿ� �ش��ϴ� �Լ��� ������ �����մϴ�.
	�����ڴ� UUserWdiget�� NativeConstruct() �Լ��� �������̵��Ͽ� �ۼ��ϸ� �ǰ�
	�� �����ڴ� ������ ����Ʈ�� �߰��Ǹ� (AddToViewPort) ȣ���մϴ�.*/
	virtual void NativeConstruct() override;

	/*ü�¹� ������ �����ϱ� ���� ������ �����մϴ�.
	meta = (BindWidget) : UI������ Ŭ������ ������ �������ݴϴ�.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar;

	/*ü�¹ٿ� Value�� ������ �ִ� �Լ��� ������ �����մϴ�.*/
	void SetHealthBarValue(float Value);


	
	
};
