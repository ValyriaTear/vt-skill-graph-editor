///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "node_model.h"

const SkillData EmptySkillData = SkillData();
const QString EmptyQString = QString();
const NodeData EmptyNodeData = NodeData();

NodeModel::NodeModel(QObject* parent):
    QStandardItemModel(parent)
{
    // Link actions
    connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));
}

bool NodeModel::removeRow(int row, const QModelIndex& parent)
{
    if (!QAbstractItemModel::removeRow(row, parent))
        return false;

    // The row was removed, check each node data
    for (NodesData::iterator it = _nodes_data.begin();
            it != _nodes_data.end();) {

        NodeData& node_data = *it;
        // Remove all information (stats, items, ...) is row data is found
        if (node_data.node_id == row) {
            it = _nodes_data.erase(it);
            continue;
        }

        // Update node ids above the removed row
        if (node_data.node_id > row) {
            (*it).node_id--;
        }

        // Update links data
        for (NodeLinksData::iterator it2 = node_data.node_links.begin();
                it2 != node_data.node_links.end();) {
            int32_t linked_id = (*it2);
            // Remove links of now non existing node
            if (linked_id == row) {
               it2 = node_data.node_links.erase(it2);
               continue;
            }

            // Also update the links id if above the row value
            if (linked_id > row) {
                // If row is below id, decrement the id of links
                (*it2) = linked_id - 1;
            }

            ++it2;
        }

        ++it;
    }

    return true;
}

void NodeModel::addLink(int32_t start_id, int32_t end_id)
{
    bool node_found = false;
    for (NodeData& node_data : _nodes_data) {
        if (node_data.node_id != start_id)
            continue;

        node_found = true;
        // The node already has data, add a new link
        for (int32_t dest_id : node_data.node_links) {
            // Link already exists
            if (dest_id == end_id)
                return;
        }
        node_data.node_links.push_back(end_id);
        return;
    }
    if (!node_found) {
        // No data yet, create node data.
        _nodes_data.push_back(NodeData());
        NodeData& node_data = _nodes_data.back();
        node_data.node_id = start_id;
        node_data.node_links.push_back(end_id);
    }
}

void NodeModel::onItemChanged(QStandardItem* item)
{
    if (!item)
        return;

    // Validate the item depending on its column
    switch (item->column()) {
        default:
            break;
        // Position and XP cost are unsigned integers
        case NodesTableIds::PositionX:
        case NodesTableIds::PositionY:
        case NodesTableIds::XPCost: {
            QVariant value = item->data(Qt::DisplayRole);
            if (!value.isValid())
                value = 0;
            else
                value = value.toUInt();
            item->setData(value, Qt::DisplayRole);
            break;
        }
        // items and skill bonuses data are non editable through the list
        case NodesTableIds::Data:
            break;
    }
}

const NodeData& NodeModel::getNodeData(int32_t node_id) const
{
    // Search the node id
    for (const auto& data : _nodes_data) {
        if (data.node_id != node_id)
            continue;
        // Get the data if found
        return data;
    }
    return EmptyNodeData;
}

const SkillData& NodeModel::getNodeStatsData(int32_t node_id) const
{
    // Search the node id
    for (const NodeData& data : _nodes_data) {
        if (data.node_id != node_id)
            continue;
        // Get the data if found
        return data.stats_data;
    }
    return EmptySkillData;
}

const SkillData& NodeModel::getNodeItemsData(int32_t node_id) const
{
    // Search the node id
    for (const NodeData& data : _nodes_data) {
        if (data.node_id != node_id)
            continue;
        // Get the data if found
        return data.items_data;
    }
    return EmptySkillData;
}

const QString& NodeModel::getNodeIconFilename(int32_t node_id) const
{
    // Search the node id
    for (const NodeData& data : _nodes_data) {
        if (data.node_id != node_id)
            continue;
        // Get the data if found
        return data.icon_filename;
    }
    return EmptyQString;
}

void NodeModel::updateNodeData(int32_t node_id,
                               const SkillData& stats_data,
                               const SkillData& items_data,
                               const QString& icon_filename)
{
    bool node_found = false;
    for (NodeData& node_data : _nodes_data) {
        if (node_data.node_id != node_id)
            continue;

        node_found = true;
        node_data.stats_data = stats_data;
        node_data.items_data = items_data;
        node_data.icon_filename = icon_filename;
    }
    // Adds the data if it was never added before
    if (!node_found) {
        _nodes_data.push_back(NodeData());
        NodeData& node_data = _nodes_data.back();
        node_data.node_id = node_id;
        node_data.stats_data = stats_data;
        node_data.items_data = items_data;
        node_data.icon_filename = icon_filename;
    }
}
