#include <fstream>
#include <memory>
#include <filesystem>
#include <iostream>
#include <tinyply.h>

using namespace std;

struct float3 {
    float x, y, z;
};

template<typename T>
vector<T> extract(tinyply::PlyFile& file, shared_ptr<tinyply::PlyData> data) {
    vector<T> vector(data->count);
    size_t size = data->buffer.size_bytes();
    std::memcpy(vector.data(), data->buffer.get(), size);
    return vector;
}

int main() {
    try {
        tinyply::PlyFile file;
        filesystem::path path("./clouds/0.ply");
        ifstream fileStream(path, ios::binary);
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
        return EXIT_SUCCESS;
    } catch (exception const& e) {
        cerr << "runtime exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}