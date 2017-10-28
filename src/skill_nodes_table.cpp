///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "skill_nodes_table.h"

#include "graph_scene.h"
#include "skill_data_dialog.h"

#include <QLineF>

SkillNodesTable::SkillNodesTable(QWidget* parent):
    QTableView(parent),
    _selected_node_id(-1),
    _data_modified(false),
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
    _model->setColumnCount(NodesTableIds::ColumnsNumber);
    for (uint32_t index = 0; index < NodesTableIds::ColumnsNumber; ++index) {
        // N.B.: The model takes ownership of the pointer
        _model->setHorizontalHeaderItem(index, new QStandardItem(NodesHeaders[index]));
    }

    setModel(_model);
    show();
}

void SkillNodesTable::clearData()
{
    setupSkillNodesTableView();
    _scene->repaint();
    _data_modified = false;
}

int32_t SkillNodesTable::appendNodeRow(uint32_t x,
                                       uint32_t y,
                                       uint32_t xp_cost)
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(x))
                      << new QStandardItem(QString::number(y))
                      << new QStandardItem(QString::number(xp_cost)));

    setRowFormat(_model->rowCount() - 1);

    _data_modified = true;

    return _model->rowCount();
}

void SkillNodesTable::setRowFormat(int32_t row)
{
    for (uint32_t i = 0; i < NodesTableIds::ColumnsNumber; ++i) {
        switch(i) {
            // Nothing special
            default:
                continue;
                break;
            // Set the columns as non editable and with gray background
            case NodesTableIds::Data: {
                QStandardItem* item = _model->item(row, i);
                // If no item, create it first
                if (!item) {
                    item = new QStandardItem();
                    _model->setItem(row, i, item);
                }
                item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
                item->setBackground(QBrush(QColor(125, 125, 125, 125)));
                item->setData(QVariant(tr("Click to edit")), Qt::DisplayRole);
            }
        }
    }
}

void SkillNodesTable::removeNodeRow(int32_t row_id)
{
    if (row_id >= _model->rowCount()) {
        qWarning("Invalid row id %d", row_id);
        return;
    }
    _model->removeRow(row_id);
    // Clear selected node
    _selected_node_id = -1;

    // Repaint and resync node ids with table view.
    _scene->repaint();

    _data_modified = true;
}

void SkillNodesTable::selectNodeAndRow(int32_t row_id)
{
    if (row_id >= _model->rowCount()) {
        qWarning("Invalid row id %d", row_id);
        return;
    }

    // If a node was already selected, and it is not the same node,
    // add a link between them
    if (row_id != -1 && _selected_node_id != -1 && row_id != _selected_node_id) {
        _model->addLink(_selected_node_id, row_id);
        // Reset selection
        _selected_node_id = -1;
        _scene->repaint();

        _data_modified = true;
    }
    // Select new node (or unselect it if -1)
    else {
        _selected_node_id = row_id;
        _scene->repaint();
    }

    // Center table on row
    if (row_id != -1)
        selectRow(row_id);
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
    return UnfoundNode;
}

void SkillNodesTable::setNodeRowCoord(int32_t row_id, uint32_t x, uint32_t y)
{
    QStandardItem* item = _model->item(row_id, PositionX);
    if (item) {
        item->setData(QVariant(x), Qt::DisplayRole);
        _data_modified = true;
    }
    item = _model->item(row_id, PositionY);
    if (item) {
        item->setData(QVariant(y), Qt::DisplayRole);
        _data_modified = true;
    }
}

void SkillNodesTable::onDataChanged()
{
    _scene->repaint();
    _data_modified = true;
}

void SkillNodesTable::onRowsremoved()
{
    _scene->repaint();
    _data_modified = true;
}

void SkillNodesTable::clicked(const QModelIndex& index)
{
    QStandardItem* item = _model->itemFromIndex(index);
    if (!item)
        return;
    switch (item->column()) {
    default:
        break;
    case NodesTableIds::Data:
        // Open corresponding edition dialog
        int32_t node_id = item->row();
        SkillDataDialog data_dialog(QString::number(node_id + 1), this);
        // Load current data state into dialog
        data_dialog.loadData(_model->getNodeData(node_id));
        if (data_dialog.exec()) {
            // Set data back in the common ones
            const NodeData& node_data = data_dialog.getData();
            _model->updateNodeData(node_id, node_data);
            _data_modified = true;
        }
        break;
    }
}
