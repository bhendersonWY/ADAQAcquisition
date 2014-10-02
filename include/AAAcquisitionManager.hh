#ifndef __AAAcquisitionManager_hh__
#define __AAAcquisitionManager_hh__ 1

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TGraph.h>

#ifndef __CINT__
#include <boost/cstdint.hpp>

#include "ADAQDigitizer.hh"
#endif

#include <vector>
#include <string>
using namespace std;

#include "ADAQRootClasses.hh"

#include "AATypes.hh"
#include "AAInterface.hh"
#include "AASettings.hh"

class AAAcquisitionManager : public TObject
{
public:
  AAAcquisitionManager();
  ~AAAcquisitionManager();

  static AAAcquisitionManager *GetInstance();

  void PreAcquisition();
  void StartAcquisition();
  void StopAcquisition();
  
  void CreateADAQFile(string);
  void CloseADAQFile();

  bool AddCalibrationPoint(int, int, double, double );
  bool EnableCalibration(int);
  bool ResetCalibration(int);
  bool LoadCalibration(int, string, int &);
  bool WriteCalibration(int, string);

  void SaveSpectrum(string);

  bool GetCalibrationEnable(int C) {return CalibrationEnable[C];}
  CalibrationDataStruct GetCalibrationDataStruct(int C) {return CalibrationData[C];}
  
  void SetAcquisitionEnable(bool ATE) {AcquisitionEnable = ATE;}
  bool GetAcquisitionEnable() {return AcquisitionEnable;}
  
  void SetAcquisitionTimerEnable(bool ATE) {AcquisitionTimerEnable = ATE;}
  bool GetAcquisitionTimerEnable() {return AcquisitionTimerEnable;}

  void SetADAQFileIsOpen(bool AFIO) {ADAQFileIsOpen = AFIO;}
  bool GetADAQFileIsOpen() {return ADAQFileIsOpen;}

  void SetAcquisitionTimeStart(double T) {AcquisitionTimeStart = T;}
  void SetAcquisitionTimeStop(double T) {AcquisitionTimeStop = T;}
  
  void SetInterfacePointer(AAInterface *TI) {TheInterface = TI;}
  void SetSettingsPointer(AASettings *TS) {TheSettings = TS;}

  TH1F *GetSpectrum(int C) {return Spectrum_H[C];}
  TGraph *GetCalibrationCurve(int C) {return CalibrationCurves[C];}

  ClassDef(AAAcquisitionManager, 0);
  
private:
  static AAAcquisitionManager *TheAcquisitionManager;

  bool AcquisitionEnable;

  //Objects for controlling timed acquisition periods
  bool AcquisitionTimerEnable;
  double AcquisitionTimeStart, AcquisitionTimeStop;
  time_t AcquisitionTimeNow, AcquisitionTimePrev;

#ifndef __CINT__
  // Variables for CAEN digitizer readout
  char *EventPointer;
  CAEN_DGTZ_EventInfo_t EventInfo;
  CAEN_DGTZ_UINT16_EVENT_t *EventWaveform;

  // Variables for PC buffer readout
  char *Buffer;
  uint32_t BufferSize, ReadSize, FPGAEvents, PCEvents;
  vector<vector<Short_t> > Waveforms;
  vector<bool> BufferFull;
#endif

  int WaveformLength;
  vector<int> BaselineStart, BaselineStop, BaselineLength;
  vector<double> BaselineValue;
  vector<double> Polarity;

  int LLD, ULD;
  double SampleHeight, TriggerHeight;
  double PulseHeight, PulseArea;

  vector<bool> CalibrationEnable;
  vector<TGraph *> CalibrationCurves;
  vector<CalibrationDataStruct> CalibrationData;
  
  vector<TH1F *> Spectrum_H;
  vector<bool> SpectrumExists;

  TTree *WaveformTree;
  bool FillWaveformTree;

  bool ADAQFileIsOpen;
  TFile *ADAQFile;
  
  ADAQRootMeasParams *Parameters;
  TObjString *Comment;
  
  // Strings for file names, extensions
  string DataFileName, DataFileExtension;
  string SpectrumFileName, SpectrumFileExtension;
  string GraphicsFileName, GraphicsFileExtension;

  AAInterface *TheInterface;
  AASettings *TheSettings;


};

#endif