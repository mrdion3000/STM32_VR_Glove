#ifndef _VR_GLOVE
#define _VR_GLOVE
#include "main.h"
#define LED_OFF GPIO_PIN_SET //светодиод подключен к плюсу, зажигается минусом на пине. 
#define LED_ON GPIO_PIN_RESET
#define ANALOG_MAX 1023
typedef struct
{
	uint16_t finger_position[5]; //положение пальцев отмасштабированое	
	uint16_t flex_sensor_rawdata[7]; //сырые данные с датчиков и джойстика
	uint16_t joy_axis[2]; // отмасштабированые оси джойстика
	uint16_t joy_button; 
	uint16_t trigger;
	uint16_t grub;
	uint16_t pinch;
	uint16_t calib;
	uint16_t A_button;
	uint16_t B_button;
	uint16_t menu_button;
	uint16_t finger_zero[5]; //нулевые значения датчиков
	float finger_calibr[5]; //множитель для масштаба
	char serializedData[75]; //строка с данными для передачи
	ADC_HandleTypeDef* hadc1; 


}VR_Glove;
int16_t VR_Glove_Calibration(VR_Glove*); //калибровка
int16_t VR_Glove_Input(VR_Glove*); //чтение датчиков и кнопок
int16_t VR_Glove_Scale(VR_Glove*); //масштабирование
int16_t VR_Glove_Serialization(VR_Glove*); //сериализация
int16_t VR_Glove_Transmit(VR_Glove*); //передача
int16_t VR_Glove_Gestures(VR_Glove*); //вычисление жестов

#endif
