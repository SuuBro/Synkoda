#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       filterFreqLfo;     //xy=1173.6666793823242,2384.888731956482
AudioSynthWaveform       oscCR1; //xy=1306.6665878295898,2509.88885307312
AudioSynthNoiseWhite     noiseR1; //xy=1306.666576385498,2556.8887329101562
AudioSynthWaveform       oscBR1; //xy=1307.6665878295898,2468.88885307312
AudioSynthWaveform       oscAR1; //xy=1308.6665878295898,2424.88885307312
AudioSynthWaveform       oscCL1;          //xy=1311.6666259765625,2279.888916015625
AudioSynthNoiseWhite     noiseL1;         //xy=1311.6666259765625,2326.888916015625
AudioSynthWaveform       oscBL1;          //xy=1312.6666259765625,2238.888916015625
AudioSynthWaveform       oscAL1;          //xy=1313.6665878295898,2194.888916015625
AudioSynthWaveformDc     voiceEnvL1;      //xy=1485.6666259765625,2360.888916015625
AudioSynthWaveformDc     voiceEnvR1; //xy=1486.6665687561035,2448.889012336731
AudioMixer4              voiceMixR1; //xy=1488.6665687561035,2504.8888969421387
AudioMixer4              voiceMixL1;      //xy=1495.6666259765625,2298.888916015625
AudioEffectMultiply      envMulitplyL1; //xy=1659.6666259765625,2336.888916015625
AudioEffectMultiply      envMultiplyR1; //xy=1660.6666564941406,2469.8890056610107
AudioFilterStateVariable filterL1;   //xy=1862.6666259765625,2388.888916015625
AudioFilterStateVariable filterR1; //xy=1863.6666412353516,2442.8889598846436
AudioOutputI2S           i2s1;           //xy=2129.666515350342,2382.888870239258
AudioConnection          patchCord1(filterFreqLfo, 0, filterL1, 1);
AudioConnection          patchCord2(filterFreqLfo, 0, filterR1, 1);
AudioConnection          patchCord3(oscCR1, 0, voiceMixR1, 2);
AudioConnection          patchCord4(noiseR1, 0, voiceMixR1, 3);
AudioConnection          patchCord5(oscBR1, 0, voiceMixR1, 1);
AudioConnection          patchCord6(oscAR1, 0, voiceMixR1, 0);
AudioConnection          patchCord7(oscCL1, 0, voiceMixL1, 2);
AudioConnection          patchCord8(noiseL1, 0, voiceMixL1, 3);
AudioConnection          patchCord9(oscBL1, 0, voiceMixL1, 1);
AudioConnection          patchCord10(oscAL1, 0, voiceMixL1, 0);
AudioConnection          patchCord11(voiceEnvL1, 0, envMulitplyL1, 1);
AudioConnection          patchCord12(voiceEnvR1, 0, envMultiplyR1, 0);
AudioConnection          patchCord13(voiceMixR1, 0, envMultiplyR1, 1);
AudioConnection          patchCord14(voiceMixL1, 0, envMulitplyL1, 0);
AudioConnection          patchCord15(envMulitplyL1, 0, filterL1, 0);
AudioConnection          patchCord16(envMultiplyR1, 0, filterR1, 0);
AudioConnection          patchCord17(filterL1, 0, i2s1, 0);
AudioConnection          patchCord18(filterR1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1745.6666259765625,2114.888916015625
// GUItool: end automatically generated code


#include "Gaussian.h"

//a table to store CC values coming in from the usbMIDI interface
//later any value can be recalled for each parameter
float ccValues[128];
Gaussian jitterSource;
float jitterTrack;

void MIDIccData(int CCnumber, float ccValue)
{
  if(CCnumber == 104)
  {
    if(ccValue == 0){ ccValues[CCnumber] = 174.61f; }
    if(ccValue == 1){ ccValues[CCnumber] = 196.00f; }
    if(ccValue == 2){ ccValues[CCnumber] = 207.65f; }
    if(ccValue == 3){ ccValues[CCnumber] = 233.08f; }
    return;
  }
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
  
  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  ccValues[100] = 0.0f;
  ccValues[101] = 64.0f;
  ccValues[102] = 64.0f;
  ccValues[103] = 64.0f;
  ccValues[104] = 196.00f;

  usbMIDI.setHandleControlChange(OnControlChange);

  voiceEnvL1.amplitude(1);
  voiceEnvR1.amplitude(1);
  filterL1.resonance(0.7);
  filterL1.frequency(10000);
  filterR1.resonance(0.7);
  filterR1.frequency(10000);

  filterFreqLfo.offset(0);
  filterFreqLfo.begin(0.2f, 0.05, WAVEFORM_TRIANGLE);

  oscAL1.begin(1.0f, ccValues[104], WAVEFORM_SINE);
  oscBL1.begin(1.0f, ccValues[104], WAVEFORM_SAWTOOTH);
  oscCL1.begin(1.0f, ccValues[104], WAVEFORM_TRIANGLE);

  oscAR1.begin(1.0f, ccValues[104], WAVEFORM_SINE);
  oscBR1.begin(1.0f, ccValues[104], WAVEFORM_SAWTOOTH);
  oscCR1.begin(1.0f, ccValues[104], WAVEFORM_TRIANGLE);
  
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
  filterL1.frequency(filterFreq - 20 + jitL*20.0);
  filterR1.frequency(filterFreq - 20 + jitR*20.0);
}

void loop() {

  usbMIDI.read();
  float jitL = jitter();
  float jitR = jitter();

  float freq = ccValues[104]; 
  float oscAFreq = freq;
  float oscBFreq = oscAFreq + (ccValues[103]-64.0)/8.0;
  float oscCFreq = oscAFreq + (ccValues[103]-64.0)/4.0;
  
  oscAL1.frequency(oscAFreq + jitL * oscAFreq/62.0);
  oscBL1.frequency(oscBFreq + jitL * oscAFreq/31.0);
  oscCL1.frequency(oscCFreq + jitL * oscAFreq/43.0);  
  oscAR1.frequency(oscAFreq + jitR * oscAFreq/58.0);
  oscBR1.frequency(oscBFreq + jitR * oscAFreq/29.0);
  oscCR1.frequency(oscCFreq + jitR * oscAFreq/47.0);  

  updateFilterFreq(jitL, jitR);

  Serial.print("      CPUMAX: ");
  Serial.println(AudioProcessorUsageMax());
}
