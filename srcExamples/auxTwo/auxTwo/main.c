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

struct io_descriptor *ioUsart0;
char flagTx = NOBUSY;
int idxRcv = 0;
char bufferTrans[SIZE_BUFFER_TX];
char bufferRcv[SIZE_BUFFER_RX];
char ackFlag = 0;
char cleanBufferFlag = 0;

static void tranUsartHdl(const struct usart_async_descriptor *const io_descr) {
	/* End transmision data */
	flagTx = NOBUSY;
}

static void rcvUsartHdl(const struct usart_async_descriptor *const io_descr) {
	/* Received data */
	uint8_t dataRcv[1];
	uint8_t numDataRcv = 0;
	
	char *tmpStr;
	
	numDataRcv = io_read(ioUsart0, dataRcv, 1);
	if (1 <= numDataRcv) {
		if ( SIZE_BUFFER_RX <= idxRcv) {
			for (int i=1; SIZE_BUFFER_RX > i; i++) {
				bufferRcv[i-1] = bufferRcv[i];
			}
			bufferRcv[SIZE_BUFFER_RX-1] = dataRcv[0];
			idxRcv = SIZE_BUFFER_RX;
		} else {
			bufferRcv[idxRcv] = dataRcv[0];
			idxRcv++;
		}
		
		// Study for some valid package
		if ( (3 < idxRcv) && ( 0x0A == bufferRcv[idxRcv-1] ) && ( 0x0D == bufferRcv[idxRcv-2] )) {
			// process data
			int result = 0;
			ackFlag = NOACK;
			result = strncmp(bufferRcv, "led0aux2=", 5);
			if ( 0 == result) {
				tmpStr = getStrBetweenTwoStr(bufferRcv, "led0aux2=", "\r\n");
				result = strcmp(tmpStr, "on");
				if (0 == result) {
					// led on
					gpio_set_pin_level(LED0, false);
					ackFlag = ACK;
				}
				result = strcmp(tmpStr, "off");
				if (0 == result) {
					// led off
					gpio_set_pin_level(LED0, true);
					ackFlag = ACK;
				}
			}
			cleanBufferFlag = 1;	
		}
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tranUsartHdl);
	usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rcvUsartHdl);
	usart_async_get_io_descriptor(&USART_0, &ioUsart0);
	usart_async_enable(&USART_0);

	/* Replace with your application code */
	while (1) {
		if (cleanBufferFlag) {
			for (int i=0; i < idxRcv; i++ ) {
				bufferRcv[i] = 0;
				idxRcv = 0;
			}
			cleanBufferFlag = 0;
		}
		
		switch (ackFlag) {
			case ACK:
				if ( NOBUSY == flagTx) {
					ackFlag = NOBUSY;
					int sizeTransmit = snprintf(bufferTrans, SIZE_BUFFER_TX, "ACK\r\n");
					flagTx = BUSY;
					io_write(ioUsart0, (uint8_t *)bufferTrans, sizeTransmit);
				}
			case NOACK:
				if ( NOBUSY == flagTx ) {
					ackFlag = NOBUSY;
					int sizeTransmit = snprintf(bufferTrans, SIZE_BUFFER_TX, "NOACK\r\n");
					flagTx = BUSY;
					io_write(ioUsart0, (uint8_t *)bufferTrans, sizeTransmit);
				}
			default:
				break;
		}
	}
}
