///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "skill_data_dialog.h"

#include <QTableView>

#include <QDialogButtonBox>
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>

SkillDataDialog::SkillDataDialog(const QString& node_id, QWidget* parent):
    QDialog(parent),
    _tab_widget(new QTabWidget()),
    _button_box(new QDialogButtonBox(QDialogButtonBox::Ok
                                    | QDialogButtonBox::Cancel)),
    _stats_tab(nullptr),
    _items_tab(nullptr),
    _icon_tab(nullptr)
{
    setWindowTitle(tr("Node %1 data").arg(node_id));

    _stats_tab = new StatsTab(_tab_widget);
    _items_tab = new ItemsTab(_tab_widget);
    _icon_tab = new IconFileTab(_tab_widget);
    _tab_widget->addTab(_stats_tab, tr("Stats & Icon"));
    _tab_widget->addTab(_items_tab, tr("Needed Items"));
    _tab_widget->addTab(_icon_tab, tr("Icon Filename"));

    // Link button box
    connect(_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(_tab_widget);
    main_layout->addWidget(_button_box);
    setLayout(main_layout);

    resize(300, 300);
}

SkillDataDialog::~SkillDataDialog()
{
    delete _tab_widget;
    delete _button_box;
}

void SkillDataDialog::loadData(const SkillData& stats_data,
                               const SkillData& items_data,
                               const QString& icon_filename)
{
    _stats_tab->loadData(stats_data);
    _items_tab->loadData(items_data);
    _icon_tab->loadData(icon_filename);
}

void SkillDataDialog::getData(SkillData& stats_data,
                              SkillData& items_data,
                              QString& icon_filename) const
{
    stats_data = _stats_tab->getData();
    items_data = _items_tab->getData();
    icon_filename = _icon_tab->getData();
}

// Item tab

ItemsTab::ItemsTab(QWidget* parent)
    : QWidget(parent),
      _items_table_view(new QTableView(this)),
      _model(nullptr)
{
    // Set the table view model
    QItemSelectionModel* old_model = _items_table_view->selectionModel();
    _model = new QStandardItemModel(_items_table_view);
    _items_table_view->setModel(_model);
    delete old_model;

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

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(_items_table_view);
    main_layout->addWidget(button_splitter);
    main_layout->addStretch(1);

    setLayout(main_layout);
}

ItemsTab::~ItemsTab()
{
    delete _items_table_view;
}

void ItemsTab::appendRow(int32_t item_id, int32_t item_number)
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(item_id))
                      << new QStandardItem(QString::number(item_number)));
}

void ItemsTab::removeRow()
{
    QModelIndexList index_list = _items_table_view->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < index_list.count(); ++i)
    {
        QModelIndex index = index_list.at(i);
        _model->removeRow(index.row());
    }
}

void ItemsTab::loadData(const SkillData& items_data)
{
    // Clear list, except headers
    _model->removeRows(0, _model->rowCount());

    // Fill with data
    for (const DataPair& data : items_data) {
        appendRow(data.first, data.second);
    }
}

SkillData ItemsTab::getData() const
{
    SkillData data;
    for (int32_t i = 0; i < _model->rowCount(); ++i) {
        QStandardItem* item = _model->item(i, 0);
        if (item == nullptr)
            continue;
        int32_t id = item->data(Qt::DisplayRole).toInt();
        item = _model->item(i, 1);
        if (item == nullptr)
            continue;
        int32_t number = item->data(Qt::DisplayRole).toInt();
        data.push_back(DataPair(id, number));
    }
    return data;
}

// Stats tab

StatsTab::StatsTab(QWidget* parent)
    : QWidget(parent),
      _stats_table_view(new QTableView()),
      _model(nullptr)
{
    // Set the table view model
    QItemSelectionModel* old_model = _stats_table_view->selectionModel();
    _model = new QStandardItemModel(_stats_table_view);
    _stats_table_view->setModel(_model);
    delete old_model;

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
    mainLayout->addWidget(_stats_table_view);
    mainLayout->addWidget(button_splitter);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

StatsTab::~StatsTab()
{
    delete _stats_table_view;
}

void StatsTab::appendRow(int32_t stat_id, int32_t stat_bonus)
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(stat_id))
                      << new QStandardItem(QString::number(stat_bonus)));
}

void StatsTab::removeRow()
{
    QModelIndexList index_list = _stats_table_view->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < index_list.count(); ++i)
    {
        QModelIndex index = index_list.at(i);
        _model->removeRow(index.row());
    }
}

void StatsTab::loadData(const SkillData& stats_data)
{
    // Clear list, except headers
    _model->removeRows(0, _model->rowCount());

    // Fill with data
    for (const DataPair& data : stats_data) {
        appendRow(data.first, data.second);
    }
}

SkillData StatsTab::getData() const
{
    SkillData data;
    for (int32_t i = 0; i < _model->rowCount(); ++i) {
        QStandardItem* item = _model->item(i, 0);
        if (item == nullptr)
            continue;
        int32_t id = item->data(Qt::DisplayRole).toInt();
        item = _model->item(i, 1);
        if (item == nullptr)
            continue;
        int32_t bonus = item->data(Qt::DisplayRole).toInt();
        data.push_back(DataPair(id, bonus));
    }
    return data;
}

// Icon filename tab

IconFileTab::IconFileTab(QWidget* parent)
    : QWidget(parent),
      _filename_title(new QLabel(tr("Icon Filename:"), this)),
      _filename_edit(new QLineEdit(this))
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_filename_title);
    mainLayout->addWidget(_filename_edit);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

void IconFileTab::loadData(const QString& icon_filename)
{
    _filename_edit->setText(icon_filename);
}

QString IconFileTab::getData() const
{
    return _filename_edit->text();
}
