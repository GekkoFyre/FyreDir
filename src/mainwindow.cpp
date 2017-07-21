#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <boost/exception/all.hpp>
#include <exception>
#include <stdexcept>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCore/QCoreApplication>

namespace fs = boost::filesystem;
namespace sys = boost::system;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ext_lineEdit->setText(".MDS");
    ui->new_ext_lineEdit->setText(".mds");
    dir_routine = std::make_unique<GekkoFyre::Dir>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::findFiles will search a directory for files with a specified extension and output them
 * as a vector list at the end.
 * @note  <http://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c>
 *        <http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c>
 *        <http://www.boost.org/doc/libs/1_37_0/libs/filesystem/example/simple_ls.cpp>
 * @param destDir
 * @param file_ext
 * @return
 */
GekkoFyre::File::Stats MainWindow::findFiles(const fs::path &destDir, const std::string &file_ext)
{
    GekkoFyre::File::Stats file_stat;
    file_stat.err_count = 0;
    file_stat.file_count = 0;
    fs::path full_path = fs::system_complete(destDir);
    if (fs::exists(full_path)) {
        // Destination directory exists!
        if (ui->recursively_enter_checkBox->isChecked()) {
            fs::recursive_directory_iterator end_iter;
            for (fs::recursive_directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr) {
                try {
                    if (fs::is_directory(dir_itr->status())) {
                        ++file_stat.dir_count;
                        continue;
                    } else if (fs::is_regular_file(dir_itr->status())) {
                        if (fs::extension(dir_itr->path().filename()) == file_ext) {
                            ++file_stat.file_count;
                            file_stat.files_vec.push_back(dir_itr->path());
                            continue;
                        }
                    } else {
                        continue;
                    }
                } catch (const std::exception &e) {
                    ++file_stat.err_count;
                }
            }
        } else {
            fs::directory_iterator end_iter;
            for (fs::directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr) {
                try {
                    if (fs::is_directory(dir_itr->status())) {
                        ++file_stat.dir_count;
                        continue;
                    } else if (fs::is_regular_file(dir_itr->status())) {
                        if (fs::extension(dir_itr->path().filename()) == file_ext) {
                            ++file_stat.file_count;
                            file_stat.files_vec.push_back(dir_itr->path());
                            continue;
                        }
                    } else {
                        continue;
                    }
                } catch (const std::exception &e) {
                    ++file_stat.err_count;
                }
            }
        }

        return file_stat;
    } else {
        throw std::invalid_argument(tr("The path does not exist any longer!").toStdString());
    }
}

void MainWindow::on_apply_pushButton_clicked()
{
    try {
        QString dirDest = ui->dir_lineEdit->text();
        QString newExt = ui->new_ext_lineEdit->text();
        QString curExt = ui->ext_lineEdit->text();

        fs::path boost_dirDest(dirDest.toStdString());
        if (fs::exists(boost_dirDest)) {
            QMessageBox file_ask;
            file_ask.setWindowTitle(tr("ExtRenamer"));
            file_ask.setText(tr("Are you sure you wish to modify the directory, \"%1\", and its contents?").arg(dirDest));
            file_ask.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            file_ask.setDefaultButton(QMessageBox::No);
            file_ask.setModal(false); // Prevents certain bugs from possibly occuring within the application
            int ret_file_ask = file_ask.exec();

            switch (ret_file_ask) {
            case QMessageBox::Yes:
            {
                GekkoFyre::File::Stats file_stat = findFiles(boost_dirDest, curExt.toStdString());

                QMessageBox confirm;
                confirm.setWindowTitle(tr("Proceed..."));
                if (ui->recursively_enter_checkBox->isChecked()) {
                    confirm.setText(tr("Files found with the given extension: %1\nDirectories scanned: "
                                               "%2\nErrors that occurred: %3\n\nProceed with renaming?")
                                            .arg(QString::number(file_stat.file_count))
                                            .arg(QString::number(file_stat.dir_count))
                                            .arg(QString::number(file_stat.err_count)));
                } else {
                    confirm.setText(tr("Files found with the given extension: %1\nErrors that occurred: "
                                               "%2\n\nProceed with renaming?")
                                            .arg(QString::number(file_stat.file_count))
                                            .arg(QString::number(file_stat.err_count)));
                }

                confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                confirm.setDefaultButton(QMessageBox::No);
                confirm.setModal(true); // Prevents certain bugs from possibly occuring within the application
                int ret_confirm = confirm.exec();

                switch (ret_confirm) {
                case QMessageBox::Yes:
                    if (file_stat.files_vec.size() > 0) {
                        for (size_t i = 0; i < file_stat.files_vec.size(); ++i) {
                            dir_routine->changeFileExt(file_stat.files_vec.at(i), newExt);
                        }
                    }

                    QMessageBox::information(this, tr("ExtRenamer"), tr("Operation completed sucessfully!"), QMessageBox::Ok);
                case QMessageBox::No:
                    return;
                case QMessageBox::Cancel:
                    return;
                }
            }
            case QMessageBox::No:
                return;
            case QMessageBox::Cancel:
                return;
            }
        } else {
            throw std::invalid_argument(tr("The path does not exist any longer!").toStdString());
        }
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("Error!"), QString("%1").arg(e.what()), QMessageBox::Ok);
    }
}

void MainWindow::on_exit_pushButton_clicked()
{
    QCoreApplication::quit(); // Exit with status code '0'
    return;
}

void MainWindow::on_reset_pushButton_clicked()
{
    ui->dir_lineEdit->clear();
    ui->ext_lineEdit->clear();
    ui->new_ext_lineEdit->clear();
}

void MainWindow::on_dir_toolButton_clicked()
{
    QString dirDest = QFileDialog::getExistingDirectory(this, tr("Choose destination..."),
                                                        QDir::homePath(), QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);
    ui->dir_lineEdit->setText(dirDest);
}

void MainWindow::on_actionE_xit_triggered()
{
    QCoreApplication::quit(); // Exit with status code '0'
    return;
}

void MainWindow::on_action_Documentation_triggered()
{
    QMessageBox::information(this, tr("ExtRenamer"), tr("This function is not implemented yet!"), QMessageBox::Ok);
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::information(this, tr("ExtRenamer"), tr("This function is not implemented yet!"), QMessageBox::Ok);
}
