#include <cfloat>
#include "TelData.h"

//-------------------------------------------------------------------------------------------------------
TelData::TelData(const int size):
    minY(DBL_MAX),
    maxY(-DBL_MAX)
{
    m_data = new QCPDataMap;
    setSize(size);
}
//-------------------------------------------------------------------------------------------------------
TelData::~TelData()
{
    delete m_data;
}
//-------------------------------------------------------------------------------------------------------
void TelData::setSize(const int value)
{
    QCPData tmp;
    tmp.value = 0;
    for(int i = 0; i < value; i++)
    {
        m_data->insert(m_data->end(), i, tmp);
    }
    m_size = value;
    position = 0;
    minY = DBL_MAX;
    maxY = -DBL_MAX;
}
//-------------------------------------------------------------------------------------------------------
const int TelData::getSize()
{
    return m_data->size();
}
//-------------------------------------------------------------------------------------------------------
void TelData::setWrapMode(bool value)
{
    m_wrapmode = value;
    setSize(m_size); // reinit m_data
}
//-------------------------------------------------------------------------------------------------------
bool TelData::getWrapMode()
{
    return m_wrapmode;
}
//-------------------------------------------------------------------------------------------------------
void TelData::addData(double value)
{
    if(m_wrapmode)
    {
        (*m_data)[position].value = value;
        (*m_data)[position].key = position;
        position++;
        if(position >= m_size) position = 0;
    } else
    {
        m_data->remove(position - m_size);
        position++;
        QCPData tmp;
        tmp.value = value;
        tmp.key = position;
        m_data->insert(position, tmp);
    }

    if(value >= maxY) maxY = value;
    if(value <= minY) minY = value;
}
//-------------------------------------------------------------------------------------------------------
QCPDataMap &TelData::getDataMap()
{
    return *m_data;
}
//-------------------------------------------------------------------------------------------------------
const QVector<double> TelData::getDataVector()
{
    m_vector_tmp.clear();

    int size = m_data->size();

    for(int i = 0; i < size; i++)
    {
        if(m_wrapmode) m_vector_tmp.append(m_data->value(i).value);
        else m_vector_tmp.append(m_data->value((position-m_size) + i).value);
    }

    return std::move(m_vector_tmp);
}
//-------------------------------------------------------------------------------------------------------
const QCPRange TelData::getXRange() const
{
    QCPRange tmp;

    if(m_wrapmode) tmp = QCPRange(0, m_size);
        else tmp = QCPRange(position - m_size, position);

    return tmp;
}
//-------------------------------------------------------------------------------------------------------
const QCPRange TelData::getYRange(bool recalc)
{
    QCPRange tmp;
    double value;

    if(recalc)
    {
        minY = DBL_MAX;
        maxY = -DBL_MAX;

        int size = m_data->size();
        for(int i = 0; i < size; i++)
        {
            value = m_data->value(i).value;
            if(value >= maxY) maxY = value;
            if(value <= minY) minY = value;
        }
    }

    tmp.lower = minY;
    tmp.upper = maxY;

    return tmp;
}
//-------------------------------------------------------------------------------------------------------
void TelData::clear()
{
    for(int i = 0; i < m_size; i++) (*m_data)[i].value = 0;
    position = 0;

    minY = DBL_MAX;
    maxY = -DBL_MAX;
}
