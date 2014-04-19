/**
  ******************************************************************************
  * @file    stm32f0xx_gl865.h
  * @author  Aytac Dilek
  * @version V1.0.0
  * @date    08-April-2014
  * @brief   This file contains all the functions prototypes for the TELIT-GL865
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0XX_GL865_H__
#define __STM32F0XX_GL865_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_usart.h"

 /* Private typedef -----------------------------------------------------------*/
 /* Private define ------------------------------------------------------------*/
#define GL865_RST_PORT	GPIOA
#define GL865_RST_PIN	GPIO_Pin_0

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
const uint8_tGSM_UNREAD                   	= 3;

//sbit RTS at RE0_bit;
//sbit RTS_Direction at TRISE0_bit;

//char Digital_INPUT            at PORTB;
//char Digital_INPUT_Direction  at TRISB;
//char Digital_OUTPUT           at LATD;
//char Digital_OUTPUT_Direction at TRISD;

//sbit GM862_ON_OFF at RA2_bit;
//sbit GM862_ON_OFF_Direction at TRISA2_bit;

// SMS Message string
char SMS_Message[300];

// ADC data string
char ADC1_data[6];

// phone number string
uint8_t phone_number[20];

// State machine control variables
char gsm_state = 0;
char response_rcvd = 0;
char responseID = -1, response = -1, rsp;
char set_stat = 0, clr_stat = 0;
char PORT_flag = 0;
char Digital_O = 0;
char gsm_number = 0;
char Unread_flag;
//
uint8_t status_req = 0; // Status request variable

//Function prototypes
void GL865_Init(void);
void GL865_SendData(const char *s);
void GL865_SendRAM(char *s);
char Get_Response(void);
void Wait_Response(char rspns);
void Send_Message(char* msg);
uint8_t ComposeMessage(char* Message);
void Send_Status(void);
void GL865_Set(void);
void GL865_Reset(void);

#endif	//__STM32F0XX_GL865_H__
