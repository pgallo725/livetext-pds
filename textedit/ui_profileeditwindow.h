/********************************************************************************
** Form generated from reading UI file 'profileeditwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILEEDITWINDOW_H
#define UI_PROFILEEDITWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProfileEditWindow
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label;
    QSpacerItem *verticalSpacer_12;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_10;
    QSpacerItem *verticalSpacer_2;
    QFormLayout *formLayout_2;
    QLabel *label_2;
    QLabel *label_username;
    QLabel *label_5;
    QLineEdit *lineEdit_editNick;
    QLabel *label_6;
    QLineEdit *lineEdit_password;
    QLabel *label_11;
    QLineEdit *lineEdit_editPsw;
    QLabel *label_4;
    QLineEdit *lineEdit_editPswConf;
    QSpacerItem *verticalSpacer_14;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_UsrIcon;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit_UsrIconPath;
    QPushButton *pushButton_browse;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_incorrect_edit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_updateProfile;
    QSpacerItem *verticalSpacer_13;

    void setupUi(QDialog *ProfileEditWindow)
    {
        if (ProfileEditWindow->objectName().isEmpty())
            ProfileEditWindow->setObjectName(QString::fromUtf8("ProfileEditWindow"));
        ProfileEditWindow->resize(807, 460);
        gridLayout_2 = new QGridLayout(ProfileEditWindow);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(ProfileEditWindow);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font;
        font.setPointSize(10);
        groupBox->setFont(font);
        verticalLayout_6 = new QVBoxLayout(groupBox);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalSpacer_11 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(14);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label);


        verticalLayout_6->addLayout(horizontalLayout_10);

        verticalSpacer_12 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(40);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(40, -1, 40, -1);
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_10->addItem(verticalSpacer_2);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setLabelAlignment(Qt::AlignCenter);
        formLayout_2->setHorizontalSpacing(14);
        formLayout_2->setVerticalSpacing(30);
        formLayout_2->setContentsMargins(-1, -1, -1, 0);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_2);

        label_username = new QLabel(groupBox);
        label_username->setObjectName(QString::fromUtf8("label_username"));
        QPalette palette;
        QBrush brush(QColor(111, 111, 111, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_username->setPalette(palette);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_username);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        lineEdit_editNick = new QLineEdit(groupBox);
        lineEdit_editNick->setObjectName(QString::fromUtf8("lineEdit_editNick"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_editNick->sizePolicy().hasHeightForWidth());
        lineEdit_editNick->setSizePolicy(sizePolicy);
        lineEdit_editNick->setMaximumSize(QSize(320, 16777215));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit_editNick);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        lineEdit_password = new QLineEdit(groupBox);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        sizePolicy.setHeightForWidth(lineEdit_password->sizePolicy().hasHeightForWidth());
        lineEdit_password->setSizePolicy(sizePolicy);
        lineEdit_password->setMaximumSize(QSize(320, 16777215));
        lineEdit_password->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_password);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_11);

        lineEdit_editPsw = new QLineEdit(groupBox);
        lineEdit_editPsw->setObjectName(QString::fromUtf8("lineEdit_editPsw"));
        sizePolicy.setHeightForWidth(lineEdit_editPsw->sizePolicy().hasHeightForWidth());
        lineEdit_editPsw->setSizePolicy(sizePolicy);
        lineEdit_editPsw->setMaximumSize(QSize(320, 16777215));
        lineEdit_editPsw->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEdit_editPsw);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_4);

        lineEdit_editPswConf = new QLineEdit(groupBox);
        lineEdit_editPswConf->setObjectName(QString::fromUtf8("lineEdit_editPswConf"));
        lineEdit_editPswConf->setMaximumSize(QSize(320, 16777215));
        lineEdit_editPswConf->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, lineEdit_editPswConf);


        verticalLayout_10->addLayout(formLayout_2);

        verticalSpacer_14 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_10->addItem(verticalSpacer_14);


        horizontalLayout_11->addLayout(verticalLayout_10);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, -1, -1, -1);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        label_UsrIcon = new QLabel(groupBox);
        label_UsrIcon->setObjectName(QString::fromUtf8("label_UsrIcon"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_UsrIcon->sizePolicy().hasHeightForWidth());
        label_UsrIcon->setSizePolicy(sizePolicy1);
        label_UsrIcon->setMinimumSize(QSize(130, 130));
        label_UsrIcon->setMaximumSize(QSize(130, 130));
        label_UsrIcon->setFrameShape(QFrame::StyledPanel);
        label_UsrIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_UsrIcon);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

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
        pushButton_browse->setFont(font);

        horizontalLayout_14->addWidget(pushButton_browse);


        verticalLayout->addLayout(horizontalLayout_14);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        label_incorrect_edit = new QLabel(groupBox);
        label_incorrect_edit->setObjectName(QString::fromUtf8("label_incorrect_edit"));
        label_incorrect_edit->setMinimumSize(QSize(0, 30));
        QPalette palette1;
        QBrush brush2(QColor(255, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_incorrect_edit->setPalette(palette1);
        label_incorrect_edit->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_incorrect_edit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        pushButton_cancel = new QPushButton(groupBox);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));
        pushButton_cancel->setMinimumSize(QSize(0, 28));

        horizontalLayout_2->addWidget(pushButton_cancel);

        pushButton_updateProfile = new QPushButton(groupBox);
        pushButton_updateProfile->setObjectName(QString::fromUtf8("pushButton_updateProfile"));
        pushButton_updateProfile->setMinimumSize(QSize(190, 28));
        pushButton_updateProfile->setFont(font);

        horizontalLayout_2->addWidget(pushButton_updateProfile);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_11->addLayout(verticalLayout);


        verticalLayout_6->addLayout(horizontalLayout_11);

        verticalSpacer_13 = new QSpacerItem(20, 60, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_6->addItem(verticalSpacer_13);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(ProfileEditWindow);

        QMetaObject::connectSlotsByName(ProfileEditWindow);
    } // setupUi

    void retranslateUi(QDialog *ProfileEditWindow)
    {
        ProfileEditWindow->setWindowTitle(QCoreApplication::translate("ProfileEditWindow", "Edit profile", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ProfileEditWindow", "Edit profile", nullptr));
        label->setText(QCoreApplication::translate("ProfileEditWindow", "Update the form to edit your profile informations", nullptr));
        label_2->setText(QCoreApplication::translate("ProfileEditWindow", "Username", nullptr));
        label_username->setText(QString());
        label_5->setText(QCoreApplication::translate("ProfileEditWindow", "Nickname\n"
"(optional)", nullptr));
        label_6->setText(QCoreApplication::translate("ProfileEditWindow", "Current\n"
"password", nullptr));
        label_11->setText(QCoreApplication::translate("ProfileEditWindow", "New\n"
"password", nullptr));
        label_4->setText(QCoreApplication::translate("ProfileEditWindow", "Confirm\n"
"password", nullptr));
        label_UsrIcon->setText(QString());
        label_3->setText(QCoreApplication::translate("ProfileEditWindow", "Choose your profile icon (optional)", nullptr));
        pushButton_browse->setText(QCoreApplication::translate("ProfileEditWindow", "Browse...", nullptr));
        label_incorrect_edit->setText(QString());
        pushButton_cancel->setText(QCoreApplication::translate("ProfileEditWindow", "Cancel", nullptr));
        pushButton_updateProfile->setText(QCoreApplication::translate("ProfileEditWindow", "Update profile", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProfileEditWindow: public Ui_ProfileEditWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILEEDITWINDOW_H
