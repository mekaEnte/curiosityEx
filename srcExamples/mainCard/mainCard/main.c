#include <atmel_start.h>
#include <stdio.h>
#include <string.h>
#include <getDataFromStr.h>

#define SIZE_BUFFER_RX 128
#define SIZE_BUFFER_TX 128
#define ACK            1
#define NOACK          2
#define NOBUSY         0
#define BUSY           1

struct io_descriptor *ioSerialPC;
char flagTxPc = NOBUSY;
int idxRcvPc = 0;
char bufferTransPC[SIZE_BUFFER_TX];
char bufferRcvPC[SIZE_BUFFER_RX];
char ackFlagPc = 0;
char cleanBufferFlagPc = 0;

struct io_descriptor *ioSerialAuxTwo;
char flagTxAuxTwo = NOBUSY;
int idxRcvAuxTwo = 0;
char bufferTransAuxTwo[SIZE_BUFFER_TX];
char bufferRcvAuxTwo[SIZE_BUFFER_RX];
char ackFlagAuxTwo = 0;
char cleanBufferFlagAuxTwo = 0;

struct io_descriptor *ioSerialAuxOne;
char flagTxAuxOne = NOBUSY;
int idxRcvAuxOne = 0;
char bufferTransAuxOne[SIZE_BUFFER_TX];
char bufferRcvAuxOne[SIZE_BUFFER_RX];
char ackFlagAuxOne = 0;
char cleanBufferFlagAuxOne = 0;

typedef enum {
	LED0_NULL,
	LED0_AUX_ON,
	LED0_AUX_OFF,
	WAIT_ACK	
} cmd_aux_two_t;

cmd_aux_two_t cmd_aux_two = LED0_NULL;
cmd_aux_two_t cmd_aux_one = LED0_NULL;

static void tranUsartHdlPc(const struct usart_async_descriptor *const io_descr) {
	/* End transmision data */
	flagTxPc = NOBUSY;
}

static void rcvUsartHdlPc(const struct usart_async_descriptor *const io_descr) {
	/* Received data */
	uint8_t dataRcv[1];
	uint8_t numDataRcv = 0;
	
	char *tmpStr;
	
	numDataRcv = io_read(ioSerialPC, dataRcv, 1);
	if (1 <= numDataRcv) {
		if ( SIZE_BUFFER_RX <= idxRcvPc) {
			for (int i=1; SIZE_BUFFER_RX > i; i++) {
				bufferRcvPC[i-1] = bufferRcvPC[i];
			}
			bufferRcvPC[SIZE_BUFFER_RX-1] = dataRcv[0];
			idxRcvPc = SIZE_BUFFER_RX;
		} else {
			bufferRcvPC[idxRcvPc] = dataRcv[0];
			idxRcvPc++;
		}
		
		// Study for some valid package
		if ( (3 < idxRcvPc) && ( 0x0A == bufferRcvPC[idxRcvPc-1] ) && ( 0x0D == bufferRcvPC[idxRcvPc-2] )) {
			// process data
			int result = 0;
			ackFlagPc = NOACK;
			result = strncmp(bufferRcvPC, "led0main=", 9);
			if ( 0 == result) {
				tmpStr = getStrBetweenTwoStr(bufferRcvPC, "led0main=", "\r\n");
				result = strcmp(tmpStr, "on");
				if (0 == result) {
					// led on
					gpio_set_pin_level(LED0, false);
					ackFlagPc = ACK;
				}
				result = strcmp(tmpStr, "off");
				if (0 == result) {
					// led off
					gpio_set_pin_level(LED0, true);
					ackFlagPc = ACK;
				}
			}
			
			result = strncmp(bufferRcvPC, "led0aux1=", 9);
			if (0 == result) {
				tmpStr = getStrBetweenTwoStr(bufferRcvPC, "led0aux1=", "\r\n");
				result = strcmp(tmpStr, "on");
				if (0 == result) {
					// send command led on aux1
					cmd_aux_one = LED0_AUX_ON;
					ackFlagPc = ACK;
				}
				result = strcmp(tmpStr, "off");
				if (0 == result) {
					// send command led off aux1
					cmd_aux_one = LED0_AUX_OFF;
					ackFlagPc = ACK;
				}
			}
			
			result = strncmp(bufferRcvPC, "led0aux2=", 9);
			if (0 == result) {
				tmpStr = getStrBetweenTwoStr(bufferRcvPC, "led0aux2=", "\r\n");
				result = strcmp(tmpStr, "on");
				if (0 == result) {
					// send command led on aux2
					cmd_aux_two = LED0_AUX_ON;
					ackFlagPc = ACK;
				}
				result = strcmp(tmpStr, "off");
				if (0 == result) {
					// send command led off aux2
					cmd_aux_two = LED0_AUX_OFF;
					ackFlagPc = ACK;
				}
			}
			cleanBufferFlagPc = 1;
		}
	}
}

