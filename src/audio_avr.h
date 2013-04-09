/*
 * Authors (alphabetical order)
 * - Andre Bernet <bernet.andre@gmail.com>
 * - Andreas Weitl
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bryan J. Rentoul (Gruvin) <gruvin@gmail.com>
 * - Cameron Weeks <th9xer@gmail.com>
 * - Erez Raviv
 * - Gabriel Birkus
 * - Jean-Pierre Parisy
 * - Karl Szmutny
 * - Michael Blandford
 * - Michal Hlavinka
 * - Pat Mackenzie
 * - Philip Moss
 * - Rob Thomson
 * - Romolo Manfredini <romolo.manfredini@gmail.com>
 * - Thomas Husterer
 *
 * opentx is based on code named
 * gruvin9x by Bryan J. Rentoul: http://code.google.com/p/gruvin9x/,
 * er9x by Erez Raviv: http://code.google.com/p/er9x/,
 * and the original (and ongoing) project by
 * Thomas Husterer, th9x: http://code.google.com/p/th9x/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef audio_h
#define audio_h

#if defined(PCBSTD)
#define SPEAKER_ON   BUZZER_ON
#define SPEAKER_OFF  toneFreq=0; BUZZER_OFF
#endif

//audio
#define AUDIO_QUEUE_LENGTH (8)  // 8 seems to suit most alerts
#define BEEP_DEFAULT_FREQ  (70)
#define BEEP_OFFSET        (10)
#define BEEP_KEY_UP_FREQ   (BEEP_DEFAULT_FREQ+5)
#define BEEP_KEY_DOWN_FREQ (BEEP_DEFAULT_FREQ-5)

class audioQueue
{
  public:

    audioQueue();

    void play(uint8_t tFreq, uint8_t tLen, uint8_t tPause, uint8_t tFlags=0);
    void pause(uint8_t tLen);
    
    inline bool busy() { return (toneTimeLeft > 0); }

    void event(uint8_t e, uint8_t f=BEEP_DEFAULT_FREQ);

    inline void driver() {
      if (toneFreq) {
        toneCounter += toneFreq;
        if ((toneCounter & 0x80) == 0x80)
          BUZZER_ON;
        else
          BUZZER_OFF;
      }
    }

    // heartbeat is responsibile for issueing the audio tones and general square waves
    // it is essentially the life of the class.
    void heartbeat();

    inline bool empty() {
      return (t_queueRidx == t_queueWidx);
    }

  protected:
    inline uint8_t getToneLength(uint8_t tLen);

  private:
    uint8_t t_queueRidx;
    uint8_t t_queueWidx;

    uint8_t toneFreq;
    int8_t toneFreqIncr;
    uint8_t toneTimeLeft;
    uint8_t tonePause;

    // vario has less priority
    uint8_t tone2Freq;
    uint8_t tone2TimeLeft;
	  
    // queue arrays
    uint8_t queueToneFreq[AUDIO_QUEUE_LENGTH];
    int8_t queueToneFreqIncr[AUDIO_QUEUE_LENGTH];
    uint8_t queueToneLength[AUDIO_QUEUE_LENGTH];
    uint8_t queueTonePause[AUDIO_QUEUE_LENGTH];
    uint8_t queueToneRepeat[AUDIO_QUEUE_LENGTH];

    uint8_t toneCounter;

};

extern audioQueue audio;

void audioDefevent(uint8_t e);

#if defined(BUZZER)
  #define AUDIO_BUZZER(evt, bz)  do { audioDefevent(evt); beep(bz); } while(0)
#else
  #define AUDIO_BUZZER(evt, bz)  audioDefevent(evt)
#endif

#define AUDIO_KEYPAD_UP()      AUDIO_BUZZER(AU_KEYPAD_UP, 0)
#define AUDIO_KEYPAD_DOWN()    AUDIO_BUZZER(AU_KEYPAD_DOWN, 0)
#define AUDIO_MENUS()          AUDIO_BUZZER(AU_MENUS, 0)
#define AUDIO_WARNING1()       AUDIO_BUZZER(AU_WARNING1, 3)
#define AUDIO_WARNING2()       AUDIO_BUZZER(AU_WARNING2, 2)
#define AUDIO_ERROR()          AUDIO_BUZZER(AU_ERROR, 4)

#if defined(VOICE)
  #define AUDIO_TADA()           PUSH_SYSTEM_PROMPT(AU_TADA)
  #define AUDIO_TX_BATTERY_LOW() PUSH_SYSTEM_PROMPT(AU_TX_BATTERY_LOW)
  #define AUDIO_INACTIVITY()     PUSH_SYSTEM_PROMPT(AU_INACTIVITY)
  #define AUDIO_ERROR_MESSAGE(e) PUSH_SYSTEM_PROMPT((e))
  #define AUDIO_TIMER_MINUTE(t)  playDuration(t)
  #define AUDIO_TIMER_30()       PUSH_SYSTEM_PROMPT(AU_TIMER_30)
  #define AUDIO_TIMER_20()       PUSH_SYSTEM_PROMPT(AU_TIMER_20)
  #define AUDIO_TIMER_10()       PUSH_SYSTEM_PROMPT(AU_TIMER_10)
  #define AUDIO_TIMER_LT3(x)     PUSH_SYSTEM_PROMPT(AU_TIMER_LT3)
#else
  #define AUDIO_TADA()
  #define AUDIO_TX_BATTERY_LOW() AUDIO_BUZZER(AU_TX_BATTERY_LOW, 4)
  #define AUDIO_INACTIVITY()     AUDIO_BUZZER(AU_INACTIVITY, 3)
  #define AUDIO_ERROR_MESSAGE(e) AUDIO_BUZZER(AU_ERROR, 4)
  #define AUDIO_TIMER_MINUTE(t)  AUDIO_BUZZER(AU_WARNING1, 2)
  #define AUDIO_TIMER_30()       audioDefevent(AU_TIMER_30)
  #define AUDIO_TIMER_20()       audioDefevent(AU_TIMER_20)
  #define AUDIO_TIMER_10()       AUDIO_BUZZER(AU_TIMER_10, 2)
  #define AUDIO_TIMER_LT3(x)     AUDIO_BUZZER(AU_TIMER_LT3, 2)
#endif

/* TODO BUZZER mode for these events
#define AUDIO_TRIM(event, f)     { if (!IS_KEY_FIRST(event)) warble = true; beep(1); }
#define AUDIO_TRIM_MIDDLE(f)     beep(2)
#define AUDIO_TIMER_30()       { beepAgain=2; beep(2); }
#define AUDIO_TIMER_20()       { beepAgain=1; beep(2); }
*/

#define AUDIO_MIX_WARNING(x)     AUDIO_BUZZER(AU_MIX_WARNING_1+x-1, 1)
#define AUDIO_POT_STICK_MIDDLE() AUDIO_BUZZER(AU_POT_STICK_MIDDLE, 2)
#define AUDIO_VARIO_UP()         audioDefevent(AU_KEYPAD_UP)
#define AUDIO_VARIO_DOWN()       audioDefevent(AU_KEYPAD_DOWN)
#define AUDIO_TRIM_MIDDLE(f)     audio.event(AU_TRIM_MIDDLE, f)
#define AUDIO_TRIM_END(f)        AUDIO_BUZZER(f, 2)
#define AUDIO_TRIM(event, f)     audio.event(AU_TRIM_MOVE, f)
#define AUDIO_PLAY(p)            audio.event(p)
#define AUDIO_VARIO(f, t)        audio.play(f, t, 0, PLAY_BACKGROUND)

#define AUDIO_DRIVER()           audio.driver()
#define AUDIO_HEARTBEAT()        audio.heartbeat()
#define IS_AUDIO_BUSY()          audio.busy()
#define AUDIO_RESET()

#define PLAY_PHASE_OFF(phase)
#define PLAY_PHASE_ON(phase)

#endif
