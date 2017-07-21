#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "config.hpp"
#include "dir.hpp"
#include <QMainWindow>
#include <boost/filesystem.hpp>
#include <QString>
#include <string>
#include <vector>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    GekkoFyre::File::Stats findFiles(const boost::filesystem::path &destDir, const std::string &file_ext);
    std::unique_ptr<GekkoFyre::Dir> dir_routine;

private slots:
    void on_apply_pushButton_clicked();
    void on_exit_pushButton_clicked();
    void on_reset_pushButton_clicked();
    void on_dir_toolButton_clicked();
    void on_actionE_xit_triggered();
    void on_action_Documentation_triggered();
    void on_action_About_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
