/*
 * 
 * 001GrowTent
 * Monitor_GrowTent.h
 * 
 * File Purpose:
 *
 *  Created on: 19 Apr 2022
 *  Author: Armon H. James
 */

#ifndef INC_MONITOR_GROWTENT_H_
#define INC_MONITOR_GROWTENT_H_

/* Values for operating grow tent at nominal --------------------------------------------------*/

#define HUMIDITY_LOW					60U
#define HUMIDITY_NOMINAL				65U
#define HUMIDITY_HIGH					70U
#define TEMPERATURE_LOW					21U
#define TEMPERATURE_NOMINAL				24U
#define TEMPERATURE_HIGH				26U

/* Public Functions ----------------------------------------------------------------------------*/
void GrowTent_Mode(void);

#endif /* INC_MONITOR_GROWTENT_H_ */
