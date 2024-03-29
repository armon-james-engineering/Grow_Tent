/*
 * 
 * 001GrowTent
 * TempHumidity.h
 * 
 * File Purpose:
 *
 *  Created on: 2 Apr 2022
 *  Author: Armon H. James
 */

#ifndef INC_TEMPHUMIDITY_CONTROL_H_
#define INC_TEMPHUMIDITY_CONTROL_H_

/* Define macros for DHT22 communication */

#define DHT22_START_COMM_TIME				2000U		//Time to send initialise pulse to DHT22 = 20ms
#define DHT22_WAIT_FOR_RESPONSE_TIME		100U			//Maximum time to wait for complete flag from Input Capture ~30ms


/* Read sequence check status */
#define DHT22_NOT_STARTED					0x00U		//Read sequence not started.
#define DHT22_INIT_READ						0x01U		//Read sequence has been completed
#define DHT22_WAIT_FOR_ACK					0x03U		//Wait for acknowledge from sensor
#define DHT22_CHECK_ACK						0x04U		//Check acknowledgment time from sensor
#define DHT22_READ_DATA						0x05U		//Data read is complete
#define DHT22_TEMPERATURE_ASCII				0x06U		//Temperature Data
#define DHT22_RH_ASCII						0x07U		//Relative Humidity
#define DHT22_CHSUM_ASCII					0x08U		//Checksum

#define DHT22_INIT_ERROR					0x00U		//Error on the data line. No pull up
#define DHT22_WAIT_FOR_ACK_ERROR			0x01U		//Timeout waiting for acknowledge
#define DHT22_CHECK_ACK_ERROR				0x02U		//Acknowledge pulse too long
#define DHT22_READ_ERROR					0x03U		//Read data error
#define DHT22_CHKSUM_ERROR					0x04U		//Checksum Error

#define DHT22_ACK_LOW_POS					0U			//This number is the LOW acknowledgement bit
#define DHT22_ACK_HIGH_POS					1U			//This number is the HIGH acknowledgement bit
#define DHT22_DATA_START_BIT_POS			2U			//This number is where the data bits start
#define DHT22_EXPECTED_DATA					40U			//Number of bits of data expected after acknowledgement
#define DHT22_ACK_LOW_L						60000U		//Minimum length of acknowledgement bit LOW
#define DHT22_ACK_LOW_H						95000U		//Maximum length of acknowledgement bit	HIGH
#define DHT22_ACK_HIGH_L					60000U		//Minimum length of acknowledgement bit	LOW
#define DHT22_ACK_HIGH_H					95000U		//Maximum length of acknowledgement bit	HIGH
#define DHT22_PRE_L							35000U		//Data line being held LOW for minimum 42us and
#define DHT22_PRE_H							49000U		//43us is preceding low line before data bit
#define DHT22_DATA_LOW_L					10000U		//Data line being held LOW for minimum 12us and
#define DHT22_DATA_LOW_H					30000U		//30us is LOW
#define DHT22_DATA_HIGH_L					50000U		//Data line being held HIGH for minimum 50us and
#define DHT22_DATA_HIGH_H					80000U		//60us is HIGH

#define DHT22_STORED_VALUE_NUMBER			5U			//Number of actual integer values after reading completed
#define DHT22_NUMBER_OF_SENSORS				2U			//Number of sensors attached to system
#define DHT22_PRINT_AVERAGE					255U		//Tell system to print average
#define	DHT22_RH_INTEGRAL_START				0U			//Start of Relative Humidity data bits in 40bit data stream
#define	DHT22_RH_FRACTIONAL_START			8U			//Start of Relative Humidity data bits in 40bit data stream
#define	DHT22_TEMP_INTEGRAL_START			16U			//Start of Temperature data bits in 40bit data stream
#define	DHT22_TEMP_FRACTIONAL_START			24U			//Start of Temperature data bits in 40bit data stream
#define	DHT22_CHSUM_START					32U			//Start of check sum

/* Private function prototypes -----------------------------------------------*/
void TempHumid_Init();
int TempHumid_Read(uint8_t sensorNumber);

#endif /* INC_TEMPHUMIDITY_CONTROL_H_ */
