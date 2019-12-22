#include <MIDI_Controller.h>
HairlessMIDI_Interface midiInterface;

#define INFO_LED 50
#define CONTROL_BTN 12

// Note number 60 is defined as middle C in the MIDI specification

const uint8_t CC_Unspec = 14;

int led_status = HIGH;
int current_bank_number = 0;

Digital buttons[] = {
  {2, 12, 1}, // C-1
  {3, 13, 1},
  {4, 14, 1},
  {5, 15, 1},
  {6, 16, 1},
  {7, 17, 1}  
};

Analog analogs[] = {
  {A0, MIDI_CC::Expression_Controller, 1},
  {A1, CC_Unspec, 1}
};

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

