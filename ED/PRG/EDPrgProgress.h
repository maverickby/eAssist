#ifndef EDPRGPROGRESS_H
#define EDPRGPROGRESS_H

class EDPrgProgress
{
public:
    EDPrgProgress();

    virtual void setProgressValue(unsigned int progress) = 0;
    virtual void setProgressVisible(bool state) = 0;
    virtual void setProgressParam(unsigned int min_value, unsigned int max_value) = 0;
};

#endif // EDPRGPROGRESS_H
