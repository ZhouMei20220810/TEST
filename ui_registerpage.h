/********************************************************************************
** Form generated from reading UI file 'registerpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERPAGE_H
#define UI_REGISTERPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterPage
{
public:
    QFrame *frame_9;
    QHBoxLayout *horizontalLayout_11;
    QLineEdit *lineEditPassword_3;
    QPushButton *pushButton;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_9;
    QLabel *labelPhoneImage_2;
    QLineEdit *lineEditPhone_2;
    QPushButton *btnArrow_2;
    QFrame *frame_8;
    QHBoxLayout *horizontalLayout_10;
    QLabel *labelPWImage_2;
    QLineEdit *lineEditPassword_2;
    QLabel *label;
    QLineEdit *lineEditPassword_4;
    QPushButton *btnReturn;
    QPushButton *btnCustomerService;
    QCheckBox *checkBoxPolicy;
    QPushButton *btnRegister;

    void setupUi(QWidget *RegisterPage)
    {
        if (RegisterPage->objectName().isEmpty())
            RegisterPage->setObjectName("RegisterPage");
        RegisterPage->resize(350, 354);
        frame_9 = new QFrame(RegisterPage);
        frame_9->setObjectName("frame_9");
        frame_9->setGeometry(QRect(30, 165, 279, 33));
        frame_9->setMinimumSize(QSize(0, 33));
        frame_9->setMaximumSize(QSize(16777215, 33));
        frame_9->setStyleSheet(QString::fromUtf8("color:#FF4A4A4A;\n"
"background: #FFE7E8EE;\n"
"border-radius: 3px;"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        horizontalLayout_11 = new QHBoxLayout(frame_9);
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        lineEditPassword_3 = new QLineEdit(frame_9);
        lineEditPassword_3->setObjectName("lineEditPassword_3");
        lineEditPassword_3->setMinimumSize(QSize(0, 0));
        lineEditPassword_3->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(10);
        lineEditPassword_3->setFont(font);
        lineEditPassword_3->setStyleSheet(QString::fromUtf8("border:none"));
        lineEditPassword_3->setEchoMode(QLineEdit::Password);

        horizontalLayout_11->addWidget(lineEditPassword_3);

        pushButton = new QPushButton(frame_9);
        pushButton->setObjectName("pushButton");

        horizontalLayout_11->addWidget(pushButton);

        frame_7 = new QFrame(RegisterPage);
        frame_7->setObjectName("frame_7");
        frame_7->setGeometry(QRect(30, 85, 279, 33));
        frame_7->setMinimumSize(QSize(0, 33));
        frame_7->setMaximumSize(QSize(16777215, 33));
        frame_7->setStyleSheet(QString::fromUtf8("color:#FF4A4A4A;\n"
"background: #FFE7E8EE;\n"
"border-radius: 3px;"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        horizontalLayout_9 = new QHBoxLayout(frame_7);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        labelPhoneImage_2 = new QLabel(frame_7);
        labelPhoneImage_2->setObjectName("labelPhoneImage_2");
        labelPhoneImage_2->setPixmap(QPixmap(QString::fromUtf8(":/login/resource/login/ico_phone.png")));

        horizontalLayout_9->addWidget(labelPhoneImage_2);

        lineEditPhone_2 = new QLineEdit(frame_7);
        lineEditPhone_2->setObjectName("lineEditPhone_2");
        lineEditPhone_2->setFont(font);
        lineEditPhone_2->setStyleSheet(QString::fromUtf8("border:none"));

        horizontalLayout_9->addWidget(lineEditPhone_2);

        btnArrow_2 = new QPushButton(frame_7);
        btnArrow_2->setObjectName("btnArrow_2");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/login/resource/login/ico_down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnArrow_2->setIcon(icon);
        btnArrow_2->setFlat(true);

        horizontalLayout_9->addWidget(btnArrow_2);

        frame_8 = new QFrame(RegisterPage);
        frame_8->setObjectName("frame_8");
        frame_8->setGeometry(QRect(30, 125, 279, 33));
        frame_8->setMinimumSize(QSize(0, 33));
        frame_8->setMaximumSize(QSize(16777215, 33));
        frame_8->setStyleSheet(QString::fromUtf8("color:#FF4A4A4A;\n"
"background: #FFE7E8EE;\n"
"border-radius: 3px;"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        horizontalLayout_10 = new QHBoxLayout(frame_8);
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        labelPWImage_2 = new QLabel(frame_8);
        labelPWImage_2->setObjectName("labelPWImage_2");
        labelPWImage_2->setPixmap(QPixmap(QString::fromUtf8(":/login/resource/login/ico_lock.png")));

        horizontalLayout_10->addWidget(labelPWImage_2);

        lineEditPassword_2 = new QLineEdit(frame_8);
        lineEditPassword_2->setObjectName("lineEditPassword_2");
        lineEditPassword_2->setMinimumSize(QSize(0, 0));
        lineEditPassword_2->setMaximumSize(QSize(16777215, 16777215));
        lineEditPassword_2->setFont(font);
        lineEditPassword_2->setStyleSheet(QString::fromUtf8("border:none"));
        lineEditPassword_2->setEchoMode(QLineEdit::Password);

        horizontalLayout_10->addWidget(lineEditPassword_2);

        label = new QLabel(frame_8);
        label->setObjectName("label");

        horizontalLayout_10->addWidget(label);

        lineEditPassword_4 = new QLineEdit(RegisterPage);
        lineEditPassword_4->setObjectName("lineEditPassword_4");
        lineEditPassword_4->setGeometry(QRect(30, 210, 281, 31));
        lineEditPassword_4->setMinimumSize(QSize(0, 0));
        lineEditPassword_4->setMaximumSize(QSize(16777215, 16777215));
        lineEditPassword_4->setFont(font);
        lineEditPassword_4->setStyleSheet(QString::fromUtf8("border:none"));
        lineEditPassword_4->setEchoMode(QLineEdit::Normal);
        lineEditPassword_4->setAlignment(Qt::AlignCenter);
        btnReturn = new QPushButton(RegisterPage);
        btnReturn->setObjectName("btnReturn");
        btnReturn->setGeometry(QRect(0, 0, 41, 23));
        btnCustomerService = new QPushButton(RegisterPage);
        btnCustomerService->setObjectName("btnCustomerService");
        btnCustomerService->setGeometry(QRect(290, 0, 41, 23));
        checkBoxPolicy = new QCheckBox(RegisterPage);
        checkBoxPolicy->setObjectName("checkBoxPolicy");
        checkBoxPolicy->setGeometry(QRect(41, 255, 294, 16));
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
        btnRegister = new QPushButton(RegisterPage);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setGeometry(QRect(68, 286, 231, 28));
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

        retranslateUi(RegisterPage);

        QMetaObject::connectSlotsByName(RegisterPage);
    } // setupUi

    void retranslateUi(QWidget *RegisterPage)
    {
        RegisterPage->setWindowTitle(QCoreApplication::translate("RegisterPage", "Form", nullptr));
        lineEditPassword_3->setPlaceholderText(QCoreApplication::translate("RegisterPage", "\347\237\255\344\277\241\351\252\214\350\257\201\347\240\201", nullptr));
        pushButton->setText(QCoreApplication::translate("RegisterPage", "\350\216\267\345\217\226\351\252\214\350\257\201\347\240\201", nullptr));
        labelPhoneImage_2->setText(QString());
        lineEditPhone_2->setPlaceholderText(QCoreApplication::translate("RegisterPage", "\350\276\223\345\205\245\346\202\250\347\232\204\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        btnArrow_2->setText(QString());
        labelPWImage_2->setText(QString());
        lineEditPassword_2->setPlaceholderText(QCoreApplication::translate("RegisterPage", "\350\276\223\345\205\245\345\233\276\345\275\242\351\252\214\350\257\201\347\240\201", nullptr));
        label->setText(QCoreApplication::translate("RegisterPage", "\345\233\276\345\275\242\351\252\214\350\257\201\347\240\201", nullptr));
        lineEditPassword_4->setText(QCoreApplication::translate("RegisterPage", "\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        lineEditPassword_4->setPlaceholderText(QCoreApplication::translate("RegisterPage", "\347\237\255\344\277\241\351\252\214\350\257\201\347\240\201", nullptr));
        btnReturn->setText(QCoreApplication::translate("RegisterPage", "\350\277\224\345\233\236", nullptr));
        btnCustomerService->setText(QCoreApplication::translate("RegisterPage", "\345\256\242\346\234\215", nullptr));
        checkBoxPolicy->setText(QCoreApplication::translate("RegisterPage", "\346\210\221\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\347\224\250\346\210\267\345\215\217\350\256\256\357\274\214\351\232\220\347\247\201\346\224\277\347\255\226\345\222\214\345\205\215\350\264\243\345\243\260\346\230\216", nullptr));
        btnRegister->setText(QCoreApplication::translate("RegisterPage", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterPage: public Ui_RegisterPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERPAGE_H
