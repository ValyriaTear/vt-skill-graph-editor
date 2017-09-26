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
        // Links and data are non editable through the list
        case NodesTableIds::Links:
        case NodesTableIds::Data:
            break;
    }
}
