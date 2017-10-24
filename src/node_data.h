///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef NODE_DATA_H
#define NODE_DATA_H

#include <vector>
#include <cstdint>

//! \brief Links between nodes
//! node id; vector of linked ids.
using node_links_data = std::pair<int32_t, std::vector<int32_t> >;

//! \brief Used for items and stats data
using skill_data = std::pair<int32_t, int32_t>;

//! \brief The node combined items ans stats data
struct node_data {
    //!  \brief Lists of stats bonuses
    std::vector<skill_data> stats_data;

    //!  \brief Lists of required items
    std::vector<skill_data> item_data;

    //!  \brief Lists of stats bonuses
    std::vector<node_links_data> node_links;
};

#endif // NODE_DATA_H
