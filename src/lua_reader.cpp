///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2018 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "lua_reader.h"

#include "script/script_read.h"

#include <algorithm>

bool LuaReader::readFile(const QString& file_path, NodeModel& node_model)
{
    vt_script::ReadScriptDescriptor script;
    if (!script.OpenFile(file_path.toStdString()))
        return false;

    if (!script.OpenTable("skill_graph"))
        return false;

    // Prevent the model to emit view change signals during the reset
    node_model.beginResetModel();

    // Clear the data first
    node_model.clearData();

    // Read each node ids
    std::vector<int32_t> table_keys;
    script.ReadTableKeys(table_keys);
    // Sort the node ids to prevent unsync of data
    std::sort(table_keys.begin(), table_keys.end());

    for (size_t i = 0; i < table_keys.size(); ++i) {
        int32_t node_id = table_keys[i];
        if (!script.OpenTable(node_id))
            continue;

        // Check the next row_id will match the node id
        // FIXME: Design must be changed to permit non contiguous ids and more robust loading
        if (node_id != node_model.rowCount()) {
            std::cout << "Mismatch in row counts!!!" << std::endl;
        }

        NodeData node_data = NodeData();
        node_data.node_id = node_id;
        node_data.icon_filename = QString::fromStdString(script.ReadString("icon_file"));
        node_data.skill_id = script.ReadInt("skill_id_learned");

        // Get main row data
        uint32_t x_pos = script.ReadUInt("x_location");
        uint32_t y_pos = script.ReadUInt("y_location");
        uint32_t xp_cost = script.ReadUInt("experience_points_needed");

        if (script.OpenTable("items_needed")) {
            std::vector<int32_t> item_keys;
            script.ReadTableKeys(item_keys);
            for (int32_t item_id : item_keys) {
                int32_t item_number = script.ReadInt(item_id);
                node_data.items_data.emplace_back(DataPair(item_id, item_number));
            }
            script.CloseTable();
        }

        if (script.OpenTable("stats")) {
            std::vector<int32_t> stat_keys;
            script.ReadTableKeys(stat_keys);
            for (int32_t stat_id : stat_keys) {
                int32_t stat_add = script.ReadInt(stat_id);
                node_data.stats_data.emplace_back(DataPair(stat_id, stat_add));
            }
            script.CloseTable();
        }

        script.ReadUIntVector("links", node_data.node_links);

        // Add row data in model
        node_model.newRow(x_pos, y_pos, xp_cost, node_data);

        // node id table
        script.CloseTable();
    }

    // skill_graph table
    script.CloseTable();

    // Emit the view update
    node_model.endResetModel();

    return true;
}
