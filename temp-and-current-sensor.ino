#include <math.h>
#include <VirtualWire.h>

const int led_pin = 13;
const int transmit_pin = 2;
const int receive_pin = 3;
const int transmit_en_pin = 3;
const int currentSensorIn = A5;

int temp;
int currentSensor;

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  return Temp;
}

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(2,OUTPUT);
  
    // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
}

void loop() {
  temp=int(Thermistor(analogRead(0))*10);
  currentSensor=map(analogRead(currentSensorIn),0,1023,0,255);
  
  char msg[9]; // 2 three digit numbers with sign and a field separator
  sprintf (msg,"%d %d",temp,currentSensor);
  
  Serial.println (msg);
  
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 9);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);

  delay(60000);
}
