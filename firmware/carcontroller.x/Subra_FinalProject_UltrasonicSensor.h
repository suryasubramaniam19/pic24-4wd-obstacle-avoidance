/* 
 * File:   Subra_FinalProject_UltrasonicSensor.h
 * Author: suryasubramaniam
 *
 * Created on April 19, 2026, 5:37 PM
 */

#ifndef SUBRA_FINALPROJECT_ULTRASONICSENSOR_H
#define	SUBRA_FINALPROJECT_ULTRASONICSENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void triggerInput(); //Sends initial trigger to sensor to start ultrasonic pulse
void initEchoCapture(); //Sets up timer 2 and input capture 1 for timestamps of edges
void obstacle_scan(void);
int Obstacle_Detection(void);
void ultra_initial(void);
void Ultrasonic_Reset(void);
void delay(int delay_in_us);
void delayms(int delay_in_ms);

#ifdef	__cplusplus
}
#endif

#endif	/* SUBRA_FINALPROJECT_ULTRASONICSENSOR_H */

