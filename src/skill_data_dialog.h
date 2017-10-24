///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILL_DATA_DIALOG_H
#define SKILL_DATA_DIALOG_H

#include "node_data.h"

#include <QDialog>
#include <QStandardItem>

class QTabWidget;
class QDialogButtonBox;
class QTableView;

class StatsTab;
class ItemsTab;

//! \brief Handler of all skill data edition for a given node
class SkillDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SkillDataDialog(const QString& node_id,
                             QWidget* parent = Q_NULLPTR);

    ~SkillDataDialog();

    //! \brief Loads node data into dialog
    void loadData(const SkillData& stats_data,
                  const SkillData& items_data);

    //! \brief Gets a copy of updated data from dialog
    void getData(SkillData& stats_data,
                 SkillData& items_data) const;

private:
    //! \brief Tab widget separating both data types
    QTabWidget* _tab_widget;

    //! \brief Accept / Cancel button pair
    QDialogButtonBox* _button_box;

    //! \brief References to tab containers. Do not delete them.
    StatsTab* _stats_tab;
    ItemsTab* _items_tab;
};

class StatsTab : public QWidget
{
    Q_OBJECT

public:
    explicit StatsTab(QWidget* parent = Q_NULLPTR);
    ~StatsTab();

    //! \brief Loads items data into dialog
    void loadData(const SkillData& stat_data);

    //! \brief Gets a copy of updated item data from dialog
    SkillData getData() const;

private slots:
    //! \brief When Append button is clicked
    void appendRow(int32_t stat_id = 0, int32_t stat_bonus = 0);

    //! \brief When Remove button is clicked
    void removeRow();

private:
    //! \brief Table view containing stats data
    QTableView* _stats_table_view;

    //! \brief The table view model. Do not delete it.
    QStandardItemModel* _model;
};

class ItemsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsTab(QWidget* parent = Q_NULLPTR);
    ~ItemsTab();

    //! \brief Loads items data into dialog
    void loadData(const SkillData& items_data);

    //! \brief Gets a copy of updated item data from dialog
    SkillData getData() const;

private slots:
    //! \brief When Append button is clicked
    void appendRow(int32_t item_id = 0, int32_t item_number = 0);

    //! \brief When Remove button is clicked
    void removeRow();

private:
    //! \brief Table view containing items data
    QTableView* _items_table_view;

    //! \brief The table view model. Do not delete it.
    QStandardItemModel* _model;
};

#endif // SKILL_DATA_DIALOG_H
