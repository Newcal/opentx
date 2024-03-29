/*
 * Authors (alphabetical order)
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bernet Andre <bernet.andre@gmail.com>
 * - Helle <hrenz62961@aol.com>
 * - Kjell Kernen <kjell.kernen@gmail.com>
 * - Martin Hotar <mhotar@gmail.com>
 * - Romolo Manfredini <romolo.manfredini@gmail.com>
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
 *
 * !!!!! DO NOT EDIT de.h - EDIT de.h.txt INSTEAD !!!!!!!
 *
 * In order to make translations easier de.h.txt is parsed and national
 * characters are replaced by bitmap codes. The result is de.h.
 * 
 * See translate.py in the util catalog for the list of character codes  
 *
 * !!!!! DO NOT EDIT de.h - EDIT de.h.txt INSTEAD !!!!!!!   
 */

 
// NON ZERO TERMINATED STRINGS
#define LEN_OFFON              "\003"
#define TR_OFFON               "AUS""EIN"

#define LEN_MMMINV             "\003"
#define TR_MMMINV              "---""INV"

#define LEN_NCHANNELS          "\004"
#define TR_NCHANNELS           "\0014CH\0016CH\0018CH10CH12CH14CH16CH"

#define LEN_VBEEPMODE          "\005"
#define TR_VBEEPMODE           "Stumm""Alarm""NoKey""Alle\0"

#define LEN_VBEEPLEN           "\005"
#define TR_VBEEPLEN            "0====""=0===""==0==""===0=""====0"

#define LEN_VRENAVIG           "\003"
#define TR_VRENAVIG            "AUSDGaDGb"

#define LEN_VBLMODE            "\005"
#define TR_VBLMODE             "AUS\0 ""Taste""Stks\0""Beide""EIN\0 " // Anpassung

#define LEN_TRNMODE            "\003"
#define TR_TRNMODE             "AUS"" +="" :="

#define LEN_TRNCHN             "\003"
#define TR_TRNCHN              "CH1CH2CH3CH4"

#define LEN_DATETIME           "\005"
#define TR_DATETIME            "DATE:""ZEIT:"

#define LEN_VLCD               "\006"
#define TR_VLCD                "NormalOptrex"

#define LEN_COUNTRYCODES       TR("\002", "\007")
#define TR_COUNTRYCODES        TR("US""JP""EU", "Amerika""Japan\0 ""Europa\0")

#define LEN_VTRIMINC           TR("\006","\013")
#define TR_VTRIMINC            TR("Expo  ""ExFein""Fein  ""Mittel""Grob  ","Exponential""Extra Fein   ""Fein       ""Mittel     ""Grob       ")

#define LEN_RETA123            "\001"

#if defined(PCBGRUVIN9X)
  #if ROTARY_ENCODERS > 2
    #define TR_RETA123         "SHGQ123abcd"
  #else
    #define TR_RETA123         "SHGQ123ab"
  #endif
#elif defined(PCBTARANIS)
  #define TR_RETA123           "SHGQ12LR"
#else
  #define TR_RETA123           "SHGQ123"
#endif

#define LEN_VPROTOS            "\006"

#if defined(PXX)
  #define TR_PXX               "PXX\0  "
#elif defined(DSM2) || defined(IRPROTOS)
  #define TR_PXX               "[PXX]\0"
#else
  #define TR_PXX
#endif

#if defined(DSM2)
  #define TR_DSM2              "LP45\0 ""DSM2\0 ""DSMX\0 "
#elif defined(IRPROTOS)
  #define TR_DSM2              "[LP45]""[DSM2]""[DSMX]"
#else
  #define TR_DSM2
#endif

#if defined(IRPROTOS)
  #define TR_IRPROTOS          "SILV  TRAC09PICZ  SWIFT\0"
#else
  #define TR_IRPROTOS
#endif

#if defined(CPUARM)
  #define TR_XPPM
#else
  #define TR_XPPM              "PPM16\0""PPMsim"
#endif

#define TR_VPROTOS             "PPM\0  " TR_XPPM TR_PXX TR_DSM2 TR_IRPROTOS

#define LEN_POSNEG             "\003"
#define TR_POSNEG              "POS""NEG"

#define LEN_VCURVEFUNC         "\003"
#define TR_VCURVEFUNC          "---""x>0""x<0""|x|""f>0""f<0""|f|"

#define LEN_VMLTPX             "\010"
#define TR_VMLTPX              "Addiere ""Multipl.""Ersetze "

#define LEN_VMLTPX2            "\002"
#define TR_VMLTPX2             "+=""*="":="

#define LEN_VMIXTRIMS          "\003"
#define TR_VMIXTRIMS           "AUS""EIN""Sei""H\203e""Gas""Que" //Mixer Trim Werte

#define LEN_VCSWFUNC           "\005"
#define TR_VCSWFUNC            "---\0 ""a{x\0 ""a>x\0 ""a<x\0 ""|a|>x""|a|<x""AND\0 ""OR\0  ""XOR\0 ""a=b\0 ""a>b\0 ""a<b\0 ""d}x\0 ""|d|}x""TIM\0"

#define LEN_VFSWFUNC           "\012"

#if defined(VARIO)		
  #define TR_VVARIO            "Vario\0    " // nur 10 Zeichen String!
#else
  #define TR_VVARIO            "[Vario]\0  "
#endif

#if defined(AUDIO)
  #define TR_SOUND             "Spiel T\203ne"
