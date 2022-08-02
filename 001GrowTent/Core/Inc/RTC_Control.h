/*
 * 
 * 001GrowTent
 * RTC_Control.h
 * 
 * File Purpose:
 *
 *  Created on: Apr 12, 2022
 *  Author: Armon H. James
 */

#ifndef INC_RTC_CONTROL_H_
#define INC_RTC_CONTROL_H_


/* Set data and time macros -----------------------------------------------------------*/

#define CURRENT_WEEKDAY			RTC_WEEKDAY_THURSDAY
#define CURRENT_DATE 			16
#define CURRENT_MONTH			RTC_MONTH_JULY
#define CURRENT_YEAR			22

#define CURRENT_SECONDS			00
#define CURRENT_MINUTES			2
#define CURRENT_HOURS			20

#define DAY_CYCLE_BEGIN			8
#define DAY_CYCLE_END			23

#define ALARM_DAY_HOURS			8
#define ALARM_DAY_MINUTES		0

#define ALARM_NIGHT_HOURS		00
#define ALARM_NIGHT_MINUTES		00

#define SECONDS					0
#define MINUTES					1
#define HOURS					2
#define WEEKDAY					3
#define DATE					4
#define MONTH					5
#define YEAR					6

#define DATE_ASCII				0x00
#define TIME_ASCII				0x01
#define PERIOD_ASCII			0x02
#define SEMICOLON_ASCII			0x03
#define ALARM_ASCII				0x05
#define RTC_NOT_READY_ASCII		0x06


/* Values for DS1307 RTC backup module -----------------------------------------------*/
#define DS1307_I2C_ADDRESS		(0x68 << 1)

#define	DS1307_SEC_ADDRESS		(uint8_t)0x00

/* 	Seconds
 *
 *	BIT: 76543210
 *		  | |	|_ _ _
 *		  | |   			Seconds in BCD format. 		Range 00-59
 *		  | |_ _ _ _
 *		  |_ _ _ _ _ 		10 Seconds in BCD format. 	Range 00-59
 */

#define	DS1307_MINS_ADDRESS		(uint8_t)0x01

/* 	Minutes
 *
 *	BIT: 76543210
 *		  | |	|_ _ _
 *		  | |	   			Minutes in BCD format.		Range 00-59
 *		  | |_ _ _ _
 *		  |_ _ _ _ _ 		10 Minutes in BCD format. 	Range 00-59
 */

#define	DS1307_HOURS_ADDRESS	(uint8_t)0x02

/* 	Hours
 *
 *	BIT: 76543210
 *		  |||	|_ _ _
 *		  |||	   			Hours in BCD format.		Range 01-12 +AM/PM / 00 - 23 24Hrs
 *		  |||
 *		  |||_ _ _ _		10 Minutes in BCD format. 	Range 01-12 +AM/PM / 00 - 23 24Hrs
 *		  ||
 *		  ||_ _ _ _ 		10 Hour / PM/AM
 *		  |_ _ _ _ _ 		12/24
 */

#define	DS1307_DAY_ADDRESS		(uint8_t)0x03

/* 	Day
 *
 *	BIT: 76543210
 *		    |	|_ _ _
 *		    |	   			Day in BCD format.			Range 01-07
 *		    |_ _ _ _ _
 *
 */
#define	DS1307_DATE_ADDRESS		(uint8_t)0x04

/* 	Date
 *
 *	BIT: 76543210
 *		   ||	|_ _ _
 *		   ||	   			Date in BCD format.		Range 01-31
 *		   ||_ _ _ _
 *		   |_ _ _ _ _ 		10 Date in BCD format. 	Range 01-31
 */

#define	DS1307_MONTH_ADDRESS	(uint8_t)0x05

/* 	Month
 *
 *	BIT: 76543210
 *		    |	|_ _ _
 *		    |	   			Month in BCD format.		Range 01-12
 *		    |_ _ _ _
 *		             		10 Month in BCD format. 	Range 01-12
 */

#define	DS1307_YEAR_ADDRESS		(uint8_t)0x06

/* 	Year
 *
 *	BIT: 76543210
 *		 |  |	|_ _ _
 *		 |  |	   			Year in BCD format.		Range 00-99
 *		 |  |_ _ _ _
 *		 |_ _ _ _ _ _ 		10 Year in BCD format. 	Range 00-99
 */




/* Public Functions ------------------------------------------------------------------*/
void RTC_GetTimeDate(void);
void RTC_Setup(void);

#endif /* INC_RTC_CONTROL_H_ */
