
#include <XBee.h>

XBee xbee = XBee();
char payload[] = "hello world\n";
Tx16Request tx = Tx16Request(0x1874, (uint8_t *)&payload[0], sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

int RXLED = 17;  // The RX LED has a defined Arduino pin
// The TX LED was not so lucky, we'll need to use pre-defined
// macros (TXLED1, TXLED0) to control that.
// (We could use the same macros for the RX LED too -- RXLED1,
//  and RXLED0.)

void setup()
{
 pinMode(RXLED, OUTPUT);  // Set RX LED as an output
 // TX LED is set as an output behind the scenes

 Serial.begin(9600); //This pipes to the serial monitor
 Serial1.begin(9600); //This is the UART, pipes to xbee
 
 xbee.setSerial(Serial1);
 
}

void loop()
{
 Serial.println("Hello world");  // Print "Hello World" to the Serial Monitor
// Serial1.println("Hello!");  // Print "Hello!" over hardware UART

 xbee.send(tx);

 // after sending a tx request, we expect a status response
 // wait up to 5 seconds for the status response
 if (xbee.readPacket(5000)) {
         	
    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getZBTxStatusResponse(txStatus);
    		
    	   // get the delivery status, the fifth byte
           if (txStatus.getStatus() == SUCCESS) {
            	// success.  time to celebrate
                Serial.println("tx: success\n");
           } else {
            	// the remote XBee did not receive our packet. is it powered on?
                Serial.println("tx: no ack\n");
           } 
        } else {
          Serial.println("tx: got unknown response");
          Serial.println(xbee.getResponse().getApiId());
          Serial.println("\n");
        }
 } else if (xbee.getResponse().isError()) {
   Serial.println("Error reading packet: ");
   Serial.println(xbee.getResponse().getErrorCode());
   Serial.println("\n");
 } else{
   Serial.println("Unknown error\n");
 }

 digitalWrite(RXLED, LOW);   // set the LED on
 TXLED0; //TX LED is not tied to a normally controlled pin
 delay(1000);              // wait for a second
 digitalWrite(RXLED, HIGH);    // set the LED off
 TXLED1;
 delay(1000);              // wait for a second
}


