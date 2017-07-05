#ifndef TELDATA_H
#define TELDATA_H

#include <QVector>
#include "QCP/qcustomplot.h"

class TelData
{
public:
    explicit TelData(const int size);
    ~TelData();

    void setSize(const int value);
    const int getSize();

    void setWrapMode(bool value);
    bool getWrapMode();

    void addData(double value);
    QCPDataMap & getDataMap();
    const QVector<double> getDataVector();

    const QCPRange getXRange() const;
    const QCPRange getYRange(bool recalc = false);

    void clear();

private:
    QCPDataMap *m_data;
    bool m_wrapmode;
    int m_size;
    long int position;

    QVector<double> m_vector_tmp;

    double minY;
    double maxY;
};

#endif // TELDATA_H
