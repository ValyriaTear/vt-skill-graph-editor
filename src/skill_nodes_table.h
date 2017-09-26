///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILL_NODES_TABLE_H
#define SKILL_NODES_TABLE_H

#include <QTableView>

#include "node_model.h"

class GraphScene;

//! \brief Unfound node row id
const int32_t UNFOUND_NODE = -1;

//! \brief DEFAULT Search area for nodes coords in pixels
const uint32_t DEFAULT_SEARCH_AREA = 10;

//! \brief Number of columns in the Table view
const int32_t SKILL_TABLE_COL_NB = 5;

//! \brief the nodes table view column ids
enum NodesTableIds : int32_t {
    PositionX = 0,
    PositionY,
    Links,
    XPCost,
    Data
};

//! \brief The Node table headers
const QString NodesHeaders[SKILL_TABLE_COL_NB] = {
    QString("X"),
    QString("Y"),
    QString("Links"),
    QString("XP Cost"),
    QString("Data")
};

//! \brief Handler of all skill node operation in the table view
class SkillNodesTable : public QTableView
{
    Q_OBJECT

public:
    SkillNodesTable(QWidget* parent = Q_NULLPTR);

    ~SkillNodesTable() {
        delete _model;
    }

    //! \brief Decorator for the table view
    void setupSkillNodesTableView();

    //! \brief Set the current node scene
    void setScene(GraphScene* graph_scene) {
        _scene = graph_scene;
    }

    //! \brief Append an empty node row
    //! \returns the node Id
    int32_t appendNodeRow();

    //! \brief Append a node row with given coordinates
    //! \returns the node Id
    int32_t appendNodeRow(uint32_t x, uint32_t y);

    //! \brief Remove the node row at the given index
    void removeNodeRow(int32_t row_id);

    //! \brief Search a node in the list and return its corresponding row id
    //! if found, or UNFOUND_NODE if not.
    //! \param the search zone is in pixels around the center of the given point.
    int32_t findNode(uint32_t x, uint32_t y,
                     uint32_t search_zone = DEFAULT_SEARCH_AREA);

    //! \brief Returns the table view data
    const NodeModel* getData() const {
        return _model;
    }

private slots:
    //! \brief Triggered when data is changed in the table view
    void onDataChanged();

    //! \brief Triggered when rows are removed from the table view
    void onRowsremoved();

    //! \brief Triggered when an item in the list is clicked
    void clicked(const QModelIndex& index);

private:
    //! \brief Set the table item data format
    void setDataItemFormat(int32_t row);

    //! \brief Data model which contains the actual data
    NodeModel* _model;

    //! \brief The graph scene.
    //! Handled by Main Window, don't delete it.
    GraphScene* _scene;
};

#endif // SKILL_NODES_TABLE_H