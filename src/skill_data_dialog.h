///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILL_DATA_DIALOG_H
#define SKILL_DATA_DIALOG_H

#include <QDialog>
#include <QStandardItem>

class QTabWidget;
class QDialogButtonBox;
class QTableView;

//! \brief Handler of all skill data edition for a given node
class SkillDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SkillDataDialog(const QString& node_id,
                             QWidget* parent = Q_NULLPTR);

    ~SkillDataDialog();

private slots:


private:
    //! \brief Tab widget separating both data types
    QTabWidget* _tabWidget;

    //! \brief Accept / Cancel button pair
    QDialogButtonBox* _buttonBox;
};

class StatsTab : public QWidget
{
    Q_OBJECT

public:
    explicit StatsTab(QWidget* parent = Q_NULLPTR);
    ~StatsTab();

private slots:
    //! \brief When Append button is clicked
    void appendRow();

    //! \brief When Remove button is clicked
    void removeRow();

private:
    //! \brief table view containing stats data
    QTableView* _statsTableView;

    //! \brief The table view model. Do not delete it.
    QStandardItemModel* _model;
};

class ItemsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsTab(QWidget* parent = Q_NULLPTR);
    ~ItemsTab();

private slots:
    //! \brief When Append button is clicked
    void appendRow();

    //! \brief When Remove button is clicked
    void removeRow();

private:
    //! \brief table view containing items data
    QTableView* _itemsTableView;

    //! \brief The table view model. Do not delete it.
    QStandardItemModel* _model;
};

#endif // SKILL_DATA_DIALOG_H
