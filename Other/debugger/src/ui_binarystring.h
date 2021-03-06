/********************************************************************************
** Form generated from reading UI file 'binarystring.ui'
**
** Created: Sat Sep 15 17:39:58 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BINARYSTRING_H
#define UI_BINARYSTRING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BinaryStringWidget
{
public:
    QGridLayout *gridLayout;
    QLineEdit *txtHex;
    QLineEdit *txtAscii;
    QLabel *lblUTF16;
    QLabel *lblAscii;
    QLineEdit *txtUTF16;
    QLabel *lblHex;

    void setupUi(QWidget *BinaryStringWidget)
    {
        if (BinaryStringWidget->objectName().isEmpty())
            BinaryStringWidget->setObjectName(QString::fromUtf8("BinaryStringWidget"));
        BinaryStringWidget->resize(344, 105);
        gridLayout = new QGridLayout(BinaryStringWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        txtHex = new QLineEdit(BinaryStringWidget);
        txtHex->setObjectName(QString::fromUtf8("txtHex"));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        txtHex->setFont(font);

        gridLayout->addWidget(txtHex, 2, 1, 1, 1);

        txtAscii = new QLineEdit(BinaryStringWidget);
        txtAscii->setObjectName(QString::fromUtf8("txtAscii"));
        txtAscii->setFont(font);

        gridLayout->addWidget(txtAscii, 0, 1, 1, 1);

        lblUTF16 = new QLabel(BinaryStringWidget);
        lblUTF16->setObjectName(QString::fromUtf8("lblUTF16"));

        gridLayout->addWidget(lblUTF16, 1, 0, 1, 1);

        lblAscii = new QLabel(BinaryStringWidget);
        lblAscii->setObjectName(QString::fromUtf8("lblAscii"));

        gridLayout->addWidget(lblAscii, 0, 0, 1, 1);

        txtUTF16 = new QLineEdit(BinaryStringWidget);
        txtUTF16->setObjectName(QString::fromUtf8("txtUTF16"));
        txtUTF16->setFont(font);

        gridLayout->addWidget(txtUTF16, 1, 1, 1, 1);

        lblHex = new QLabel(BinaryStringWidget);
        lblHex->setObjectName(QString::fromUtf8("lblHex"));

        gridLayout->addWidget(lblHex, 2, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        lblUTF16->setBuddy(txtUTF16);
        lblAscii->setBuddy(txtAscii);
        lblHex->setBuddy(txtHex);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(txtAscii, txtUTF16);
        QWidget::setTabOrder(txtUTF16, txtHex);

        retranslateUi(BinaryStringWidget);

        QMetaObject::connectSlotsByName(BinaryStringWidget);
    } // setupUi

    void retranslateUi(QWidget *BinaryStringWidget)
    {
        BinaryStringWidget->setWindowTitle(QApplication::translate("BinaryStringWidget", "BinaryString", 0, QApplication::UnicodeUTF8));
        lblUTF16->setText(QApplication::translate("BinaryStringWidget", "UTF-16", 0, QApplication::UnicodeUTF8));
        lblAscii->setText(QApplication::translate("BinaryStringWidget", "ASCII", 0, QApplication::UnicodeUTF8));
        lblHex->setText(QApplication::translate("BinaryStringWidget", "Hex", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BinaryStringWidget: public Ui_BinaryStringWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BINARYSTRING_H
