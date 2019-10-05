/********************************************************************************
** Form generated from reading UI file 'landingpage.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LANDINGPAGE_H
#define UI_LANDINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
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
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LandingPage
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_17;
    QSpacerItem *verticalSpacer_25;
    QLabel *label;
    QSpacerItem *verticalSpacer_7;
    QFormLayout *formLayout_11;
    QLabel *label_27;
    QLineEdit *lineEdit_usr;
    QLabel *label_28;
    QLineEdit *lineEdit_psw;
    QSpacerItem *verticalSpacer_10;
    QCheckBox *checkBox_saveCredential;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_register;
    QSpacerItem *verticalSpacer_24;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_UsrIcon;
    QSpacerItem *verticalSpacer_2;
    QFormLayout *formLayout_2;
    QLabel *label_15;
    QLabel *label_2;
    QLineEdit *lineEdit_regUsr;
    QLabel *label_5;
    QLineEdit *lineEdit_regNick;
    QLabel *label_6;
    QLineEdit *lineEdit_regPsw;
    QLabel *label_11;
    QLineEdit *lineEdit_regPswConf;
    QHBoxLayout *horizontalLayout_14;
    QLineEdit *lineEdit_UsrIconPath;
    QPushButton *pushButton_browse;
    QLabel *label_9;
    QSpacerItem *verticalSpacer_3;
    QFrame *line;
    QVBoxLayout *verticalLayout_14;
    QSpacerItem *verticalSpacer_22;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_logo;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_8;
    QLabel *label_7;
    QSpacerItem *verticalSpacer_21;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_15;
    QFormLayout *formLayout_7;
    QLabel *label_19;
    QLineEdit *lineEdit_serverIP;
    QLabel *label_20;
    QLineEdit *lineEdit_serverPort;
    QLabel *label_incorrect_operation;
    QPushButton *pushButton_confirmOperation;
    QSpacerItem *verticalSpacer_23;
    QWidget *page_open;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_9;
    QSpacerItem *verticalSpacer_17;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_12;
    QVBoxLayout *verticalLayout_11;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_remove;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_open;
    QSpacerItem *verticalSpacer_8;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_back;
    QSpacerItem *verticalSpacer_16;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_userNick;
    QLabel *label_userUsername;
    QPushButton *pushButton_editProfile;
    QLabel *label_userProfilePhoto;
    QSpacerItem *verticalSpacer_19;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *pushButton_new;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_openuri;
    QSpacerItem *verticalSpacer_9;
    QLabel *label_incorrect_file_operation;
    QSpacerItem *verticalSpacer_20;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LandingPage)
    {
        if (LandingPage->objectName().isEmpty())
            LandingPage->setObjectName(QString::fromUtf8("LandingPage"));
        LandingPage->resize(1068, 602);
        LandingPage->setMinimumSize(QSize(0, 0));
        LandingPage->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(10);
        font.setKerning(true);
        LandingPage->setFont(font);
        centralWidget = new QWidget(LandingPage);
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
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        horizontalLayout_2 = new QHBoxLayout(page_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tabWidget = new QTabWidget(page_2);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::West);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_17 = new QVBoxLayout(tab);
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setContentsMargins(11, 11, 11, 11);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        verticalSpacer_25 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_17->addItem(verticalSpacer_25);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(15);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(label);

        verticalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_17->addItem(verticalSpacer_7);

        formLayout_11 = new QFormLayout();
        formLayout_11->setSpacing(6);
        formLayout_11->setObjectName(QString::fromUtf8("formLayout_11"));
        formLayout_11->setFormAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout_11->setHorizontalSpacing(12);
        formLayout_11->setVerticalSpacing(15);
        label_27 = new QLabel(tab);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        formLayout_11->setWidget(0, QFormLayout::LabelRole, label_27);

        lineEdit_usr = new QLineEdit(tab);
        lineEdit_usr->setObjectName(QString::fromUtf8("lineEdit_usr"));

        formLayout_11->setWidget(0, QFormLayout::FieldRole, lineEdit_usr);

        label_28 = new QLabel(tab);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        formLayout_11->setWidget(1, QFormLayout::LabelRole, label_28);

        lineEdit_psw = new QLineEdit(tab);
        lineEdit_psw->setObjectName(QString::fromUtf8("lineEdit_psw"));
        lineEdit_psw->setEchoMode(QLineEdit::Password);

        formLayout_11->setWidget(1, QFormLayout::FieldRole, lineEdit_psw);


        verticalLayout_17->addLayout(formLayout_11);

        verticalSpacer_10 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_17->addItem(verticalSpacer_10);

        checkBox_saveCredential = new QCheckBox(tab);
        checkBox_saveCredential->setObjectName(QString::fromUtf8("checkBox_saveCredential"));

        verticalLayout_17->addWidget(checkBox_saveCredential);

        verticalSpacer_6 = new QSpacerItem(20, 70, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_17->addItem(verticalSpacer_6);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font2;
        font2.setPointSize(12);
        font2.setUnderline(false);
        label_4->setFont(font2);
        label_4->setAlignment(Qt::AlignCenter);
        label_4->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);

        verticalLayout_17->addWidget(label_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, 0, -1);
        pushButton_register = new QPushButton(tab);
        pushButton_register->setObjectName(QString::fromUtf8("pushButton_register"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_register->sizePolicy().hasHeightForWidth());
        pushButton_register->setSizePolicy(sizePolicy1);
        pushButton_register->setMaximumSize(QSize(75, 16777215));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 85, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        pushButton_register->setPalette(palette);
        QFont font3;
        font3.setPointSize(12);
        font3.setUnderline(true);
        pushButton_register->setFont(font3);
        pushButton_register->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_register->setFlat(true);

        horizontalLayout->addWidget(pushButton_register);


        verticalLayout_17->addLayout(horizontalLayout);

        verticalSpacer_24 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_17->addItem(verticalSpacer_24);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout = new QVBoxLayout(tab_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font4;
        font4.setPointSize(14);
        label_3->setFont(font4);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_UsrIcon = new QLabel(tab_2);
        label_UsrIcon->setObjectName(QString::fromUtf8("label_UsrIcon"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_UsrIcon->sizePolicy().hasHeightForWidth());
        label_UsrIcon->setSizePolicy(sizePolicy2);
        label_UsrIcon->setMinimumSize(QSize(130, 130));
        label_UsrIcon->setMaximumSize(QSize(16777215, 16777215));
        label_UsrIcon->setBaseSize(QSize(0, 0));
        label_UsrIcon->setFrameShape(QFrame::StyledPanel);
        label_UsrIcon->setScaledContents(false);
        label_UsrIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_15->addWidget(label_UsrIcon);


        verticalLayout->addLayout(horizontalLayout_15);

        verticalSpacer_2 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setLabelAlignment(Qt::AlignCenter);
        formLayout_2->setHorizontalSpacing(14);
        formLayout_2->setVerticalSpacing(30);
        formLayout_2->setContentsMargins(-1, -1, -1, 0);
        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_15);

        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_regUsr = new QLineEdit(tab_2);
        lineEdit_regUsr->setObjectName(QString::fromUtf8("lineEdit_regUsr"));
        sizePolicy1.setHeightForWidth(lineEdit_regUsr->sizePolicy().hasHeightForWidth());
        lineEdit_regUsr->setSizePolicy(sizePolicy1);
        lineEdit_regUsr->setMaximumSize(QSize(16777215, 16777215));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit_regUsr);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_5);

        lineEdit_regNick = new QLineEdit(tab_2);
        lineEdit_regNick->setObjectName(QString::fromUtf8("lineEdit_regNick"));
        sizePolicy1.setHeightForWidth(lineEdit_regNick->sizePolicy().hasHeightForWidth());
        lineEdit_regNick->setSizePolicy(sizePolicy1);
        lineEdit_regNick->setMaximumSize(QSize(16777215, 16777215));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_regNick);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_6);

        lineEdit_regPsw = new QLineEdit(tab_2);
        lineEdit_regPsw->setObjectName(QString::fromUtf8("lineEdit_regPsw"));
        sizePolicy1.setHeightForWidth(lineEdit_regPsw->sizePolicy().hasHeightForWidth());
        lineEdit_regPsw->setSizePolicy(sizePolicy1);
        lineEdit_regPsw->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_regPsw->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEdit_regPsw);

        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_11);

        lineEdit_regPswConf = new QLineEdit(tab_2);
        lineEdit_regPswConf->setObjectName(QString::fromUtf8("lineEdit_regPswConf"));
        sizePolicy1.setHeightForWidth(lineEdit_regPswConf->sizePolicy().hasHeightForWidth());
        lineEdit_regPswConf->setSizePolicy(sizePolicy1);
        lineEdit_regPswConf->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_regPswConf->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, lineEdit_regPswConf);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        lineEdit_UsrIconPath = new QLineEdit(tab_2);
        lineEdit_UsrIconPath->setObjectName(QString::fromUtf8("lineEdit_UsrIconPath"));

        horizontalLayout_14->addWidget(lineEdit_UsrIconPath);

        pushButton_browse = new QPushButton(tab_2);
        pushButton_browse->setObjectName(QString::fromUtf8("pushButton_browse"));
        QFont font5;
        font5.setPointSize(10);
        pushButton_browse->setFont(font5);

        horizontalLayout_14->addWidget(pushButton_browse);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_14);


        verticalLayout->addLayout(formLayout_2);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setEnabled(false);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_9);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_2, QString());

        horizontalLayout_2->addWidget(tabWidget);

        line = new QFrame(page_2);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        verticalLayout_14->setContentsMargins(-1, -1, 0, -1);
        verticalSpacer_22 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_14->addItem(verticalSpacer_22);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_logo = new QLabel(page_2);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        sizePolicy2.setHeightForWidth(label_logo->sizePolicy().hasHeightForWidth());
        label_logo->setSizePolicy(sizePolicy2);
        label_logo->setMinimumSize(QSize(200, 200));
        label_logo->setMaximumSize(QSize(16777215, 16777215));
        label_logo->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_logo);


        verticalLayout_14->addLayout(horizontalLayout_5);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_14->addItem(verticalSpacer_5);

        label_8 = new QLabel(page_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        QFont font6;
        font6.setPointSize(18);
        font6.setBold(true);
        font6.setWeight(75);
        label_8->setFont(font6);
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_8);

        label_7 = new QLabel(page_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font7;
        font7.setBold(false);
        font7.setWeight(50);
        label_7->setFont(font7);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_7);

        verticalSpacer_21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_14->addItem(verticalSpacer_21);

        groupBox_5 = new QGroupBox(page_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy3);
        groupBox_5->setMinimumSize(QSize(300, 0));
        verticalLayout_15 = new QVBoxLayout(groupBox_5);
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(9, -1, -1, -1);
        formLayout_7 = new QFormLayout();
        formLayout_7->setSpacing(6);
        formLayout_7->setObjectName(QString::fromUtf8("formLayout_7"));
        formLayout_7->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout_7->setFormAlignment(Qt::AlignCenter);
        formLayout_7->setHorizontalSpacing(15);
        formLayout_7->setVerticalSpacing(7);
        formLayout_7->setContentsMargins(-1, -1, -1, 0);
        label_19 = new QLabel(groupBox_5);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, label_19);

        lineEdit_serverIP = new QLineEdit(groupBox_5);
        lineEdit_serverIP->setObjectName(QString::fromUtf8("lineEdit_serverIP"));
        sizePolicy3.setHeightForWidth(lineEdit_serverIP->sizePolicy().hasHeightForWidth());
        lineEdit_serverIP->setSizePolicy(sizePolicy3);

        formLayout_7->setWidget(0, QFormLayout::FieldRole, lineEdit_serverIP);

        label_20 = new QLabel(groupBox_5);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        formLayout_7->setWidget(1, QFormLayout::LabelRole, label_20);

        lineEdit_serverPort = new QLineEdit(groupBox_5);
        lineEdit_serverPort->setObjectName(QString::fromUtf8("lineEdit_serverPort"));
        sizePolicy3.setHeightForWidth(lineEdit_serverPort->sizePolicy().hasHeightForWidth());
        lineEdit_serverPort->setSizePolicy(sizePolicy3);

        formLayout_7->setWidget(1, QFormLayout::FieldRole, lineEdit_serverPort);


        verticalLayout_15->addLayout(formLayout_7);

        label_incorrect_operation = new QLabel(groupBox_5);
        label_incorrect_operation->setObjectName(QString::fromUtf8("label_incorrect_operation"));
        label_incorrect_operation->setMinimumSize(QSize(0, 30));
        QPalette palette1;
        QBrush brush3(QColor(255, 0, 0, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        label_incorrect_operation->setPalette(palette1);
        label_incorrect_operation->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(label_incorrect_operation);

        pushButton_confirmOperation = new QPushButton(groupBox_5);
        pushButton_confirmOperation->setObjectName(QString::fromUtf8("pushButton_confirmOperation"));
        pushButton_confirmOperation->setMinimumSize(QSize(0, 30));

        verticalLayout_15->addWidget(pushButton_confirmOperation);


        verticalLayout_14->addWidget(groupBox_5);

        verticalSpacer_23 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_14->addItem(verticalSpacer_23);


        horizontalLayout_2->addLayout(verticalLayout_14);

        stackedWidget->addWidget(page_2);
        page_open = new QWidget();
        page_open->setObjectName(QString::fromUtf8("page_open"));
        horizontalLayout_3 = new QHBoxLayout(page_open);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(80, -1, 40, -1);
        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_17);

        groupBox_2 = new QGroupBox(page_open);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(500, 0));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Calibri"));
        font8.setPointSize(14);
        font8.setBold(false);
        font8.setWeight(50);
        groupBox_2->setFont(font8);
        verticalLayout_12 = new QVBoxLayout(groupBox_2);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        listWidget = new QListWidget(groupBox_2);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        sizePolicy3.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy3);

        verticalLayout_11->addWidget(listWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton_remove = new QPushButton(groupBox_2);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));
        pushButton_remove->setEnabled(false);
        pushButton_remove->setMinimumSize(QSize(0, 30));
        QFont font9;
        font9.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font9.setPointSize(10);
        pushButton_remove->setFont(font9);

        horizontalLayout_4->addWidget(pushButton_remove);

        horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_open = new QPushButton(groupBox_2);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));
        pushButton_open->setEnabled(false);
        sizePolicy3.setHeightForWidth(pushButton_open->sizePolicy().hasHeightForWidth());
        pushButton_open->setSizePolicy(sizePolicy3);
        pushButton_open->setMinimumSize(QSize(0, 30));
        QFont font10;
        font10.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font10.setPointSize(10);
        font10.setBold(false);
        font10.setWeight(50);
        pushButton_open->setFont(font10);

        horizontalLayout_4->addWidget(pushButton_open);


        verticalLayout_11->addLayout(horizontalLayout_4);


        verticalLayout_12->addLayout(verticalLayout_11);


        verticalLayout_9->addWidget(groupBox_2);

        verticalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_9->addItem(verticalSpacer_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, -1, 0, 0);
        pushButton_back = new QPushButton(page_open);
        pushButton_back->setObjectName(QString::fromUtf8("pushButton_back"));
        pushButton_back->setMinimumSize(QSize(0, 30));
        pushButton_back->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_6->addWidget(pushButton_back);


        verticalLayout_9->addLayout(horizontalLayout_6);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_16);


        horizontalLayout_3->addLayout(verticalLayout_9);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(40, -1, 40, -1);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, 0, 0, -1);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 0, -1, -1);
        label_userNick = new QLabel(page_open);
        label_userNick->setObjectName(QString::fromUtf8("label_userNick"));
        sizePolicy3.setHeightForWidth(label_userNick->sizePolicy().hasHeightForWidth());
        label_userNick->setSizePolicy(sizePolicy3);
        QFont font11;
        font11.setPointSize(11);
        font11.setBold(true);
        font11.setWeight(75);
        label_userNick->setFont(font11);
        label_userNick->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_userNick);

        label_userUsername = new QLabel(page_open);
        label_userUsername->setObjectName(QString::fromUtf8("label_userUsername"));
        sizePolicy3.setHeightForWidth(label_userUsername->sizePolicy().hasHeightForWidth());
        label_userUsername->setSizePolicy(sizePolicy3);
        QFont font12;
        font12.setPointSize(11);
        label_userUsername->setFont(font12);
        label_userUsername->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_userUsername);

        pushButton_editProfile = new QPushButton(page_open);
        pushButton_editProfile->setObjectName(QString::fromUtf8("pushButton_editProfile"));
        sizePolicy3.setHeightForWidth(pushButton_editProfile->sizePolicy().hasHeightForWidth());
        pushButton_editProfile->setSizePolicy(sizePolicy3);
        pushButton_editProfile->setMaximumSize(QSize(16777212, 16777215));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        pushButton_editProfile->setPalette(palette2);
        QFont font13;
        font13.setUnderline(true);
        pushButton_editProfile->setFont(font13);
        pushButton_editProfile->setFocusPolicy(Qt::NoFocus);
        pushButton_editProfile->setFlat(true);

        verticalLayout_2->addWidget(pushButton_editProfile);


        horizontalLayout_7->addLayout(verticalLayout_2);

        label_userProfilePhoto = new QLabel(page_open);
        label_userProfilePhoto->setObjectName(QString::fromUtf8("label_userProfilePhoto"));
        sizePolicy2.setHeightForWidth(label_userProfilePhoto->sizePolicy().hasHeightForWidth());
        label_userProfilePhoto->setSizePolicy(sizePolicy2);
        label_userProfilePhoto->setMinimumSize(QSize(70, 70));
        label_userProfilePhoto->setFrameShape(QFrame::Box);
        label_userProfilePhoto->setFrameShadow(QFrame::Plain);

        horizontalLayout_7->addWidget(label_userProfilePhoto);


        verticalLayout_8->addLayout(horizontalLayout_7);

        verticalSpacer_19 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_19);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, 0, -1, -1);
        pushButton_new = new QPushButton(page_open);
        pushButton_new->setObjectName(QString::fromUtf8("pushButton_new"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pushButton_new->sizePolicy().hasHeightForWidth());
        pushButton_new->setSizePolicy(sizePolicy4);
        pushButton_new->setMinimumSize(QSize(228, 100));
        pushButton_new->setMaximumSize(QSize(500, 16777215));

        horizontalLayout_9->addWidget(pushButton_new);


        verticalLayout_8->addLayout(horizontalLayout_9);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_8->addItem(verticalSpacer_11);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(-1, 0, -1, -1);
        pushButton_openuri = new QPushButton(page_open);
        pushButton_openuri->setObjectName(QString::fromUtf8("pushButton_openuri"));
        sizePolicy4.setHeightForWidth(pushButton_openuri->sizePolicy().hasHeightForWidth());
        pushButton_openuri->setSizePolicy(sizePolicy4);
        pushButton_openuri->setMinimumSize(QSize(228, 100));
        pushButton_openuri->setMaximumSize(QSize(500, 16777215));

        horizontalLayout_8->addWidget(pushButton_openuri);


        verticalLayout_8->addLayout(horizontalLayout_8);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_8->addItem(verticalSpacer_9);

        label_incorrect_file_operation = new QLabel(page_open);
        label_incorrect_file_operation->setObjectName(QString::fromUtf8("label_incorrect_file_operation"));
        label_incorrect_file_operation->setMinimumSize(QSize(0, 30));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush4(QColor(255, 0, 0, 128));
        brush4.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Active, QPalette::PlaceholderText, brush4);
#endif
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush4);
#endif
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        QBrush brush5(QColor(0, 0, 0, 128));
        brush5.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush5);
#endif
        label_incorrect_file_operation->setPalette(palette3);
        label_incorrect_file_operation->setTextFormat(Qt::AutoText);
        label_incorrect_file_operation->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_incorrect_file_operation);

        verticalSpacer_20 = new QSpacerItem(20, 49, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_20);


        horizontalLayout_3->addLayout(verticalLayout_8);

        stackedWidget->addWidget(page_open);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);

        LandingPage->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(LandingPage);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);
        LandingPage->setStatusBar(statusBar);

        retranslateUi(LandingPage);

        stackedWidget->setCurrentIndex(1);
        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LandingPage);
    } // setupUi

    void retranslateUi(QMainWindow *LandingPage)
    {
        LandingPage->setWindowTitle(QCoreApplication::translate("LandingPage", "LiveText", nullptr));
        label->setText(QCoreApplication::translate("LandingPage", "Enter your credentials to login into your account", nullptr));
        label_27->setText(QCoreApplication::translate("LandingPage", "Username", nullptr));
        label_28->setText(QCoreApplication::translate("LandingPage", "Password", nullptr));
        checkBox_saveCredential->setText(QCoreApplication::translate("LandingPage", "Save this settings as default", nullptr));
        label_4->setText(QCoreApplication::translate("LandingPage", "If you don't have an account", nullptr));
        pushButton_register->setText(QCoreApplication::translate("LandingPage", "Register", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QString());
        label_3->setText(QCoreApplication::translate("LandingPage", "Please fill the form with required information to\n"
"create your account", nullptr));
        label_UsrIcon->setText(QString());
        label_15->setText(QCoreApplication::translate("LandingPage", "Profile icon", nullptr));
        label_2->setText(QCoreApplication::translate("LandingPage", "Username*", nullptr));
        label_5->setText(QCoreApplication::translate("LandingPage", "Nickname", nullptr));
        label_6->setText(QCoreApplication::translate("LandingPage", "Password*", nullptr));
        label_11->setText(QCoreApplication::translate("LandingPage", "Confirm\n"
"password*", nullptr));
        pushButton_browse->setText(QString());
        label_9->setText(QCoreApplication::translate("LandingPage", "* = Required field", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QString());
        label_logo->setText(QString());
        label_8->setText(QCoreApplication::translate("LandingPage", "LiveText", nullptr));
        label_7->setText(QCoreApplication::translate("LandingPage", "Politecnico di Torino\n"
"a.a 2018/2019", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("LandingPage", "Server settings", nullptr));
        label_19->setText(QCoreApplication::translate("LandingPage", "Address", nullptr));
        label_20->setText(QCoreApplication::translate("LandingPage", "Port", nullptr));
        label_incorrect_operation->setText(QString());
        pushButton_confirmOperation->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("LandingPage", "Open existing file", nullptr));
        pushButton_remove->setText(QCoreApplication::translate("LandingPage", "Remove selected", nullptr));
        pushButton_open->setText(QCoreApplication::translate("LandingPage", "Open selected", nullptr));
        pushButton_back->setText(QCoreApplication::translate("LandingPage", "Logout", nullptr));
        label_userNick->setText(QString());
        label_userUsername->setText(QString());
        pushButton_editProfile->setText(QCoreApplication::translate("LandingPage", "Edit profile...", nullptr));
        label_userProfilePhoto->setText(QString());
        pushButton_new->setText(QString());
        pushButton_openuri->setText(QString());
        label_incorrect_file_operation->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LandingPage: public Ui_LandingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LANDINGPAGE_H
