#ifndef FIRMWARE_H
#define FIRMWARE_H

#include "ED/ED.h"
#include "firmwaredumpviewer.h"

class Firmware
{
public:
    Firmware(ED &ed, EDPrgProgress &progress);
    ~Firmware();

    void Open();
    void Save();
    void Erase(int device_id);
    void Write(int device_id);
    void ReadAll(int device_id);
    void ReadDataOnly(int device_id);
    bool Compare();
    void ResetDevice(int device_id);
    void Auto(int device_id);
    void ViewFirmwareDump();

private:
    ED &m_ed;
    EDPrgProgress &m_progress;

    EDFirmware m_firmware;
    EDFirmware m_dump;

    FirmwareDumpViewer *m_firmwaredumpviewer;
};

#endif // FIRMWARE_H
