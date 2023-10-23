// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

/*ProgressBar UI ������ �����ϱ� ����*/
#include "Components/ProgressBar.h"

/* ü�¹ٿ� Value�� �������ִ� �Լ��� �����մϴ�.*/
void UHealthBar::SetHealthBarValue(float Value)
{
	/*ü�¹��� value�� �ۼ�Ʈ�� �������ݴϴ�.
	���ӿ����� ���� 0~1 ������ ��ֶ���¡(����ȭ)�� ������ ó���մϴ�.*/
	HealthBar->SetPercent(Value);

}


// �����ڿ� �ش��ϴ� �Լ��� �����մϴ�.
void UHealthBar :: NativeConstruct() {

	//�θ� Ŭ������ �Լ��� ������ ��� �����ɴϴ�.
	Super::NativeConstruct();

	/*������ UProgressBar�� ����ȯ ���ݴϴ�.*/
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));


}