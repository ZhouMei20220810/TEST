#ifndef QMOVEDIALOG_H
#define QMOVEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QMouseEvent>

class QMoveDialog : public QDialog
{
    Q_OBJECT
public:
    QMoveDialog(QWidget* parent=nullptr);
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
private:
    QPoint  m_LastPos;
    bool m_bMoving= false;
};

#endif // QMOVEDIALOG_H
