#include "vr_glove.h"
#include "usb_device.h"
void ADC_Select_Channel(uint32_t ch, VR_Glove* vr_glove) {
    ADC_ChannelConfTypeDef conf = {
        .Channel = ch,
        .Rank = 1,
        .SamplingTime = ADC_SAMPLETIME_28CYCLES_5,
    };
    if (HAL_ADC_ConfigChannel(vr_glove->hadc1, &conf) != HAL_OK) {
        Error_Handler();
    }
}
int16_t VR_Glove_Calibration(VR_Glove* vr_glove)
{
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,LED_ON);
	HAL_Delay(2000);
	VR_Glove_Input(vr_glove);
	for (int i = 0; i < 5; i++)
	{
		vr_glove->finger_zero[i] = vr_glove->flex_sensor_rawdata[i];
	}
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,LED_OFF);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,LED_ON);
	HAL_Delay(2000);
	VR_Glove_Input(vr_glove);
	for (int i = 0; i < 5; i++)
	{
		vr_glove->finger_calibr[i] =(float)(1023.0/(vr_glove->flex_sensor_rawdata[i]-vr_glove->finger_zero[i]));
	}
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,LED_OFF);
	HAL_Delay(1000);
	return 0;

}

int16_t VR_Glove_Input(VR_Glove* vr_glove)
{
	uint32_t channels[7]={ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_4,ADC_CHANNEL_5,ADC_CHANNEL_6};

	vr_glove->calib = 0;
	vr_glove->A_button = 1-HAL_GPIO_ReadPin(A_GPIO_Port,A_Pin);
	vr_glove->B_button = 1-HAL_GPIO_ReadPin(B_GPIO_Port,B_Pin);
	vr_glove->menu_button = 1-HAL_GPIO_ReadPin(menu_GPIO_Port,menu_Pin);
	vr_glove->joy_button = 1-HAL_GPIO_ReadPin(joy_button_GPIO_Port,joy_button_Pin);

	for(int i = 0; i < 7; i++)
	{
		ADC_Select_Channel(channels[i],vr_glove);
		HAL_ADC_Start(vr_glove->hadc1);
		HAL_ADC_PollForConversion(vr_glove->hadc1, HAL_MAX_DELAY);
		vr_glove->flex_sensor_rawdata[i] = HAL_ADC_GetValue(vr_glove->hadc1);
		HAL_ADC_Stop(vr_glove->hadc1);
	}


	return 0;
}

int16_t VR_Glove_Scale(VR_Glove* vr_glove)
{
	int32_t d = 0;
	for (int i = 0; i < 5; i++)
	{

		d = vr_glove->flex_sensor_rawdata[i] - vr_glove->finger_zero[i];
		if (d <= 0)
		{
			vr_glove->finger_position[i] = 0;
		}
		else
		{
			vr_glove->finger_position[i] = (uint16_t)(d * vr_glove->finger_calibr[i]);
		}
		if (vr_glove->finger_position[i] > ANALOG_MAX)
		{
			vr_glove->finger_position[i] = ANALOG_MAX;
		}
	}
	vr_glove->joy_axis[0] = vr_glove->flex_sensor_rawdata[5]/4;
	vr_glove->joy_axis[1] = vr_glove->flex_sensor_rawdata[6]/4;
	return 0;

}

int16_t VR_Glove_Serialization(VR_Glove* vr_glove)
{
	sprintf(vr_glove->serializedData, "A%dB%dC%dD%dE%dF%dG%d%s%s%s%s%s%s%s%s\n",
	  vr_glove->finger_position[0], vr_glove->finger_position[1], vr_glove->finger_position[2], vr_glove->finger_position[3], vr_glove->finger_position[4],
	  vr_glove->joy_axis[0], vr_glove->joy_axis[1], vr_glove->joy_button?"H":"",
	  vr_glove->trigger?"I":"", vr_glove->A_button?"J":"", vr_glove->B_button?"K":"", vr_glove->grub?"L":"", vr_glove->pinch?"M":"", vr_glove->menu_button?"N":"", vr_glove->calib?"O":""
	  );
	return 0;
}
int16_t VR_Glove_Transmit(VR_Glove* vr_glove)
{
	//int buf_len = 75;
	CDC_Transmit_FS(vr_glove->serializedData,strlen(vr_glove->serializedData));
	return 0;
}

int16_t VR_Glove_Gestures(VR_Glove* vr_glove)
{
	vr_glove->grub = (vr_glove->finger_position[1]+vr_glove->finger_position[2]+vr_glove->finger_position[3]+vr_glove->finger_position[4]) / 4 <= ANALOG_MAX/2 ? 0:1;
	vr_glove->pinch = (vr_glove->finger_position[0]+vr_glove->finger_position[1]) / 2 <= ANALOG_MAX/2 ? 0:1;
	vr_glove->trigger = (vr_glove->finger_position[1]) <= ANALOG_MAX/2 ? 0:1;
	return 0;
}