#else
  #define TR_SOUND             "Spiel\0    "
#endif

#if defined(PCBTARANIS)
  #define TR_HAPTIC
#elif defined(HAPTIC)
  #define TR_HAPTIC            "Haptic\0   "
#else
  #define TR_HAPTIC            "[Haptic]\0 "
#endif

#if defined(VOICE)
  #if defined(PCBSKY9X)
    #define TR_PLAY_TRACK      "Spiel\0    "
  #else
    #define TR_PLAY_TRACK      "Sag Text\0 "
  #endif
  #define TR_PLAY_BOTH         "Sag Beide "  
  #define TR_PLAY_VALUE        "Sag Wert\0 "
#else
  #define TR_PLAY_TRACK        "[Sag Text]"
  #define TR_PLAY_BOTH         "[SagBeide]"
  #define TR_PLAY_VALUE        "[Sag Wert]"
#endif

#define TR_CFN_VOLUME          "Lautst\201rke"
#define TR_CFN_BG_MUSIC        "MusikStart""MusikPause"

#if defined(SDCARD)
  #define TR_SDCLOGS           "SD Aufz.\0 "
#else
  #define TR_SDCLOGS           "[SD Aufz.]"
#endif

#ifdef GVARS
  #define TR_CFN_ADJUST_GVAR   "\200ndere \0  "
#else
  #define TR_CFN_ADJUST_GVAR
#endif

#ifdef DEBUG
  #define TR_CFN_TEST          "Test\0"
#else
  #define TR_CFN_TEST
#endif

#if defined(CPUARM)
  #define TR_VFSWFUNC          "Sicher\0   ""Lehrer \0  ""Inst. Trim" TR_SOUND TR_HAPTIC "R\205cksetz.\0" TR_VVARIO TR_PLAY_TRACK TR_PLAY_VALUE TR_SDCLOGS TR_CFN_VOLUME "LCD Licht\0" TR_CFN_BG_MUSIC TR_CFN_ADJUST_GVAR TR_CFN_TEST
#elif defined(PCBGRUVIN9X)
  #define TR_VFSWFUNC          "Sicher\0   ""Lehrer \0  ""Inst. Trim" TR_SOUND TR_HAPTIC "R\205cksetz.\0" TR_VVARIO TR_PLAY_TRACK TR_PLAY_BOTH TR_PLAY_VALUE TR_SDCLOGS "LCD Licht\0" TR_CFN_ADJUST_GVAR TR_CFN_TEST
#else
  #define TR_VFSWFUNC          "Sicher\0   ""Lehrer \0  ""Inst. Trim" TR_SOUND TR_HAPTIC "R\205cksetz.\0" TR_VVARIO TR_PLAY_TRACK TR_PLAY_BOTH TR_PLAY_VALUE "LCD Licht\0" TR_CFN_ADJUST_GVAR TR_CFN_TEST
#endif

#define LEN_VFSWRESET          TR("\005", "\011")

#if defined(FRSKY)
  #define TR_FSW_RESET_TELEM   TR("Telem", "Telemetrie ") //\200nderung
#else
  #define TR_FSW_RESET_TELEM    
#endif

#if ROTARY_ENCODERS == 2
  #define TR_FSW_RESET_ROTENC  TR("REa\0 ""REb\0 ", "RotEnc A\0""RotEnc B\0")
#elif ROTARY_ENCODERS == 1
  #define TR_FSW_RESET_ROTENC  TR("R.Enc", "RotEnc\0  ")
#else
  #define TR_FSW_RESET_ROTENC
#endif

#define TR_VFSWRESET           TR("Tmr1\0""Tmr2\0""Alle\0" TR_FSW_RESET_TELEM TR_FSW_RESET_ROTENC, "Timer 1  ""Timer 2  ""Alle Tmr." TR_FSW_RESET_TELEM TR_FSW_RESET_ROTENC)

#define LEN_FUNCSOUNDS         TR("\004", "\006")
#define TR_FUNCSOUNDS          TR("Bp1\0""Bp2\0""Bp3\0""Wrn1""Wrn2""Chee""Rata""Tick""Sirn""Ring""SciF""Robt""Chrp""Tada""Crck""Alrm", "Piep1\0""Piep2\0""Piep3\0""Warn1 ""Warn2 ""Cheep ""Ratata""Tick  ""Siren ""Ring  ""SciFi ""Robot ""Chirp ""Tada  ""Crickt""AlmClk")

#define LEN_VTELEMCHNS         "\004"
#if defined(PCBTARANIS)
  #define TR_RSSI_0            "SWR\0"
  #define TR_RSSI_1            "RSSI"
#else
  #define TR_RSSI_0            "Tx\0 "
  #define TR_RSSI_1            "Rx\0 "
#endif
#define TR_VTELEMCHNS          "---\0""Batt""Uhr1""Uhr2" TR_RSSI_0 TR_RSSI_1 "A1\0 ""A2\0 ""H\203he""Umdr""Fuel""T1\0 ""T2\0 ""Gesc""Dist""GH\203h""Zell""Zels""Vfas""Strm""Verb""Leis""BesX""BesY""BesZ""Rich""VGes""A1-\0""A2-\0""H\203h-""H\203h+""Umd+""T1+\0""T2+\0""Ges+""Dst+""Str+""Pwr+""Besc""Zeit"

