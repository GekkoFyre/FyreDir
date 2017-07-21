#ifndef FDCONFIG_HPP
#define FDCONFIG_HPP

#include <boost/filesystem/path.hpp>
#include <vector>

namespace GekkoFyre {
#define FYREDIR_DEFAULT_RESOLUTION_WIDTH 1920L
#define FYREDIR_DEFAULT_RATIO_TO_COMPARE 1.1

    namespace File {
        struct Stats {
            unsigned long file_count;                       // The number of files found
            unsigned long dir_count;                        // The number of directories found
            unsigned long err_count;                        // The number of errors that occurred
            std::vector<boost::filesystem::path> files_vec; // Vector containing strictly file paths, with the desired extension
        };
    }
}

#endif // FDCONFIG_HPP