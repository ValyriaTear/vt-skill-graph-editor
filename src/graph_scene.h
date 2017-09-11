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
class SkillNodesTable;

//! \brief Handler of all skill graph view painting operations.
class GraphScene : public QGraphicsScene
{
public:
    GraphScene(QGraphicsView* view, SkillNodesTable* node_handler);
    ~GraphScene()
    {}

    //! \brief Repaint the whole scene from scratch
    void repaint();

protected:
    //! \name Mouse Processing Functions
    void mousePressEvent(QGraphicsSceneMouseEvent* evt);

private:
    //! \brief Adds a node on the scene with the given id
    void addNode(int32_t id, uint32_t x, uint32_t y);

    //! \brief The graphic view.
    //! Handled by main window, don't delete it.
    QGraphicsView* _view;

    //! \brief The skill nodes handler.
    //! Handled by main window, don't delete it.
    SkillNodesTable* _node_handler;
};

#endif // GRAPH_SCENE_H
