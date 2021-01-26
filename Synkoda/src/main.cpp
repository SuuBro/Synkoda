#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <WS2812Serial.h>

#include "AudioSynthPatch.h"
#include "Gaussian.h"
#include "ViewLevel.h"

const int numled = 80;
const int pin = 24;
byte drawingMemory[numled*3];         //  3 bytes per LED
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED
WS2812Serial pixels(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

Encoder myEnc0(25, 26);
Encoder myEnc1(27, 28);
Encoder myEnc2(33, 34);
Encoder myEnc3(29, 30);
Encoder myEnc4(32, 31);

const int numInputs = 5;

Encoder* encoders[numInputs] = {&myEnc0, &myEnc1, &myEnc2, &myEnc3, &myEnc4};
long ccAssignments[numInputs] = {101, 102, 103, 104, 105};

ViewLevel lev0(&pixels, 64, 6, 0,   10, 10);
ViewLevel lev1(&pixels, 48, 5, 0,   6, 23);
ViewLevel lev2(&pixels, 16, 0, 0,   3, 16);
ViewLevel lev3(&pixels, 32, 1, 0,   0, 8);
ViewLevel lev4(&pixels, 0, 13, 3,  10, 10);

ViewLevel* levelIndicators[numInputs] = {&lev0, &lev1, &lev2, &lev3, &lev4};

int buttonPins[numInputs] = {37, 38, 41, 39, 40};
int buttonStates[numInputs] = {-999, -999, -999, -999, -999};

//a table to store CC values coming in from the usbMIDI interface
//later any value can be recalled for each parameter
int ccValues[128] = {0};
Gaussian jitterSource;
float jitterTrack;

void MIDIccData(int CCnumber, float ccValue)
{
  //assign values to the CC lookup table
  ccValues[CCnumber] = ccValue;
}

void OnControlChange (byte Channel, byte control, byte value)
{
  MIDIccData(control, value);
}

void setup(void)
{
  Serial.println("SETUP");

  for(int i = 0; i < numInputs; ++i)
  {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }

  pixels.begin();
  pixels.show();

  AudioMemory(320);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  ccValues[100] = 64;
  ccValues[101] = 64;
  ccValues[102] = 64;
  ccValues[103] = 64;
  ccValues[104] = 64;

  usbMIDI.setHandleControlChange(OnControlChange);

  envelopeL1.noteOn();
  envelopeR1.noteOn();
  envelopeL1.release(1000);
  envelopeR1.release(990);

  filterL1.resonance(0.7);
  filterL1.frequency(10000);
  filterR1.resonance(0.7);
  filterR1.frequency(10000);

  filterFreqLfo.offset(0);
  filterFreqLfo.begin(0.3f, 0.15, WAVEFORM_TRIANGLE);

  oscFreqLfo1.begin(0.001f, 0.2f, WAVEFORM_SINE);
  oscFreqLfo2.begin(0.001f, 0.2f, WAVEFORM_SINE);

  oscAL1.begin(1.0f, ccValues[104], WAVEFORM_SQUARE);
  oscAL1.frequencyModulation(1);
  oscBL1.begin(1.0f, ccValues[104], WAVEFORM_TRIANGLE);
  oscBL1.frequencyModulation(1);
  oscCL1.begin(1.0f, ccValues[104], WAVEFORM_SQUARE);
  oscCL1.frequencyModulation(1);

  oscAR1.begin(1.0f, ccValues[104], WAVEFORM_SQUARE);
  oscAL1.frequencyModulation(1);
  oscBR1.begin(1.0f, ccValues[104], WAVEFORM_TRIANGLE);
  oscBL1.frequencyModulation(1);
  oscCR1.begin(1.0f, ccValues[104], WAVEFORM_SQUARE);
  oscCL1.frequencyModulation(1);

  freeverbL1.roomsize(0.5);
  freeverbL1.damping(0.5);
  freeverbR1.roomsize(0.5);
  freeverbR1.damping(0.5);

  Serial.println("SETUP FINISHED");
}

float jitter()
{
  int jitterInput = ccValues[100];
  if(jitterInput < 1)
  {
    return 0;
  }
  else {
      jitterSource = Gaussian(0, jitterInput);
      float jitter = jitterSource.random()/1000.0f;
      if(abs(jitterTrack) > jitterInput){ // Head toward 0
        jitterTrack += jitterTrack < 0.0f ? abs(jitter) : -1*abs(jitter);
      }
      else{ // Head randomly
        jitterTrack += jitter;
      }
      return jitterTrack != 0 ? jitterTrack/128.0f : 0.0f;
  }
}

void updateFilterFreq(float jitL, float jitR)
{
  float filterFreq = map(ccValues[101], 0, 127, 0, 5000);
  filterL1.frequency(filterFreq - 20 + jitL*60.0);
  filterR1.frequency(filterFreq - 20 - jitR*60.0);
}

float maxCpu = 0;

void loop() {

  usbMIDI.read();

  for(int i = 0; i < numInputs; ++i)
  {
    int newBtnState = digitalRead(buttonPins[i]);
    if(newBtnState != buttonStates[i])
    {
      buttonStates[i] = newBtnState;
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(buttonStates[i] == 0 ? ": Off" : ": On");
      if(buttonStates[i] == 1){
        ccValues[ccAssignments[i]] = 0;
      }
    }

    long change = encoders[i]->readAndReset();
    if(change != 0)
    {
      Serial.print("Encoder ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(change);
      Serial.print(" ");
      int newValue = ccValues[ccAssignments[i]] + change;
      if(newValue < 0){
        newValue = 0;
      }
      else if(newValue >= 127){
        newValue = 127;
      }
      ccValues[ccAssignments[i]] = newValue;
    }
    levelIndicators[i]->setLevel(ccValues[ccAssignments[i]]);
  }

  float jitL = jitter();
  float jitR = jitter();

  float freq = ccValues[104];
  float oscAFreq = freq;
  float oscBFreq = oscAFreq + (ccValues[103]-64.0)/18.0;
  float oscCFreq = oscAFreq + (ccValues[103]-64.0)/16.0;

  oscAL1.frequency(max(0, oscAFreq + jitL * oscAFreq/48.0));
  oscBL1.frequency(max(0, oscBFreq - jitL * oscAFreq/31.0));
  oscCL1.frequency(max(0, oscCFreq + jitL * oscAFreq/43.0));
  oscAR1.frequency(max(0, oscAFreq + jitR * oscAFreq/37.0));
  oscBR1.frequency(max(0, oscBFreq - jitR * oscAFreq/29.0));
  oscCR1.frequency(max(0, oscCFreq + jitR * oscAFreq/47.0));

  updateFilterFreq(jitL, jitR);

  if(AudioProcessorUsageMax() > maxCpu){
    maxCpu = AudioProcessorUsageMax();
    Serial.print("CPUMAX: ");
    Serial.println(maxCpu);
  }
}
