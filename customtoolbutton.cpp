#include "customtoolbutton.h"

CustomToolButton::CustomToolButton(QWidget *parent):QToolButton(parent)
{

}

QVariant CustomToolButton::data(int role) const
{
    return m_var;
}
void CustomToolButton::setData(int role, const QVariant &value)
{
    m_var = value;
}
