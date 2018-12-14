#pragma once

/*---------------------------------------------------------------------------*/

// COMMON
#define S_COMMON_SECTION_NAME                       "Common"

#define S_COMMON_DEVICE_NAME                        "DeviceName"
#define S_ADDMODES_MASK                             "AddModesMask"

// MISC
#define S_MISC_SECTION_NAME                         "Misc"

#define S_MISC_CHANNEL_LABEL                        "Label"


// TUNER
#define S_TUNER_SECTION_NAME                        "Tuner"                                 //  {1}

#define S_TUNER_FREQUENCY                           "Frequency"                             //  {1.1}
#define S_TUNER_FILTER_BAND                         "FilterBandIndex"                            //  {1.2}
#define S_TUNER_MANUAL_FILTER_MODE                  "ManualFilterMode"                      //  {1.3}
#define S_TUNER_SPECTRUM_INVERSION                  "SpectrumInversion"                     //  {1.3}
// #define S_TUNER_SPECTRUM_ANALYZER_SHIFT             "SpectrumAnalyzerShift"                 //  {1.4}
// #define S_TUNER_SPECTRUM_ANALYZER_ENABLE            "SpectrumAnalyzerEnable"                //  {1.5}
#define S_TUNER_SUPPLY_VOLTAGE                      "SupplyVoltage"                         //  {1.6}
#define S_TUNER_REFERENCE_VOLTAGE_OLD               "UrefOld"                               //  {1.6}
#define S_TUNER_REFERENCE_VOLTAGE_NEW               "Uref"                                  //  {1.6}
#define S_TUNER_REFERENCE_VOLTAGE_OLD_NO_SA         "UrefOldNoSA"                           //  {1.6}
#define S_TUNER_REFERENCE_VOLTAGE_NEW_NO_SA         "UrefNoSA"                              //  {1.6}
#define S_TUNER_MAX_SA_SYMBOL_RATE                  "MaxSASymbolRate"                       //  {1.7}
// #define S_TUNER_KFBB                                "Kfbb"                                  //  {1.12}
                                                                                            //
#define S_TUNER_REFERENCE                           "Reference"                             //  {1.}
#define S_TUNER_SERIAL                              "Serial"                                //  {1.}
#define S_TUNER_RELEASE                             "Release"                               //  {1.}
#define S_TUNER_WRITEEEPROM                         "WriteEEPROM"                           //  {1.}

#define S_AGC_INITIAL_GAIN1                         "InitialGain1"                          //  {1.}
#define S_AGC_INITIAL_GAIN2                         "InitialGain2"                          //  {1.}
#define S_TUNER_FMIN                                "Fmin"                                  //  {1.}
#define S_TUNER_FMAX                                "Fmax"                                  //  {1.}
#define S_TUNER_LPFMIN                              "LPFmin"                                  //  {1.}
#define S_TUNER_LPFMAX                              "LPFmax"                                  //  {1.}

// AGC
#define S_AGC_SECTION_NAME                          "AGC"                                   //  {1}

#define S_AGC_MANUAL_L_BAND_GAIN                    "ManualLBandGain"                       //  {1.10}
#define S_AGC_MANUAL_BASEBAND_GAIN                  "ManualBasebandGain"                    //  {1.11}

#define S_AGC_MODE                                  "AGCMode"                                  //  {1.9}

