/********************************************************************************
** Form generated from reading UI file 'bookmarks.ui'
**
** Created: Sat Sep 15 17:41:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKMARKS_H
#define UI_BOOKMARKS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Bookmarks
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *btnAdd;
    QPushButton *btnDel;
    QPushButton *btnClear;

    void setupUi(QWidget *Bookmarks)
    {
        if (Bookmarks->objectName().isEmpty())
            Bookmarks->setObjectName(QString::fromUtf8("Bookmarks"));
        Bookmarks->resize(306, 193);
        gridLayout = new QGridLayout(Bookmarks);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(Bookmarks);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setWordWrap(false);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 3);

        btnAdd = new QPushButton(Bookmarks);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));

        gridLayout->addWidget(btnAdd, 1, 0, 1, 1);

        btnDel = new QPushButton(Bookmarks);
        btnDel->setObjectName(QString::fromUtf8("btnDel"));

        gridLayout->addWidget(btnDel, 1, 1, 1, 1);

        btnClear = new QPushButton(Bookmarks);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));

        gridLayout->addWidget(btnClear, 1, 2, 1, 1);


        retranslateUi(Bookmarks);

        QMetaObject::connectSlotsByName(Bookmarks);
    } // setupUi

    void retranslateUi(QWidget *Bookmarks)
    {
        Bookmarks->setWindowTitle(QApplication::translate("Bookmarks", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Bookmarks", "Address", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Bookmarks", "Comment", 0, QApplication::UnicodeUTF8));
        btnAdd->setText(QApplication::translate("Bookmarks", "Add", 0, QApplication::UnicodeUTF8));
        btnDel->setText(QApplication::translate("Bookmarks", "Del", 0, QApplication::UnicodeUTF8));
        btnClear->setText(QApplication::translate("Bookmarks", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Bookmarks: public Ui_Bookmarks {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKMARKS_H
