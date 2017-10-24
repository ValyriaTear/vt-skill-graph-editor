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

public:
    explicit NodeModel(QObject* parent = Q_NULLPTR);
    virtual ~NodeModel()
    {}

    //! \brief Adds a link between two nodes.
    void addLink(int32_t start_id, int32_t end_id);

    //! \brief Provide reference to node links
    const std::vector<node_links_data>& getNodeLinks() const {
        return _node_data.node_links;
    }

private slots:
    //! \brief Triggered for each changed items in the model
    void onItemChanged(QStandardItem* item);

private:
    //! \brief Nodes specific data
    node_data _node_data;
};

#endif // NODE_MODEL_H
