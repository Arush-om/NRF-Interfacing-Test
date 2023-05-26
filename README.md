# NRF-Interfacing-Test

![Screenshot (1037)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/61c130db-3627-49b7-b71f-1c39be96d517)

If you have hard-time 3d printing stuff and other materials which i have provided in this project please refer the professionals for the help, [JLCPCB](https://jlcpcb.com?from=ayu ) is one of the best company from shenzhen china they provide, PCB manufacturing, PCBA and 3D printing services to people in need, they provide good quality products in all sectors

[JLCPCB](https://jlcpcb.com?from=ayu)


Please use the following link to register an account in [JLCPCB](https://jlcpcb.com?from=ayu )

https://jlcpcb.com?from=ayu 

Pcb Manufacturing

----------

2 layers

4 layers

6 layers

jlcpcb.com/RNA

PCBA Services

[JLCPCB](https://jlcpcb.com?from=ayu ) have 350k+ Components In-stock. You don’t have to worry about parts sourcing, this helps you to save time and hassle, also keeps your costs down.

Moreover, you can pre-order parts and hold the inventory at [JLCPCB](https://jlcpcb.com?from=ayu ), giving you peace-of-mind that you won't run into any last minute part shortages. jlcpcb.com?from=ayu 


3d printing

-------------------

SLA -- MJF --SLM -- FDM -- & SLS. easy order and fast shipping makes [JLCPCB](https://jlcpcb.com?from=ayu ) better companion among other manufactures try out [JLCPCB](https://jlcpcb.com?from=ayu ) 3D Printing servies

[JLCPCB](https://jlcpcb.com?from=ayu ) 3D Printing starts at $1 &Get $54 Coupons for new users

![Screenshot (1041)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/8d03f079-5881-4e58-acce-04d704119d55)

For explaining the wireless communication we will make two examples, the first one will be sending a simple “Hello World” message from one Arduino to another, and in the second example we will have a bi-directional communication between the Arduino boards, where using the Joystick at the first Arduino we will control the servo motor at the second Arduino, and vice versa, using the push button at the second Arduino we will control the LED at the first Arduino.

Let’s take a closer look at the NRF24L01 transceiver module. It uses the 2.4 GHz band and it can operate with baud rates from 250 kbps up to 2 Mbps. If used in open space and with lower baud rate its range can reach up to 100 meters.

The power consumption of this module is just around 12mA during transmission, which is even lower than a single LED. The operating voltage of the module is from 1.9 to 3.6V, but the good thing is that the other pins tolerate 5V logic, so we can easily connect it to an Arduino without using any logic level converters. Three of these pins are for the SPI communication and they need to be connected to the SPI pins of the Arduino, but note that each Arduino board has different SPI pins. The pins CSN and CE can be connected to any digital pin of the Arduino board and they are used for setting the module in standby or active mode, as well as for switching between transmit or command mode. The last pin is an interrupt pin which doesn’t have to be used.

The second variation, instead of on-board antenna, it has a SMA connector and which we can attach a duck antenna for better transmission range.

![Screenshot (1042)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/dc5e03cb-752d-45f6-b13a-c0f1f592ea42)

The third variation shown here, in addition to the duck antenna, it has a RFX2401C chip which includes PA (Power Amplifier) and LNA (Low-Noise Amplifier). This amplifies the NRF24L01 signal and enables even better transmission range of up to 1000 meters in open space.

radio.openWritingPipe(addresses[1]); // 00001
radio.openReadingPipe(1, addresses[0]); // 00002
Code language: Arduino (arduino)
// at the Receiver
radio.openWritingPipe(addresses[0]); // 00002
radio.openReadingPipe(1, addresses[1]); // 00001
Code language: Arduino (arduino)
In the loop section using the radio.stopListening() function we set the first Arduino as transmitter, read and map the value of Joystick from 0 to 180, and using the radio.write() function send the data to the receiver.

radio.stopListening();
int potValue = analogRead(A0);
int angleValue = map(potValue, 0, 1023, 0, 180);
radio.write(&angleValue, sizeof(angleValue));
Code language: Arduino (arduino)
On the other side, using the radio.startListening() function we set the second Arduino as receiver and we check whether there is available data. While there is data available we will read it, save it to the “angleV” variable and then use that value to rotate the servo motor.

radio.startListening();
  if ( radio.available()) {
    while (radio.available()) {
      int angleV = 0;
      radio.read(&angleV, sizeof(angleV));
      myServo.write(angleV);
    }
Code language: Arduino (arduino)
Next, at the transmitter, we set the first Arduino as receiver and with an empty “while” loop we wait for the second Arduino the send data, and that’s the data for the state of the push button whether is pressed or not. If the button is pressed the LED will light up. So these process constantly repeats and both Arduino boards are constantly sending and receiving data.  

Once we connect the NRF24L01 modules to the Arduino boards we are ready to make the codes for both the transmitter and the receiver.

First we need to download and install the RF24 library which makes the programming less difficult. We can also install this library directly from the Arduino IDE Library Manager. Just search for “rf24” and find and install the one by “TMRh20, Avamander”.

Here are the two codes for the wireless communication and below is the description of them.

Transmitter Code

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
}

Receiver Code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}

By using the “&” before the variable name we actually set an indicating of the variable that stores the data that we want to be sent and using the second argument we set the number of bytes that we want to take from that variable. In this case the sizeof() function gets all bytes of the strings “text”. At the end of the program we will add 1 second delay.

Using the radio.write() function we can send maximum of 32 bytes at a time.

On the other side, at the receiver, in the loop section using the radio.available() function we check whether there is data to be received. If that’s true, first we create an array of 32 elements, called “text”, in which we will save the incoming data.

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}
Code language: Arduino (arduino)
Using the radion.read() function we read and store the data into the “text” variable. At the end we just print text on the serial monitor. So once we upload both programs, we can run the serial monitor at the receiver and we will notice the message “Hello World” gets printed each second.

![Screenshot (1054)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/0d255ca5-913f-494d-86e1-f6267e8176d5)
![Screenshot (1043)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/1ea8de92-5562-4297-a86e-0efe07e8429d)
![Screenshot (1045)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/8fc909e3-305b-462e-bf46-1405edea3e4e)
![Screenshot (1044)](https://github.com/Arush-om/NRF-Interfacing-Test/assets/134673721/44949df5-3157-405a-aaa4-ab8375110d50)

Troubleshooting
It’s worth noting that power supply noise is one of the most common issues people experience when trying to make successful communication with the NRF24L01 modules. Generally, RF circuits or radio frequency signals are sensitive to power supply noise. Therefore, it’s always a good idea to include a decoupling capacitor across the power supply line. The capacitor can be anything from 10uF to 100uF.
