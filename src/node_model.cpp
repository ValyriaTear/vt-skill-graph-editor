///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "node_model.h"

NodeModel::NodeModel(QObject* parent):
    QStandardItemModel(parent)
{
    // Link actions
    connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));
}

void NodeModel::addLink(int32_t start_id, int32_t end_id)
{
    for (node_links_data& link_pair : _node_links) {
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
    _node_links.push_back(std::pair<int32_t, std::vector<int32_t> >(start_id, std::vector<int32_t>()));
    _node_links.back().second.push_back(end_id);
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
