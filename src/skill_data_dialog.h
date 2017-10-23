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
    QTabWidget* _tabWidget;
    QDialogButtonBox* _buttonBox;

};

class StatsTab : public QWidget
{
    Q_OBJECT

public:
    explicit StatsTab(QWidget* parent = Q_NULLPTR);
    ~StatsTab();

private:
    QTableView* _statsTableView;
};

class ItemsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsTab(QWidget* parent = Q_NULLPTR);
    ~ItemsTab();

private:
    QTableView* _itemsTableView;
};

#endif // SKILL_DATA_DIALOG_H