static void tranUsartHdlAuxTwo(const struct usart_async_descriptor *const io_descr) {
	/* End transmision data */
	flagTxAuxTwo = NOBUSY;
}

static void rcvUsartHdlAuxTwo(const struct usart_async_descriptor *const io_descr) {
	/* Received data */
	uint8_t dataRcv[1];
	uint8_t numDataRcv = 0;
	
	//char *tmpStr;
	
	numDataRcv = io_read(ioSerialAuxTwo, dataRcv, 1);
	if (1 <= numDataRcv) {
		if ( SIZE_BUFFER_RX <= idxRcvAuxTwo) {
			for (int i=1; SIZE_BUFFER_RX > i; i++) {
				bufferRcvAuxTwo[i-1] = bufferRcvAuxTwo[i];
			}
			bufferRcvAuxTwo[SIZE_BUFFER_RX-1] = dataRcv[0];
			idxRcvAuxTwo = SIZE_BUFFER_RX;
		} else {
			bufferRcvAuxTwo[idxRcvAuxTwo] = dataRcv[0];
			idxRcvAuxTwo++;
		}
		
		// Study for some valid package
		if ( (3 < idxRcvAuxTwo) && ( 0x0A == bufferRcvAuxTwo[idxRcvAuxTwo-1] ) && ( 0x0D == bufferRcvAuxTwo[idxRcvAuxTwo-2] )) {
			// process data
			int result = 0;
			ackFlagAuxTwo = NOACK;
			result = strncmp(bufferRcvAuxTwo, "ACK", 3);
			if ( 0 == result) {
				cmd_aux_two = LED0_NULL;
				ackFlagAuxTwo = ACK;
			}
			cleanBufferFlagAuxTwo = 1;
		}
	}
}

static void tranUsartHdlAuxOne(const struct usart_async_descriptor *const io_descr) {
	/* End transmision data */
	flagTxAuxOne = NOBUSY;
}

