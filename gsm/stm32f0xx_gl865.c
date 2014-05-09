//#include "stm32f0xx_gl865.h"
//
//USART_InitTypeDef USART_InitStructure;
//GPIO_InitTypeDef GPIO_InitStructure;
//
//void GL865_Init(void)
//{
//	//RCC Configuration
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
//	//Configure USART2 pins:  Rx and Tx ----------------------------
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//Configure USART2 setting:         ----------------------------
//	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART2, &USART_InitStructure);
//	//Enable USART2
//	USART_Cmd(USART2,ENABLE);
//
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Pin = GL865_POWERON_PIN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_ResetBits(GL865_POWERON_PORT, GL865_POWERON_PIN);
//}
//
//void GL865_PowerOn(void)
//{
//	GPIO_ResetBits(GL865_POWERON_PORT, GL865_POWERON_PIN);
//}
//
//void GL865_PowerOff(void)
//{
//	GPIO_SetBits(GL865_POWERON_PORT, GL865_POWERON_PIN);
//}
//
//// Send command or data to the Telit GL865 Module - (const)
//void GL865_SendData(const char *s)
//{
//	//Send command or data string
//	while(*s)
//	{
//		USART_SendData(USART2, (uint16_t)*s & 0x00FF);
//		s++;
//	}
//	USART_SendData(USART2, (uint16_t) 0x000D);	// Terminatation by CR
//}
//
//// Send command or data to the Telit GL865 Module - (RAM)
//void GL865_SendRAM(char *s)
//{
//	//Send command or data string
//	while(*s)
//	{
//		USART_SendData(USART2, (uint16_t)*s & 0x00FF);
//		s++;
//	}
//	USART_SendData(USART2, (uint16_t) 0x000D);	// Terminatation by CR
//}
//
//// Get GSM response, if there is any
//char Get_Response(void) {
//    if (response_rcvd) {
//      response_rcvd = 0;
//      return responseID;
//    }
//    else
//      return -1;
//}
//
//// Wait for GSM response (infinite loop)
//void Wait_Response(char rspns) {
//	char test = 1;
//
//	while (test){
//		test = Get_Response();
//		if ((test == rspns) || (test == GSM_ERROR))
//			test = 0;
//		else
//			test = 1;
//	}
//}
//
//// Send Status SMS
//void Send_Message(char* msg){
//	char atc[33];
//
//	atc[0] = 0;                        // clear atc string
//	strcat(atc, atc3);                 // atc3 command for sending messages
//	strcat(atc, phone_number);         // add phone number
//	strcat(atc, "\"");                 // complete AT command
//	GL865_SendRAM(atc);               // send AT command for SMS sending
//	Wait_Response(GSM_Ready_To_Receive_Message); // Wait for appropriate ready signal
//
//	GL865_SendRAM(msg);               // Send message content
//	USART_SendData(USART2, (uint16_t) 0x001A);                 // Send CTRL + Z as end character
//	USART_SendData(USART2, (uint16_t) 0x000D);                 // Send CR
//	Wait_Response(GSM_OK);             // Wait OK as confirmation that the message was sent
//}
//
//// Compose Status SMS
//uint8_t ComposeMessage(char* Message){
////  unsigned adc_value1, adc_value2;
////	char temp_txt[15];
////  char PORTB_temp, i, PORTD_temp;
////
//////  RC1IE_bit = 0;                // Disable Rx1 intterupts
////	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
////
////	Message[0] = '\0';
////
////	// SMS header
////	strcat(Message, "INFO:");
////	strcat(Message, "\r\n");      // Add new line (CR + LF)
////
////
////	temp_txt[0] = 0;
////	strcat(temp_txt, "PORTB ");
////	//  PORTB_temp = Digital_INPUT;
////	// Add the status of Digital Inputs to the SMS Message
////	for (i = 0; i < 8; i ++){
////		temp_txt[13 - i] = (PORTB_temp % 2) + 48;
////		PORTB_temp = PORTB_temp / 2;
////	}
////  temp_txt[14] = 0;
////  strcat(Message, temp_txt);
////  strcat(Message, "\r\n");      // Add new line (CR + LF)*/
////  //
////
////  temp_txt[0] = 0;
////  strcat(temp_txt, "PORTD ");
////  PORTD_temp = Digital_O;
////  // Add the status of Digital Inputs to the SMS Message
////  for (i = 0; i < 8; i ++){
////    temp_txt[13 - i] = (PORTD_temp % 2) + 48;
////    PORTD_temp = PORTD_temp / 2;
////  }
////  temp_txt[14] = 0;
////  strcat(Message, temp_txt);
////  strcat(Message, "\r\n");      // Add new line (CR + LF)*/
////  //
////
////  // Adding ADC values to the SMS
////  adc_value1 = ADC_Get_Sample(0);  // RA0
////  WordToStr(adc_value1, ADC1_data);
////  strcat(Message, "AN0 - ");  // AN1
////  strcat(Message, ADC1_data); // Add ADC1 data
////  strcat(Message, "\r\n");    // Add new line (CR + LF)
////  //
////
////  // SMS footer
////  strcat(Message, "End.");
////  strcat(Message, "\r\n");      // Add new line (CR + LF)
////  //
////
//////  RC1IE_bit = 1;                // Enable Rx1 intterupts
////  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
////
////  return strlen(Message);
//}
//
//// Send status SMS to the cell phone number defined by the atc3 const string
//void Send_Status(void){
//	ComposeMessage(SMS_Message);
//	Send_Message(SMS_Message);
//}
//
