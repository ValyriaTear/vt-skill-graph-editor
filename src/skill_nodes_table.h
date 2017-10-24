///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILL_NODES_TABLE_H
#define SKILL_NODES_TABLE_H

#include <QTableView>

#include "node_model.h"

class GraphScene;

//! \brief Unfound node row id
const int32_t UnfoundNode = -1;

//! \brief DEFAULT Search area for nodes coords in pixels
const uint32_t DefaultSearchArea = 10;

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

    //! \brief Append a node row with given coordinates
    //! \returns the node Id
    int32_t appendNodeRow(uint32_t x = 0,
                          uint32_t y = 0,
                          uint32_t xp_cost = 0);

    //! \brief Remove the node row at the given index
    void removeNodeRow(int32_t row_id);

    //! \brief Select node and center view on row
    void selectNodeAndRow(int32_t row_id);

    //! \brief Search a node in the list and return its corresponding row id
    //! if found, or UNFOUND_NODE if not.
    //! \param the search zone is in pixels around the center of the given point.
    int32_t findNode(uint32_t x, uint32_t y,
                     uint32_t search_zone = DefaultSearchArea);

    //! \brief Update the node coordinates if the node exists
    void setNodeRowCoord(int32_t row_id, uint32_t x, uint32_t y);

    //! \brief Returns the selected node id or -1 if none
    int32_t getSelectedNodeId() const {
        return _selected_node_id;
    }

    //! \brief Clear selected node id
    void clearSelectedNodeId() {
        _selected_node_id = -1;
    }

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
    void setRowFormat(int32_t row);

    //! \brief The currently selected node id, or -1 if none.
    int32_t _selected_node_id;

    //! \brief Data model which contains the actual data
    NodeModel* _model;

    //! \brief The graph scene.
    //! Handled by Main Window, don't delete it.
    GraphScene* _scene;
};

#endif // SKILL_NODES_TABLE_H