#if defined(CPUARM)
  #define LEN_VUNITSSYSTEM     TR("\006", "\010")
  #define TR_VUNITSSYSTEM      TR("Metrik""Imper.", "Metrisch\0 ""Imperial")
  #define LEN_VTELEMUNIT_NORM  "\003"
  #define TR_VTELEMUNIT_NORM   "V\0 ""A\0 ""m/s""-\0 ""kmh""m\0 ""@\0 ""%\0 ""mA\0""mAh""W\0 "
  #define LEN_VTELEMUNIT_IMP   "\003"
  #define TR_VTELEMUNIT_IMP    "V\0 ""A\0 ""m/s""-\0 ""kts""ft\0""@\0 ""%\0 ""mA\0""mAh""W\0 "
#else
  #if defined(IMPERIAL_UNITS)
    #define LENGTH_UNIT        "ft\0"
    #define SPEED_UNIT         "kts"
  #else
    #define LENGTH_UNIT        "m\0 "
    #define SPEED_UNIT         "Kmh"
  #endif
  #define LEN_VTELEMUNIT       "\003"
  #define TR_VTELEMUNIT        "V\0 ""A\0 ""m/s""-\0 " SPEED_UNIT LENGTH_UNIT "@\0 ""%\0 ""mA\0""mAh""W\0 "
#endif

#define STR_V                  (STR_VTELEMUNIT+1)
#define STR_A                  (STR_VTELEMUNIT+4)

#define LEN_VALARM             "\003"
#define TR_VALARM              "---""Gel""Org""Rot"

#define LEN_VALARMFN           "\001"
#define TR_VALARMFN            "<>"

#define LEN_VTELPROTO          "\007"
#define TR_VTELPROTO           "Kein\0  ""Hub\0   ""WSHHigh""Halcyon"

#define LEN_VOLTSRC            "\003"
#define TR_VOLTSRC             "---""A1\0""A2\0""FAS""Cel"

#define LEN_VARIOSRC           "\005"
#if defined(FRSKY_SPORT)
  #define TR_VARIOSRC          "Vario""A1\0  ""A2\0"
#else
  #define TR_VARIOSRC          "Alti\0""Alti+""Vario""A1\0  ""A2\0"
#endif

#define LEN_VSCREEN  	       "\006"
#define TR_VSCREEN   	       "Werte\0""Balken" //Telemetrie Werte oder Balken

#define LEN_GPSFORMAT          "\004"
#define TR_GPSFORMAT           "HMS\0""NMEA"  //Koordinatenanzeige

#define LEN2_VTEMPLATES        15  // max String L\201nge f\205r Men\205 (original=13)
#define LEN_VTEMPLATES         "\017"  // max String L\201nge 15+cr+lf
#define TR_VTEMPLATES          "Misch. L\203sch.\0 ""Einfach. 4-CH\0 ""Fixe Gassperre\0""V-Stabilisator\0""Delta Mischer\0 ""eCCPM\0         ""Hubschrauber\0  ""Servo Tester\0  "

#define LEN_VSWASHTYPE         "\004"
#define TR_VSWASHTYPE          "--- ""120 ""120X""140 ""90\0"

#define LEN_VKEYS              "\005"
#define TR_VKEYS               TR(" Menu"" Exit"" Down""   Up""Right"" Left", " Menu"" Exit""Enter"" Page"" Plus""Minus")

#define LEN_VRENCODERS         "\003"
#define TR_VRENCODERS          "DGa""DGb" //Digitalgeber a und b = Drehgeber

#define LEN_VSWITCHES          "\003"
#define LEN_VSRCRAW            "\004"

#if defined(PCBTARANIS)
  #define TR_POTS_VSRCRAW      "S1\0 ""S2\0 ""LS\0 ""RS\0 "  //4 Potis S1,S2,Links,Rechts  
  #define TR_SW_VSRCRAW        "SA\0 ""SB\0 ""SC\0 ""SD\0 ""SE\0 ""SF\0 ""SG\0 ""SH\0 "
#elif defined(EXTRA_3POS)
  #define TR_POTS_VSRCRAW      "P1\0 ""P2\0 "
  #define TR_SW_VSRCRAW        "3P1\0""3P2\0"
  #define TR_9X_3POS_SWITCHES  "ID0""ID1""ID2""ID3""ID4""ID5"
#else
  #define TR_POTS_VSRCRAW      "P1\0 ""P2\0 ""P3\0 "
  #define TR_SW_VSRCRAW        "3POS"
  #define TR_9X_3POS_SWITCHES  "ID0""ID1""ID2"
#endif

#if defined(CPUARM)
#define TR_CUSTOMSW          "PS1""PS2""PS3""PS4""PS5""PS6""PS7""PS8""PS9""PSA""PSB""PSC""PSD""PSE""PSF""PSG""PSH""PSI""PSJ""PSK""PSL""PSM""PSN""PSO""PSP""PSQ""PSR""PSS""PST""PSU""PSV""PSW"
#elif defined(PCBGRUVIN9X) || defined(CPUM2561) || defined(CPUM128)
  #define TR_CUSTOMSW          "PS1""PS2""PS3""PS4""PS5""PS6""PS7""PS8""PS9""PSA""PSB""PSC""PSD""PSE""PSF"
#else
  #define TR_CUSTOMSW          "PS1""PS2""PS3""PS4""PS5""PS6""PS7""PS8""PS9""PSA""PSB""PSC"
