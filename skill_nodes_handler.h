#ifndef SKILLNODESHANDLER_H
#define SKILLNODESHANDLER_H

#include <QTableView>
#include <QStandardItemModel>

const int32_t SKILL_TABLE_COL_NB = 6;

//! \brief the nodes table view column ids
enum NodesTableIds : int32_t {
    PositionX = 0,
    PositionY,
    StatsLearned,
    SkillLearned,
    ItemsNeeded,
    XPCost
};

//! \brief The Node table headers
const QString NodesHeaders[SKILL_TABLE_COL_NB] = {
    QString("X"),
    QString("Y"),
    QString("Stats Learned"),
    QString("Skill Learned"),
    QString("Items Needed"),
    QString("XP Cost")
};

/*
//! \brief Items info
struct ItemInfo {
    ItemInfo()
    {}

    ItemInfo(uint32_t id, uint32_t number):
        item_id(id),
        number(number)
    {}

    uint32_t item_id;
    uint32_t number;
};

//! \brief Stats info
struct StatInfo {
    StatInfo()
    {}

    StatInfo(uint32_t id, uint32_t increase):
        item_id(id),
        increase(increase)
    {}

    uint32_t item_id;
    uint32_t increase;
};

//! \brief Node Info struct
struct NodeInfo {
    NodeInfo(uint32_t id,
             float position_x,
             float position_y,
             const std::vector<StatInfo>& stats_info,
             const std::vector<ItemInfo>& items_info,
             int32_t skill_learned,
             uint32_t xp_cost):
        id(id),
        position_x(position_x),
        position_y(position_y),
        skill_learned(skill_learned),
        xp_cost(xp_cost)
    {
        this->stats_info.emplace(stats_info);
        this->items_info.emplace(items_info);
    }

    uint32_t id;
    float position_x;
    float position_y;
    std::vector<StatInfo> stats_info;
    std::vector<ItemInfo> items_info;
    int32_t skill_learned;
    uint32_t xp_cost;
};
*/
class SkillNodesHandler
{
public:
    SkillNodesHandler(QTableView* tableView);

    //! \brief Decorator for the table view
    void SetupSkillNodesTableView(QTableView* tableView);

    void AppendNodeInfo();
private:
    //! \brief Data model which contains the actual data
    QStandardItemModel* _model;
};

#endif // SKILLNODESHANDLER_H
