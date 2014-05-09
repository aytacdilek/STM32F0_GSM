#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;

//Set of basic AT commands
const char atc0[] = "AT";					// Every AT command starts with "AT"
const char atc1[] = "ATE0";                 // Disable command echo
const char atc2[] = "AT+CMGF=1";            // TXT messages
const char atc3[] = "AT+CMGS=\"";           // sends SMS to desired number
const char atc4[] = "AT+CMGR=1";            // Command for reading message from location 1 from inbox
const char atc5[] = "AT+CMGD=1,4";          // Erasing all messages from inbox
const char atc6[] = "AT+CMGL=\"ALL\"";      // Check status of received SMS

// Responses to parse
const uint8_t GSM_OK               			= 0;
const uint8_t GSM_Ready_To_Receive_Message 	= 1;
const uint8_t GSM_ERROR                    	= 2;
const uint8_t GSM_UNREAD                   	= 3;

// SMS Message string
char SMS_Message[300];

// ADC data string
char ADC1_data[6];

// phone number string
char phone_number[20] = "05056887918";

// State machine control variables
char gsm_state = 0;
char response_rcvd = 0;
char responseID = -1, response = -1, rsp;
char set_stat = 0;
char clr_stat = 0;
char PORT_flag = 0;
char Digital_O = 0;
char gsm_number = 0;
char Unread_flag;
uint8_t status_req = 0; // Status request variable

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	GL865_Init();

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
//	GM862_ON_OFF = 1;
//	Delay_ms(2500);
//	GM862_ON_OFF = 0;
	GL865_PowerOff();		// hardware reset
	Delay(2500);			// hold it at least for two seconds
	GL865_PowerOn();		// hardware set
	Delay(3000);			// Wait a while till the GSM network is configured

	// Negotiate baud rate
	while(1)
	{
		GL865_SendData(atc0);			// Send "AT" string until GSM862 sets up its baud rate
		Delay(100);						// and gets it correctly
		if(Get_Response() == GSM_OK)	// If GSM862 says "OK" on our baud rate we program can continue
			break;
	}

	GL865_SendData(atc1);		// Disable command echo
	Wait_Response(GSM_OK);

	GL865_SendData(atc2);		// Set message type as TXT
	Wait_Response(GSM_OK);

//	while(1){
//		GL865_SendData(atc5);	// Delete all messages (if any)
//		if (Get_Response() == GSM_OK) // If messages are deleted
//			break; // break from while
//		Delay(500);
//	}
	Send_Message("deneme");
	// blink as a sign that initialization is successfully completed
