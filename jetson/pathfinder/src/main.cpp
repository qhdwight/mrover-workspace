#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <viewer.hpp>
#include <pointcloud.hpp>

using namespace std;

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

        vector<PointCloud> pointClouds;
        for (auto const& dirEntry: filesystem::recursive_directory_iterator(plyDir)) {
            filesystem::path path = dirEntry.path();
            if (path.extension() != ".ply") {
                continue;
            }

            cout << path.string() << endl;

            pointClouds.emplace_back(path);
        }

        Viewer viewer;
        viewer.initGraphics();
        while (viewer.open()) {
            viewer.updatePointCloud(pointClouds[0]);
//            viewer.draw();
        }

        return EXIT_SUCCESS;
    } catch (exception const& e) {
        cerr << "Runtime exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}