#endif

#if defined(PCBTARANIS)
  #define TR_VSWITCHES         "SA\300""SA-""SA\301""SB\300""SB-""SB\301""SC\300""SC-""SC\301""SD\300""SD-""SD\301""SE\300""SE-""SE\301""SF\300""SF\301""SG\300""SG-""SG\301""SH\300""SH\301" TR_CUSTOMSW "ONE"
#else
  #define TR_VSWITCHES         TR_9X_3POS_SWITCHES "THR""RUD""ELE""AIL""GEA""TRN" TR_CUSTOMSW "ONE"
#endif

#if defined(PCBSKY9X)
  #define TR_ROTARY_ENCODERS_VSRCRAW "Dreh"
#elif defined(PCBGRUVIN9X) && ROTARY_ENCODERS > 2
  #define TR_ROTARY_ENCODERS_VSRCRAW "DGa ""DGb ""DGc ""DGd "
#elif defined(PCBGRUVIN9X) && ROTARY_ENCODERS <= 2
  #define TR_ROTARY_ENCODERS_VSRCRAW "DGa ""DGb "
#else
  #define TR_ROTARY_ENCODERS_VSRCRAW
#endif

#if defined(HELI)
#define TR_CYC_VSRCRAW         "ZYK1""ZYK2""ZYK3"
#else
#define TR_CYC_VSRCRAW         "[C1]""[C2]""[C3]"
#endif

#define TR_VSRCRAW             "---\0""Sei\0""H\203h\0""Gas\0""Que\0" TR_POTS_VSRCRAW TR_ROTARY_ENCODERS_VSRCRAW "MAX\0" TR_CYC_VSRCRAW "TrmS" "TrmH" "TrmG" "TrmQ" TR_SW_VSRCRAW

#define LEN_VTMRMODES          "\003"
#define TR_VTMRMODES           "AUS""ABS""GSs""GS%""GSt"

#define LEN_VTRAINERMODES      "\007"                     // "\006"
#define TR_VTRAINERMODES       "Lehrer\0""Sch\205ler\0"  // "Master""Slave\0"

#define LEN_VFAILSAFE          "\011"
#define TR_VFAILSAFE           "Hold\0    ""Custom\0  ""No pulses"

#if defined(MAVLINK)
  #define LEN_MAVLINK_BAUDS		"\006"
  #define TR_MAVLINK_BAUDS		"4800  ""9600  ""14400 ""19200 ""38400 ""57600 ""76800 ""115200"
  #define LEN_MAVLINK_AC_MODES	"\011"
  #define TR_MAVLINK_AC_MODES	"Stabilize""Acro     ""Alt Hold ""Auto     ""Guided   ""Loiter   ""RTL      ""Circle   ""Pos Hold ""Land     ""OF Loiter""Toy A    ""Toy M    ""INVALID  "
  #define LEN_MAVLINK_AP_MODES	"\015"
  #define TR_MAVLINK_AP_MODES	"Manual       ""Circle       ""Stabilize    ""Training     ""Fly by Wire A""Fly by Wire A""Auto         ""RTL          ""Loiter       ""Guided       ""Initialising ""INVALID      "
#endif

// ZERO TERMINATED STRINGS
#define INDENT                 "\001"
#define LEN_INDENT             1
#define INDENT_WIDTH           (FW/2)

#if defined(PCBTARANIS)
  #define TR_ENTER             "[ENTER]"
#else
  #define TR_ENTER             "[MENU]"
#endif

#define TR_POPUPS              TR_ENTER"\010[EXIT]"
#define OFS_EXIT               sizeof(TR_ENTER)

