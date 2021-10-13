#ifndef _VR_GLOVE
#define _VR_GLOVE
#include "main.h"
#define LED_OFF GPIO_PIN_SET
#define LED_ON GPIO_PIN_RESET
#define ANALOG_MAX 1023
typedef struct
{
	uint16_t finger_position[5];
	uint16_t flex_sensor_rawdata[7];
	uint16_t joy_axis[2];
	uint16_t joy_button;
	uint16_t trigger;
	uint16_t grub;
	uint16_t pinch;
	uint16_t calib;
	uint16_t A_button;
	uint16_t B_button;
	uint16_t menu_button;
	uint16_t finger_zero[5];
	uint16_t finger_calibr[5];
	char serializedData[75];
	ADC_HandleTypeDef* hadc1;


}VR_Glove;
int16_t VR_Glove_Calibration(VR_Glove*);
int16_t VR_Glove_Input(VR_Glove*);
int16_t VR_Glove_Scale(VR_Glove*);
int16_t VR_Glove_Serialization(VR_Glove*);
int16_t VR_Glove_Transmit(VR_Glove*);
int16_t VR_Glove_Gestures(VR_Glove*);

#endif
