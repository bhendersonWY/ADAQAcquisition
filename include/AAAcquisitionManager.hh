/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                           Copyright (C) 2012-2016                           //
//                 Zachary Seth Hartwig : All rights reserved                  //
//                                                                             //
//      The ADAQAcquisition source code is licensed under the GNU GPL v3.0.    //
//      You have the right to modify and/or redistribute this source code      //      
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at $ADAQACQUISITION/License.txt.     //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __AAAcquisitionManager_hh__
#define __AAAcquisitionManager_hh__ 1

// ROOT
#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>

// Boost
#ifndef __CINT__
#include <boost/cstdint.hpp>

#include "ADAQDigitizer.hh"
#endif

// C++
#include <vector>
#include <list>
#include <string>
using namespace std;

// ADAQ
#include "ADAQRootClasses.hh"
#include "ADAQReadoutManager.hh"
#include "ADAQWaveformData.hh"

// ADAQAcquisition
#include "AATypes.hh"
#include "AAInterface.hh"
#include "AASettings.hh"

class AAAcquisitionManager : public TObject
{
public:
  AAAcquisitionManager();
  ~AAAcquisitionManager();

  static AAAcquisitionManager *GetInstance();

  void Initialize();

  void PrepareAcquisition();
  void StartAcquisition();
  void StopAcquisition();
  
  void CreateADAQFile(string);
  void CloseADAQFile();

  Bool_t AddCalibrationPoint(Int_t, Int_t, Double_t , Double_t  );
  Bool_t EnableCalibration(Int_t);
  Bool_t ResetCalibration(Int_t);
  Bool_t LoadCalibration(Int_t, string, Int_t &);
  Bool_t WriteCalibration(Int_t, string);
  
  void SaveObjectData(string, string);
  
  Bool_t GetCalibrationEnable(Int_t C) {return CalibrationEnable[C];}
  CalibrationDataStruct GetCalibrationDataStruct(Int_t C) {return CalibrationData[C];}
  
  void SetAcquisitionEnable(Bool_t ATE) {AcquisitionEnable = ATE;}
  Bool_t GetAcquisitionEnable() {return AcquisitionEnable;}
  
  void SetAcquisitionTimerEnable(Bool_t ATE) {AcquisitionTimerEnable = ATE;}
  Bool_t GetAcquisitionTimerEnable() {return AcquisitionTimerEnable;}
  
  Bool_t GetADAQFileIsOpen() {return TheReadoutManager->GetADAQFileOpen();}
  
  void SetAcquisitionTimeStart(Double_t  T) {AcquisitionTimeStart = T;}
  void SetAcquisitionTimeStop(Double_t  T) {AcquisitionTimeStop = T;}
  
  void SetInterfacePointer(AAInterface *TI) {TheInterface = TI;}
  void SetSettingsPointer(AASettings *TS) {TheSettings = TS;}

  TH1F *GetSpectrum(Int_t C) {return Spectrum_H[C];}
  TGraph *GetCalibrationCurve(Int_t C) {return CalibrationCurves[C];}

  void SetupRateVector();
  list<unsigned int> * GetRateList(Int_t C) {return Rate_C[C];}

  TH2F *GetPSDHistogram(Int_t C) {return PSDHistogram_H[C];}
  
  TString GetADAQFileComment() {return TheReadoutManager->GetFileComment();}
  void SetADAQFileComment(TString AFC) {TheReadoutManager->SetFileComment(AFC);}
  
  ClassDef(AAAcquisitionManager, 1);
  
private:
  static AAAcquisitionManager *TheAcquisitionManager;

  Bool_t AcquisitionEnable;

  // Objects for controlling timed acquisition periods

  Bool_t AcquisitionTimerEnable;
  Double_t AcquisitionTimeStart, AcquisitionTimeStop;
  time_t AcquisitionTimeNow, AcquisitionTimePrev;

