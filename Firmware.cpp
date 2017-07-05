#include <QFileDialog>
#include "Firmware.h"
#include "ED/EDLogger.h"

//-----------------------------------------------------------------------------------------------------------
Firmware::Firmware(ED &ed, EDPrgProgress &progress):
    m_ed(ed),
    m_progress(progress)
{
    m_firmwaredumpviewer = new FirmwareDumpViewer();
}
//-----------------------------------------------------------------------------------------------------------
Firmware::~Firmware()
{
    delete m_firmwaredumpviewer;
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::Open()
{
    QFileDialog dialog;

    QString filtr = "*.bin *.ldr *.srec";

    dialog.setFileMode(QFileDialog::ExistingFile);
    QString filename = dialog.getOpenFileName(nullptr, "Open File", "", "Binary Files (*.bin *.ldr *.srec)", &filtr);
    if(QFile::exists(filename))
    {
        m_firmware.ReadFromFile(filename, EDFirmware::FFT_BINARY);
        logger().WriteLn("Opened file: " + filename, Qt::blue);
    } else
    {
        logger().WriteLn("File not found", Qt::red);
    }
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::Save()
{
    if(m_dump.getData().size() == 0)
    {
        logger().WriteLnMessageCritical("There is no data to save", Qt::red);
        return;
    }

    QFileDialog dialog;

    QString filtr = "*.bin *.ldr *.srec";

    dialog.setFileMode(QFileDialog::ExistingFile);
    QString filename = dialog.getSaveFileName(nullptr, "Save File", "d:/", "Binary Files (*.bin *.ldr *.srec)", &filtr);
    m_dump.SaveToFile(filename, EDFirmware::FFT_BINARY);
    logger().WriteLn("Data saved to file: " + filename, Qt::blue);
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::ReadAll(int device_id)
{
    m_ed.Prg().ReadAll(device_id, m_dump, m_progress);
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::ReadDataOnly(int device_id)
{
    m_ed.Prg().Read(device_id, m_dump, m_firmware.getData().size(), m_progress);
}
//-----------------------------------------------------------------------------------------------------------
bool Firmware::Compare()
{
    uint num_errors = 0;
    const QByteArray & f = m_firmware.getData();
    const QByteArray & d = m_dump.getData();
    uint size = f.size();

    if( (f.size() == 0) || (d.size() == 0) )
    {
        logger().WriteLnMessageCritical("No data to compare.", Qt::red);
        return false;
    } else
    if( f.size() > d.size() )
    {
        logger().WriteLnMessageCritical("Error: firmware sise is larger than dump.", Qt::red);
        return false;
    } else
    {
        for(uint i = 0; i < size; i++)
        {
            if(f.at(i) != d.at(i)) num_errors++;
        }
        if(num_errors > 0)
            logger().WriteLnMessageCritical("Found " + QString::number(num_errors, 10) + " error", Qt::red);
        else
            logger().WriteLn("Comparision completed succefully", Qt::green);
    }

    return (num_errors == 0);
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::ResetDevice(int device_id)
{
    m_ed.Prg().Restart(device_id);
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::Auto(int device_id)
{
	EDPrg &prg = m_ed.Prg();
	
    if(!prg.Available()) return;

    if(m_firmware.getData().size() == 0)
    {
        logger().WriteLnMessageCritical("Firmware file not opened.", Qt::red);
        return;
    }

    logger().WriteLn("Firmware update...", Qt::blue);

	if (prg.Erase(device_id)) // Erase
	if (prg.Write(device_id, m_firmware, m_progress)) // Write
	if (prg.Read(device_id, m_dump, m_firmware.getData().size(), m_progress))// Read
    if(Compare())   // Verify
	if (prg.Descriptors().at(device_id).restart) prg.Restart(device_id);
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::ViewFirmwareDump()
{
    logger().WriteLn("Open firmware\\dump viewer...", Qt::gray);

    m_firmwaredumpviewer->setFirmware(m_firmware.getData());
    m_firmwaredumpviewer->setDump(m_dump.getData());
    m_firmwaredumpviewer->show();
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::Erase(int device_id)
{
    m_ed.Prg().Erase(device_id);
}
//-----------------------------------------------------------------------------------------------------------
void Firmware::Write(int device_id)
{
    m_ed.Prg().Write(device_id, m_firmware, m_progress);
}
