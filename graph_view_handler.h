///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef GRAPH_VIEW_HANDLER_H
#define GRAPH_VIEW_HANDLER_H

class QGraphicsView;

//! \brief Handler of all skill graph view painting operations.
class GraphViewHandler
{
public:
    GraphViewHandler(QGraphicsView* view);

    void Repaint();

private:
    //! \brief The graphic view
    QGraphicsView* _view;
};

#endif // GRAPH_VIEW_HANDLER_H
