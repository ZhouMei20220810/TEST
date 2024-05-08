/********************************************************************************
** Form generated from reading UI file 'forgetpwdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORGETPWDIALOG_H
#define UI_FORGETPWDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ForgetPWDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnReturn;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelPhoneImage;
    QLineEdit *lineEditPhone;
    QFrame *frame_2;
    QLineEdit *lineEditPhone_2;
    QPushButton *pushButton;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelPWImage;
    QLineEdit *lineEdit;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *labelPWImage_2;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnClose;

    void setupUi(QDialog *ForgetPWDialog)
    {
        if (ForgetPWDialog->objectName().isEmpty())
            ForgetPWDialog->setObjectName("ForgetPWDialog");
        ForgetPWDialog->resize(360, 390);
        layoutWidget = new QWidget(ForgetPWDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(1, 50, 361, 241));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnReturn = new QPushButton(layoutWidget);
        btnReturn->setObjectName("btnReturn");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/login/resource/login/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnReturn->setIcon(icon);
        btnReturn->setFlat(true);

        horizontalLayout->addWidget(btnReturn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        frame_3 = new QFrame(layoutWidget);
        frame_3->setObjectName("frame_3");
        frame_3->setMinimumSize(QSize(0, 33));
        frame_3->setMaximumSize(QSize(16777215, 33));
        frame_3->setStyleSheet(QString::fromUtf8("color:#FF4A4A4A;\n"
"background: #FFE7E8EE;\n"
"border-radius: 3px;"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        labelPhoneImage = new QLabel(frame_3);
        labelPhoneImage->setObjectName("labelPhoneImage");
        labelPhoneImage->setPixmap(QPixmap(QString::fromUtf8(":/login/resource/login/ico_phone.png")));

        horizontalLayout_3->addWidget(labelPhoneImage);

        lineEditPhone = new QLineEdit(frame_3);
        lineEditPhone->setObjectName("lineEditPhone");
        lineEditPhone->setStyleSheet(QString::fromUtf8("border:none"));

        horizontalLayout_3->addWidget(lineEditPhone);


        verticalLayout->addWidget(frame_3);

        frame_2 = new QFrame(layoutWidget);
        frame_2->setObjectName("frame_2");
        frame_2->setMinimumSize(QSize(0, 33));
        frame_2->setMaximumSize(QSize(16777215, 33));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        lineEditPhone_2 = new QLineEdit(frame_2);
        lineEditPhone_2->setObjectName("lineEditPhone_2");
        lineEditPhone_2->setGeometry(QRect(0, 0, 131, 33));
        lineEditPhone_2->setMinimumSize(QSize(0, 33));
        lineEditPhone_2->setMaximumSize(QSize(16777215, 33));
        lineEditPhone_2->setStyleSheet(QString::fromUtf8(" QLineEdit {\n"
"     border: none;\n"
"     border-radius: 3px;\n"
"     padding: 1 30px;\n"
"     color:#FF4A4A4A;\n"
"     background: #FFE7E8EE;\n"
"     selection-background-color: #FF0078D7;\n"
" }"));
        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(220, 0, 75, 33));
        pushButton->setMinimumSize(QSize(0, 33));
        pushButton->setMaximumSize(QSize(16777215, 33));

        verticalLayout->addWidget(frame_2);

        frame_5 = new QFrame(layoutWidget);
        frame_5->setObjectName("frame_5");
        frame_5->setMinimumSize(QSize(0, 33));
        frame_5->setMaximumSize(QSize(16777215, 33));
        frame_5->setStyleSheet(QString::fromUtf8("color:#FF4A4A4A;\n"
"background: #FFE7E8EE;\n"
"border-radius: 3px;"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_5);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        labelPWImage = new QLabel(frame_5);
        labelPWImage->setObjectName("labelPWImage");
        labelPWImage->setPixmap(QPixmap(QString::fromUtf8(":/login/resource/login/ico_lock.png")));

        horizontalLayout_6->addWidget(labelPWImage);

        lineEdit = new QLineEdit(frame_5);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setMinimumSize(QSize(0, 0));
        lineEdit->setMaximumSize(QSize(16777215, 16777215));
        lineEdit->setStyleSheet(QString::fromUtf8("border:none"));
        lineEdit->setMaxLength(20);
        lineEdit->setEchoMode(QLineEdit::Password);

        horizontalLayout_6->addWidget(lineEdit);


        verticalLayout->addWidget(frame_5);

        frame_6 = new QFrame(layoutWidget);
        frame_6->setObjectName("frame_6");
        frame_6->setMinimumSize(QSize(0, 33));
        frame_6->setMaximumSize(QSize(16777215, 33));
        frame_6->setStyleSheet(QString::fromUtf8("color:#FF4A4A4A;\n"
"background: #FFE7E8EE;\n"
"border-radius: 3px;"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_6);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        labelPWImage_2 = new QLabel(frame_6);
        labelPWImage_2->setObjectName("labelPWImage_2");
        labelPWImage_2->setPixmap(QPixmap(QString::fromUtf8(":/login/resource/login/ico_lock.png")));

        horizontalLayout_7->addWidget(labelPWImage_2);

        lineEdit_2 = new QLineEdit(frame_6);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setMinimumSize(QSize(0, 0));
        lineEdit_2->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_2->setStyleSheet(QString::fromUtf8("border:none"));
        lineEdit_2->setMaxLength(20);
        lineEdit_2->setEchoMode(QLineEdit::Password);

        horizontalLayout_7->addWidget(lineEdit_2);


        verticalLayout->addWidget(frame_6);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setMinimumSize(QSize(0, 33));
        pushButton_2->setMaximumSize(QSize(16777215, 33));

        verticalLayout->addWidget(pushButton_2);

        frame = new QFrame(ForgetPWDialog);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 620, 32));
        frame->setMinimumSize(QSize(0, 32));
        frame->setMaximumSize(QSize(16777215, 32));
        QPalette palette;
        QBrush brush(QColor(0, 128, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        frame->setPalette(palette);
        frame->setStyleSheet(QString::fromUtf8("background-color:#FF0080FF;"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(12, 0, 12, 0);
        label = new QLabel(frame);
        label->setObjectName("label");
        QPalette palette1;
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label->setPalette(palette1);
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(507, 9, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnClose = new QPushButton(frame);
        btnClose->setObjectName("btnClose");
        btnClose->setMinimumSize(QSize(24, 32));
        btnClose->setMaximumSize(QSize(24, 32));
        btnClose->setStyleSheet(QString::fromUtf8("QPushButton:hover {\n"
"     background-color: #FFD31111\n"
" }\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/login/resource/login/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon1);
        btnClose->setIconSize(QSize(24, 32));
        btnClose->setFlat(true);

        horizontalLayout_2->addWidget(btnClose);


        retranslateUi(ForgetPWDialog);

        QMetaObject::connectSlotsByName(ForgetPWDialog);
    } // setupUi

    void retranslateUi(QDialog *ForgetPWDialog)
    {
        ForgetPWDialog->setWindowTitle(QCoreApplication::translate("ForgetPWDialog", "Dialog", nullptr));
        btnReturn->setText(QCoreApplication::translate("ForgetPWDialog", "\350\277\224\345\233\236", nullptr));
        labelPhoneImage->setText(QString());
        lineEditPhone->setPlaceholderText(QCoreApplication::translate("ForgetPWDialog", "\350\276\223\345\205\245\346\202\250\347\232\204\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        lineEditPhone_2->setText(QCoreApplication::translate("ForgetPWDialog", "\347\237\255\344\277\241\351\252\214\350\257\201\347\240\201", nullptr));
        lineEditPhone_2->setPlaceholderText(QCoreApplication::translate("ForgetPWDialog", "\350\276\223\345\205\245\346\202\250\347\232\204\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        pushButton->setText(QCoreApplication::translate("ForgetPWDialog", "\350\216\267\345\217\226\351\252\214\350\257\201\347\240\201", nullptr));
        labelPWImage->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("ForgetPWDialog", "\350\276\223\345\205\245\346\226\260\345\257\206\347\240\201", nullptr));
        labelPWImage_2->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("ForgetPWDialog", "\350\276\223\345\205\245\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ForgetPWDialog", "\347\241\256\345\256\232", nullptr));
        label->setText(QCoreApplication::translate("ForgetPWDialog", "\345\205\205\345\200\274\345\257\206\347\240\201", nullptr));
        btnClose->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ForgetPWDialog: public Ui_ForgetPWDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORGETPWDIALOG_H
