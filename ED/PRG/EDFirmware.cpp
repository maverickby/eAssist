#include <QFile>
#include <QMessageBox>
#include "EDFirmware.h"

//---------------------------------------------------------------------------------------------------------
EDFirmware::EDFirmware():
    m_filename("")
{}
//---------------------------------------------------------------------------------------------------------
const QByteArray &EDFirmware::ReadFromFile(const QString &filename, EDFirmwareFileType type)
{
    QFile file(filename);

    if(QFile::exists(filename))
    {
        switch(type)
        {
        case  FFT_BINARY:
            if(file.open(QIODevice::ReadOnly))
            {
                m_data = file.readAll();
                m_filename  = filename;
                file.close();
            }
            return m_data;
            break;

        case FFT_HEX:
            QMessageBox::critical(nullptr, "Error", "Hex file type not yet supported");
            m_data.clear();
            return m_data;
            break;

        default:
            QMessageBox::critical(nullptr, "Error", "Unknown file type");
            m_data.clear();
            return m_data;
            break;
        }
    } else
    {
        QMessageBox::critical(nullptr, "Error", "File not found");
        m_data.clear();
        return m_data;
    }
}
//---------------------------------------------------------------------------------------------------------
void EDFirmware::SaveToFile(const QString &filename, EDFirmwareFileType type)
{
    QFile file(filename);

    switch(type)
    {
    case  FFT_BINARY:
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(m_data);
            m_filename  = filename;
            file.close();
        }
        break;

    case FFT_HEX:
        QMessageBox::critical(nullptr, "Error", "Hex file type not yet supported");
        break;

    default:
        QMessageBox::critical(nullptr, "Error", "Unknown file type");
        break;
    }
}
//---------------------------------------------------------------------------------------------------------
const QByteArray &EDFirmware::getData() const
{
    return m_data;
}
//---------------------------------------------------------------------------------------------------------
void EDFirmware::setData(const QByteArray &data)
{
    m_data = data;
}
//---------------------------------------------------------------------------------------------------------
const QString &EDFirmware::getFileName() const
{
    return m_filename;
}