static void rcvUsartHdlAuxOne(const struct usart_async_descriptor *const io_descr) {
	/* Received data */
	uint8_t dataRcv[1];
	uint8_t numDataRcv = 0;
	
	//char *tmpStr;
	
	numDataRcv = io_read(ioSerialAuxOne, dataRcv, 1);
	if (1 <= numDataRcv) {
		if ( SIZE_BUFFER_RX <= idxRcvAuxOne) {
			for (int i=1; SIZE_BUFFER_RX > i; i++) {
				bufferRcvAuxOne[i-1] = bufferRcvAuxOne[i];
			}
			bufferRcvAuxOne[SIZE_BUFFER_RX-1] = dataRcv[0];
			idxRcvAuxOne = SIZE_BUFFER_RX;
			} else {
			bufferRcvAuxOne[idxRcvAuxOne] = dataRcv[0];
			idxRcvAuxOne++;
		}
		
		// Study for some valid package
		if ( (3 < idxRcvAuxOne) && ( 0x0A == bufferRcvAuxOne[idxRcvAuxOne-1] ) && ( 0x0D == bufferRcvAuxOne[idxRcvAuxOne-2] )) {
			// process data
			int result = 0;
			ackFlagAuxOne = NOACK;
			result = strncmp(bufferRcvAuxOne, "ACK", 3);
			if ( 0 == result) {
				cmd_aux_two = LED0_NULL;
				ackFlagAuxOne = ACK;
			}
			cleanBufferFlagAuxOne = 1;
		}
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	usart_async_register_callback(&SERIAL_PC, USART_ASYNC_TXC_CB, tranUsartHdlPc);
	usart_async_register_callback(&SERIAL_PC, USART_ASYNC_RXC_CB, rcvUsartHdlPc);
	usart_async_get_io_descriptor(&SERIAL_PC, &ioSerialPC);
	usart_async_enable(&SERIAL_PC);
	
	usart_async_register_callback(&SERIAL_AUXTWO, USART_ASYNC_TXC_CB, tranUsartHdlAuxTwo);
	usart_async_register_callback(&SERIAL_AUXTWO, USART_ASYNC_RXC_CB, rcvUsartHdlAuxTwo);
	usart_async_get_io_descriptor(&SERIAL_AUXTWO, &ioSerialAuxTwo);
	usart_async_enable(&SERIAL_AUXTWO);
	
	usart_async_register_callback(&SERIAL_AUXONE, USART_ASYNC_TXC_CB, tranUsartHdlAuxOne);
	usart_async_register_callback(&SERIAL_AUXONE, USART_ASYNC_RXC_CB, rcvUsartHdlAuxOne);
	usart_async_get_io_descriptor(&SERIAL_AUXONE, &ioSerialAuxOne);
	usart_async_enable(&SERIAL_AUXONE);
	
	/* Replace with your application code */
	while (1) {
		
		if (cleanBufferFlagPc) {
			for (int i=0; i < idxRcvPc; i++ ) {
				bufferRcvPC[i] = 0;
			}
			idxRcvPc = 0;
			cleanBufferFlagPc = 0;
		}
		
		if (cleanBufferFlagAuxTwo) {
			for (int i=0; i < idxRcvAuxTwo; i++ ) {
				bufferRcvAuxTwo[i] = 0;
			}
			idxRcvAuxTwo = 0;
			cleanBufferFlagAuxTwo = 0;
		}
		
		if (cleanBufferFlagAuxOne) {
			for (int i=0; i < idxRcvAuxOne; i++) {
				bufferRcvAuxOne[i] = 0;
			}
			idxRcvAuxOne = 0;
			cleanBufferFlagAuxOne = 0;
		}
		
		switch (ackFlagPc) {
			case ACK:
			if ( NOBUSY == flagTxPc) {
				ackFlagPc = NOBUSY;
				int sizeTransmit = snprintf(bufferTransPC, SIZE_BUFFER_TX, "ACK\r\n");
				flagTxPc = BUSY;
				io_write(ioSerialPC, (uint8_t *)bufferTransPC, sizeTransmit);
			}
			case NOACK:
			if ( NOBUSY == flagTxPc ) {
				ackFlagPc = NOBUSY;
				int sizeTransmit = snprintf(bufferTransPC, SIZE_BUFFER_TX, "NOACK\r\n");
				flagTxPc = BUSY;
				io_write(ioSerialPC, (uint8_t *)bufferTransPC, sizeTransmit);
			}
			default:
			break;
		}
		
		switch (cmd_aux_two) {
			case LED0_AUX_ON:
				if (NOBUSY == flagTxAuxTwo) {
					cmd_aux_two = WAIT_ACK;
					int sizeTransmit = snprintf(bufferTransAuxTwo, SIZE_BUFFER_TX, "led0aux2=on\r\n");
					flagTxAuxTwo = BUSY;
					io_write(ioSerialAuxTwo, (uint8_t *)bufferTransAuxTwo, sizeTransmit);
				}
				break;
			case LED0_AUX_OFF:
				if (NOBUSY == flagTxAuxTwo) {
					cmd_aux_two = WAIT_ACK;
					int sizeTransmit = snprintf(bufferTransAuxTwo, SIZE_BUFFER_TX, "led0aux2=off\r\n");
					flagTxAuxTwo = BUSY;
					io_write(ioSerialAuxTwo, (uint8_t *)bufferTransAuxTwo, sizeTransmit);
				}
				break;
			case LED0_NULL:
			default:
			break;
		}
		
		switch (cmd_aux_one) {
			case LED0_AUX_ON:
				if (NOBUSY == flagTxAuxOne) {
					cmd_aux_one = WAIT_ACK;
					int sizeTransmit = snprintf(bufferTransAuxOne, SIZE_BUFFER_TX, "led0aux1=on\r\n");
					flagTxAuxOne = BUSY;
					io_write(ioSerialAuxOne, (uint8_t *)bufferTransAuxOne, sizeTransmit);
				}
			break;
			case LED0_AUX_OFF:
				if (NOBUSY == flagTxAuxOne) {
					cmd_aux_one = WAIT_ACK;
					int sizeTransmit = snprintf(bufferTransAuxOne, SIZE_BUFFER_TX, "led0aux1=off\r\n");
					flagTxAuxOne = BUSY;
					io_write(ioSerialAuxOne, (uint8_t *)bufferTransAuxOne, sizeTransmit);
				}
			break;
			case LED0_NULL:
			default:
			break;
		}
	}
}
