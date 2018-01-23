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

#include <QMessageBox>

bool LuaReader::read_file(const QString& file_path, const NodeModel& node_model)
{
    vt_script::ReadScriptDescriptor script;
    if (!script.OpenFile(file_path.toStdString()))
        return false;

    if (!script.OpenTable("skill_graph"))
        return false;

    // Read each node data
    std::vector<int32_t> table_keys;
    script.ReadTableKeys(table_keys);

    // Prepare new node data
    NodesData nodes_data;
    nodes_data.resize(table_keys.size());

    for (size_t i = 0; i < table_keys.size(); ++i) {
        int32_t key = table_keys[i];
        if (!script.OpenTable(key))
            continue;

        NodeData& node_data = nodes_data[i];
        node_data.node_id = key;
        node_data.icon_filename = QString::fromStdString(script.ReadString("icon_file"));
        node_data.skill_id = script.ReadInt("skill_id_learned");

        // TODO Move this back to node data We don't actually need that from the model.
        uint32_t x_location = script.ReadUInt("x_location");
        uint32_t y_location = script.ReadUInt("y_location");
        uint32_t xp_cost = script.ReadUInt("experience_points_needed");

        if (script.OpenTable("items_needed")) {
            // TODO
            script.CloseTable();
        }

        if (script.OpenTable("stats")) {
            // TODO
            script.CloseTable();
        }

        if (script.OpenTable("links")) {
            // TODO
            script.CloseTable();
        }

        // node id table
        script.CloseTable();
    }

    // skill_graph table
    script.CloseTable();

    return false;
}