#define TR_MENUWHENDONE        CENTER"\006"TR_ENTER" > Weiter"
#define TR_FREE                "frei"
#define TR_DELETEMODEL         "Modell l\203schen?"
#define TR_COPYINGMODEL        "Kopiere Modell"
#define TR_MOVINGMODEL         "Schiebe Modell"
#define TR_LOADINGMODEL        "Lade Modell..."
#define TR_NAME                "Name"
#define TR_MODELNAME           "Modell Name"
#define TR_PHASENAME           "Phase Name"
#define TR_MIXNAME             "Mix Name"
#define TR_EXPONAME            "Expo Name"
#define TR_BITMAP              "Modell Bild"
#define TR_TIMER               TR("Stoppuhr","Stoppuhr ")
#define TR_ELIMITS             TR("Erw.Limit","Erweit. Limits")
#define TR_ETRIMS              TR("Erw.Trims","Erweit. Trimmung")
#define TR_TRIMINC             TR("Trim Schr.","Trimm Schritte")
#define TR_TTRACE              TR("Gasquelle","Gas Quelle")
#define TR_TTRIM               TR("Gastrim","Gas Trimmung")
#define TR_BEEPCTR             TR("Mitt.Piep","Mittelst. Pieps")
#define TR_PROTO               TR(INDENT"Proto.",INDENT"Protokoll")
#define TR_PPMFRAME            "PPM Einst."
#define TR_MS                  "ms"
#define TR_SWITCH              TR("Schalt.","Schalter")
#define TR_TRIMS               "Trims"
#define TR_FADEIN              "Langs. Ein"
#define TR_FADEOUT             "Langs. Aus"
#define TR_DEFAULT             "(Normal)"
#define TR_CHECKTRIMS          "\006Trims\012Pr\205fen"
#define OFS_CHECKTRIMS         (9*FW)
#define TR_SWASHTYPE           TR("Typ Taumelsch","Typ Taumelscheibe")
#define TR_COLLECTIVE          TR("Kollekt.Pitch","Kollekt. Pitch Kanal")
#define TR_SWASHRING           TR("Zykl. Begrenz","Zyklische Begrenzung")
#define TR_ELEDIRECTION        TR("Nick Richtung"," Nick Servo Richtung")
#define TR_AILDIRECTION        TR("Roll Richtung"," Roll Servo Richtung") 
#define TR_COLDIRECTION        TR("Pit. Richtung","Pitch Servo Richtung")
#define TR_MODE                INDENT"Modus"
#define TR_NOFREEEXPO          "Expos voll!"
#define TR_NOFREEMIXER         "Mischer voll!"
#define TR_INSERTMIX           "MISCHER HINZ."
#define TR_EDITMIX             "MISCHER Edit"
#define TR_SOURCE              INDENT"Quelle"
#define TR_WEIGHT              "Gewicht"
#define TR_EXPO                TR("Expo","Exponential")
#define TR_SIDE                "Seite"
#define TR_DIFFERENTIAL        "Diff"
#define TR_OFFSET              INDENT"Offset"
#define TR_TRIM                "Trim"
#define TR_DREX                "DRex"
#define TR_CURVE               "Kurve"
#define TR_FLMODE              TR("Phase","Phasen")
#define TR_MIXWARNING          "Warnung"
#define TR_OFF                 "AUS"
#define TR_MULTPX              "Wirkung"
#define TR_DELAYDOWN           "Verz. Dn"
#define TR_DELAYUP             "Verz. Up"
#define TR_SLOWDOWN            "Langs.Dn"
#define TR_SLOWUP              "Langs.Up"
#define TR_MIXER               "MISCHER"
#define TR_CV                  "KV"
#define TR_GV                  "GV"
#define TR_ACHANNEL            "A\004Kanal  "
#define TR_RANGE               INDENT"Skala"
#define TR_BAR                 "Streifen"
#define TR_ALARM               INDENT"Alarm"
#define TR_USRDATA             "Daten"
#define TR_BLADES              INDENT"Bl\201tter"
#define TR_SCREEN              "Bild "
#define TR_SOUND_LABEL         "T\203ne"
#define TR_LENGTH	       INDENT"Dauer"
#define TR_SPKRPITCH           INDENT"Tonh\203he"
#define TR_HAPTIC_LABEL        "Vibration"
#define TR_HAPTICSTRENGTH      INDENT"St\201rke"
#define TR_CONTRAST            "Kontrast"
#define TR_ALARMS_LABEL        "Alarm wenn"
#define TR_BATTERY_RANGE       TR("Akku Bereich","Akku Spg-Bereich") // Symbol Akku Ladezustand
#define TR_BATTERYWARNING      INDENT"Akku Spg kl."
#define TR_INACTIVITYALARM     INDENT"Inaktivit\201t "
#define TR_MEMORYWARNING       INDENT"Speicher voll"
#define TR_ALARMWARNING        INDENT"Ton aus"
#define TR_RENAVIG             "Drehgeb. Navig."
#define TR_THROTTLEREVERSE     TR("Gas invers", "Vollgas hinten?") //\200nderung wg TH9x, Taranis
#define TR_MINUTEBEEP          TR(INDENT"Minute",INDENT"Jede Minute")
#define TR_BEEPCOUNTDOWN       INDENT"Countdown"
#define TR_PERSISTENT          TR(INDENT"M-Zeit",INDENT"Modell Zeit")
#define TR_BACKLIGHT_LABEL     "LCD Licht an"
#define TR_BLDELAY             INDENT"Dauer"
#define TR_BLONBRIGHTNESS      INDENT"An Helligkeit"
#define TR_BLOFFBRIGHTNESS     INDENT"Aus Helligkeit"
#define TR_SPLASHSCREEN        "Startbild"
#define TR_THROTTLEWARNING     "Gas Alarm"
#define TR_SWITCHWARNING       TR("Sch. Alarm","Schalter Alarm")
#define TR_TIMEZONE            TR("Zeitzone","GPS Zeit Zone")
#define TR_RXCHANNELORD        TR("Kanalanordnung","Kanalvoreinstellung")
#define TR_SLAVE               "Sch\205ler PPM1-8 als Ausgang"
#define TR_MODESRC             " Modus\002% Quelle"
#define TR_MULTIPLIER          "Multiplikator"
#define TR_CAL                 "Kal."
#define TR_VTRIM               "Trim- +"
#define TR_BG                  "BG:"
#define TR_MENUTOSTART         CENTER"\006"TR_ENTER" zum Start"
#define TR_SETMIDPOINT         CENTER"Sticks+Potis auf Mitte"
#define TR_MOVESTICKSPOTS      CENTER"Sticks+Potis bewegen"
#define TR_RXBATT              "Rx Batt:"
#define TR_TXnRX               "Tx:\0Rx:"
#define OFS_RX                 4
#define TR_ACCEL               "Acc:"
#define TR_NODATA              CENTER"NO DATA"
#define TR_TM1TM2              "TM1\032TM2"
#define TR_THRTHP              "THR\032TH%"
#define TR_TOT                 "TOT"
#define TR_TMR1LATMAXUS        "Tmr1Lat max\006us"
#define STR_US (STR_TMR1LATMAXUS+12)
#define TR_TMR1LATMINUS        "Tmr1Lat min\006us"
#define TR_TMR1JITTERUS        "Tmr1 Jitter\006us"

