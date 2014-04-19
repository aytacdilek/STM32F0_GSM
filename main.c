#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void){
//	ANSELA = 0;             // Set all ports as digital
//	ANSELB = 0;
//	ANSELC = 0;
//	ANSELD = 0;
//	ANSELE = 0;

//	SLRCON = 0;             // Set output slew rate on all ports at standard rate

//	ANSELA |= 0x01;         // Set A0 as Analog input
//	TRISA0_bit = 1;

//	Digital_INPUT_Direction = 0xFF;
//	Digital_OUTPUT_Direction = 0;
//	Digital_OUTPUT = 0;

	// Setup interrupts
//	RC1IE_bit = 1;          // Enable Rx1 intterupts
//	PEIE_bit = 1;           // Enable peripheral interrupts
//	GIE_bit  = 1;           // Enable global interrupts
	//

//	GM862_ON_OFF = 0;
//	GM862_ON_OFF_Direction = 0;

	//  Set RTS pin to zero (we will use only RX i TX)
//	RTS_Direction  = 0;     // RTS pin
//	RTS = 0;




	/* Setup SysTick Timer for 1 msec interrupts.
	 ------------------------------------------
	1. The SysTick_Config() function is a CMSIS function which configure:
	   - The SysTick Reload register with value passed as function parameter.
	   - Configure the SysTick IRQ priority to the lowest value (0x0F).
	   - Reset the SysTick Counter register.
	   - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
	   - Enable the SysTick Interrupt.
	   - Start the SysTick Counter.

	2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
	   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
	   SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
	   inside the stm32f0xx_misc.c file.

	3. You can change the SysTick IRQ priority by calling the
	   NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function
	   call. The NVIC_SetPriority() is defined inside the core_cm0.h file.

	4. To adjust the SysTick time base, use the following formula:

		 Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)

	   - Reload Value is the parameter to be passed for SysTick_Config() function
	   - Reload Value should not exceed 0xFFFFFF
	*/
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}

	// Turn on the GM862 module
//	GM862_ON_OFF = 1;       // hardware reset
//	Delay_ms(2500);         // hold it at least for two seconds
//	GM862_ON_OFF = 0;
	GL865_Set();
	Delay(2500);
	GL865_Reset();


//	Wait();                 // Wait a while till the GSM network is configured



	// Negotiate baud rate
	while(1) {
		GM862_Send(atc0);                 // Send "AT" string until GSM862 sets up its baud rate
		Delay(100);                    // and gets it correctly
		if (Get_Response() == GSM_OK)     // If GSM862 says "OK" on our baud rate we program can continue
		  break;
	}

	GM862_Send(atc1);        // Disable command echo
	Wait_response(GSM_OK);

	GM862_Send(atc2);        // Set message type as TXT
	Wait_response(GSM_OK);

	while(1){
		GM862_Send(atc5);      // Delete all messages (if any)
		if (get_response() == GSM_OK) // If messages are deleted
			break; // break from while
		Delay_ms(500);
	}

	// blink as a sign that initialization is successfully completed
	Digital_OUTPUT = 0xFF;
	Delay_ms(500);
	Digital_OUTPUT = 0;

	while(1) {
		GM862_Send(atc6);        // Read status of the messages and read message it self
		Delay_ms(100);           // Wait until the message is read

		while(1) {
			GM862_Send(atc0);      // Wait until the module is ready
			Delay_ms(50);
			if (Get_response() == GSM_OK)
			break;
		}

		if (Unread_flag){
			if (PORT_flag){        // Turn ON/OFF port D LEDs if there were new commands
				PORT_flag = 0;
				Digital_Output = Digital_O;
	        }

			while(1) {
				GM862_Send(atc0);    // Wait until the module is ready
				Delay_ms(50);
				if (Get_response() == GSM_OK)
				break;
			}

	        if (status_req){       // Send status SMS if it's been requested
				status_req = 0;
				Send_Status();
	        }

	        Unread_flag = 0;
	      }

		while(1){
			GM862_Send(atc5);  // Delete all messages (if any)
			Delay_ms(50);
	        if (get_response() == GSM_OK) // If messages are deleted
	        	break;           // break from while
	        Delay_ms(50);
	        if (Unread_flag){  // if we have received message in mean time
				Unread_flag = 0;
				break;           // break from while
	        }
		}
		Wait();
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
