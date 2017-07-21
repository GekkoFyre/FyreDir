#ifndef FDDIR_HPP
#define FDDIR_HPP

#include "config.hpp"
#include <boost/filesystem/path.hpp>
#include <QObject>
#include <QString>
#include <string>

namespace GekkoFyre {
class Dir : public QObject
{
    Q_OBJECT

public:
    explicit Dir(QObject *parent = 0);
    ~Dir();

    bool changeFileExt(boost::filesystem::path file_path, const QString &newExt);
    bool detectIfHasFileExt(const boost::filesystem::path &file_path, const std::string &file_ext);
};
}

#endif // FDDIR_HPP