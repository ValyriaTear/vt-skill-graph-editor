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

//! \brief Links between nodes
//! node id; vector of linked ids.
using nodeLinksData = std::pair<int32_t, std::vector<int32_t> >;

//! \brief Used for items and stats data
using skillData = std::pair<int32_t, int32_t>;

//! \brief The node combined items ans stats data
struct nodeData {
    //!  \brief Lists of stats bonuses
    std::vector<skillData> stats_data;

    //!  \brief Lists of required items
    std::vector<skillData> item_data;

    //!  \brief Lists of stats bonuses
    std::vector<nodeLinksData> node_links;
};

#endif // NODE_DATA_H
