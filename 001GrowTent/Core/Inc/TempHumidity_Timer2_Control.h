/*
 * 
 * 001GrowTent
 * TempHumidity_Timer2_Control.h
 * 
 * File Purpose:
 *
 *  Created on: 7 Apr 2022
 *  Author: Armon H. James
 */

#ifndef INC_TEMPHUMIDITY_TIMER2_CONTROL_H_
#define INC_TEMPHUMIDITY_TIMER2_CONTROL_H_

/* Public functions -------------------------------------------------------------------------*/

void tempHumidity_Timer_Start(uint16_t timerPeriod, uint8_t sensorNumber);
void tempHumidity_EndTimers(void);

/* Private Defines -------------------------------------------------------------------------*/


#define uS_TIMER_PRESCALER_VALUE			15U					//With a clock of 48Mhz, this gives a timer clock period of 333ns
#define ARR_1uS_VALUE						3U					//With a clock of 48Mhz and prescaler of 48Mhz, this gives 1us
#define ns_CAL_VALUE						333U				//This is the clock period as an integer used to calculate the length of a pulse
#define DHT22_EXPECTED_TRANSITIONS			82U					//Maximum number of edges once DHT22 replies

#define DHT22_SENSOR_1						TIM_CHANNEL_1		//Which channel of timer 2 is the sensor attached to. There are two TempHumidity sensors
#define DHT22_SENSOR_2						TIM_CHANNEL_2		//Which channel of timer 2 is the sensor attached to. There are two TempHumidity sensors

#endif /* INC_TEMPHUMIDITY_TIMER2_CONTROL_H_ */