//	Digital_OUTPUT = 0xFF;
//	Delay_ms(500);
//	Digital_OUTPUT = 0;

	while(1) {
//		GL865_SendData(atc6);        	// Read status of the messages and read message it self
//		Delay(100);           			// Wait until the message is read
//
//		while(1) {
//			GL865_SendData(atc0);      // Wait until the module is ready
//			Delay(50);
//			if (Get_Response() == GSM_OK)
//			break;
//		}
//
//		if (Unread_flag){
//			if (PORT_flag){        // Turn ON/OFF port D LEDs if there were new commands
//				PORT_flag = 0;
//				Digital_Output = Digital_O;
//	        }
//
//			while(1) {
//				GM862_Send(atc0);    // Wait until the module is ready
//				Delay_ms(50);
//				if (Get_response() == GSM_OK)
//				break;
//			}
//
//	        if (status_req){       // Send status SMS if it's been requested
//				status_req = 0;
//				Send_Status();
//	        }
//
//	        Unread_flag = 0;
//	      }
//
//		while(1){
//			GM862_Send(atc5);  // Delete all messages (if any)
//			Delay_ms(50);
//	        if (get_response() == GSM_OK) // If messages are deleted
//	        	break;           // break from while
//	        Delay_ms(50);
//	        if (Unread_flag){  // if we have received message in mean time
//				Unread_flag = 0;
//				break;           // break from while
//	        }
//		}
//		Delay(3000);
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
void USART1_IRQHandler(void)
{
	char tmp;
	char i;

	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
	{
		// Do we have uart rx interrupt request?
		tmp = USART_ReceiveData(USART2);						// Get received byte

		// Process reception through state machine
		// We are parsing only "OK" and "> " responses
		switch (gsm_state) {
		case  0: {
			response = -1;                   					// Clear response
			if (tmp == 'O')                  					// We have 'O', it could be "OK"
				gsm_state = 1;                 					// Expecting 'K'
			if (tmp == '>')                  					// We have '>', it could be "> "
				gsm_state = 10;                					// Expecting ' '
			if (tmp == 'E')                  					// We have 'E', it could be "> "
				gsm_state = 30;                					// Expecting 'R'
			if (tmp == 'S')                  					// We have 'S', it could be "Status?" or "Set"
				gsm_state = 40;                					// Expecting 't'
			if (tmp == 'R')                  // We have 'R', it could be "RDx" or
				gsm_state = 50;                // Expecting D
			if (tmp == 'C')                  // We have 'C', it could be "Clear"
				gsm_state = 110;               // Expecting l
			if (tmp == 'U')                  // We have 'U', it could be "UNREAD"
				gsm_state = 120;               // Expecting l
			break;
		}
		case  1: {
			if (tmp == 'K') {                // We have 'K' ->
				response = GSM_OK;             // We have "OK" response
				gsm_state = 20;                // Expecting CR+LF
			}
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 10: {
			if (tmp == ' ') {
				response_rcvd = 1;             // We have "> " response
				response = GSM_Ready_To_Receive_Message; // Set reception flag
				responseID = response;         // Set response ID
			}
			gsm_state = 0;                   // Reset state machine
			break;
		}
		case 20: {
			if (tmp == 13)                   // We have 13, it could be CR+LF
				gsm_state = 21;                // Expecting LF
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 21: {
			if (tmp == 10) {                 // We have LF, response is complete
				response_rcvd = 1;             // Set reception flag
				responseID = response;         // Set response ID
			}
			gsm_state = 0;                   // Reset state machine
			break;
		}

		case 30: {
			if (tmp == 'R')                  // We have 'R', it could be "ERROR"
				gsm_state = 31;                // Expecting 'R'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 31: {
			if (tmp == 'R')                  // We have 'R', it could be "ERROR"
				gsm_state = 32;                // Expecting 'O'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 32: {
			if (tmp == 'O')                  // We have 'O', it could be "ERROR"
				gsm_state = 33;                // Expecting 'R'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 33: {
			if (tmp == 'R'){                 // We have 'R'
				response_rcvd = 1;             // We have "ERROR" response
				response = GSM_ERROR;          // Set reception flag
				responseID = response;         // Set response ID
			}
			gsm_state = 0;                   // Reset state machine
			break;
		}
		case 40: {
			if (tmp == 't')                  // We have 't', it could be "Status?"
				gsm_state = 41;                // Expecting 'a'
			else
				if (tmp == 'e')                // We have 'e'. it could be "Set"
					gsm_state = 100;
				else
					gsm_state = 0;               // Reset state machine
		}; break;
		case 41: {
			if (tmp == 'a')                  // We have 'a', it could be "Status?"
				gsm_state = 42;                // Expecting 't'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 42: {
			if (tmp == 't')                  // We have 't', it could be "Status?"
				gsm_state = 43;                // Expecting 'u'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}

		case 43: {
			if (tmp == 'u')                  // We have 'u', it could be "Status?"
				gsm_state = 44;                // Expecting 's'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}

		case 44: {
			if (tmp == 's')                  // We have 's', it could be "Status?"
				gsm_state = 45;                // Expecting '?'
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}

		case 45: {
			if (tmp == '?'){                 // We have '?'
				status_req = 1;                // Status has been requested!
			}
			gsm_state = 0;                   // Reset state machine
			break;
		}

		case 50: {
			if (tmp == 'D')                  // We have 'D', it could be "RDx"
				gsm_state = 51;                // Expecting number
			else
				gsm_state = 0;                 // Reset state machine
			break;
		}
		case 51: {
			if (tmp >= '0' && tmp <= '7'){   // We have pin number, it could be "RDx"
				if (set_stat)
					Digital_O |= 1 << (tmp - '0');
				if (clr_stat)
					Digital_O &= (0xFF ^(1 << (tmp - '0')));
			}
			PORT_flag = 1;
			gsm_state = 0;                 // Reset state machine
		}; break;

		case 100: {
			if (tmp == 't'){              // We have 't', we received set command
				set_stat = 1;               // Set following bits
				clr_stat = 0;               // Do not clear following bits
			}
			gsm_state = 0;
		}; break;
		case 110: {
			if (tmp == 'l'){              // We have 'l', it could be Clear
				gsm_state = 111;
			}
			else
				gsm_state = 0;
		}; break;
		case 111: {
			if (tmp == 'e'){              // We have 'e', it could be Clear
				gsm_state = 112;
			}
			else
				gsm_state = 0;
		}; break;
		case 112: {
			if (tmp == 'a'){              // We have 'a', it could be Clear
				gsm_state = 113;
			}
			else
				gsm_state = 0;
		}; break;
		case 113: {
			if (tmp == 'r'){              // We have 'r', we have received Clear
				clr_stat = 1;               // Clear following bits
				set_stat = 0;               // Do not set following bits
			}
			gsm_state = 0;
		}; break;
		case 120: {
			if (tmp == 'N')
				gsm_state = 121;
			else
				gsm_state = 0;
		}; break;
		case 121: {
			if (tmp == 'R')
				gsm_state = 122;
			else
				gsm_state = 0;
		}; break;
		case 122: {
			if (tmp == 'E')
				gsm_state = 123;
			else
				gsm_state = 0;
		}; break;
		case 123: {
			if (tmp == 'A')
				gsm_state = 124;
			else
				gsm_state = 0;
		}; break;
		case 124: {
			if (tmp == 'D'){
				response_rcvd = 1;             // We have "ERROR" response
				response = GSM_UNREAD;         // Set reception flag
				responseID = response;         // Set response ID
				Unread_flag = 1;
			}
			gsm_state = 0;
		}; break;
		default: {                                 // Unwanted character
			gsm_state = 0;                   // Reset state machine
		}
		}
		// parse phone number on which we should reply
		switch (gsm_number) {
		case 0 :{
			if (tmp == '"'){
				gsm_number = 1;
				i = 0;
			}
		}; break;
		case 1 :{
			if (tmp == '+'){
				phone_number[i] = tmp;
				i ++;
				gsm_number = 2;
			}
			else{
				gsm_number = 0;
				i = 0;
			}
		}; break;
		case 2 :{
			if (tmp >= '0' && tmp <= '9'){
				phone_number[i] = tmp;
				i ++;
			}
			else
				if (tmp == '"'){
					phone_number[i] = 0;
					i = 0;
					gsm_number = 0;
				}
				else{
					phone_number[0] = 0;
					i = 0;
					gsm_number = 0;
				}
		}; break;
		default : {
			phone_number[0] = 0;
			i = 0;
			gsm_number = 0;
			}
		}
	}
}

void GL865_Init(void)
{
	//RCC Configuration
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	//Configure USART2 pins:  Rx and Tx ----------------------------
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Configure USART2 setting:         ----------------------------
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	//Enable USART2
	USART_Cmd(USART2,ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//Enable RXNE interrupt

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GL865_POWERON_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GL865_POWERON_PORT, GL865_POWERON_PIN);
}

void GL865_PowerOn(void)
{
	GPIO_ResetBits(GL865_POWERON_PORT, GL865_POWERON_PIN);
}

void GL865_PowerOff(void)
{
	GPIO_SetBits(GL865_POWERON_PORT, GL865_POWERON_PIN);
}

// Send command or data to the Telit GL865 Module - (const)
void GL865_SendData(const char *s)
{
	//Send command or data string
	while(*s)
	{
		USART_SendData(USART2, (uint16_t)*s & 0x00FF);
		s++;
	}
	USART_SendData(USART2, (uint16_t) 0x000D);	// Terminatation by CR
}

// Send command or data to the Telit GL865 Module - (RAM)
void GL865_SendRAM(char *s)
{
	//Send command or data string
	while(*s)
	{
		USART_SendData(USART2, (uint16_t)*s & 0x00FF);
		s++;
	}
	USART_SendData(USART2, (uint16_t) 0x000D);	// Terminatation by CR
}

// Get GSM response, if there is any
char Get_Response(void) {
    if (response_rcvd) {
      response_rcvd = 0;
      return responseID;
    }
    else
      return -1;
}

// Wait for GSM response (infinite loop)
void Wait_Response(char rspns) {
	char test = 1;

	while (test){
		test = Get_Response();
		if ((test == rspns) || (test == GSM_ERROR))
			test = 0;
		else
			test = 1;
	}
}

// Send Status SMS
void Send_Message(char* msg){
	char atc[33];

	atc[0] = 0;                        // clear atc string
	strcat(atc, atc3);                 // atc3 command for sending messages
	strcat(atc, phone_number);         // add phone number
	strcat(atc, "\"");                 // complete AT command
	GL865_SendRAM(atc);               // send AT command for SMS sending
	Wait_Response(GSM_Ready_To_Receive_Message); // Wait for appropriate ready signal

	GL865_SendRAM(msg);               // Send message content
	USART_SendData(USART2, (uint16_t) 0x001A);                 // Send CTRL + Z as end character
	USART_SendData(USART2, (uint16_t) 0x000D);                 // Send CR
	Wait_Response(GSM_OK);             // Wait OK as confirmation that the message was sent
}

// Send status SMS to the cell phone number defined by the atc3 const string
void Send_Status(void){
	ComposeMessage(SMS_Message);
	Send_Message(SMS_Message);
}
