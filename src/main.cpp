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
#include "MidiCC.h"

const int NUM_ENCODERS = 5;
const int NUM_VALUES = 4;
const int NUM_CHANNELS = 8;
const int NUM_MIDICCS = 128;

Encoder myEnc0(25, 26);
Encoder myEnc1(33, 34);
Encoder myEnc2(32, 31);
Encoder myEnc3(27, 28);
Encoder myEnc4(29, 30);

Encoder* encoders[NUM_ENCODERS] = {&myEnc0, &myEnc1, &myEnc2, &myEnc3, &myEnc4};

Display display = Display();

int buttonPins[NUM_ENCODERS] = {37, 41, 40, 38, 39};
int buttonStates[NUM_ENCODERS] = {-999, -999, -999, -999, -999};

CHSV _bankColours[NUM_VALUES] = {
  CHSV(100, 255, 255),
  CHSV(120, 255, 255),
  CHSV(160, 255, 255),
  CHSV(220, 255, 255),
};

int _bankCCMap[NUM_VALUES][NUM_VALUES] = {
  {MIDICC_OSCP1, MIDICC_OSCP2, MIDICC_OSCP3, MIDICC_OSCP4},
  {MIDICC_ENV1ATTACK, MIDICC_ENV1DECAY, MIDICC_ENV1SUSTAIN, MIDICC_ENV1RELEASE},
  {MIDICC_VOLUME, MIDICC_PAN, MIDICC_EQBASS, MIDICC_EQTREBLE},
  {MIDICC_DELAYAMOUNT, MIDICC_DELAYRATE, MIDICC_REVERBAMOUNT, MIDICC_SIDECHAINAMOUNT},
};

int _currentBank = 1;
int _ccValues[NUM_CHANNELS][NUM_MIDICCS] = {0};
int _channel = 0;
int _channelState = 0;
float _maxCpu = 0;

void OnControlChange (byte channel, byte control, byte value)
{
  _ccValues[channel][control] = value;
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

  for(int i = 0; i < NUM_ENCODERS; ++i)
  {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }

  for(int channel = 0; channel < NUM_CHANNELS; ++channel)
  for(int cc = 0; cc < NUM_MIDICCS; ++cc)
  {
    _ccValues[channel][cc] = 67;
  }

  LEDS.setBrightness(150);

  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);

  Serial.println("SETUP FINISHED");
}

void onButtonPress(int buttonIndex)
{
  if (buttonIndex < 4)
  {
    for (size_t ring = 0; ring < 4; ring++)
    {
      _currentBank = buttonIndex;
      display.setColour(ring, _bankColours[buttonIndex]);
    }
  }
}

void checkForButtonPress(int index)
{
  int newBtnState = digitalRead(buttonPins[index]);
  if (newBtnState != buttonStates[index])
  {
    buttonStates[index] = newBtnState;
    if (buttonStates[index] == 1)
    {
      onButtonPress(index);
    }
  }
}

void checkForEncoderTurn(int index)
{
  long change = encoders[index]->read();
  if (change != 0)
  {
    change = encoders[index]->readAndReset();
    Serial.print("Encoder ");
    Serial.print(index);
    Serial.print(": ");
    int cc = _bankCCMap[_currentBank][index];
    int oldValue = _ccValues[_channel][cc];
    int newValue = oldValue + change;
    if (newValue < 0)
    {
      newValue = 0;
    }
    else if (newValue >= 127)
    {
      newValue = 127;
    }
    Serial.print(newValue);
    Serial.println(" ");
    _ccValues[_channel][cc] = newValue;
    usbMIDI.sendControlChange(cc, newValue, _channel);
  }
}

void checkForChannelChange()
{
  long channelChange = encoders[4]->readAndReset();
  if(channelChange != 0){
    Serial.print("Encoder 4:");
    _channelState += channelChange;
    if(_channelState < 0){
        _channelState = 0;
    }
    else if(_channelState > 127){
      _channelState = 127;
    }
    _channel = _channelState/16;
    Serial.println(_channel);
  }
}

void loop() {

  usbMIDI.read();

  for(int i = 0; i < NUM_VALUES; ++i)
  {
    checkForButtonPress(i);
    checkForEncoderTurn(i);

    display.setLevel(i, _ccValues[_channel][_bankCCMap[_currentBank][i]]);
  }

  checkForChannelChange();

  display.setLevel(4, _channel);

  if(AudioProcessorUsageMax() > _maxCpu){
    _maxCpu = AudioProcessorUsageMax();
    Serial.print("CPUMAX: ");
    Serial.println(_maxCpu);
  }
}
