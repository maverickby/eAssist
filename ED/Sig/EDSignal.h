#ifndef EDSIGNAL_H
#define EDSIGNAL_H

#include <QString>

typedef enum {SCL_STD = 0, SCL_ENUM = 1, SCL_ARRAY1 = 2, SCL_ARRAY2 = 3} EDSig_Class;

typedef enum {STYPE_STRING = 0, STYPE_BOOL = 1, STYPE_BYTE = 2, STYPE_UINT = 3,
              STYPE_INT = 4, STYPE_LONG = 5, STYPE_ULONG = 6, STYPE_FLOAT = 7} EDSig_Type;

typedef enum {SMT_NONE = 0, SMT_VOLTAGE = 1, SMT_AMPERE = 2, SMT_ANG_DEGREE = 3,
              SMT_RADIAN = 4, SMT_METER = 5, SMT_VOLT_ANG_DEGREE = 6,
              SMT_METER_SECOND = 7, SMT_CELSIUS = 8, SMT_ANG_DEGREE_SECOND = 9} EDSig_Measurement_type;

typedef enum {CM_NO = 0, CM_FIXED = 1, CM_SIGNATURE = 2} EDSigCtrl_Mode;

//------------------------------------------------------------------------------------------------------
typedef struct
{
    QString                  name;
    EDSig_Class              type_class;
    EDSig_Type               type;
    bool                     en_ctrl;
    bool                     is_parent;
    bool                     en_signature;
    bool                     en_read;
    bool                     en_setting;
    bool                     en_telemetry;
    int                      parent_id;
    uint                     period;
    EDSig_Measurement_type   measure_type;
    float                    coef;
    void                     *property;
} EDSignalDescriptor;

QDataStream &operator <<(QDataStream &out, const EDSignalDescriptor &descriptor);
QDataStream &operator >>(QDataStream &in, EDSignalDescriptor &descriptor);

//------------------------------------------------------------------------------------------------------
class EDSignal
{
    friend QDataStream &operator >>(QDataStream &in, EDSignal &signal);
    friend class EDSigGrp;
    friend class EDSignalsList;

public:
    EDSignal();

    static uint getStdSignalSize(EDSig_Type type_class);
    static QString SignalValueToString(const EDSignal &sig, bool hex);

    const EDSignalDescriptor  &getDescriptor() const;
    int                        getSignal_id() const;
    void                      *getValue() const;
    uint                       getSize() const;
    EDSigCtrl_Mode             getCtrl_mode() const;
    bool                       getIntelemetry() const;
    const void                *getUser_data() const;
    void                       setUser_data(void *data);
    int                        getCnt_fixed_childs() const;
    bool                       isParent() const;
    const QString             &getFullName() const;

private:
    EDSignalDescriptor  descriptor;
    int                 signal_id;
    void                *value;
    uint                size;
    EDSigCtrl_Mode      ctrl_mode;
    bool                intelemetry;
    void                *user_data;
    int                 cnt_fixed_childs;
    bool                isparent;
    QString             fullname;
};

QDataStream &operator <<(QDataStream &out, const EDSignal &signal);
QDataStream &operator >>(QDataStream &in, EDSignal &signal);

#endif // EDSIGNAL_H
