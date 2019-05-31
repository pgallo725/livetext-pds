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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_logo;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_3;
    QFormLayout *formLayout_3;
    QLabel *label_8;
    QLineEdit *lineEdit_serverIP;
    QLabel *label_9;
    QLineEdit *lineEdit_serverPort;
    QSpacerItem *verticalSpacer_4;
    QFrame *line_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout_5;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *lineEdit_usr;
    QLineEdit *lineEdit_psw;
    QSpacerItem *verticalSpacer_9;
    QPushButton *pushButton_login;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_incorrect_login;
    QSpacerItem *verticalSpacer_6;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_10;
    QPushButton *pushButton_register;
    QSpacerItem *verticalSpacer_8;
    QWidget *page_register;
    QGroupBox *groupBox_3;
    QLabel *label_3;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_regUsr;
    QLabel *label_regNick;
    QLabel *label_regPsw;
    QLabel *label_regPswConf;
    QLineEdit *lineEdit_regUsr;
    QLineEdit *lineEdit_regNick;
    QLineEdit *lineEdit_regPsw;
    QLineEdit *lineEdit_regPswConf;
    QLabel *label_UsrIcon;
    QLabel *label_4;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_UsrIconPath;
    QPushButton *pushButton_browse;
    QPushButton *pushButton_regConf;
    QLabel *label_incorrect_reg;
    QCommandLinkButton *commandLinkButton;
    QWidget *page_open;
    QPushButton *pushButton_new;
    QGroupBox *groupBox_2;
    QListWidget *listWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_openuri;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WelcomeWindow)
    {
        if (WelcomeWindow->objectName().isEmpty())
            WelcomeWindow->setObjectName(QString::fromUtf8("WelcomeWindow"));
        WelcomeWindow->resize(700, 491);
        WelcomeWindow->setMinimumSize(QSize(700, 490));
        WelcomeWindow->setMaximumSize(QSize(10000, 10000));
        QFont font;
        font.setPointSize(10);
        font.setKerning(true);
        WelcomeWindow->setFont(font);
        centralWidget = new QWidget(WelcomeWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        stackedWidget->setFont(font);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        horizontalLayout = new QHBoxLayout(page);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(15);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(40, -1, 40, -1);
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_logo = new QLabel(page);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_logo->sizePolicy().hasHeightForWidth());
        label_logo->setSizePolicy(sizePolicy1);
        label_logo->setMinimumSize(QSize(170, 170));
        label_logo->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_logo);


        verticalLayout_2->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_7 = new QLabel(page);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_7);


        verticalLayout_2->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_5 = new QGroupBox(page);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy2);
        groupBox_5->setMinimumSize(QSize(0, 135));
        verticalLayout_3 = new QVBoxLayout(groupBox_5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setSpacing(6);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout_3->setFormAlignment(Qt::AlignCenter);
        formLayout_3->setHorizontalSpacing(12);
        formLayout_3->setVerticalSpacing(15);
        formLayout_3->setContentsMargins(-1, -1, -1, 0);
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_8);

        lineEdit_serverIP = new QLineEdit(groupBox_5);
        lineEdit_serverIP->setObjectName(QString::fromUtf8("lineEdit_serverIP"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lineEdit_serverIP->sizePolicy().hasHeightForWidth());
        lineEdit_serverIP->setSizePolicy(sizePolicy3);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, lineEdit_serverIP);

        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_9);

        lineEdit_serverPort = new QLineEdit(groupBox_5);
        lineEdit_serverPort->setObjectName(QString::fromUtf8("lineEdit_serverPort"));
        sizePolicy3.setHeightForWidth(lineEdit_serverPort->sizePolicy().hasHeightForWidth());
        lineEdit_serverPort->setSizePolicy(sizePolicy3);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, lineEdit_serverPort);


        verticalLayout_3->addLayout(formLayout_3);


        horizontalLayout_2->addWidget(groupBox_5);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        horizontalLayout->addLayout(verticalLayout_2);

        line_2 = new QFrame(page);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy4);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(40, -1, 40, -1);
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_7);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        groupBox_6 = new QGroupBox(page);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        sizePolicy3.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy3);
        groupBox_6->setMinimumSize(QSize(135, 0));
        verticalLayout_4 = new QVBoxLayout(groupBox_6);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        formLayout_5 = new QFormLayout();
        formLayout_5->setSpacing(6);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFormAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout_5->setHorizontalSpacing(12);
        formLayout_5->setVerticalSpacing(15);
        label_12 = new QLabel(groupBox_6);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_12);

        label_13 = new QLabel(groupBox_6);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, label_13);

        lineEdit_usr = new QLineEdit(groupBox_6);
        lineEdit_usr->setObjectName(QString::fromUtf8("lineEdit_usr"));

        formLayout_5->setWidget(0, QFormLayout::FieldRole, lineEdit_usr);

        lineEdit_psw = new QLineEdit(groupBox_6);
        lineEdit_psw->setObjectName(QString::fromUtf8("lineEdit_psw"));
        lineEdit_psw->setEchoMode(QLineEdit::Password);

        formLayout_5->setWidget(1, QFormLayout::FieldRole, lineEdit_psw);


        verticalLayout_4->addLayout(formLayout_5);

        verticalSpacer_9 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_9);

        pushButton_login = new QPushButton(groupBox_6);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(pushButton_login->sizePolicy().hasHeightForWidth());
        pushButton_login->setSizePolicy(sizePolicy5);
        pushButton_login->setMinimumSize(QSize(0, 30));
        pushButton_login->setAutoDefault(false);

        verticalLayout_4->addWidget(pushButton_login);


        horizontalLayout_7->addWidget(groupBox_6);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_incorrect_login = new QLabel(page);
        label_incorrect_login->setObjectName(QString::fromUtf8("label_incorrect_login"));
        label_incorrect_login->setMinimumSize(QSize(0, 30));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_incorrect_login->setPalette(palette);
        label_incorrect_login->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_incorrect_login);


        verticalLayout->addLayout(horizontalLayout_8);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_5->addItem(verticalSpacer_5);

        label_10 = new QLabel(page);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(0, 30));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_10);

        pushButton_register = new QPushButton(page);
        pushButton_register->setObjectName(QString::fromUtf8("pushButton_register"));
        pushButton_register->setMinimumSize(QSize(0, 30));

        verticalLayout_5->addWidget(pushButton_register);


        verticalLayout->addLayout(verticalLayout_5);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_8);


        horizontalLayout->addLayout(verticalLayout);

        stackedWidget->addWidget(page);
        page_register = new QWidget();
        page_register->setObjectName(QString::fromUtf8("page_register"));
        groupBox_3 = new QGroupBox(page_register);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 0, 621, 381));
        QFont font1;
        font1.setPointSize(11);
        font1.setKerning(true);
        groupBox_3->setFont(font1);
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 40, 581, 31));
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(false);
        font2.setWeight(50);
        label_3->setFont(font2);
        label_3->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        formLayoutWidget = new QWidget(groupBox_3);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(30, 90, 271, 221));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignHCenter|Qt::AlignTop);
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        formLayout->setVerticalSpacing(30);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_regUsr = new QLabel(formLayoutWidget);
        label_regUsr->setObjectName(QString::fromUtf8("label_regUsr"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_regUsr);

        label_regNick = new QLabel(formLayoutWidget);
        label_regNick->setObjectName(QString::fromUtf8("label_regNick"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_regNick);

        label_regPsw = new QLabel(formLayoutWidget);
        label_regPsw->setObjectName(QString::fromUtf8("label_regPsw"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_regPsw);

        label_regPswConf = new QLabel(formLayoutWidget);
        label_regPswConf->setObjectName(QString::fromUtf8("label_regPswConf"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_regPswConf);

        lineEdit_regUsr = new QLineEdit(formLayoutWidget);
        lineEdit_regUsr->setObjectName(QString::fromUtf8("lineEdit_regUsr"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_regUsr);

        lineEdit_regNick = new QLineEdit(formLayoutWidget);
        lineEdit_regNick->setObjectName(QString::fromUtf8("lineEdit_regNick"));
        lineEdit_regNick->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_regNick);

        lineEdit_regPsw = new QLineEdit(formLayoutWidget);
        lineEdit_regPsw->setObjectName(QString::fromUtf8("lineEdit_regPsw"));
        lineEdit_regPsw->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_regPsw);

        lineEdit_regPswConf = new QLineEdit(formLayoutWidget);
        lineEdit_regPswConf->setObjectName(QString::fromUtf8("lineEdit_regPswConf"));
        lineEdit_regPswConf->setEchoMode(QLineEdit::Normal);

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_regPswConf);

        label_UsrIcon = new QLabel(groupBox_3);
        label_UsrIcon->setObjectName(QString::fromUtf8("label_UsrIcon"));
        label_UsrIcon->setGeometry(QRect(420, 90, 131, 131));
        QFont font3;
        font3.setStrikeOut(false);
        label_UsrIcon->setFont(font3);
        label_UsrIcon->setFrameShape(QFrame::StyledPanel);
        label_UsrIcon->setFrameShadow(QFrame::Plain);
        label_UsrIcon->setLineWidth(1);
        label_UsrIcon->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(370, 240, 231, 21));
        label_4->setAlignment(Qt::AlignCenter);
        horizontalLayoutWidget_3 = new QWidget(groupBox_3);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(370, 260, 231, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        lineEdit_UsrIconPath = new QLineEdit(horizontalLayoutWidget_3);
        lineEdit_UsrIconPath->setObjectName(QString::fromUtf8("lineEdit_UsrIconPath"));

        horizontalLayout_3->addWidget(lineEdit_UsrIconPath);

        pushButton_browse = new QPushButton(horizontalLayoutWidget_3);
        pushButton_browse->setObjectName(QString::fromUtf8("pushButton_browse"));
        QFont font4;
        font4.setPointSize(10);
        pushButton_browse->setFont(font4);

        horizontalLayout_3->addWidget(pushButton_browse);

        pushButton_regConf = new QPushButton(groupBox_3);
        pushButton_regConf->setObjectName(QString::fromUtf8("pushButton_regConf"));
        pushButton_regConf->setEnabled(true);
        pushButton_regConf->setGeometry(QRect(370, 340, 231, 31));
        QFont font5;
        font5.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font5.setPointSize(10);
        pushButton_regConf->setFont(font5);
        label_incorrect_reg = new QLabel(groupBox_3);
        label_incorrect_reg->setObjectName(QString::fromUtf8("label_incorrect_reg"));
        label_incorrect_reg->setGeometry(QRect(350, 300, 271, 31));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_incorrect_reg->setPalette(palette1);
        label_incorrect_reg->setTextFormat(Qt::AutoText);
        label_incorrect_reg->setAlignment(Qt::AlignCenter);
        commandLinkButton = new QCommandLinkButton(groupBox_3);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setEnabled(true);
        commandLinkButton->setGeometry(QRect(30, 340, 71, 41));
        sizePolicy3.setHeightForWidth(commandLinkButton->sizePolicy().hasHeightForWidth());
        commandLinkButton->setSizePolicy(sizePolicy3);
        QFont font6;
        font6.setFamily(QString::fromUtf8("Segoe UI"));
        font6.setKerning(true);
        commandLinkButton->setFont(font6);
        commandLinkButton->setAcceptDrops(false);
        commandLinkButton->setAutoFillBackground(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8("images/win/WelcomeWindow/backarrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        commandLinkButton->setIcon(icon);
        stackedWidget->addWidget(page_register);
        page_open = new QWidget();
        page_open->setObjectName(QString::fromUtf8("page_open"));
        pushButton_new = new QPushButton(page_open);
        pushButton_new->setObjectName(QString::fromUtf8("pushButton_new"));
        pushButton_new->setGeometry(QRect(380, 80, 211, 91));
        groupBox_2 = new QGroupBox(page_open);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 50, 301, 291));
        listWidget = new QListWidget(groupBox_2);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 30, 281, 211));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(180, 250, 111, 31));
        pushButton_openuri = new QPushButton(page_open);
        pushButton_openuri->setObjectName(QString::fromUtf8("pushButton_openuri"));
        pushButton_openuri->setGeometry(QRect(380, 200, 211, 91));
        stackedWidget->addWidget(page_open);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        WelcomeWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(WelcomeWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);
        WelcomeWindow->setStatusBar(statusBar);

        retranslateUi(WelcomeWindow);

        stackedWidget->setCurrentIndex(0);
        pushButton_login->setDefault(true);


        QMetaObject::connectSlotsByName(WelcomeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WelcomeWindow)
    {
        WelcomeWindow->setWindowTitle(QApplication::translate("WelcomeWindow", "LiveText", nullptr));
        label_logo->setText(QString());
        label_7->setText(QApplication::translate("WelcomeWindow", "Politecnico di Torino\n"
"a.a 2018/2019", nullptr));
        groupBox_5->setTitle(QApplication::translate("WelcomeWindow", "Server settings", nullptr));
        label_8->setText(QApplication::translate("WelcomeWindow", "Address", nullptr));
        label_9->setText(QApplication::translate("WelcomeWindow", "Port", nullptr));
        groupBox_6->setTitle(QApplication::translate("WelcomeWindow", "Sign in", nullptr));
        label_12->setText(QApplication::translate("WelcomeWindow", "Username", nullptr));
        label_13->setText(QApplication::translate("WelcomeWindow", "Password", nullptr));
        pushButton_login->setText(QApplication::translate("WelcomeWindow", "Login", nullptr));
        label_incorrect_login->setText(QString());
        label_10->setText(QApplication::translate("WelcomeWindow", "If you don't have an account", nullptr));
        pushButton_register->setText(QApplication::translate("WelcomeWindow", "Register", nullptr));
        groupBox_3->setTitle(QApplication::translate("WelcomeWindow", "Registration form", nullptr));
        label_3->setText(QApplication::translate("WelcomeWindow", "Please fill the form with required information to create your account", nullptr));
        label_regUsr->setText(QApplication::translate("WelcomeWindow", "Username", nullptr));
        label_regNick->setText(QApplication::translate("WelcomeWindow", "Nickname\n"
"(optional)", nullptr));
        label_regPsw->setText(QApplication::translate("WelcomeWindow", "Password", nullptr));
        label_regPswConf->setText(QApplication::translate("WelcomeWindow", "Confirm\n"
"password", nullptr));
        label_UsrIcon->setText(QString());
        label_4->setText(QApplication::translate("WelcomeWindow", "Choose your profile icon (optional)", nullptr));
        pushButton_browse->setText(QApplication::translate("WelcomeWindow", "Browse...", nullptr));
        pushButton_regConf->setText(QApplication::translate("WelcomeWindow", "Register", nullptr));
        label_incorrect_reg->setText(QString());
        commandLinkButton->setText(QApplication::translate("WelcomeWindow", "Back", nullptr));
        pushButton_new->setText(QString());
        groupBox_2->setTitle(QApplication::translate("WelcomeWindow", "Open existing file", nullptr));
        pushButton->setText(QApplication::translate("WelcomeWindow", "Open selected", nullptr));
        pushButton_openuri->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WelcomeWindow: public Ui_WelcomeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWINDOW_H
