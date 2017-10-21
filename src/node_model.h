///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef NODE_MODEL_H
#define NODE_MODEL_H

#include <QStandardItemModel>

using node_links_data = std::pair<int32_t, std::vector<int32_t> >;

//! \brief the nodes table view column ids
enum NodesTableIds : int32_t {
    PositionX = 0,
    PositionY,
    XPCost,
    Links,
    Data,
    ColumnsNumber
};

//! \brief The Node table headers
const QString NodesHeaders[ColumnsNumber] = {
    QString("X"),
    QString("Y"),
    QString("XP Cost"),
    QString("Links"),
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
        return _node_links;
    }

private slots:
    //! \brief Triggered for each changed items in the model
    void onItemChanged(QStandardItem* item);

private:
    //! \brief List of node links
    //! pair of node id: list of linked ids.
    std::vector<node_links_data> _node_links;
};

#endif // NODE_MODEL_H
