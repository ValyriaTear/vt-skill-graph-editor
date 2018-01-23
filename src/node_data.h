///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef NODE_DATA_H
#define NODE_DATA_H

#include <vector>
#include <cstdint>

#include <QString>

//! \brief Links between nodes
//! vector of linked ids.
using NodeLinksData = std::vector<int32_t>;

//! \brief Used for items and stats data
using DataPair = std::pair<int32_t, int32_t>;
using SkillData = std::vector<DataPair>;

//! \brief One node specific data
struct NodeData {
    NodeData():
        node_id(-1),
        skill_id(-1)
    {}

    //! \brief The node id
    int32_t node_id;

    //!  \brief Lists of stats bonuses
    SkillData stats_data;

    //!  \brief Lists of required items
    SkillData items_data;

    //!  \brief Lists of stats bonuses
    NodeLinksData node_links;

    //! \brief Node icon filenames
    QString icon_filename;

    //! \brief Node skill_id
    int32_t skill_id;
};

using NodesData = std::vector<NodeData>;

#endif // NODE_DATA_H
