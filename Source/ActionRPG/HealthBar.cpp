// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

/*ProgressBar UI 위젯과 접근하기 위한*/
#include "Components/ProgressBar.h"

/* 체력바에 Value를 설정해주는 함수를 정의합니다.*/
void UHealthBar::SetHealthBarValue(float Value)
{
	/*체력바의 value를 퍼센트로 조절해줍니다.
	게임에서는 값을 0~1 사이의 노멀라이징(정규화)된 값으로 처리합니다.*/
	HealthBar->SetPercent(Value);

}


// 생성자에 해당하는 함수를 정의합니다.
void UHealthBar :: NativeConstruct() {

	//부모 클래스의 함수의 내용을 모두 가져옵니다.
	Super::NativeConstruct();

	/*위젯을 UProgressBar로 형변환 해줍니다.*/
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));


}