///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef LUA_WRITER_H
#define LUA_WRITER_H

#include "node_model.h"

#include <QString>

//! \brief Simple file writer using lua table format to write data out
class LuaWriter {
public:
    LuaWriter()
    {}

    ~LuaWriter()
    {}

    //! \brief Writes the given data to the lua table format
    //! using the provided file and model data.
    bool save(const QString& file_path, const NodeModel& node_model);
};

#endif // LUA_WRITER_H
