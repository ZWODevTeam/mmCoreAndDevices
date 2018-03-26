///////////////////////////////////////////////////////////////////////////////
// FILE:          IntegratedLaserEngine.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   IntegratedLaserEngine controller adapter
//
// Based off the AndorLaserCombiner adapter from Karl Hoover, UCSF
//
//

#ifndef _INTEGRATEDLASERENGINE_H_
#define _INTEGRATEDLASERENGINE_H_

#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <vector>
#include "ILEWrapperInterface.h"

#define ERR_PORTS_INIT 101
#define ERR_ACTIVEBLANKING_INIT 102
#define ERR_LOWPOWERMODE_INIT 103
#define ERR_LASERS_INIT 104
#define ERR_DEVICE_INDEXINVALID 105
#define ERR_DEVICE_CONNECTIONFAILED 106
#define ERR_LASER_STATE_READ 201
#define ERR_INTERLOCK 202
#define ERR_CLASSIV_INTERLOCK 203
#define ERR_DEVICE_NOT_CONNECTED 204
#define ERR_LASER_SET 205
#define ERR_ACTIVEBLANKING_SET 401
#define ERR_LOWPOWERMODE_SET 501
#define ERR_PORTS_SET 601


class IALC_REVObject3;
class CLasers;

class CIntegratedLaserEngine : public CShutterBase<CIntegratedLaserEngine>
{
public:
  CIntegratedLaserEngine( const std::string& Description, int NbDevices );
  virtual ~CIntegratedLaserEngine();

  // MMDevice API
  int Initialize();
  virtual int Shutdown();

  void GetName( char* Name ) const;
  bool Busy();

  // Action interface
  int OnDeviceChange( MM::PropertyBase* Prop, MM::ActionType Act, long DeviceIndex );
  int OnResetDevice( MM::PropertyBase* Prop, MM::ActionType Act );

  // Shutter API
  int SetOpen( bool Open = true );
  int GetOpen( bool& Open );
  int Fire( double DeltaT );

  // Helper functions
  void LogMMMessage( std::string Message, bool DebugOnly = false );
  MM::MMTime GetCurrentTime();

  void CheckAndUpdateLasers();
  void ActiveClassIVInterlock();
  void UpdatePropertyUI( const char* PropertyName, const char* PropertyValue );

protected:
  IILEWrapperInterface* ILEWrapper_;
  IALC_REVObject3 *ILEDevice_;
  std::vector<std::string> DevicesNames_;

private:   
  IILEWrapperInterface::TDeviceList DeviceList_;
  CLasers* Lasers_;
  MM::PropertyBase* ResetDeviceProperty_;

  bool Initialized_;
  MM::MMTime ChangedTime_;

  CIntegratedLaserEngine& operator = ( CIntegratedLaserEngine& /*rhs*/ )
  {
    assert( false );
    return *this;
  }

  virtual std::string GetDeviceName() const = 0;
  void CreateDeviceSelectionProperty( int DeviceID, int DeviceIndex );
  virtual bool CreateILE() = 0;
  virtual void DeleteILE() = 0;
  virtual int InitializePorts() = 0;
  virtual int InitializeActiveBlanking() = 0;
  virtual int InitializeLowPowerMode() = 0;
  virtual void DisconnectILEInterfaces() = 0;
  virtual void ReconnectILEInterfaces() = 0;
};

#endif
