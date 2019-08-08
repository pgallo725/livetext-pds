/********************************************************************************
** Form generated from reading UI file 'openuriwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENURIWINDOW_H
#define UI_OPENURIWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OpenUriWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_uri;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_incorrectUri;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OpenUriWindow)
    {
        if (OpenUriWindow->objectName().isEmpty())
            OpenUriWindow->setObjectName(QString::fromUtf8("OpenUriWindow"));
        OpenUriWindow->resize(420, 130);
        OpenUriWindow->setMinimumSize(QSize(420, 130));
        OpenUriWindow->setMaximumSize(QSize(420, 130));
        verticalLayout = new QVBoxLayout(OpenUriWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(OpenUriWindow);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(16777215, 1677215));
        QFont font;
        font.setPointSize(11);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(13);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(OpenUriWindow);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(10);
        label->setFont(font1);

        horizontalLayout->addWidget(label);

        lineEdit_uri = new QLineEdit(OpenUriWindow);
        lineEdit_uri->setObjectName(QString::fromUtf8("lineEdit_uri"));
        lineEdit_uri->setMinimumSize(QSize(0, 27));
        lineEdit_uri->setFont(font1);

        horizontalLayout->addWidget(lineEdit_uri);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_incorrectUri = new QLabel(OpenUriWindow);
        label_incorrectUri->setObjectName(QString::fromUtf8("label_incorrectUri"));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_incorrectUri->setPalette(palette);
        label_incorrectUri->setFont(font1);

        horizontalLayout_2->addWidget(label_incorrectUri);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonBox = new QDialogButtonBox(OpenUriWindow);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setFont(font1);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(OpenUriWindow);

        QMetaObject::connectSlotsByName(OpenUriWindow);
    } // setupUi

    void retranslateUi(QDialog *OpenUriWindow)
    {
        OpenUriWindow->setWindowTitle(QCoreApplication::translate("OpenUriWindow", "Open from URI", nullptr));
        label_2->setText(QCoreApplication::translate("OpenUriWindow", "Insert a shared URI to open the document", nullptr));
        label->setText(QCoreApplication::translate("OpenUriWindow", "Document URI", nullptr));
        label_incorrectUri->setText(QCoreApplication::translate("OpenUriWindow", "Inserted URI is not valid", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OpenUriWindow: public Ui_OpenUriWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENURIWINDOW_H
