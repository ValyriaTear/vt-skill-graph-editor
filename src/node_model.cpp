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

    // The row was removed, update the link and other node data
    for (std::vector<NodeLinksData>::iterator it = _node_data.node_links.begin();
            it != _node_data.node_links.end();) {
        int32_t node_id = (*it).first;
        // Remove links of now non existing node
        if (node_id == row) {
           it = _node_data.node_links.erase(it);
           continue;
        }

        if (node_id > row) {
            // If row is below id, decrement the id of links
            (*it).first = node_id - 1;
        }

        // Also update the links id if above the row value
        for (std::vector<int32_t>::iterator it2 = (*it).second.begin();
                it2 != (*it).second.end();) {
            int32_t linked_id = (*it2);
            // Remove linked id if node was removed
            if (linked_id == row) {
                it2 = (*it).second.erase(it2);
                continue;
            }

            // Update the link id if it's above the row value
            if (linked_id > row) {
                (*it2) = linked_id - 1;
            }

            ++it2;
        }

        ++it;
    }

    // Update the stats data
    for (NodeSkillData::iterator it = _node_data.stats_data.begin();
            it != _node_data.stats_data.end();) {

        int32_t node_id = (*it).first;
        if (node_id == row) {
            it = _node_data.stats_data.erase(it);
            continue;
        }

        if (node_id > row) {
            (*it).first = node_id - 1;
        }

        ++it;
    }

    // Update the items data
    for (NodeSkillData::iterator it = _node_data.items_data.begin();
            it != _node_data.items_data.end();) {

        int32_t node_id = (*it).first;
        if (node_id == row) {
            it = _node_data.items_data.erase(it);
            continue;
        }

        if (node_id > row) {
            (*it).first = node_id - 1;
        }

        ++it;
    }

    // Update the icon data
    for (std::vector<std::pair<int32_t, QString>>::iterator it = _node_data.icon_filenames.begin();
            it != _node_data.icon_filenames.end();) {

        int32_t node_id = (*it).first;
        if (node_id == row) {
            it = _node_data.icon_filenames.erase(it);
            continue;
        }

        if (node_id > row) {
            (*it).first = node_id - 1;
        }

        ++it;
    }

    return true;
}

void NodeModel::addLink(int32_t start_id, int32_t end_id)
{
    for (NodeLinksData& link_pair : _node_data.node_links) {
        if (start_id != link_pair.first)
            continue;

        std::vector<int32_t>& links = link_pair.second;
        // The node already has a link, add a new one
        for (int32_t dest_id : links) {
            // Link already exists
            if (dest_id == end_id)
                return;
        }
        links.push_back(end_id);
    }
    // The node does have a link from the start node yet
    _node_data.node_links.push_back(std::pair<int32_t, std::vector<int32_t> >(start_id, std::vector<int32_t>()));
    _node_data.node_links.back().second.push_back(end_id);
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

const SkillData& NodeModel::getNodeStatsData(int32_t node_id) const
{
    // Search the node id
    for (const auto& data : _node_data.stats_data) {
        if (data.first != node_id)
            continue;
        // Get the data if found
        return data.second;
    }
    return EmptySkillData;
}

const SkillData& NodeModel::getNodeItemsData(int32_t node_id) const
{
    // Search the node id
    for (const auto& data : _node_data.items_data) {
        if (data.first != node_id)
            continue;
        // Get the data if found
        return data.second;
    }
    return EmptySkillData;
}

const QString& NodeModel::getNodeIconFilename(int32_t node_id) const
{
    // Search the node id
    for (const auto& data : _node_data.icon_filenames) {
        if (data.first != node_id)
            continue;
        // Get the data if found
        return data.second;
    }
    return EmptyQString;
}

void NodeModel::updateNodeData(int32_t node_id,
                               const SkillData& stats_data,
                               const SkillData& items_data,
                               const QString& icon_filename)
{
    bool found = false;
    for (auto& icon_data_pair : _node_data.icon_filenames) {
        int32_t id = icon_data_pair.first;
        if (id != node_id)
            continue;

        found = true;
        icon_data_pair.second = icon_filename;
    }
    // Adds the data if it was never added before
    if (!found) {
        _node_data.icon_filenames.push_back(std::pair<int32_t, QString>(node_id, icon_filename));
    }

    found = false;
    for (auto& stats_data_pair : _node_data.stats_data) {
        int32_t id = stats_data_pair.first;
        if (id != node_id)
            continue;

        found = true;
        stats_data_pair.second = stats_data;
    }
    // Adds the data if it was never added before
    if (!found) {
        _node_data.stats_data.push_back(NodeDataPair(node_id, stats_data));
    }

    found = false;
    for (auto& items_data_pair : _node_data.items_data) {
        int32_t id = items_data_pair.first;
        if (id != node_id)
            continue;

        found = true;
        items_data_pair.second = items_data;
    }
    // Adds the data if it was never added before
    if (!found) {
        _node_data.items_data.push_back(NodeDataPair(node_id, items_data));
    }
}
