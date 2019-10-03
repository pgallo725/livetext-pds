/********************************************************************************
** Form generated from reading UI file 'newfilewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWFILEWINDOW_H
#define UI_NEWFILEWINDOW_H

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

class Ui_NewFileWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_fileName;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_incorrectFilename;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewFileWindow)
    {
        if (NewFileWindow->objectName().isEmpty())
            NewFileWindow->setObjectName(QString::fromUtf8("NewFileWindow"));
        NewFileWindow->resize(420, 130);
        NewFileWindow->setMinimumSize(QSize(420, 130));
        NewFileWindow->setMaximumSize(QSize(420, 130));
        verticalLayout = new QVBoxLayout(NewFileWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(NewFileWindow);
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
        label = new QLabel(NewFileWindow);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(10);
        label->setFont(font1);

        horizontalLayout->addWidget(label);

        lineEdit_fileName = new QLineEdit(NewFileWindow);
        lineEdit_fileName->setObjectName(QString::fromUtf8("lineEdit_fileName"));
        lineEdit_fileName->setMinimumSize(QSize(0, 27));
        lineEdit_fileName->setFont(font1);
        lineEdit_fileName->setMaxLength(50);

        horizontalLayout->addWidget(lineEdit_fileName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_incorrectFilename = new QLabel(NewFileWindow);
        label_incorrectFilename->setObjectName(QString::fromUtf8("label_incorrectFilename"));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_incorrectFilename->setPalette(palette);
        label_incorrectFilename->setFont(font1);

        horizontalLayout_2->addWidget(label_incorrectFilename);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonBox = new QDialogButtonBox(NewFileWindow);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setFont(font1);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(NewFileWindow);

        QMetaObject::connectSlotsByName(NewFileWindow);
    } // setupUi

    void retranslateUi(QDialog *NewFileWindow)
    {
        NewFileWindow->setWindowTitle(QApplication::translate("NewFileWindow", "New File", nullptr));
        label_2->setText(QApplication::translate("NewFileWindow", "Choose filename to create a new document", nullptr));
        label->setText(QApplication::translate("NewFileWindow", "Filename", nullptr));
        label_incorrectFilename->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class NewFileWindow: public Ui_NewFileWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWFILEWINDOW_H
