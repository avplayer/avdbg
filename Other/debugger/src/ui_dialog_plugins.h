/********************************************************************************
** Form generated from reading UI file 'dialog_plugins.ui'
**
** Created: Sat Sep 15 17:39:58 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_PLUGINS_H
#define UI_DIALOG_PLUGINS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogPlugins
{
public:
    QGridLayout *gridLayout;
    QTableWidget *plugins_table;
    QDialogButtonBox *button_box;

    void setupUi(QDialog *DialogPlugins)
    {
        if (DialogPlugins->objectName().isEmpty())
            DialogPlugins->setObjectName(QString::fromUtf8("DialogPlugins"));
        DialogPlugins->resize(758, 350);
        gridLayout = new QGridLayout(DialogPlugins);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plugins_table = new QTableWidget(DialogPlugins);
        if (plugins_table->columnCount() < 4)
            plugins_table->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        plugins_table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        plugins_table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        plugins_table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        plugins_table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        plugins_table->setObjectName(QString::fromUtf8("plugins_table"));
        plugins_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        plugins_table->setAlternatingRowColors(true);
        plugins_table->setSelectionMode(QAbstractItemView::SingleSelection);
        plugins_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        plugins_table->verticalHeader()->setVisible(false);

        gridLayout->addWidget(plugins_table, 0, 0, 1, 1);

        button_box = new QDialogButtonBox(DialogPlugins);
        button_box->setObjectName(QString::fromUtf8("button_box"));
        button_box->setOrientation(Qt::Horizontal);
        button_box->setStandardButtons(QDialogButtonBox::Close);

        gridLayout->addWidget(button_box, 1, 0, 1, 1);


        retranslateUi(DialogPlugins);
        QObject::connect(button_box, SIGNAL(accepted()), DialogPlugins, SLOT(accept()));
        QObject::connect(button_box, SIGNAL(rejected()), DialogPlugins, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogPlugins);
    } // setupUi

    void retranslateUi(QDialog *DialogPlugins)
    {
        DialogPlugins->setWindowTitle(QApplication::translate("DialogPlugins", "Plugins", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = plugins_table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DialogPlugins", "Filename", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = plugins_table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DialogPlugins", "Plugin Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = plugins_table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("DialogPlugins", "Author", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = plugins_table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("DialogPlugins", "Website", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogPlugins: public Ui_DialogPlugins {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_PLUGINS_H
