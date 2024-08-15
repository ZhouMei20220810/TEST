#ifndef TPLAINTEXTEDIT_H
#define TPLAINTEXTEDIT_H

#include <QObject>
#include <QPlainTextEdit>

class TPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    TPlainTextEdit(QWidget* parent = nullptr);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
signals:
    void returnPressed();
};

#endif // TPLAINTEXTEDIT_H