  Bool_t UseSTDFirmware, UsePSDFirmware;
  Bool_t AnalyzePSDList, AnalyzePSDWaveform;

#ifndef __CINT__

  //////////////////////////////////
  // Variables for digitizer readout

  // CAEN standard firmware
  char *EventPointer;
  CAEN_DGTZ_EventInfo_t EventInfo;
  CAEN_DGTZ_UINT16_EVENT_t *EventWaveform;

  // CAEN DPP-PSD firmware
  
  CAEN_DGTZ_DPP_PSD_Params_t *PSDParams[16];
  CAEN_DGTZ_DPP_PSD_Event_t *PSDEvents[16];
  CAEN_DGTZ_DPP_PSD_Waveforms_t *PSDWaveforms;
  vector<uint32_t> NumPSDEvents;
  
  // Variables for PC buffer readout
  char *Buffer; 
  uint32_t BufferSize, PSDEventSize, PSDWaveformSize, ReadSize;
  uint32_t FPGAEvents, PCEvents;
  vector<bool> BufferFull;

  uint32_t ReadoutType, ReadoutTypeBit, ReadoutTypeMask;

  uint32_t *ZLEDataWords;
  uint32_t ZLEEventSizeMask, ZLEEventSize;
  uint32_t ZLESampleAMask, ZLESampleBMask;
  uint32_t ZLENumWordMask, ZLEControlMask;
#endif

  vector<Int_t> WaveformLength;
  vector<Int_t> BaselineStart, BaselineStop, BaselineLength;
  vector<Double_t > BaselineValue;
  vector<Int_t> PSDTotalAbsStart, PSDTotalAbsStop;
  vector<Int_t> PSDTailAbsStart, PSDTailAbsStop;
  vector<Int_t> PeakPosition;
  vector<Double_t > Polarity;
  
  ULong64_t EventCounter;
  Int_t LLD, ULD;
  Double_t  SampleHeight, TriggerHeight;
  Double_t  PulseHeight, PulseArea;
  Double_t  PSDTotal, PSDTail;
  
  vector<ULong64_t> CorrectedTimeStamp;
#ifndef __CINT__
  vector<uint32_t> PrevTimeStamp, TimeStampRollovers;
  vector<ULong64_t> PrevCorTimeStamp;
  uint32_t RawTimeStamp;
#endif

  vector<bool> CalibrationEnable;
  vector<TGraph *> CalibrationCurves;
  vector<CalibrationDataStruct> CalibrationData;
  
  vector<TH1F *> Spectrum_H;
  vector<Bool_t> SpectrumExists;

  // vector<TGraph *> Rate_P;
  vector< list<unsigned int> * > Rate_C;
  unsigned int RateAccum;
  vector<double> Rate_Lead;
  vector<Bool_t> RateExists;
  
  vector<TH2F *> PSDHistogram_H;
  vector<Bool_t> PSDHistogramExists;
  
  TTree *WaveformTree;
  Bool_t FillWaveformTree;

  ADAQRootMeasParams *Parameters;
  TObjString *Comment;
  
  // Strings for file names, extensions
  string DataFileName, DataFileExtension;
  string SpectrumFileName, SpectrumFileExtension;
  string GraphicsFileName, GraphicsFileExtension;

  AAInterface *TheInterface;
  AASettings *TheSettings;
  
  ADAQReadoutManager *TheReadoutManager;
#ifndef __CINT__
  // Waveforms receives digitized waveform data during readout and is
  // used for on-the-fly waveform analysis and graphing
  vector<vector<uint16_t> > Waveforms;
  
  // Waveforms4Storage has its addressed tied to the ROOT TTree in the
  // ADAQ file for persistently storing waveforms to disk
  vector<vector<uint16_t> > Waveforms4Storage;
#endif
  vector<ADAQWaveformData *> WaveformData;

  // Variables to prevent duplicate event writing
  double oldPulseArea;
  double oldPulseHeight;
  double oldBaseline;
  double oldTotal;
  double oldTail;  
};

#endif
