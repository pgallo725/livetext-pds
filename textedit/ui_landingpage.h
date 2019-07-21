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

class Ui_LandingPage
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
    QHBoxLayout *horizontalLayout_9;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label;
    QSpacerItem *verticalSpacer_12;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_10;
    QFormLayout *formLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_regUsr;
    QLabel *label_5;
    QLineEdit *lineEdit_regNick;
    QLabel *label_6;
    QLineEdit *lineEdit_regPsw;
    QLabel *label_11;
    QLineEdit *lineEdit_regPswConf;
    QSpacerItem *verticalSpacer_14;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pushButton_back;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_UsrIcon;
    QSpacerItem *verticalSpacer_10;
    QLabel *label_15;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit_UsrIconPath;
    QPushButton *pushButton_browse;
    QSpacerItem *verticalSpacer_15;
    QLabel *label_incorrect_reg;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *pushButton_regConf;
    QSpacerItem *verticalSpacer_13;
    QWidget *page_open;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_9;
    QSpacerItem *verticalSpacer_17;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_12;
    QVBoxLayout *verticalLayout_11;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_open;
    QSpacerItem *verticalSpacer_16;
    QVBoxLayout *verticalLayout_8;
    QSpacerItem *verticalSpacer_19;
    QPushButton *pushButton_new;
    QSpacerItem *verticalSpacer_18;
    QPushButton *pushButton_openuri;
    QSpacerItem *verticalSpacer_20;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_13;
    QFormLayout *formLayout_3;
    QLabel *label_8;
    QLineEdit *lineEdit_serverIP;
    QLineEdit *lineEdit_serverPort;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_4;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LandingPage)
    {
        if (LandingPage->objectName().isEmpty())
            LandingPage->setObjectName(QString::fromUtf8("LandingPage"));
        LandingPage->resize(914, 560);
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
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
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


        horizontalLayout->addLayout(verticalLayout_2);

        line_2 = new QFrame(page);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy2);
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
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
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

        verticalSpacer_9 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_9);

        pushButton_login = new QPushButton(groupBox_6);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pushButton_login->sizePolicy().hasHeightForWidth());
        pushButton_login->setSizePolicy(sizePolicy4);
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
        horizontalLayout_9 = new QHBoxLayout(page_register);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        groupBox = new QGroupBox(page_register);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font1;
        font1.setPointSize(10);
        groupBox->setFont(font1);
        verticalLayout_6 = new QVBoxLayout(groupBox);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalSpacer_11 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font2;
        font2.setPointSize(14);
        label->setFont(font2);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label);


        verticalLayout_6->addLayout(horizontalLayout_10);

        verticalSpacer_12 = new QSpacerItem(20, 70, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(40);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(40, -1, 40, -1);
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setLabelAlignment(Qt::AlignCenter);
        formLayout_2->setHorizontalSpacing(14);
        formLayout_2->setVerticalSpacing(30);
        formLayout_2->setContentsMargins(-1, -1, -1, 0);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEdit_regUsr = new QLineEdit(groupBox);
        lineEdit_regUsr->setObjectName(QString::fromUtf8("lineEdit_regUsr"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(lineEdit_regUsr->sizePolicy().hasHeightForWidth());
        lineEdit_regUsr->setSizePolicy(sizePolicy5);
        lineEdit_regUsr->setMaximumSize(QSize(320, 16777215));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineEdit_regUsr);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        lineEdit_regNick = new QLineEdit(groupBox);
        lineEdit_regNick->setObjectName(QString::fromUtf8("lineEdit_regNick"));
        sizePolicy5.setHeightForWidth(lineEdit_regNick->sizePolicy().hasHeightForWidth());
        lineEdit_regNick->setSizePolicy(sizePolicy5);
        lineEdit_regNick->setMaximumSize(QSize(320, 16777215));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit_regNick);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        lineEdit_regPsw = new QLineEdit(groupBox);
        lineEdit_regPsw->setObjectName(QString::fromUtf8("lineEdit_regPsw"));
        sizePolicy5.setHeightForWidth(lineEdit_regPsw->sizePolicy().hasHeightForWidth());
        lineEdit_regPsw->setSizePolicy(sizePolicy5);
        lineEdit_regPsw->setMaximumSize(QSize(320, 16777215));
        lineEdit_regPsw->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_regPsw);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_11);

        lineEdit_regPswConf = new QLineEdit(groupBox);
        lineEdit_regPswConf->setObjectName(QString::fromUtf8("lineEdit_regPswConf"));
        sizePolicy5.setHeightForWidth(lineEdit_regPswConf->sizePolicy().hasHeightForWidth());
        lineEdit_regPswConf->setSizePolicy(sizePolicy5);
        lineEdit_regPswConf->setMaximumSize(QSize(320, 16777215));
        lineEdit_regPswConf->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEdit_regPswConf);


        verticalLayout_10->addLayout(formLayout_2);

        verticalSpacer_14 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_10->addItem(verticalSpacer_14);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        pushButton_back = new QPushButton(groupBox);
        pushButton_back->setObjectName(QString::fromUtf8("pushButton_back"));
        pushButton_back->setMinimumSize(QSize(0, 28));

        horizontalLayout_12->addWidget(pushButton_back);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_3);


        verticalLayout_10->addLayout(horizontalLayout_12);


        horizontalLayout_11->addLayout(verticalLayout_10);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_UsrIcon = new QLabel(groupBox);
        label_UsrIcon->setObjectName(QString::fromUtf8("label_UsrIcon"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_UsrIcon->sizePolicy().hasHeightForWidth());
        label_UsrIcon->setSizePolicy(sizePolicy6);
        label_UsrIcon->setMinimumSize(QSize(130, 130));
        label_UsrIcon->setMaximumSize(QSize(130, 130));
        label_UsrIcon->setFrameShape(QFrame::StyledPanel);
        label_UsrIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_15->addWidget(label_UsrIcon);


        verticalLayout_7->addLayout(horizontalLayout_15);

        verticalSpacer_10 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_7->addItem(verticalSpacer_10);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_15);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_2);

        lineEdit_UsrIconPath = new QLineEdit(groupBox);
        lineEdit_UsrIconPath->setObjectName(QString::fromUtf8("lineEdit_UsrIconPath"));

        horizontalLayout_14->addWidget(lineEdit_UsrIconPath);

        pushButton_browse = new QPushButton(groupBox);
        pushButton_browse->setObjectName(QString::fromUtf8("pushButton_browse"));
        pushButton_browse->setFont(font1);

        horizontalLayout_14->addWidget(pushButton_browse);


        verticalLayout_7->addLayout(horizontalLayout_14);

        verticalSpacer_15 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_7->addItem(verticalSpacer_15);

        label_incorrect_reg = new QLabel(groupBox);
        label_incorrect_reg->setObjectName(QString::fromUtf8("label_incorrect_reg"));
        label_incorrect_reg->setMinimumSize(QSize(0, 30));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_incorrect_reg->setPalette(palette1);
        label_incorrect_reg->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_incorrect_reg);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        pushButton_regConf = new QPushButton(groupBox);
        pushButton_regConf->setObjectName(QString::fromUtf8("pushButton_regConf"));
        sizePolicy5.setHeightForWidth(pushButton_regConf->sizePolicy().hasHeightForWidth());
        pushButton_regConf->setSizePolicy(sizePolicy5);
        pushButton_regConf->setMinimumSize(QSize(230, 28));
        pushButton_regConf->setMaximumSize(QSize(350, 16777215));
        pushButton_regConf->setFont(font1);

        horizontalLayout_16->addWidget(pushButton_regConf);


        verticalLayout_7->addLayout(horizontalLayout_16);


        horizontalLayout_11->addLayout(verticalLayout_7);


        verticalLayout_6->addLayout(horizontalLayout_11);

        verticalSpacer_13 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_13);


        horizontalLayout_9->addWidget(groupBox);

        stackedWidget->addWidget(page_register);
        page_open = new QWidget();
        page_open->setObjectName(QString::fromUtf8("page_open"));
        horizontalLayout_3 = new QHBoxLayout(page_open);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(40, -1, 20, -1);
        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_17);

        groupBox_2 = new QGroupBox(page_open);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Calibri"));
        font3.setPointSize(14);
        font3.setBold(false);
        font3.setWeight(50);
        groupBox_2->setFont(font3);
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
        horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_open = new QPushButton(groupBox_2);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));
        pushButton_open->setEnabled(true);
        sizePolicy3.setHeightForWidth(pushButton_open->sizePolicy().hasHeightForWidth());
        pushButton_open->setSizePolicy(sizePolicy3);
        pushButton_open->setMinimumSize(QSize(0, 30));
        QFont font4;
        font4.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setWeight(50);
        pushButton_open->setFont(font4);

        horizontalLayout_4->addWidget(pushButton_open);


        verticalLayout_11->addLayout(horizontalLayout_4);


        verticalLayout_12->addLayout(verticalLayout_11);


        verticalLayout_9->addWidget(groupBox_2);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_16);


        horizontalLayout_3->addLayout(verticalLayout_9);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(20, -1, 40, -1);
        verticalSpacer_19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_19);

        pushButton_new = new QPushButton(page_open);
        pushButton_new->setObjectName(QString::fromUtf8("pushButton_new"));
        pushButton_new->setMinimumSize(QSize(228, 100));

        verticalLayout_8->addWidget(pushButton_new);

        verticalSpacer_18 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_8->addItem(verticalSpacer_18);

        pushButton_openuri = new QPushButton(page_open);
        pushButton_openuri->setObjectName(QString::fromUtf8("pushButton_openuri"));
        pushButton_openuri->setMinimumSize(QSize(228, 100));

        verticalLayout_8->addWidget(pushButton_openuri);

        verticalSpacer_20 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_20);


        horizontalLayout_3->addLayout(verticalLayout_8);

        stackedWidget->addWidget(page_open);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(10, -1, 10, -1);
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        QSizePolicy sizePolicy7(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy7);
        groupBox_5->setMinimumSize(QSize(0, 0));
        horizontalLayout_13 = new QHBoxLayout(groupBox_5);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(9, -1, -1, -1);
        formLayout_3 = new QFormLayout();
        formLayout_3->setSpacing(6);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout_3->setFormAlignment(Qt::AlignCenter);
        formLayout_3->setHorizontalSpacing(5);
        formLayout_3->setVerticalSpacing(7);
        formLayout_3->setContentsMargins(-1, -1, -1, 0);
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_8);

        lineEdit_serverIP = new QLineEdit(groupBox_5);
        lineEdit_serverIP->setObjectName(QString::fromUtf8("lineEdit_serverIP"));
        sizePolicy3.setHeightForWidth(lineEdit_serverIP->sizePolicy().hasHeightForWidth());
        lineEdit_serverIP->setSizePolicy(sizePolicy3);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, lineEdit_serverIP);

        lineEdit_serverPort = new QLineEdit(groupBox_5);
        lineEdit_serverPort->setObjectName(QString::fromUtf8("lineEdit_serverPort"));
        sizePolicy3.setHeightForWidth(lineEdit_serverPort->sizePolicy().hasHeightForWidth());
        lineEdit_serverPort->setSizePolicy(sizePolicy3);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, lineEdit_serverPort);

        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_9);


        horizontalLayout_13->addLayout(formLayout_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_4);


        horizontalLayout_2->addWidget(groupBox_5);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        LandingPage->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(LandingPage);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);
        LandingPage->setStatusBar(statusBar);

        retranslateUi(LandingPage);

        stackedWidget->setCurrentIndex(2);
        pushButton_login->setDefault(true);


        QMetaObject::connectSlotsByName(LandingPage);
    } // setupUi

    void retranslateUi(QMainWindow *LandingPage)
    {
        LandingPage->setWindowTitle(QCoreApplication::translate("LandingPage", "LiveText", nullptr));
        label_logo->setText(QString());
        label_7->setText(QCoreApplication::translate("LandingPage", "Politecnico di Torino\n"
"a.a 2018/2019", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("LandingPage", "Sign in", nullptr));
        label_12->setText(QCoreApplication::translate("LandingPage", "Username", nullptr));
        label_13->setText(QCoreApplication::translate("LandingPage", "Password", nullptr));
        pushButton_login->setText(QCoreApplication::translate("LandingPage", "Login", nullptr));
        label_incorrect_login->setText(QString());
        label_10->setText(QCoreApplication::translate("LandingPage", "If you don't have an account", nullptr));
        pushButton_register->setText(QCoreApplication::translate("LandingPage", "Register", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LandingPage", "Registration form", nullptr));
        label->setText(QCoreApplication::translate("LandingPage", "Please fill the form with required information to create your account", nullptr));
        label_2->setText(QCoreApplication::translate("LandingPage", "Username", nullptr));
        label_5->setText(QCoreApplication::translate("LandingPage", "Nickname\n"
"(optional)", nullptr));
        label_6->setText(QCoreApplication::translate("LandingPage", "Password", nullptr));
        label_11->setText(QCoreApplication::translate("LandingPage", "Confirm\n"
"password", nullptr));
        pushButton_back->setText(QCoreApplication::translate("LandingPage", "Back", nullptr));
        label_UsrIcon->setText(QString());
        label_15->setText(QCoreApplication::translate("LandingPage", "Choose your profile icon (optional)", nullptr));
        pushButton_browse->setText(QCoreApplication::translate("LandingPage", "Browse...", nullptr));
        label_incorrect_reg->setText(QString());
        pushButton_regConf->setText(QCoreApplication::translate("LandingPage", "Register", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("LandingPage", "Open existing file", nullptr));
        pushButton_open->setText(QCoreApplication::translate("LandingPage", "Open selected", nullptr));
        pushButton_new->setText(QString());
        pushButton_openuri->setText(QString());
        groupBox_5->setTitle(QCoreApplication::translate("LandingPage", "Server settings", nullptr));
        label_8->setText(QCoreApplication::translate("LandingPage", "Address", nullptr));
        label_9->setText(QCoreApplication::translate("LandingPage", "Port", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LandingPage: public Ui_LandingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LANDINGPAGE_H
