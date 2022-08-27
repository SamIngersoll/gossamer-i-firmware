#include <Arduino.h>
#include <SPI.h>
#include "sdkconfig.h"
#include <MagAlpha.h>
#include <Haptic_DRV2605.h>

extern "C" {
    void app_main(void);
}

#define ARDUINO 1000
#define SPI_SCLK_FREQUENCY  10000000      //SPI SCLK Clock frequency in Hz
#define SPI_CS_PIN          5             //SPI CS pin

MagAlpha magAlpha;

// keeps track of what state the encoder is in
enum ENCODER_STATE {   
    ENC_IDLE = 0,
    ENC_REQUESTING = 1, 
    ENC_READING = 2 
};

enum ENCODER_STATE encoder_state;
byte reading1 = 0;
byte reading2 = 0;
bool enc_clk = 0;           // encoder SPI interface clock value
int encoder_read_count = 0; // counts which bit of the encoder SPI message we are reading
int16_t encoder_angle = 0;  // 16 bit encoder angle reading

int led_pin = 2;            // blue LED on board near USB port 
int encoder_cs_pin   = 5;   // chip select pin for the rotary encoder SPI interface
int encoder_miso_pin = 19;  // endocer ouput, controller input pin for encoder SPI interface
int encoder_mosi_pin = 23;  // controller ouput, encoder input pin for encoder SPI interface
int encoder_clk_pin  = 18;  // clock signal pin for encoder SPI interface
int encoder_mgh_pin  = 17;  // encoder pin high if magnetic field is too high (also available through spi interface)
int encoder_mgl_pin  = 16;  // encoder pin high if magnetic field is too low (also available through spi interface)

void app_main(void) {
  // we are bitbanging SPI instead of using the library
  // SPI.begin();                // initialize the SPI library
  Serial.begin(115200);
  Serial.println("Test");
  Serial.println("[SETUP] beginning setup");

  pinMode(led_pin, OUTPUT);
  pinMode(encoder_cs_pin, OUTPUT);
  pinMode(encoder_miso_pin, INPUT);
  pinMode(encoder_mosi_pin, OUTPUT);
  pinMode(encoder_clk_pin, OUTPUT);
  pinMode(encoder_mgh_pin, INPUT);
  pinMode(encoder_mgl_pin, INPUT);

  encoder_state = ENC_IDLE;

  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);



  magAlpha.begin(SPI_SCLK_FREQUENCY, MA_SPI_MODE_3, SPI_CS_PIN);

  while(1) {
    // set programmable LED on

    // -- READ ENCODER --
    // - When encoder chip select (CS) pin is pulled low, the encoder takes an angle reading
    // and begins to send the value on the next clock cycle. 
    // - The encoder will send 16 bits of angle reading, however, if angle readings are required
    // with higher frequency, can just read the first few bits (MSB first) and get a lower 
    // fidelity reading. Then, by pulling CS low again, the encoder will take another reader.

    digitalWrite(led_pin, HIGH);

    double angle;
    //Read the angle
    angle = magAlpha.readAngle();
    // Serial.print(angle, 3);
    // Serial.println();
    //Wait before the next angle measurement (not needed by the sensor, only targeted to make the output easier to read)
    delay(25);
  }
}