#if defined(CPUARM)
  #define TR_TMIXMAXMS         "Tmix max\012ms"
#else
  #define TR_TMIXMAXMS         "Tmix max\014ms"
#endif

#define TR_T10MSUS             "T10ms\016us"
#define TR_FREESTACKMINB       "Free Stack\010b"
#define TR_MENUTORESET         CENTER TR_ENTER" f\205r Reset"
#define TR_PPM                 "PPM"
#define TR_CH                  "CH"
#define TR_MODEL               "MODELL"
#define TR_FP                  "FP"
#define TR_MIX                 "MIX"
#define TR_EEPROMLOWMEM        "EEPROM voll"
#define TR_ALERT               "WARNUNG"
#define TR_PRESSANYKEYTOSKIP   "Taste dr\205cken"
#define TR_THROTTLENOTIDLE     "Gas nicht Null!"
#define TR_ALARMSDISABLED      "Alarme ausgeschaltet"
#define TR_PRESSANYKEY         TR("Taste dr\205cken",CENTER"Taste dr\205cken")
#define TR_BADEEPROMDATA       "EEPROM ung\205ltig"
#define TR_EEPROMFORMATTING    "EEPROM Initialisieren"
#define TR_EEPROMOVERFLOW      "EEPROM \204berlauf"
#define TR_MENURADIOSETUP      TR("SENDER EINST.","SENDER GRUNGEINSTELLUNGEN")
#define TR_MENUDATEANDTIME     "DATUM UND ZEIT"
#define TR_MENUTRAINER         TR("Lehr./Sch\204l.","LEHRER/SCH\204LER")
#define TR_MENUVERSION         "VERSION"
#define TR_MENUDIAG            TR("Schalt.","Schalter Test")
#define TR_MENUANA             "Analog Test"
#define TR_MENUCALIBRATION     TR("KALIB. ANALOG","KALIBRIERUNG STICKS+POTIS")
#define TR_TRIMS2OFFSETS       "\006Trims => Offset"
#define TR_MENUMODELSEL        TR("MODELLE","MODELL W\200HLEN")
#define TR_MENUSETUP           TR("KONF.","MODELL EINSTELLUNGEN")
#define TR_MENUFLIGHTPHASE     "FLUGPHASE"
#define TR_MENUFLIGHTPHASES    "FLUGPHASEN"
#define TR_MENUHELISETUP       "HUBSCHRAUBER"

#if defined(PPM_CENTER_ADJUSTABLE) || defined(PPM_LIMITS_SYMETRICAL) // The right menu titles for the gurus ...
  #define TR_MENUDREXPO        "KN\204PPEL"
  #define TR_MENULIMITS        "AUSG\200NGE"
#else
  #define TR_MENUDREXPO        TR("DR/EXPO","DUALRATE/EXPO")
  #define TR_MENULIMITS        TR("GRENZEN","SERVO WEG")
#endif

