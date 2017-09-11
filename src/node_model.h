///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef NODE_MODEL_H
#define NODE_MODEL_H

#include <QStandardItemModel>

//! \brief Custom model class to permit custom behavior on certain columns.
class NodeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit NodeModel(QObject* parent = Q_NULLPTR);
    virtual ~NodeModel()
    {}

private slots:
    //! \brief Triggered when data have been edited in the table view
    void onDataChanged();

    //! \brief Triggered when one or several rows are removed from the table view
    void onRowsRemoved();
};

#endif // NODE_MODEL_H
