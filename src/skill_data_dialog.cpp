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

#include <QDialogButtonBox>
#include <QSplitter>
#include <QPushButton>
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

// Item tab

ItemsTab::ItemsTab(QWidget* parent)
    : QWidget(parent),
      _itemsTableView(new QTableView(this)),
      _model(nullptr)
{
    // Set the table view model
    QItemSelectionModel* oldModel = _itemsTableView->selectionModel();
    _model = new QStandardItemModel(_itemsTableView);
    _itemsTableView->setModel(_model);
    delete oldModel;

    // N.B.: The model takes ownership of the data
    _model->setHorizontalHeaderLabels(QStringList()
                                      << tr("Item id")
                                      << tr("Item bonus"));

    // Add buttons
    QSplitter* button_splitter = new QSplitter(this);
    button_splitter->setOrientation(Qt::Horizontal);

    QPushButton* button = new QPushButton(QString("+"), button_splitter);
    button->setFixedSize(30, 30);
    button->setToolTip(tr("Add Item"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(appendRow()));
    button_splitter->addWidget(button);

    button = new QPushButton(QString("-"), button_splitter);
    button->setFixedSize(30, 30);
    button->setToolTip(tr("Remove selected items"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
    button_splitter->addWidget(button);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_itemsTableView);
    mainLayout->addWidget(button_splitter);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

ItemsTab::~ItemsTab()
{
    delete _itemsTableView;
}

void ItemsTab::appendRow()
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(0))
                      << new QStandardItem(QString::number(0)));
}

void ItemsTab::removeRow()
{
    QModelIndexList indexList = _itemsTableView->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < indexList.count(); ++i)
    {
        QModelIndex index = indexList.at(i);
        _model->removeRow(index.row());
    }
}

// Stats tab

StatsTab::StatsTab(QWidget* parent)
    : QWidget(parent),
      _statsTableView(new QTableView()),
      _model(nullptr)
{
    // Set the table view model
    QItemSelectionModel* oldModel = _statsTableView->selectionModel();
    _model = new QStandardItemModel(_statsTableView);
    _statsTableView->setModel(_model);
    delete oldModel;

    // N.B.: The model takes ownership of the data
    _model->setHorizontalHeaderLabels(QStringList()
                                      << tr("Stat id")
                                      << tr("Stat bonus"));

    // Add buttons
    QSplitter* button_splitter = new QSplitter(this);
    button_splitter->setOrientation(Qt::Horizontal);

    QPushButton* button = new QPushButton(QString("+"), button_splitter);
    button->setFixedSize(30, 30);
    button->setToolTip(tr("Add stat bonus"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(appendRow()));
    button_splitter->addWidget(button);

    button = new QPushButton(QString("-"), button_splitter);
    button->setFixedSize(30, 30);
    button->setToolTip(tr("Remove selected stats"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
    button_splitter->addWidget(button);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_statsTableView);
    mainLayout->addWidget(button_splitter);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

StatsTab::~StatsTab()
{
    delete _statsTableView;
}

void StatsTab::appendRow()
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(0))
                      << new QStandardItem(QString::number(0)));
}

void StatsTab::removeRow()
{
    QModelIndexList indexList = _statsTableView->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < indexList.count(); ++i)
    {
        QModelIndex index = indexList.at(i);
        _model->removeRow(index.row());
    }
}