#define TR_MENUCURVES          "KURVEN"
#define TR_MENUCURVE           "KURVE"
#define TR_MENUCUSTOMSWITCH    "PROG.SCHALTER"
#define TR_MENUCUSTOMSWITCHES  TR_MENUCUSTOMSWITCH
#define TR_MENUCUSTOMFUNC      "SPEZ. FUNKTIONEN"
#define TR_MENUTELEMETRY       "TELEMETRIE"
#define TR_MENUTEMPLATES       "VORLAGEN"
#define TR_MENUSTAT            "STAT"
#define TR_MENUDEBUG           "DEBUG"
#define TR_RXNUM               TR("Rx Nummer",INDENT"Empf\201nger Nr.")
#define TR_SYNCMENU            "Sync [MENU]"
#define TR_LIMIT               INDENT"Limite"
#define TR_MINRSSI             "Min. RSSI"
#define TR_LATITUDE            "Breite:"
#define TR_LONGITUDE           "L\201nge:"
#define TR_GPSCOORD            TR("Gps Koord.","GPS Koordinaten Format")
#define TR_VARIO               TR("Vario","Variometer")
#define TR_SHUTDOWN            "Herunterfahren"
#define TR_BATT_CALIB          "Akku. Kalib."
#define TR_CURRENT_CALIB       "Stromkalib."
#define TR_VOLTAGE             INDENT"Spann."
#define TR_CURRENT             INDENT"Strom"
#define TR_SELECT_MODEL        "Modell W\201hlen"
#define TR_CREATE_MODEL        "Neues Modell"
#define TR_COPY_MODEL          "Kopiere Modell"
#define TR_MOVE_MODEL          "Verschiebe Modell"
#define TR_BACKUP_MODEL        "Modell Backup"
#define TR_DELETE_MODEL        "Modell L\203schen" // TODO merged into DELETEMODEL?
#define TR_RESTORE_MODEL       "Modell Restore"
#define TR_SDCARD_ERROR        "SDcard Fehler"
#define TR_NO_SDCARD           "Keine SDcard"
#define TR_INCOMPATIBLE        "Nicht kompatibel"
#define TR_WARNING             "WARNUNG"
#define TR_EEPROMWARN          "EEPROM"
#define TR_THROTTLEWARN        "GAS"
#define TR_ALARMSWARN          "ALARM"
#define TR_SWITCHWARN          "SCHALTER"
#define TR_INVERT_THR          TR("Gas umkehren?","Vollgas hinten?") //\200nderung wg Th9x
#define TR_SPEAKER_VOLUME      INDENT "Lautst\201rke"
#define TR_LCD                 "Bildschirm"
#define TR_BRIGHTNESS          INDENT "Helligkeit"
#define TR_CPU_TEMP            "CPU Temp.\016>"
#define TR_CPU_CURRENT         "Strom\022>"
#define TR_CPU_MAH             "Verbrauch"
#define TR_COPROC              "CoProz."
#define TR_COPROC_TEMP         "MB Temp. \016>"
#define TR_CAPAWARNING         INDENT "Kapaz. niedrig"
#define TR_TEMPWARNING         INDENT "Temperatur gr\203er"
#define TR_FUNC                "Funktion"
#define TR_V1                  "V1"
#define TR_V2                  "V2"
#define TR_DURATION            "Dauer"
#define TR_DELAY               "Verz\203g."
#define TR_SD_CARD             "SD Karte"
#define TR_SDHC_CARD           "SD-HC Karte"
#define TR_NO_SOUNDS_ON_SD     "Keine T\203ne auf SD"
#define TR_NO_MODELS_ON_SD     "Keine Modelle auf SD"
#define TR_NO_BITMAPS_ON_SD    "Keine Bitmaps auf SD"
#define TR_PLAY_FILE           "Spielen"
#define TR_DELETE_FILE         "L\203schen"
#define TR_COPY_FILE           "Kopieren"
#define TR_RENAME_FILE         "Umbenennen"
#define TR_ASSIGN_BITMAP       "Assign Bitmap"
#define TR_REMOVED             " gel\203scht"
#define TR_SD_INFO             "Information"
#define TR_SD_FORMAT           "Formattieren"
#define TR_NA                  "N/V"	//NV=Nicht Verf\205gbar  Kurz-Meldung 
#define TR_HARDWARE            "Hardware"
#define TR_FORMATTING          "Formattierung..."
#define TR_TEMP_CALIB          "Temp. Kalib."
#define TR_TIME                "Uhrzeit:"
#define TR_BAUDRATE            "Baudrate"
#define TR_SD_INFO_TITLE       "SD INFO"
#define TR_SD_TYPE             "Typ:"
#define TR_SD_SPEED            "Geschw:"
#define TR_SD_SECTORS          "Sektoren:"
#define TR_SD_SIZE             "Gr\203\206e:"
#define TR_TYPE                "Typ"
#define TR_GLOBAL_VARS         "Globale Variablen"
#define TR_GLOBAL_VAR          "Global Variable"
#define TR_MENUGLOBALVARS      "GLOBALE VARIABLEN"
#define TR_OWN                 "Eigen"
#define TR_DATE                "Datum:"
#define TR_ROTARY_ENCODER      "Drehgeber"
#define TR_CHANNELS_MONITOR    "Kanal Monitor"
#define TR_INTERNALRF          "----Internes HF Modul----------"
#define TR_EXTERNALRF          "----Externes HF Modul----------"
#define TR_FAILSAFE            "Failsafe Mode"
#define TR_FAILSAFESET         "Failsafe Einst."
#define TR_COUNTRYCODE         "L\201nder Code"
#define TR_VOICELANG           "Sprach-Ansagen"
#define TR_UNITSSYSTEM         "Einheiten"
#define TR_EDIT                "Zeile Editieren"
#define TR_INSERT_BEFORE       "Neue Zeile davor"
#define TR_INSERT_AFTER        "Neue Zeile danach"
#define TR_COPY                "Zeile kopieren"
#define TR_MOVE                "Zeile verschieben"
#define TR_PASTE               "Zeile einf\205gen"
#define TR_DELETE              "Zeile l\203schen"
#define TR_INSERT              "Neue Zeile"
#define TR_RESET_FLIGHT        "Reset Flugdaten"
#define TR_RESET_TIMER1        "Reset Timer1"
#define TR_RESET_TIMER2        "Reset Timer2"
#define TR_RESET_TELEMETRY     "Reset Telemetrie"
#define TR_STATISTICS          "Statistik und Gas"
#define TR_ABOUT_US            "Die Programmierer"
#define TR_AND_SWITCH          "UND Schalt" //UND mit weiterem Schaltern 
#define TR_CF                  "SF" //Spezial Funktionen engl. CF
#define TR_SPEAKER             INDENT"Lautspr"
#define TR_BUZZER              INDENT"Summer"
#define TR_BYTES               "bytes"
#define TR_MODULE_BIND         "[Bind]"
#define TR_MODULE_RANGE        "[Range]"
#define TR_RESET               "[Reset]"
#define TR_SET                 "[Set]"
#define TR_TRAINER             "----DSC Buchse PPM In/Out------" //DSC Buchse Funktion
#define TR_ANTENNAPROBLEM      CENTER "TX Antennen Problem!"
#define TR_MODELIDUSED         TR("ID schon benutzt","Modell ID schon benutzt")
#define TR_MODULE              INDENT "Modul-Typ"
#define TR_CHANNELRANGE        INDENT "Ausgangs Kan\201le"
#define TR_LOWALARM            INDENT "Low Alarm"
#define TR_CRITICALALARM       INDENT "Kritisch Alarm"

