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
}

void NodeModel::onDataChanged()
{
    qInfo("Data changed in the table view!!!!!!");
}

void NodeModel::onRowsRemoved()
{
    qInfo("Rows removed in the table view!!!!!!");
}
