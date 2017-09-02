
#include "main_window.h"
#include "ui_main_window.h"

#include "skill_nodes_handler.h"

#include <QResizeEvent>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    // Build ui from ui file
    _ui->setupUi(this);

    // Obtain skill table reference and make it handled by the skill node handler
    _skillNodesHandler = new SkillNodesHandler(_ui->skillNodesTableView);

    // Link widgets actions
    connect(_ui->nodeAppendButton, SIGNAL(clicked()), this, SLOT(AppendNodeRow()));
    connect(_ui->nodeRemoveButton, SIGNAL(clicked()), this, SLOT(RemoveNodeRow()));
}

MainWindow::~MainWindow()
{
    // delete the ui instance
    delete _ui;
}

void MainWindow::AppendNodeRow()
{
    _skillNodesHandler->AppendNodeRow();
}

void MainWindow::RemoveNodeRow()
{
    QModelIndexList indexList = _ui->skillNodesTableView->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < indexList.count(); ++i)
    {
        QModelIndex index = indexList.at(i);
        _skillNodesHandler->RemoveNodeRow(index.row());
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   QSize size = event->size();
   int32_t central_height = size.height() - _ui->statusBar->height() - _ui->mainToolBar->height() - 20;
   // Set graphic view size
   _ui->skillGraphGraphicsView->move(0, _ui->mainToolBar->height());
   _ui->skillGraphGraphicsView->resize(size.width() - 500,
                                       central_height);
   // Set the table position and size
   _ui->skillNodesTableView->move(_ui->skillGraphGraphicsView->width() + 10,
                                  _ui->mainToolBar->height());
   _ui->skillNodesTableView->resize(size.width() - _ui->skillGraphGraphicsView->width() - 10,
                                    central_height - _ui->nodeAppendButton->height() - 10);
   // Set the button location
   _ui->nodeAppendButton->move(_ui->skillGraphGraphicsView->width() + 10,
                               _ui->mainToolBar->height() + _ui->skillNodesTableView->height() + 10);
   _ui->nodeRemoveButton->move(_ui->nodeAppendButton->pos().x() + _ui->nodeAppendButton->width() + 10,
                               _ui->nodeAppendButton->pos().y());
}
