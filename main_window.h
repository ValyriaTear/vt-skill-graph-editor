#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class SkillNodesHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    //! \brief When Append button is clicked
    void AppendNodeRow();

    //! \brief When Remove button is clicked
    void RemoveNodeRow();

    //! \brief When main window is resized
    void resizeEvent(QResizeEvent* event) override;

private:
    //! \brief Main Window UI pointer.
    Ui::MainWindow* _ui;

    // Skill node handler
    SkillNodesHandler* _skillNodesHandler;
};

#endif // MAINWINDOW_H
