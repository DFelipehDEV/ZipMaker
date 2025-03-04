#include <vector>
#include <string>

#include <zip.h>

#define GMREAL extern "C" __declspec(dllexport) double __cdecl 
#define GMSTR extern "C" __declspec(dllexport) char* __cdecl

struct ZipFile {
    std::vector<std::pair<std::string, std::string>> files;

    enum class SaveError {
        CREATE_FAILED = -1,
        OPEN_ENTRY_FAILED = -2,
        WRITE_TO_ENTRY_FAILED = -3
    };
};

static std::vector<ZipFile> ZIP_FILES;

GMREAL _zip_create() {
    ZIP_FILES.push_back(ZipFile());
    return static_cast<double>(ZIP_FILES.size() - 1);
}

GMREAL _zip_add_file(double zip_object, char* dest, char* src) {
    int index = static_cast<int>(zip_object);
    if (index < 0 || index >= ZIP_FILES.size()) return -1;

    ZIP_FILES[index].files.emplace_back(dest, src);
    return 1;
}

GMREAL _zip_save(double zip_object, char* path) {
    int index = static_cast<int>(zip_object);
    if (index < 0 || index >= ZIP_FILES.size()) return -1;

    zip_t* zip = zip_open(path, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w'); // Create ZIP
    if (!zip) return static_cast<double>(ZipFile::SaveError::CREATE_FAILED);

    for (const auto& file : ZIP_FILES[index].files) {
        const std::string& dest = file.first;
        const std::string& src = file.second;

        if (zip_entry_open(zip, dest.c_str()) != 0) {
            zip_close(zip);
            return static_cast<double>(ZipFile::SaveError::OPEN_ENTRY_FAILED);
        }

        if (zip_entry_fwrite(zip, src.c_str()) != 0) {
            zip_entry_close(zip);
            zip_close(zip);
            return static_cast<double>(ZipFile::SaveError::WRITE_TO_ENTRY_FAILED);
        }

        zip_entry_close(zip);
    }

    zip_close(zip);
    return 1.0; // Success
}

GMREAL _zip_unzip(char* file, char* target_directory) {
    return static_cast<double>(zip_extract(file, target_directory, NULL, NULL));
}
