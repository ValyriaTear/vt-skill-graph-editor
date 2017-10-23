///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "skill_data_dialog.h"

#include <QTableView>
#include <QStandardItem>

#include <QDialogButtonBox>
#include <QVBoxLayout>

SkillDataDialog::SkillDataDialog(const QString& node_id, QWidget* parent):
    QDialog(parent),
    _tabWidget(new QTabWidget()),
    _buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok
                                    | QDialogButtonBox::Cancel))
{
    setWindowTitle(tr("Node %1 data").arg(node_id));

    _tabWidget->addTab(new StatsTab(_tabWidget), tr("Stats & Icon"));
    _tabWidget->addTab(new ItemsTab(_tabWidget), tr("Needed Items"));

    // Link button box
    connect(_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_tabWidget);
    mainLayout->addWidget(_buttonBox);
    setLayout(mainLayout);

    resize(300, 300);
}

SkillDataDialog::~SkillDataDialog()
{
    delete _tabWidget;
    delete _buttonBox;
}

ItemsTab::ItemsTab(QWidget* parent)
    : QWidget(parent),
      _itemsTableView(new QTableView(this))
{
    // Set the table view model
    QItemSelectionModel* oldModel = _itemsTableView->selectionModel();
    QStandardItemModel* model = new QStandardItemModel(_itemsTableView);
    _itemsTableView->setModel(model);
     delete oldModel;

    // N.B.: The model takes ownership of the data
    model->setHorizontalHeaderLabels(QStringList()
                                     << tr("Item id")
                                     << tr("Item bonus"));
}

ItemsTab::~ItemsTab()
{
    delete _itemsTableView;
}

StatsTab::StatsTab(QWidget* parent)
    : QWidget(parent),
      _statsTableView(new QTableView(this))
{
    // Set the table view model
    QItemSelectionModel* oldModel = _statsTableView->selectionModel();
    QStandardItemModel* model = new QStandardItemModel(_statsTableView);
    _statsTableView->setModel(model);
     delete oldModel;

    // N.B.: The model takes ownership of the data
    model->setHorizontalHeaderLabels(QStringList()
                                     << tr("Stat id")
                                     << tr("Stat bonus"));
}

StatsTab::~StatsTab()
{
    delete _statsTableView;
}
