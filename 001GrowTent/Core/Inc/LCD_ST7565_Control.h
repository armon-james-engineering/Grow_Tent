/*
 * 
 * 001GrowTent
 * LCD_ST7565_Control.h
 * 
 * File Purpose:
 *
 *  Created on: 30 Jun 2022
 *  Author: Armon H. James
 */

#ifndef INC_LCD_ST7565_CONTROL_H_
#define INC_LCD_ST7565_CONTROL_H_

void LCD_ST7565_Init(void);
void LCD_ST7565_Update(void);

/* Offsets for display based on 7x9 */
#define LCD_ST7565_7x9_X_OFFSET				7															//The offset for each digit on the X axis 7x9
#define LCD_ST7565_7x9_Y_OFFSET				9															//The offset for each digit on the Y axis 7x9

#define LCD_ST7565_11x18_X_OFFSET			11															//The offset for each digit on the X axis 16x26
#define LCD_ST7565_11x18_Y_OFFSET			18															//The offset for each digit on the Y axis 16x26

#define LCD_ST7565_SPLASH_X_START			20															//Where the splash screen starts on the X axis
#define LCD_ST7565_SPLASH_Y_START			30															//Where the splash screen starts on the Y axis

#define LCD_ST7565_TIME_X_START				71															//Where the time starts on the X axis
#define LCD_ST7565_TIME_Y_START				56															//Where the time starts on the Y axis

#define LCD_ST7565_SENSOR_X_START			0															//Where the sensor starts on the X axis
#define LCD_ST7565_SENSOR_Y_START			0															//Where the sensor starts on the Y axis

#define LCD_ST7565_TEMP_X_START				0															//Where the sensor starts on the X axis
#define LCD_ST7565_TEMP_Y_START				0															//Where the sensor starts on the Y axis
#define LCD_ST7565_HUMIDITY_X_START			0															//Where the sensor starts on the Y axis
#define LCD_ST7565_HUMIDITY_Y_START			LCD_ST7565_SENSOR_Y_START+LCD_ST7565_11x18_Y_OFFSET			//Where the sensor starts on the Y axis

#define LCD_ST7565_MODE_X_START				0															//Where the mode starts on the X axis
#define LCD_ST7565_MODE_Y_START				56															//Where the mode starts on the Y axis



#endif /* INC_LCD_ST7565_CONTROL_H_ */
