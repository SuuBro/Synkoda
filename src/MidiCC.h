#ifndef midi_cc_h_
#define midi_cc_h_

// BANK 1A                          // Sub       FM
const int MIDICC_OSCP1 = 85;        // Vol1      Vol1
const int MIDICC_OSCP2 = 86;        // Vol2      Vol2
const int MIDICC_OSCP3 = 87;        // PW        Feedback
const int MIDICC_OSCP4 = 88;        // Tranpose  Transpose

// BANK 1B
const int MIDICC_MODP1 = 74;        // LPFCutoff Mod1Level
const int MIDICC_MODP2 = 71;        // LPFRes    Mod1Transpose
const int MIDICC_MODP3 = 81;        // HPFCutoff Mod2Level
const int MIDICC_MODP4 = 89;        // HPFRes    Mod2Tanspose


// BANK 2A
const int MIDICC_ENV1ATTACK = 73;
const int MIDICC_ENV1DECAY = 80;
const int MIDICC_ENV1SUSTAIN = 75;
const int MIDICC_ENV1RELEASE = 72;

// BANK 2B
const int MIDICC_ENV2ATTACK = 76;
const int MIDICC_ENV2DECAY = 78;
const int MIDICC_ENV2SUSTAIN = 79;
const int MIDICC_ENV2RELEASE = 77;


// BANK 3A
const int MIDICC_VOLUME = 7;
const int MIDICC_PAN = 10;
const int MIDICC_EQBASS = 82;
const int MIDICC_EQTREBLE = 83;

// BANK 3B
const int MIDICC_LFO1RATE = 112;
const int MIDICC_LFO2RATE = 113;
const int MIDICC_VIBRATO = 92;
const int MIDICC_PORTAMENTO = 84;


// BANK 4A
const int MIDICC_DELAYAMOUNT = 110;
const int MIDICC_DELAYRATE = 111;
const int MIDICC_REVERBAMOUNT = 91;
const int MIDICC_SIDECHAINAMOUNT = 114;

// BANK 4B
const int MIDICC_BITCRUSH = 105;
const int MIDICC_DECIMATE = 106;
const int MIDICC_CHORUSAMOUNT = 93;
const int MIDICC_CHORUSRATE = 103;

#endif