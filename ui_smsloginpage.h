/********************************************************************************
** Form generated from reading UI file 'smsloginpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMSLOGINPAGE_H
#define UI_SMSLOGINPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SMSLoginPage
{
public:
    QCheckBox *checkBoxPolicy;
    QPushButton *btnLogin;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelPhoneImage;
    QLineEdit *lineEditPhone;
    QPushButton *btnArrow;
    QLineEdit *lineEditPassword;
    QPushButton *btnGetSMSCode;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBoxAutoLogin;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnPasswordLogin;

    void setupUi(QWidget *SMSLoginPage)
    {
        if (SMSLoginPage->objectName().isEmpty())
            SMSLoginPage->setObjectName("SMSLoginPage");
        SMSLoginPage->resize(364, 300);
        checkBoxPolicy = new QCheckBox(SMSLoginPage);
        checkBoxPolicy->setObjectName("checkBoxPolicy");
        checkBoxPolicy->setGeometry(QRect(50, 150, 294, 16));
        QFont font;
        font.setPointSize(10);
        checkBoxPolicy->setFont(font);
        checkBoxPolicy->setStyleSheet(QString::fromUtf8(" QCheckBox {\n"
"     spacing: 5px;\n"
" }\n"
"\n"
" QCheckBox::indicator {\n"
"     width: 16px;\n"
"     height: 16px;\n"
" }\n"
"\n"
" QCheckBox::indicator:unchecked {\n"
"     image: url(:/login/resource/login/option_normal.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:unchecked:hover {\n"
"     image: url(:/login/resource/login/option_normal.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:unchecked:pressed {\n"
"     image: url(:/login/resource/login/option_normal.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:checked {\n"
"     image: url(:/login/resource/login/option_select.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:checked:hover {\n"
"     image: url(:/login/resource/login/option_select.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:checked:pressed {\n"
"     image: url(:/login/resource/login/option_select.png);\n"
" }\n"
""));
        btnLogin = new QPushButton(SMSLoginPage);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setGeometry(QRect(41, 241, 271, 28));
        btnLogin->setMinimumSize(QSize(100, 28));
        btnLogin->setFont(font);
        btnLogin->setStyleSheet(QString::fromUtf8(" QPushButton {\n"
"     border: 2px solid #555764;\n"
"     border-radius:2px;\n"
"	 background-color: #FF555764;\n"
"	 color:#FFFFFFFF;\n"
" }\n"
"QPushButton:hover\n"
"{\n"
"	border:2px solid  #FF27282B;\n"
"	background-color: #FF27282B;\n"
"}\n"
" QPushButton:pressed {\n"
"     border: 2px solid #555764;\n"
"     border-radius: 2px;\n"
"	 background-color: #FF555764;\n"
"	 color:#FFFFFFFF;\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }\n"
""));
        frame_3 = new QFrame(SMSLoginPage);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(40, 60, 279, 33));
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
        lineEditPhone->setFont(font);
        lineEditPhone->setStyleSheet(QString::fromUtf8("border:none"));

        horizontalLayout_3->addWidget(lineEditPhone);

        btnArrow = new QPushButton(frame_3);
        btnArrow->setObjectName("btnArrow");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/login/resource/login/ico_down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnArrow->setIcon(icon);
        btnArrow->setFlat(true);

        horizontalLayout_3->addWidget(btnArrow);

        lineEditPassword = new QLineEdit(SMSLoginPage);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setGeometry(QRect(40, 110, 151, 16));
        lineEditPassword->setMinimumSize(QSize(0, 0));
        lineEditPassword->setMaximumSize(QSize(16777215, 16777215));
        lineEditPassword->setFont(font);
        lineEditPassword->setStyleSheet(QString::fromUtf8("border:none"));
        lineEditPassword->setEchoMode(QLineEdit::Password);
        btnGetSMSCode = new QPushButton(SMSLoginPage);
        btnGetSMSCode->setObjectName("btnGetSMSCode");
        btnGetSMSCode->setGeometry(QRect(220, 110, 75, 23));
        layoutWidget = new QWidget(SMSLoginPage);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(30, 180, 320, 41));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        checkBoxAutoLogin = new QCheckBox(layoutWidget);
        checkBoxAutoLogin->setObjectName("checkBoxAutoLogin");
        checkBoxAutoLogin->setFont(font);
        checkBoxAutoLogin->setStyleSheet(QString::fromUtf8(" QCheckBox {\n"
"     spacing: 5px;\n"
" }\n"
"\n"
" QCheckBox::indicator {\n"
"     width: 16px;\n"
"     height: 16px;\n"
" }\n"
"\n"
" QCheckBox::indicator:unchecked {\n"
"     image: url(:/login/resource/login/option_normal.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:unchecked:hover {\n"
"     image: url(:/login/resource/login/option_normal.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:unchecked:pressed {\n"
"     image: url(:/login/resource/login/option_normal.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:checked {\n"
"     image: url(:/login/resource/login/option_select.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:checked:hover {\n"
"     image: url(:/login/resource/login/option_select.png);\n"
" }\n"
"\n"
" QCheckBox::indicator:checked:pressed {\n"
"     image: url(:/login/resource/login/option_select.png);\n"
" }\n"
""));

        horizontalLayout_4->addWidget(checkBoxAutoLogin);

        horizontalSpacer_5 = new QSpacerItem(69, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        btnPasswordLogin = new QPushButton(layoutWidget);
        btnPasswordLogin->setObjectName("btnPasswordLogin");
        btnPasswordLogin->setMinimumSize(QSize(100, 28));
        btnPasswordLogin->setFont(font);
        btnPasswordLogin->setStyleSheet(QString::fromUtf8(" QPushButton {\n"
"     border: 2px solid #555764;\n"
"     border-radius:2px;\n"
"	 background-color: #FF555764;\n"
"	 color:#FFFFFFFF;\n"
" }\n"
"QPushButton:hover\n"
"{\n"
"	border:2px solid  #FF27282B;\n"
"	background-color: #FF27282B;\n"
"}\n"
" QPushButton:pressed {\n"
"     border: 2px solid #555764;\n"
"     border-radius: 2px;\n"
"	 background-color: #FF555764;\n"
"	 color:#FFFFFFFF;\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }\n"
""));

        horizontalLayout_4->addWidget(btnPasswordLogin);


        retranslateUi(SMSLoginPage);

        QMetaObject::connectSlotsByName(SMSLoginPage);
    } // setupUi

    void retranslateUi(QWidget *SMSLoginPage)
    {
        SMSLoginPage->setWindowTitle(QCoreApplication::translate("SMSLoginPage", "Form", nullptr));
        checkBoxPolicy->setText(QCoreApplication::translate("SMSLoginPage", "\346\210\221\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\347\224\250\346\210\267\345\215\217\350\256\256\357\274\214\351\232\220\347\247\201\346\224\277\347\255\226\345\222\214\345\205\215\350\264\243\345\243\260\346\230\216", nullptr));
        btnLogin->setText(QCoreApplication::translate("SMSLoginPage", "\347\231\273\345\275\225", nullptr));
        labelPhoneImage->setText(QString());
        lineEditPhone->setPlaceholderText(QCoreApplication::translate("SMSLoginPage", "\350\276\223\345\205\245\346\202\250\347\232\204\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        btnArrow->setText(QString());
        lineEditPassword->setPlaceholderText(QCoreApplication::translate("SMSLoginPage", "\347\237\255\344\277\241\351\252\214\350\257\201\347\240\201", nullptr));
        btnGetSMSCode->setText(QCoreApplication::translate("SMSLoginPage", "\350\216\267\345\217\226\351\252\214\350\257\201\347\240\201", nullptr));
        checkBoxAutoLogin->setText(QCoreApplication::translate("SMSLoginPage", "\350\207\252\345\212\250\347\231\273\345\275\225", nullptr));
        btnPasswordLogin->setText(QCoreApplication::translate("SMSLoginPage", "\345\257\206\347\240\201\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SMSLoginPage: public Ui_SMSLoginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMSLOGINPAGE_H
