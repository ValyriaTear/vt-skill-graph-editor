///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "skill_nodes_table.h"

#include "graph_scene.h"

#include <QLineF>

SkillNodesTable::SkillNodesTable(QWidget* parent):
    QTableView(parent),
    _model(nullptr)
{
    // Decorate the table view
    setupSkillNodesTableView();

    // Link actions
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onDataChanged()));
    connect(_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(onRowsremoved()));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));
}

void SkillNodesTable::setupSkillNodesTableView()
{
    if (_model != nullptr)
        delete _model;

    _model = new NodeModel(this);

    // Setup the table model
    _model->setColumnCount(SKILL_TABLE_COL_NB);
    for (uint32_t index = 0; index < SKILL_TABLE_COL_NB; ++index) {
        // N.B.: The model takes ownership of the pointer
        _model->setHorizontalHeaderItem(index, new QStandardItem(NodesHeaders[index]));
    }

    setModel(_model);
    show();
}

int32_t SkillNodesTable::appendNodeRow()
{
    _model->appendRow(nullptr);
    setDataItemFormat(_model->rowCount() - 1);
    return _model->rowCount();
}

int32_t SkillNodesTable::appendNodeRow(uint32_t x, uint32_t y)
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(x))
                      << new QStandardItem(QString::number(y)));

    setDataItemFormat(_model->rowCount() - 1);

    return _model->rowCount();
}

void SkillNodesTable::setDataItemFormat(int32_t row)
{
    // Set the data item as non editable
    QStandardItem* item = _model->item(row, NodesTableIds::Data);
    // If no item, create it first
    if (!item) {
        item = new QStandardItem();
        _model->setItem(row, NodesTableIds::Data, item);
    }
    // Set the data as non editable
    item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
}

void SkillNodesTable::removeNodeRow(int32_t row_id)
{
    if (row_id >= _model->rowCount()) {
        qWarning("Invalid row id %d", row_id);
        return;
    }
    _model->removeRow(row_id);

    // Repaint and resync node ids with table view.
    _scene->repaint();
}

int32_t SkillNodesTable::findNode(uint32_t x,
                                    uint32_t y,
                                    uint32_t search_zone)
{
    for (int32_t i = 0; i < _model->rowCount(); ++i) {
        QModelIndex index = _model->index(i, PositionX, QModelIndex());
        uint32_t search_x = _model->data(index).toUInt();
        index = _model->index(i, PositionY, QModelIndex());
        uint32_t search_y = _model->data(index).toUInt();

        if (search_x == 0 || search_y == 0)
            continue;

        // Check the distance between the 2 points.
        QLineF test_line(x, y, search_x, search_y);
        if (test_line.length() <= static_cast<qreal>(search_zone)) {
            return index.row();
        }
    }
    return UNFOUND_NODE;
}

void SkillNodesTable::onDataChanged()
{
    _scene->repaint();
}

void SkillNodesTable::onRowsremoved()
{
    _scene->repaint();
}

void SkillNodesTable::clicked(const QModelIndex& index)
{
    QStandardItem* item = _model->itemFromIndex(index);
    // Do stuff with the item ...
    qInfo("item clicked");
}