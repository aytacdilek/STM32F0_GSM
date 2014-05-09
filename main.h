#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_gl865.h"

 #define GL865_POWERON_PORT	GPIOA
 #define GL865_POWERON_PIN	GPIO_Pin_0







//Function prototypes
void GL865_Init(void);
void GL865_PowerOn(void);
void GL865_PowerOff(void);
void GL865_SendData(const char *s);
void GL865_SendRAM(char *s);
char Get_Response(void);
void Wait_Response(char rspns);
void Send_Message(char* msg);
uint8_t ComposeMessage(char* Message);
void Send_Status(void);
void TimingDelay_Decrement(void);
