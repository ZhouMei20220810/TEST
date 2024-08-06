#ifndef TRICHTEXTDISPLAY_H
#define TRICHTEXTDISPLAY_H

#include <QObject>
#include <QTextEdit>
#include <QNetworkReply>

class TRichTextDisplay : public QTextEdit
{
    Q_OBJECT
public:
    TRichTextDisplay(QWidget* parent = nullptr);
    void RefreshUIData(QString strRemark);
private slots:
    void onImageDownloaded(QNetworkReply* reply);
};

#endif // TRICHTEXTDISPLAY_H
