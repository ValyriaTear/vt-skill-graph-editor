///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "skill_nodes_handler.h"

#include "graph_scene.h"

#include <QLineF>

SkillNodesHandler::SkillNodesHandler(QTableView* tableView):
    _model(new QStandardItemModel(tableView))
{
    // Decorate the table view
    SetupSkillNodesTableView(tableView);
}

void SkillNodesHandler::SetupSkillNodesTableView(QTableView* tableView)
{
    if (tableView == nullptr) {
        qInfo("Cannot construct table view. Invalid object provided.");
        return;
    }

    if (_model != nullptr) {
        delete _model;
        _model = new QStandardItemModel(tableView);
    }

    // Setup the table model
    _model->setColumnCount(SKILL_TABLE_COL_NB);
    for (uint32_t index = 0; index < SKILL_TABLE_COL_NB; ++index) {
        // N.B.: The model takes ownership of the pointer
        _model->setHorizontalHeaderItem(index, new QStandardItem(NodesHeaders[index]));
    }

    tableView->setModel(_model);
    tableView->show();
}

int32_t SkillNodesHandler::AppendNodeRow()
{
    _model->appendRow(nullptr);
    return _model->rowCount();
}

int32_t SkillNodesHandler::AppendNodeRow(uint32_t x, uint32_t y)
{
    // N.B.: The model takes ownership of the data
    _model->appendRow(QList<QStandardItem*>()
                      << new QStandardItem(QString::number(x))
                      << new QStandardItem(QString::number(y)));
    return _model->rowCount();
}

void SkillNodesHandler::RemoveNodeRow(int32_t row_id)
{
    if (row_id >= _model->rowCount()) {
        qWarning("Invalid row id %d", row_id);
        return;
    }
    _model->removeRow(row_id);

    // Repaint and resync node ids with table view.
    _scene->Repaint();
}

int32_t SkillNodesHandler::FindNode(uint32_t x,
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
