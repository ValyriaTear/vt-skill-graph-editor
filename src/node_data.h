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
//! node id; vector of linked ids.
using NodeLinksData = std::pair<int32_t, std::vector<int32_t> >;

//! \brief Used for items and stats data
using DataPair = std::pair<int32_t, int32_t>;
using SkillData = std::vector<DataPair>;

//! \brief Combines a node id with each data vector.
using NodeDataPair = std::pair<int32_t, SkillData>;
using NodeSkillData = std::vector<NodeDataPair>;

//! \brief The node combined items ans stats data
struct NodeData {

    //!  \brief Lists of stats bonuses
    NodeSkillData stats_data;

    //!  \brief Lists of required items
    NodeSkillData items_data;

    //!  \brief Lists of stats bonuses
    std::vector<NodeLinksData> node_links;

    //! \brief Node icon filenames
    std::vector<std::pair<int32_t, QString> > icon_filenames;
};

#endif // NODE_DATA_H
