#include "skill_nodes_handler.h"

SkillNodesHandler::SkillNodesHandler(QTableView* tableView):
    _model(new QStandardItemModel(tableView))
{
    // Decorate the table view
    SetupSkillNodesTableView(tableView);
}

void SkillNodesHandler::SetupSkillNodesTableView(QTableView* tableView)
{
    if (tableView == nullptr) {
        qInfo("Cannot construct table view. Invalid object provided.");
        return;
    }

    if (_model != nullptr) {
        delete _model;
        _model = new QStandardItemModel(tableView);
    }

    // Setup the table model
    _model->setColumnCount(SKILL_TABLE_COL_NB);
    for (uint32_t index = 0; index < SKILL_TABLE_COL_NB; ++index) {
        _model->setHorizontalHeaderItem(index, new QStandardItem(NodesHeaders[index]));
    }

    tableView->setModel(_model);
    tableView->show();
}

void SkillNodesHandler::AppendNodeRow()
{
    _model->appendRow(nullptr);
}

void SkillNodesHandler::RemoveNodeRow(uint32_t row_id)
{
    if (static_cast<int>(row_id) >= _model->rowCount()) {
        qWarning("Invalid row id %u", row_id);
        return;
    }
    _model->removeRow(row_id);
}
