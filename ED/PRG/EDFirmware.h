#ifndef EDFIRMWARE_H
#define EDFIRMWARE_H

#include <QString>
#include <QByteArray>

class EDFirmware
{
public:

    typedef enum {FFT_UNKNOWN, FFT_BINARY, FFT_HEX} EDFirmwareFileType;

    EDFirmware();

    const QByteArray &ReadFromFile(const QString &filename, EDFirmwareFileType type);
    void SaveToFile(const QString &filename, EDFirmwareFileType type);

    const QByteArray &getData() const;
    void setData(const QByteArray &data);

    const QString &getFileName() const;

private:
    QString m_filename;
    QByteArray m_data;

};

#endif // EDFIRMWARE_H
