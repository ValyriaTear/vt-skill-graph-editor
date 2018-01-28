///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef LUA_READER_H
#define LUA_READER_H

#include "node_model.h"

#include <QString>

class LuaReader
{
public:
    LuaReader()
    {}

    ~LuaReader()
    {}

    //! \brief Reads the data from the given
    //! and add it to the model data.
    bool readFile(const QString& file_path, NodeModel& node_model);
};

#endif // LUA_READER_H