/*
AGC.Uref: 750
AGC.DAC: 0
AGC.VPKdelay: 4
AGC.dLim: 0 
AGC.trigA: 0
AGC.trigM: 2
AGC.trigS: 0
*/
#define S_AGC_UREF                                  "Uref"
#define S_AGC_DAC                                   "DAC"
#define S_AGC_VPKDELAY                              "VPKdelay"
#define S_AGC_DLIM                                  "dLim"
#define S_AGC_TRIGA                                 "trigA"
#define S_AGC_TRIGM                                 "trigM"
#define S_AGC_TRIGS                                 "trigS"
/*
#ADRFr.DC_offs: 0

AGC.Filter: 0
AGC.PwrMode: 0 
AGC.VGA1: 1
AGC.VGA2: 3
AGC.VGA3: 3 
AGC.PostAmp: 0
AGC.DC_offs: 0
*/
#define S_AGC_ADRF_FILTER                           "Filter"
#define S_AGC_ADRF_PWR_MODE                         "PwrMode"
#define S_AGC_ADRF_VGA1                             "VGA1"
#define S_AGC_ADRF_VGA2                             "VGA2"
#define S_AGC_ADRF_VGA3                             "VGA3"
#define S_AGC_ADRF_POSTAMP                          "PostAmp"
#define S_AGC_ADRF_DC_OFFS                          "DC_offs"
/*
AGC.VGN1: 215
AGC.VGN2: 105
AGC.VGN3: 105
*/
#define S_AGC_ADRF_VGN1                             "VGN1"
#define S_AGC_ADRF_VGN2                             "VGN2"
#define S_AGC_ADRF_VGN3                             "VGN3"
/*
AGC.EEPROM_init: 0x578CBA580001759134D7696900F84000
AGC.EEPROM_write: false
*/
#define S_AGC_EEPROM_INIT                           "EEPROM_init"
#define S_AGC_EEPROM_WRITE                          "EEPROM_write"
/*
AGC.Fs_ADC: 368400000
AGC.Fs_Out: 307000000
*/
#define S_AGC_FS_ADC                                "Fs_ADC"
#define S_AGC_FS_OUT                                "Fs_Out"
// Ar + Att for colculate Pin
#define S_AGC_AR_ATT_FOR_PIN                        "Koef_Ar"

#define S_AGC_MANUAL_LBAND_GAIN_MIN                 "ManGC1min"
#define S_AGC_MANUAL_LBAND_GAIN_MAX                 "ManGC1max"

#define S_AGC_MEASURE_POTRION                       "MeasurePotrion"                        //  {1.12}
                                                                                            //
#define S_DECIMATION_SECTION_NAME                   "Decimation"                            //  {1}
#define S_DECIMATION_RATE                           "Rate"

// ADC
#define S_ADC_SECTION_NAME                          "ADC"                                   //  {2}

#define S_ADC_COARSE_GAIN_ENABLE                    "CoarseGainEnable"                      //  {2.1}
#define S_ADC_DECIMATION_RATE                       "DecimationRate"                        //  {2.2}
#define S_ADC_LOW_LATENCY_ENABLE                    "LowLatencyEnable"                      //  {2.3}
#define S_ADC_FILTER_DECIMATION_OFF                 "FilterParamNoDecimation"               //  {2.4}
#define S_ADC_FILTER_DECIMATION_2                   "FilterParamDecimationBy2"              //  {2.5}
#define S_ADC_FILTER_DECIMATION_4                   "FilterParamDecimationBy4"              //  {2.6}
#define S_ADC_FILTER_DECIMATION_8                   "FilterParamDecimationBy8"              //  {2.7}
#define S_ADC_OFFSET_CORRECTION_ENABLE              "OffsetCorrectionEnable"                //  {2.8}
#define S_ADC_FILTER_COEFF_DECIMATION_OFF           "FilterCoefficientNoDecimation"         //  {2.9}
#define S_ADC_FILTER_COEFF_DECIMATION_2             "FilterCoefficientDecimationBy2"        //  {2.10}
#define S_ADC_FILTER_COEFF_DECIMATION_4             "FilterCoefficientDecimationBy4"        //  {2.11}
#define S_ADC_FILTER_COEFF_DECIMATION_8             "FilterCoefficientDecimationBy8"        //  {2.12}
#define S_ADC_OFFSET_CORRECTION_TIME_CONSTANT       "OffsetCorrectionTimeConstant"          //  {2.13}
#define S_ADC_DECIMATION_RATE_IF_DEMODULATOR_ON     "DecimationRateIfDemodulatorOn"         //  {2.??}




