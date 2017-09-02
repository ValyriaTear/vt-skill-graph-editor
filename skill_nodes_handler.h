///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILLNODESHANDLER_H
#define SKILLNODESHANDLER_H

#include <QTableView>
#include <QStandardItemModel>

//! \briaf Number of columns in the Table view
const int32_t SKILL_TABLE_COL_NB = 6;

//! \brief the nodes table view column ids
enum NodesTableIds : int32_t {
    PositionX = 0,
    PositionY,
    StatsLearned,
    SkillLearned,
    ItemsNeeded,
    XPCost
};

//! \brief The Node table headers
const QString NodesHeaders[SKILL_TABLE_COL_NB] = {
    QString("X"),
    QString("Y"),
    QString("Stats Learned"),
    QString("Skill Learned"),
    QString("Items Needed"),
    QString("XP Cost")
};

//! \brief Handler of all skill node operation in the table view
class SkillNodesHandler
{
public:
    SkillNodesHandler(QTableView* tableView);

    //! \brief Decorator for the table view
    void SetupSkillNodesTableView(QTableView* tableView);

    //! \brief Append an empty node row
    void AppendNodeRow();
    //! \brief Remove the node row at the given index
    void RemoveNodeRow(uint32_t row_id);
private:
    //! \brief Data model which contains the actual data
    QStandardItemModel* _model;
};

#endif // SKILLNODESHANDLER_H
