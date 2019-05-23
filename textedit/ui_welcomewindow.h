/********************************************************************************
** Form generated from reading UI file 'welcomewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEWINDOW_H
#define UI_WELCOMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWindow
{
public:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *page_home;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_usr;
    QLineEdit *lineEdit_usr;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_psw;
    QPushButton *pushButton_login;
    QPushButton *pushButton_register;
    QLabel *label_or;
    QLabel *label_logo;
    QLabel *label;
    QWidget *page_register;
    QWidget *page_open;
    QPushButton *pushButton_new;
    QGroupBox *groupBox_2;
    QListWidget *listWidget;
    QPushButton *pushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WelcomeWindow)
    {
        if (WelcomeWindow->objectName().isEmpty())
            WelcomeWindow->setObjectName(QString::fromUtf8("WelcomeWindow"));
        WelcomeWindow->resize(523, 364);
        centralWidget = new QWidget(WelcomeWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(-10, 0, 521, 351));
        page_home = new QWidget();
        page_home->setObjectName(QString::fromUtf8("page_home"));
        groupBox = new QGroupBox(page_home);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(250, 60, 261, 141));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 241, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_usr = new QLabel(horizontalLayoutWidget);
        label_usr->setObjectName(QString::fromUtf8("label_usr"));

        horizontalLayout->addWidget(label_usr);

        lineEdit_usr = new QLineEdit(horizontalLayoutWidget);
        lineEdit_usr->setObjectName(QString::fromUtf8("lineEdit_usr"));

        horizontalLayout->addWidget(lineEdit_usr);

        horizontalLayoutWidget_2 = new QWidget(groupBox);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 80, 241, 51));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_psw = new QLineEdit(horizontalLayoutWidget_2);
        lineEdit_psw->setObjectName(QString::fromUtf8("lineEdit_psw"));
        lineEdit_psw->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(lineEdit_psw);

        pushButton_login = new QPushButton(page_home);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));
        pushButton_login->setGeometry(QRect(260, 210, 241, 23));
        pushButton_register = new QPushButton(page_home);
        pushButton_register->setObjectName(QString::fromUtf8("pushButton_register"));
        pushButton_register->setGeometry(QRect(260, 270, 241, 23));
        label_or = new QLabel(page_home);
        label_or->setObjectName(QString::fromUtf8("label_or"));
        label_or->setGeometry(QRect(260, 240, 241, 21));
        label_or->setAlignment(Qt::AlignCenter);
        label_logo = new QLabel(page_home);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        label_logo->setGeometry(QRect(40, 60, 161, 151));
        label = new QLabel(page_home);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 230, 161, 61));
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        stackedWidget->addWidget(page_home);
        page_register = new QWidget();
        page_register->setObjectName(QString::fromUtf8("page_register"));
        stackedWidget->addWidget(page_register);
        page_open = new QWidget();
        page_open->setObjectName(QString::fromUtf8("page_open"));
        pushButton_new = new QPushButton(page_open);
        pushButton_new->setObjectName(QString::fromUtf8("pushButton_new"));
        pushButton_new->setGeometry(QRect(350, 100, 111, 111));
        groupBox_2 = new QGroupBox(page_open);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 20, 251, 261));
        listWidget = new QListWidget(groupBox_2);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 30, 231, 221));
        pushButton = new QPushButton(page_open);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 290, 251, 23));
        stackedWidget->addWidget(page_open);
        WelcomeWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(WelcomeWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        WelcomeWindow->setStatusBar(statusBar);

        retranslateUi(WelcomeWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WelcomeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WelcomeWindow)
    {
        WelcomeWindow->setWindowTitle(QApplication::translate("WelcomeWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("WelcomeWindow", "Sign in", nullptr));
        label_usr->setText(QApplication::translate("WelcomeWindow", "Username", nullptr));
        label_2->setText(QApplication::translate("WelcomeWindow", "Password", nullptr));
        pushButton_login->setText(QApplication::translate("WelcomeWindow", "Login", nullptr));
        pushButton_register->setText(QApplication::translate("WelcomeWindow", "Register", nullptr));
        label_or->setText(QApplication::translate("WelcomeWindow", "OR", nullptr));
        label_logo->setText(QString());
        label->setText(QApplication::translate("WelcomeWindow", "Politecnico di Torino\n"
"a.a 2018/2019", nullptr));
        pushButton_new->setText(QString());
        groupBox_2->setTitle(QApplication::translate("WelcomeWindow", "Open existing file", nullptr));
        pushButton->setText(QApplication::translate("WelcomeWindow", "Open selected file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeWindow: public Ui_WelcomeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWINDOW_H
