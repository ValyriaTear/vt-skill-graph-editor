///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H

#include <QGraphicsScene>

class QGraphicsView;
class QGraphicsSceneMouseEvent;

//! \brief Handler of all skill graph view painting operations.
class GraphScene : public QGraphicsScene
{
public:
    GraphScene(QGraphicsView* view);
    ~GraphScene()
    {}

    void Repaint();

protected:
    //! \name Mouse Processing Functions
    void mousePressEvent(QGraphicsSceneMouseEvent* evt);

private:
    //! \brief The graphic view. Handled by main window, don't delete it.
    QGraphicsView* _view;
};

#endif // GRAPH_SCENE_H
