///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "lua_writer.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

const QString Description(
"-- Describes all the skills and other improvements in the skill graph the characters can get.\n\
-- Each entries are nodes that are linked to a list of other nodes, permiting to set paths between them.\n\
-- Cross a path costs experience points, items, or both.\n\
-- And also the character's start location.");

bool LuaWriter::save(const QString& file_path, const NodeModel& node_model)
{
    QFile file(file_path);

    // Open the file as write with text end lines handling
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    // Save the file
    QTextStream out(&file);

    out << Description << endl;
    out << "skill_graph = {"  << endl;

    for (int32_t id = 0; id < node_model.rowCount(); ++id) {
        // N.B.: Invalid values will be translated into 0
        QModelIndex index = node_model.index(id, PositionX, QModelIndex());
        uint32_t x_location = node_model.data(index).toUInt();

        index = node_model.index(id, PositionY, QModelIndex());
        uint32_t y_location = node_model.data(index).toUInt();

        index = node_model.index(id, XPCost, QModelIndex());
        uint32_t xp_cost = node_model.data(index).toUInt();

        const NodeData& node_data = node_model.getNodeData(id);

        // Write basic data
        out << "\t" << "[" << id << "] = {" << endl;
        out << "\t\t" << "x_location = " << x_location << "," << endl;
        out << "\t\t" << "y_location = " << y_location << "," << endl;
        out << "\t\t" << "icon_file = \"" << node_data.icon_filename << "\"," << endl;

        out << "\t\t" << "-- Experience points cost needed to reach this node" << endl;
        out << "\t\t" << "experience_points_needed = " << xp_cost << "," << endl;

        out << "\t\t" << "-- Skill id learned when reaching this node" << endl;
        out << "\t\t" << "skill_id_learned = " << -1 << "," << endl; // TODO

        out << "\t\t" << "-- items needed to reach this node" << endl;
        out << "\t\t" << "items_needed = {" << endl;
        for (const DataPair& items : node_data.items_data) {
            out << "\t\t\t" << "[" << items.first << "] = " << items.second << "," << endl;
        }
        // Items table end
        out << "\t\t" << "}," << endl;

        out << "\t\t" << "-- stats upgrade when reaching this node" << endl;
        out << "\t\t" << "stats = {" << endl;
        for (const DataPair& stats : node_data.stats_data) {
            out << "\t\t\t" << "[" << stats.first << "] = " << stats.second << "," << endl;
        }
        // Stats table end
        out << "\t\t" << "}," << endl;

        out << "\t\t" << "-- links with other nodes" << endl;
        out << "\t\t" << "links = {" << endl;
        out << "\t\t\t";
        for (int32_t link_id : node_data.node_links) {
            out << link_id << ",";
        }
        out << endl;
        // Links table end
        out << "\t\t" << "}," << endl;

        // End of node table
        out << "\t" << "}," << endl;
    }

    // End of skill grqph table
    out << "}"  << endl;
    file.close();
    return true;
}
