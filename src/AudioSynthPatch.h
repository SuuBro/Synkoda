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