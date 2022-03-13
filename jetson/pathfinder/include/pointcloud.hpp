#pragma once

#include <array>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <tinyply.h>
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;

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

static float colorAsFloat(vec3 color) {
    std::array<uint8_t, 4> byteColor{
            static_cast<uint8_t>(color.x * 255.0f),
            static_cast<uint8_t>(color.y * 255.0f),
            static_cast<uint8_t>(color.z * 255.0f),
    };
    return *reinterpret_cast<float*>(byteColor.data());
}

static std::array<float, 4> pointColors{
        colorAsFloat(vec3(1.0f, 0.0f, 0.0f)),
        colorAsFloat(vec3(0.0f, 1.0f, 0.0f)),
        colorAsFloat(vec3(0.0f, 0.0f, 1.0f)),
        colorAsFloat(vec3(1.0f, 1.0f, 0.0f)),
};

struct PointCloud {
    std::vector<glm::vec4> points;
    std::vector<uint8_t> labels;

    explicit PointCloud(std::filesystem::path const& path) {
        using namespace std;

        ifstream fileStream(path, std::ios::binary);
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

        labels = extract<uint8_t>(file, labelData);

        vector<vec3> positions = extract<vec3>(file, vertexData);
        points.reserve(positions.size());
        for (size_t i = 0; i < positions.size(); ++i) {
            float color = pointColors[labels[i] % pointColors.size()];
            vec3 point = positions[i] * 100.0f * glm::mat3{
                    vec3(1.0f, 0.0f, 0.0f),
                    vec3(0.0f, 0.0f, -1.0f),
                    vec3(0.0f, 1.0f, 0.0f),
            };
            points.emplace_back(point, color);
        }
    }
};