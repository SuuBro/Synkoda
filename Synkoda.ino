#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       oscFreqLfo1;      //xy=992.5555572509766,2290.777837753296
AudioSynthWaveformModulated filterFreqLfo;  //xy=992.5555458068848,2415.7778453826904
AudioSynthWaveform       oscFreqLfo2; //xy=993.5555419921875,2335.778112411499
AudioSynthWaveformModulated oscCR1;         //xy=1211.5555419921875,2552.77783203125
AudioSynthNoiseWhite     noiseR1;        //xy=1211.5555419921875,2599.77783203125
AudioSynthWaveformModulated oscBR1;         //xy=1212.5555419921875,2511.77783203125
AudioSynthWaveformModulated oscAR1;         //xy=1213.5555419921875,2467.77783203125
AudioSynthWaveformModulated oscCL1;         //xy=1216.5555419921875,2322.77783203125
AudioSynthNoiseWhite     noiseL1;        //xy=1216.5555419921875,2369.77783203125
AudioSynthWaveformModulated oscBL1;         //xy=1217.5555419921875,2281.77783203125
AudioSynthWaveformModulated oscAL1;         //xy=1218.5555419921875,2237.77783203125
AudioMixer4              voiceMixL1;     //xy=1407.5555419921875,2353.77783203125
AudioMixer4              voiceMixR1;     //xy=1410.5555419921875,2491.77783203125
AudioEffectEnvelope      envelopeL1;     //xy=1569.5555419921875,2353.77783203125
AudioEffectEnvelope      envelopeR1;     //xy=1569.5555419921875,2491.77783203125
AudioFilterStateVariable filterR1;       //xy=1780.5555419921875,2421.77783203125
AudioFilterStateVariable filterL1;       //xy=1781.5555419921875,2359.77783203125
AudioEffectFreeverb      freeverbR1;     //xy=1946.5555419921875,2448.77783203125
AudioEffectFreeverb      freeverbL1;     //xy=1949.5555419921875,2287.77783203125
AudioMixer4              mixOutL;        //xy=2114.5555419921875,2344.77783203125
AudioMixer4              mixOutR;        //xy=2114.5555419921875,2410.77783203125
AudioOutputI2S           i2s1;           //xy=2400.5555419921875,2380.77783203125
AudioConnection          patchCord1(oscFreqLfo1, 0, oscAL1, 0);
AudioConnection          patchCord2(oscFreqLfo1, 0, oscCL1, 0);
AudioConnection          patchCord3(oscFreqLfo1, 0, oscBR1, 0);
AudioConnection          patchCord4(filterFreqLfo, 0, filterL1, 1);
AudioConnection          patchCord5(filterFreqLfo, 0, filterR1, 1);
AudioConnection          patchCord6(oscFreqLfo2, 0, oscBL1, 0);
AudioConnection          patchCord7(oscFreqLfo2, 0, oscAR1, 0);
AudioConnection          patchCord8(oscFreqLfo2, 0, oscCR1, 0);
AudioConnection          patchCord9(oscCR1, 0, voiceMixR1, 2);
AudioConnection          patchCord10(noiseR1, 0, voiceMixR1, 3);
AudioConnection          patchCord11(oscBR1, 0, voiceMixR1, 1);
AudioConnection          patchCord12(oscAR1, 0, voiceMixR1, 0);
AudioConnection          patchCord13(oscCL1, 0, voiceMixL1, 2);
AudioConnection          patchCord14(noiseL1, 0, voiceMixL1, 3);
AudioConnection          patchCord15(oscBL1, 0, voiceMixL1, 1);
AudioConnection          patchCord16(oscAL1, 0, voiceMixL1, 0);
AudioConnection          patchCord17(voiceMixL1, envelopeL1);
AudioConnection          patchCord18(voiceMixR1, envelopeR1);
AudioConnection          patchCord19(envelopeL1, 0, filterL1, 0);
AudioConnection          patchCord20(envelopeR1, 0, filterR1, 0);
AudioConnection          patchCord21(filterR1, 0, mixOutR, 1);
AudioConnection          patchCord22(filterR1, 0, freeverbR1, 0);
AudioConnection          patchCord23(filterL1, 0, freeverbL1, 0);
AudioConnection          patchCord24(filterL1, 0, mixOutL, 1);
AudioConnection          patchCord25(freeverbR1, 0, mixOutR, 0);
AudioConnection          patchCord26(freeverbL1, 0, mixOutL, 0);
AudioConnection          patchCord27(mixOutL, 0, i2s1, 0);
AudioConnection          patchCord28(mixOutR, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=2271.5555419921875,2254.77783203125
// GUItool: end automatically generated code


#include "Gaussian.h"

//a table to store CC values coming in from the usbMIDI interface
//later any value can be recalled for each parameter
float ccValues[128];
Gaussian jitterSource;
float jitterTrack;

void MIDIccData(int CCnumber, float ccValue)
{
  if(CCnumber == 105)
  {
    envelopeL1.noteOff();
    envelopeR1.noteOff();
    return;
  }
  if(CCnumber == 104)
  {
    if(ccValue == 0){ ccValues[CCnumber] = 73.42f; }
    if(ccValue == 1){ ccValues[CCnumber] = 87.31f; }
    if(ccValue == 2){ ccValues[CCnumber] = 98.00f; }
    if(ccValue == 3){ ccValues[CCnumber] = 110.00f; }
    envelopeL1.noteOn();
    envelopeR1.noteOn();
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
  
  AudioMemory(320);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  ccValues[100] = 0.0f;
  ccValues[101] = 64.0f;
  ccValues[102] = 64.0f;
  ccValues[103] = 64.0f;
  ccValues[104] = 196.00f;

  usbMIDI.setHandleControlChange(OnControlChange);

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
  float jitL = jitter();
  float jitR = jitter();

  float freq = ccValues[104]; 
  float oscAFreq = freq;
  float oscBFreq = oscAFreq + (ccValues[103]-64.0)/8.0;
  float oscCFreq = oscAFreq + (ccValues[103]-64.0)/4.0;
  
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
