#include "tplaintextedit.h"

TPlainTextEdit::TPlainTextEdit(QWidget* parent)
    :QPlainTextEdit(parent)
{

}

void TPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        emit returnPressed();
        event->accept();
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}
