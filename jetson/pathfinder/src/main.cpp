#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>

#include <tinyply.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace std;

struct float3 {
    float x, y, z;
};

template<typename T>
vector<T> extract(tinyply::PlyFile& file, shared_ptr<tinyply::PlyData> data) {
    vector<T> vector(data->count);
    size_t size = data->buffer.size_bytes();
    memcpy(vector.data(), data->buffer.get(), size);
    return vector;
}

rapidjson::Document readConfig() {
    auto path = filesystem::current_path() / "config.json";
    ifstream ifs(path);
    if (!ifs.is_open())
        throw runtime_error("Could not open config at " + path.string());

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    return doc;
}

int main(int argc, char** argv) {
    try {
        vector<string> args(argv, argv + argc);
        rapidjson::Document config = readConfig();

        filesystem::path plyDir(config["ply_dir"].GetString());

        if (!filesystem::exists(plyDir))
            throw runtime_error("Could not find ply directory at " + plyDir.string());

        for (auto const& dirEntry: filesystem::recursive_directory_iterator(plyDir)) {
            filesystem::path path = dirEntry.path();
            if (path.extension() != ".ply") {
                continue;
            }

            cout << path.string() << endl;

            ifstream fileStream(path, ios::binary);
            if (!fileStream.is_open())
                throw runtime_error("Could not open ply file at " + path.string());

            tinyply::PlyFile file;
            file.parse_header(fileStream);

            shared_ptr<tinyply::PlyData> vertexData, labelData;
            try {
                vertexData = file.request_properties_from_element("vertex", {"x", "y", "z"});
                labelData = file.request_properties_from_element("vertex", {"label"});
            }
            catch (exception const& e) {
                cerr << "request from ply exception: " << e.what() << endl;
            }
            file.read(fileStream);

            vector<float3> vertices = extract<float3>(file, vertexData);
            vector<uint8_t> labels = extract<uint8_t>(file, labelData);
        }
        return EXIT_SUCCESS;
    } catch (exception const& e) {
        cerr << "runtime exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}