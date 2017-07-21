#include "dir.hpp"
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <exception>
#include <stdexcept>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>

namespace fs = boost::filesystem;
namespace sys = boost::system;
GekkoFyre::Dir::Dir(QObject *parent) : QObject(parent)
{}

GekkoFyre::Dir::~Dir()
{}

/**
 * @brief MainWindow::changeFileExt changes the file extension of a given file and its path.
 * @note  <http://stackoverflow.com/questions/19712429/c-boost-filesystem-how-to-modify-stem-from-a-path>
 *        <http://stackoverflow.com/questions/757933/how-do-you-change-the-filename-extension-stored-in-a-string-in-c>
 * @param file_path
 * @param newExt
 * @return
 */
bool GekkoFyre::Dir::changeFileExt(boost::filesystem::path file_path, const QString &newExt)
{
    if (fs::exists(file_path)) {
        // File exists!
        sys::error_code ec;
        fs::path parent_path = file_path.parent_path();
        fs::path stem = (fs::path(file_path.stem().string() + newExt.toStdString()).string());
        fs::path newPath = (parent_path / stem);
        fs::rename(file_path, newPath, ec); // http://www.boost.org/doc/libs/1_62_0/libs/filesystem/doc/reference.html#path-replace_extension

        if (ec.value() != sys::errc::success) {
            throw ec.category().name();
        }

        return true;
    } else {
        throw std::invalid_argument(tr("The path does not exist any longer!").toStdString());
    }
}

/**
 * @brief MainWindow::detectIfHasFileExt will detect if a given file has a certain, given extension or not.
 * @param file_path
 * @param file_ext
 * @return
 */
bool GekkoFyre::Dir::detectIfHasFileExt(const boost::filesystem::path &file_path, const std::string &file_ext)
{
    if (fs::exists(file_path)) {
        // File exists!
        return fs::extension(file_path) == file_ext;
    }

    throw std::invalid_argument(tr("The path does not exist any longer!").toStdString());
}
