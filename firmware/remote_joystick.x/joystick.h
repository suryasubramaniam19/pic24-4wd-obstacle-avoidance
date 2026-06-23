/* 
 * File:   joystick.h
 * Author: tarak
 *
 * Created on April 21, 2026, 12:55 PM
 */

#ifndef JOYSTICK_H
#define	JOYSTICK_H

#ifdef	__cplusplus
extern "C" {
#endif

void initADC(); //initializes the joystick ADC reading
unsigned int readADC(int channel); //reads the x and y values (channel=0 for x; channel=1 for y)
char joystick_cmd(void);

#ifdef	__cplusplus
}
#endif

#endif	/* JOYSTICK_H */

