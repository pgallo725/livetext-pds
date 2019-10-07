/********************************************************************************
** Form generated from reading UI file 'shareuriwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREURIWINDOW_H
#define UI_SHAREURIWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ShareUriWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_uri;
    QPushButton *pushButton_copy;

    void setupUi(QDialog *ShareUriWindow)
    {
        if (ShareUriWindow->objectName().isEmpty())
            ShareUriWindow->setObjectName(QString::fromUtf8("ShareUriWindow"));
        ShareUriWindow->resize(420, 100);
        ShareUriWindow->setMinimumSize(QSize(420, 100));
        ShareUriWindow->setMaximumSize(QSize(420, 100));
        verticalLayout = new QVBoxLayout(ShareUriWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(ShareUriWindow);
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
        lineEdit_uri = new QLineEdit(ShareUriWindow);
        lineEdit_uri->setObjectName(QString::fromUtf8("lineEdit_uri"));
        lineEdit_uri->setMinimumSize(QSize(0, 27));
        QFont font1;
        font1.setPointSize(10);
        lineEdit_uri->setFont(font1);
        lineEdit_uri->setAlignment(Qt::AlignCenter);
        lineEdit_uri->setReadOnly(true);
        lineEdit_uri->setClearButtonEnabled(false);

        horizontalLayout->addWidget(lineEdit_uri);

        pushButton_copy = new QPushButton(ShareUriWindow);
        pushButton_copy->setObjectName(QString::fromUtf8("pushButton_copy"));
        pushButton_copy->setMinimumSize(QSize(0, 30));
        pushButton_copy->setFont(font1);

        horizontalLayout->addWidget(pushButton_copy);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ShareUriWindow);

        QMetaObject::connectSlotsByName(ShareUriWindow);
    } // setupUi

    void retranslateUi(QDialog *ShareUriWindow)
    {
        ShareUriWindow->setWindowTitle(QCoreApplication::translate("ShareUriWindow", "Share URI", nullptr));
        label_2->setText(QCoreApplication::translate("ShareUriWindow", "Share the URI below to guarantee access to this document", nullptr));
        pushButton_copy->setText(QCoreApplication::translate("ShareUriWindow", "Copy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShareUriWindow: public Ui_ShareUriWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREURIWINDOW_H