#if defined(MAVLINK)
  #define TR_MAVLINK_RC_RSSI_SCALE_LABEL   "Max RSSI"
  #define TR_MAVLINK_PC_RSSI_EN_LABEL      "PC RSSI EN"
  #define TR_MAVMENUSETUP_TITLE            "Mavlink Setup"
  #define TR_MAVLINK_BAUD_LABEL            "Baudrate"
  #define TR_MAVLINK_INFOS                 "INFOS"
  #define TR_MAVLINK_MODE                  "MODE"
  #define TR_MAVLINK_CUR_MODE              "Current Mode"
  #define TR_MAVLINK_ARMED                 "Armed"
  #define TR_MAVLINK_BAT_MENU_TITLE        "BAT RSSI"
  #define TR_MAVLINK_BATTERY_LABEL         "Flight Battery status"
  #define TR_MAVLINK_RC_RSSI_LABEL         "RC RSSI"
  #define TR_MAVLINK_PC_RSSI_LABEL         "PC RSSI"
  #define TR_MAVLINK_NAV_MENU_TITLE        "NAV"
  #define TR_MAVLINK_COURSE                "Course"
  #define TR_MAVLINK_HEADING               "Heading"
  #define TR_MAVLINK_BEARING               "Bearing"
  #define TR_MAVLINK_ALTITUDE              "Altitude"
  #define TR_MAVLINK_GPS                   "GPS"
  #define TR_MAVLINK_NO_FIX                "NO Fix"
  #define TR_MAVLINK_SAT                   "SAT"
  #define TR_MAVLINK_HDOP                  "HDOP"
  #define TR_MAVLINK_LAT                   "LAT"
  #define TR_MAVLINK_LON                   "LON"
#endif

// Taranis column headers
#define TR_PHASES_HEADERS      { " Name ", " Schalter ", " Trimmung ", " Fade In ", " Fade Out " }
#define TR_LIMITS_HEADERS      { " Name ", " Subtrim ", " Min ", " Max ", " Richtung ", " PPM Mitte ", " Subtrim Mode " }
#define TR_CSW_HEADERS         { " Funktion ", " Var1 ", " Var2 ", " UND Schalter ", " Dauer ", " Delay " }

//Taranis About screen
#define TR_ABOUTUS             "\204ber OpenTx"

#define TR_ABOUT_OPENTX_1      "OpenTX ist Open Source,"
#define TR_ABOUT_OPENTX_2      "nicht kommerziell, ohne"
#define TR_ABOUT_OPENTX_3      "Funktionsgarantie, frei"
#define TR_ABOUT_OPENTX_4      "verf\205gbar. Unterst\205tzung"
#define TR_ABOUT_OPENTX_5      "durch Spenden willkommen"

#define TR_ABOUT_BERTRAND_1    "Bertrand Songis"
#define TR_ABOUT_BERTRAND_2    "OpenTX Hauptauthor"
#define TR_ABOUT_BERTRAND_3    "Companion9x Mitentwickler"

#define TR_ABOUT_MIKE_1        "Mike Blandford"
#define TR_ABOUT_MIKE_2        "Code und Treiber Guru"
#define TR_ABOUT_MIKE_3        "wohl einer der Besten"
#define TR_ABOUT_MIKE_4        "sehr inspirierend"
       
#define TR_ABOUT_ROMOLO_1      "Romolo Manfredini"
#define TR_ABOUT_ROMOLO_2      "Companion9x Hauptentwickler"
#define TR_ABOUT_ROMOLO_3      ""

#define TR_ABOUT_ANDRE_1       "Andre Bernet"
#define TR_ABOUT_ANDRE_2       "Funktionalit\201t, Tests,"
#define TR_ABOUT_ANDRE_3       "Debugging, Dokumentation"

#define TR_ABOUT_ROB_1         "Rob Thomson"
#define TR_ABOUT_ROB_2         "Openrcforums Webmaster"

#define TR_ABOUT_MARTIN_1      "Martin Hotar"
#define TR_ABOUT_MARTIN_2      "Grafik Designer"

#if defined(PCBTARANIS)
  #define TR_ABOUT_HARDWARE_1  "Firma FrSky "
  #define TR_ABOUT_HARDWARE_2  "Hardware Design/Produktion"
  #define TR_ABOUT_HARDWARE_3  "Firmware Vertrieb"
#else
  #define TR_ABOUT_HARDWARE_1  "Brent Nelson"
  #define TR_ABOUT_HARDWARE_2  "Sky9x Design/produktion"
  #define TR_ABOUT_HARDWARE_3  ""
#endif

#define TR_ABOUT_PARENTS_1     "Vorg\201nger-Projekte"
#define TR_ABOUT_PARENTS_2     "ersky9x (Mike Blandford)"
#define TR_ABOUT_PARENTS_3     "ER9X (Erez Raviv)"
#define TR_ABOUT_PARENTS_4     "TH9X (Thomas Husterer)"

#define TR_CHR_SHORT  's' //Taste short
#define TR_CHR_LONG   'l' //Taste long
#define TR_CHR_TOGGLE 't' //Taste als togglefunktion = Ein Aus Ein 
#define TR_CHR_HOUR   'h' //Stunden

#define TR_BEEP_VOLUME         "Beep Lautst"
#define TR_WAV_VOLUME          "Wav Lautst"
#define TR_VARIO_VOLUME        "Vario Lautst"
#define TR_BG_VOLUME           "Bg Lautst"
