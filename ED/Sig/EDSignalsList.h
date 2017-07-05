#ifndef EDSIGNALSLIST_H
#define EDSIGNALSLIST_H

#include <QVector>
#include <QMap>
#include <QList>
#include "EDSigGrp.h"

class EDSignalsList: public QObject
{
    Q_OBJECT
public:
    explicit EDSignalsList(EDSigGrp &siggrp);
    ~EDSignalsList();

    void Clear();
    void Populate();
    int count() const;

    EDSignal & operator [](int index);
    const EDSignal &at(int index) const;

    bool getSignalValue    (EDSignal &signal);
    bool getSignalValueStr (EDSignal &signal, QString &value, bool hex);
    bool setSignalValue    (EDSignal &signal, const void *value);
    bool setSignalValueStr (EDSignal &signal, const QString *value, int base = 10);
    bool setSignalValueList(const QList<EDSignal> &list);

    bool FixSignalValue(EDSignal &signal, bool fix);
    int getNumFixedSignals() const;

    void setSignalTelMode(EDSignal &signal, bool intelemetry);

    void FillMapWithChilds(QMap<QString, EDSignal> &map, const EDSignal &signal) const;
    EDSignal * FullnameToSignal(const QString &fullname);
    bool VerifySignalNames(const QList<QString> &list);

    void getSignalChilds(const EDSignal &signal, QVector<EDSignal*> &childs);

private:
    void update_cnt_fixed_childs(int parent_id, bool add);
    void update_parents();
    void update_fullnames();

    QVector<EDSignal>   m_sigvector;
    EDSigGrp            &m_siggrp;
    int                 m_numfixedsignals;

signals:
    void NewSignalsList();
    void SignalValueChanged(EDSignal *signal);
    void SignalTelModeChanged(EDSignal *signal);
    void SignalFixModeChanged(EDSignal *signal);

public slots:

};

#endif // EDSIGNALSLIST_H
