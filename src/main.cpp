#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

#include "Display.h"

const int numInputs = 5;

Encoder myEnc0(25, 26);
Encoder myEnc1(33, 34);
Encoder myEnc2(32, 31);
Encoder myEnc3(27, 28);
Encoder myEnc4(29, 30);

Encoder* encoders[numInputs] = {&myEnc0, &myEnc1, &myEnc2, &myEnc3, &myEnc4};

Display display = Display();

int buttonPins[numInputs] = {37, 41, 40, 38, 39};
int buttonStates[numInputs] = {-999, -999, -999, -999, -999};

int _ccValues[128] = {0};

void OnControlChange (byte Channel, byte control, byte value)
{
  _ccValues[control] = value;
}

void OnNoteOn(byte channel, byte note, byte velocity)
{

}

void OnNoteOff(byte channel, byte note, byte velocity)
{

}

void setup(void)
{
  Serial.println("SETUP");

  for(int i = 0; i < numInputs; ++i)
  {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }

  LEDS.setBrightness(100);

  _ccValues[100] = 67;
  _ccValues[101] = 67;
  _ccValues[102] = 67;
  _ccValues[103] = 67;
  _ccValues[104] = 67;

  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);

  Serial.println("SETUP FINISHED");
}

float maxCpu = 0;

int inertia = 0;

void loop() {

  usbMIDI.read();

  for(int i = 0; i < numInputs; ++i)
  {
    int newBtnState = digitalRead(buttonPins[i]);
    if(newBtnState != buttonStates[i])
    {
      inertia = 0;
      buttonStates[i] = newBtnState;
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(buttonStates[i] == 0 ? ": Off" : ": On");
      if(buttonStates[i] == 1){
        _ccValues[100+i] = 0;
      }
    }

    long change = encoders[i]->readAndReset();
    if(change != 0)
    {
      if((change > 0 && inertia <= 0) || (change < 0 && inertia >= 0)){
        inertia = change;
        break;
      }
      inertia = change;
      Serial.print("Encoder ");
      Serial.print(i);
      Serial.print(": ");
      int newValue = _ccValues[100+i] + change;
      if(newValue < 0){
        newValue = 0;
      }
      else if(newValue >= 127){
        newValue = 127;
      }
      Serial.print(newValue);
      Serial.println(" ");
      _ccValues[100+i] = newValue;
      usbMIDI.sendControlChange(100+i, newValue, 1);
    }
    display.setLevel(i, _ccValues[100+i]);
  }

  if(AudioProcessorUsageMax() > maxCpu){
    maxCpu = AudioProcessorUsageMax();
    Serial.print("CPUMAX: ");
    Serial.println(maxCpu);
  }
}
