#define USE_ROTARY_ENCODER
#include <MIDI_Controller.h>
HairlessMIDI_Interface midiInterface;

#define INFO_LED 50

#define CONTROL_BTN 12

#define ENCODER_A 18
#define ENCODER_B 19

#define MAIN_CHANNEL 1
#define ROOT_KEY 12 // C-1
                    // Note number 60 is defined as middle C in the MIDI specification

#define CC_UNSPEC_1 14
#define CC_ENCODER_1 15

int led_status = LOW;
int current_bank_number = 0;

Digital buttons[] = {
  {2, ROOT_KEY, MAIN_CHANNEL},
  {3, ROOT_KEY + 1, MAIN_CHANNEL},
  {4, ROOT_KEY + 2, MAIN_CHANNEL},
  {5, ROOT_KEY + 3, MAIN_CHANNEL},
  {6, ROOT_KEY + 4, MAIN_CHANNEL},
  {7, ROOT_KEY + 5, MAIN_CHANNEL}  
};

Analog analogs[] = {
  {A0, MIDI_CC::Expression_Controller, MAIN_CHANNEL},
  {A1, CC_UNSPEC_1, MAIN_CHANNEL}
};

#define ENCODER_MULTIPLICATION 1
RotaryEncoder encoder1(ENCODER_A, ENCODER_B, CC_ENCODER_1, MAIN_CHANNEL, ENCODER_MULTIPLICATION, NORMAL_ENCODER, TWOS_COMPLEMENT);

Bank bank(12);

void setup() {
  for(int i = 0; i < sizeof(buttons) / sizeof(Digital); i++) {
    bank.add(buttons, Bank::CHANGE_ADDRESS);
  }
  
  pinMode(CONTROL_BTN, INPUT_PULLUP);
  pinMode(INFO_LED, OUTPUT);
}

void loop() {
  // Refresh the button (check whether the button's state has changed since last time, if so, send it over MIDI)
  MIDI_Controller.refresh();

  if (digitalRead(CONTROL_BTN) == LOW) {
    make_led_signal(2);
    adjust_bank();
  }
}

void adjust_bank() {
    ++current_bank_number;
    if(3 == current_bank_number) {
      current_bank_number = 0;
    }
    bank.setBankSetting(current_bank_number);
}
void make_led_signal(uint8_t times) {
  for(int i = 0; i < 2*times; i++) {
    led_status = 1 - led_status;
    digitalWrite(INFO_LED, led_status);
    delay(200);
  }
}