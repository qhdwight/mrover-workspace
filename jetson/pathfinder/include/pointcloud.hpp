#pragma once

#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <tinyply.h>
#include <glm/vec3.hpp>

template<typename T>
std::vector<T> extract(tinyply::PlyFile& file, std::shared_ptr<tinyply::PlyData> data) {
    std::vector<T> vector(data->count);
    size_t size = data->buffer.size_bytes();
    std::memcpy(vector.data(), data->buffer.get(), size);
    return vector;
}

struct Point {
    glm::vec3 position;
    glm::vec3 color;
    uint8_t label;
};

struct PointCloud {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<uint8_t> labels;

    explicit PointCloud(std::filesystem::path const& path) {
        using namespace std;

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
            cerr << "Request from ply exception: " << e.what() << endl;
        }
        file.read(fileStream);

        positions = extract<glm::vec3>(file, vertexData);
        labels = extract<uint8_t>(file, labelData);
    }
};