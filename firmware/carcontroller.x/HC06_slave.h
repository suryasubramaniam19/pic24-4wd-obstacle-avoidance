#ifndef HC06_SLAVE_H
#define	HC06_SLAVE_H


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
int uart1_available(void); 
void bluetooth_setup(void);    
unsigned char uart1_getc(void);
void uart1_putc(const unsigned char c);
void uart1_puts(char s[]);
void Move_cmd(char direction);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

