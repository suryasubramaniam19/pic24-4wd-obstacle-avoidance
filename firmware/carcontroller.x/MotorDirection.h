#ifndef MOTORDIRECTION_H
#define	MOTORDIRECTION_H

    

void motor_init(void);
void set_car_speed(int left_speed, int right_speed);
void car_forward(int speed);
void car_backward(int speed);
void car_turn_left(int speed);
void car_turn_right(int speed);
void autodrive(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
