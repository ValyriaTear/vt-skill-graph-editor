///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef NODE_MODEL_H
#define NODE_MODEL_H

#include "node_data.h"

#include <QStandardItemModel>

class LuaReader;

//! \brief the nodes table view column ids
enum NodesTableIds : int32_t {
    PositionX = 0,
    PositionY,
    XPCost,
    Data,
    ColumnsNumber
};

//! \brief The Node table headers
const QString NodesHeaders[ColumnsNumber] = {
    QString("X"),
    QString("Y"),
    QString("XP Cost"),
    QString("Data")
};

//! \brief Custom model class to permit custom behavior on certain columns.
class NodeModel : public QStandardItemModel
{
    Q_OBJECT

    friend LuaReader;

public:
    explicit NodeModel(QObject* parent = Q_NULLPTR);
    virtual ~NodeModel()
    {}

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    //! \brief Adds a link between two nodes
    void addLink(int32_t start_id, int32_t end_id);

    //! \brief Provide reference to one node data, if available
    const NodeData& getNodeData(int32_t node_id) const;

    //! \brief Updates items, stats and other specific data for the given node
    void updateNodeData(int32_t node_id, const NodeData& node_data);

    //! \brief Tells whether the node model has data
    bool isEmpty() const {
        return _nodes_data.empty();
    }

    //! \brief Set the table item data format
    void setRowFormat(int32_t row);

protected:
    void clearData() {
        removeRows(0, rowCount());
        _nodes_data.clear();
    }

    //! \brief Adds a new row on the model with given data
    void newRow(uint32_t x_pos,
                uint32_t y_pos,
                uint32_t xp_cost,
                const NodeData& node_data);

private slots:
    //! \brief Triggered for each changed items in the model
    void onItemChanged(QStandardItem* item);

private:
    //! \brief Nodes specific data
    NodesData _nodes_data;
};

#endif // NODE_MODEL_H
