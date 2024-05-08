/********************************************************************************
** Form generated from reading UI file 'passwordloginpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDLOGINPAGE_H
#define UI_PASSWORDLOGINPAGE_H

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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PasswordLoginPage
{
public:
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnLogin;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnRegister;
    QSpacerItem *horizontalSpacer_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBoxRemberPW;
    QCheckBox *checkBoxAutoLogin;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnForgetPW;
    QPushButton *btnSMSLogin;
    QPushButton *btnCustomerService;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelPhoneImage;
    QLineEdit *lineEditPhone;
    QPushButton *btnArrow;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelPWImage;
    QLineEdit *lineEditPassword;
    QPushButton *btnReturn;

    void setupUi(QWidget *PasswordLoginPage)
    {
        if (PasswordLoginPage->objectName().isEmpty())
            PasswordLoginPage->setObjectName("PasswordLoginPage");
        PasswordLoginPage->resize(354, 300);
        layoutWidget_2 = new QWidget(PasswordLoginPage);
        layoutWidget_2->setObjectName("layoutWidget_2");
        layoutWidget_2->setGeometry(QRect(40, 210, 290, 70));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        btnLogin = new QPushButton(layoutWidget_2);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setMinimumSize(QSize(100, 28));
        QFont font;
        font.setPointSize(10);
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

        horizontalLayout_5->addWidget(btnLogin);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        btnRegister = new QPushButton(layoutWidget_2);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setMinimumSize(QSize(100, 28));
        btnRegister->setFont(font);
        btnRegister->setStyleSheet(QString::fromUtf8(" QPushButton {\n"
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

        horizontalLayout_5->addWidget(btnRegister);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        layoutWidget = new QWidget(PasswordLoginPage);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(40, 170, 295, 41));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        checkBoxRemberPW = new QCheckBox(layoutWidget);
        checkBoxRemberPW->setObjectName("checkBoxRemberPW");
        checkBoxRemberPW->setFont(font);
        checkBoxRemberPW->setStyleSheet(QString::fromUtf8(" QCheckBox {\n"
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

        horizontalLayout_4->addWidget(checkBoxRemberPW);

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

        btnForgetPW = new QPushButton(layoutWidget);
        btnForgetPW->setObjectName("btnForgetPW");
        btnForgetPW->setLayoutDirection(Qt::LeftToRight);
        btnForgetPW->setStyleSheet(QString::fromUtf8("QPushButton{text-align: right;color:#FFF96D6F;border:none;}"));
        btnForgetPW->setAutoRepeat(false);
        btnForgetPW->setAutoExclusive(false);
        btnForgetPW->setFlat(true);

        horizontalLayout_4->addWidget(btnForgetPW);

        btnSMSLogin = new QPushButton(layoutWidget);
        btnSMSLogin->setObjectName("btnSMSLogin");

        horizontalLayout_4->addWidget(btnSMSLogin);

        btnCustomerService = new QPushButton(PasswordLoginPage);
        btnCustomerService->setObjectName("btnCustomerService");
        btnCustomerService->setGeometry(QRect(300, 10, 41, 23));
        frame_4 = new QFrame(PasswordLoginPage);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(40, 60, 281, 98));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_4);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_3 = new QFrame(frame_4);
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


        verticalLayout_2->addWidget(frame_3);

        frame_5 = new QFrame(frame_4);
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

        lineEditPassword = new QLineEdit(frame_5);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setMinimumSize(QSize(0, 0));
        lineEditPassword->setMaximumSize(QSize(16777215, 16777215));
        lineEditPassword->setFont(font);
        lineEditPassword->setStyleSheet(QString::fromUtf8("border:none"));
        lineEditPassword->setEchoMode(QLineEdit::Password);

        horizontalLayout_6->addWidget(lineEditPassword);


        verticalLayout_2->addWidget(frame_5);

        btnReturn = new QPushButton(PasswordLoginPage);
        btnReturn->setObjectName("btnReturn");
        btnReturn->setGeometry(QRect(10, 10, 41, 23));

        retranslateUi(PasswordLoginPage);

        QMetaObject::connectSlotsByName(PasswordLoginPage);
    } // setupUi

    void retranslateUi(QWidget *PasswordLoginPage)
    {
        PasswordLoginPage->setWindowTitle(QCoreApplication::translate("PasswordLoginPage", "Form", nullptr));
        btnLogin->setText(QCoreApplication::translate("PasswordLoginPage", "\347\231\273\345\275\225", nullptr));
        btnRegister->setText(QCoreApplication::translate("PasswordLoginPage", "\346\263\250\345\206\214", nullptr));
        checkBoxRemberPW->setText(QCoreApplication::translate("PasswordLoginPage", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        checkBoxAutoLogin->setText(QCoreApplication::translate("PasswordLoginPage", "\350\207\252\345\212\250\347\231\273\345\275\225", nullptr));
        btnForgetPW->setText(QCoreApplication::translate("PasswordLoginPage", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        btnSMSLogin->setText(QCoreApplication::translate("PasswordLoginPage", "\351\252\214\350\257\201\347\240\201\347\231\273\345\275\225", nullptr));
        btnCustomerService->setText(QCoreApplication::translate("PasswordLoginPage", "\345\256\242\346\234\215", nullptr));
        labelPhoneImage->setText(QString());
        lineEditPhone->setPlaceholderText(QCoreApplication::translate("PasswordLoginPage", "\350\276\223\345\205\245\346\202\250\347\232\204\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        btnArrow->setText(QString());
        labelPWImage->setText(QString());
        lineEditPassword->setPlaceholderText(QCoreApplication::translate("PasswordLoginPage", "\350\276\223\345\205\245\346\202\250\347\232\204\345\257\206\347\240\201", nullptr));
        btnReturn->setText(QCoreApplication::translate("PasswordLoginPage", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PasswordLoginPage: public Ui_PasswordLoginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDLOGINPAGE_H
