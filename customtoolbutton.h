#ifndef CUSTOMTOOLBUTTON_H
#define CUSTOMTOOLBUTTON_H

#include <QObject>
#include <QToolButton>

class CustomToolButton : public QToolButton
{
    Q_OBJECT
public:
    CustomToolButton(QWidget *parent = nullptr);

    virtual QVariant data(int role) const;
    virtual void setData(int role, const QVariant &value);
private:
    QVariant m_var;
};

#endif // CUSTOMTOOLBUTTON_H
