#include <vector>
#include <mutex>
#include <utility>
#include <numeric>
#include <optional>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/rotate_vector.hpp>

#include <pointcloud.hpp>

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class PointCloudGraphics {
public:
    PointCloudGraphics() = default;

    ~PointCloudGraphics();

    void allocate();

    PointCloudGraphics(PointCloudGraphics&& other) noexcept;

    PointCloudGraphics& operator=(PointCloudGraphics other);

    void swap(PointCloudGraphics& other);

    void update(PointCloud& pc);

    void draw() const;

private:
    std::vector<vec4> pointsCPU;
    GLsizei size{};
    GLuint vaoId{};
    GLuint pointsGPU{};
};

class Object3D {
public:
    Object3D();

    Object3D(std::vector<vec3>& points, std::vector<vec3>& colors, std::vector<int>& indices);

    ~Object3D();

    Object3D(Object3D&& other) noexcept;

    Object3D& operator=(Object3D other);

    void swap(Object3D& other);

    // Change the underlying model
    void update(std::vector<vec3>& points, std::vector<vec3>& colors, std::vector<int>& indices);

    void draw() const;

    bool wireframe = true;

private:
    // GPU representation
    // https://gamedev.stackexchange.com/questions/8042/whats-the-purpose-of-opengls-vertex-array-objects

    GLsizei size{};
    GLuint vaoID{};
    GLuint pointsGPU{};
    GLuint colorsGPU{};
    GLuint indicesGPU{};
};

class Camera {
public:
    explicit Camera(glm::mat4 projection) : projection(projection) {
    }

    void setCenter(glm::vec3 inCenter) {
        center = inCenter;
    }

    void rotateY(float deltaY) {
        eulerOrientation.y += deltaY;
        eulerOrientation.y = glm::mod(eulerOrientation.y, glm::radians(360.0f));
    }

    void rotateX(float deltaX) {
        eulerOrientation.x += deltaX;
        float limit = glm::radians(80.0f);
        if (eulerOrientation.x < -limit) eulerOrientation.x = -limit;
        if (eulerOrientation.x > +limit) eulerOrientation.x = +limit;
    }

    void zoom(float scale) {
        distance += scale;
        distance = std::max(distance, 1.0f);
    }

    [[nodiscard]] glm::quat getRotation() const {
        return glm::angleAxis(eulerOrientation.x, glm::vec3{1.0f, 0.0f, 0.0f}) *
               glm::angleAxis(eulerOrientation.y, glm::vec3{0.0f, 1.0f, 0.0f});
    }

    void move(glm::vec3 displacement) {
        offset += displacement * getRotation();
    }

    [[nodiscard]] glm::mat4 getView() const {
        // rotate around y-axis then x-axis (order is reversed since it's essentially matrix multiplication)
        glm::quat rotation = getRotation();
        glm::vec3 target = center - offset;
        glm::vec3 eye = target + glm::vec3{0.0f, 0.0f, distance} * rotation;
        glm::vec3 up{0.0f, -1.0f, 0.0f}; // TODO: check logic on why inverted
        return glm::lookAt(eye, target, up);
    }

    glm::mat4 projection;
    float distance{5000.0f};
    glm::vec3 center{}, offset{};
    glm::vec3 eulerOrientation{};
};

class Shader {
public:

    Shader() = default;

    Shader(GLchar const* vs, GLchar const* fs);

    Shader(Shader&& other) noexcept;

    ~Shader();

    Shader& operator=(Shader other);

    void swap(Shader& other);

    [[nodiscard]] GLuint getProgramId() const;

    static const GLint ATTRIB_VERTICES_POS = 0;
    static const GLint ATTRIB_COLOR_POS = 1;
private:
    static bool compile(GLuint& shaderId, GLenum type, GLchar const* src);

    GLuint vertexShaderId{};
    GLuint fragmentShaderId{};
    GLuint programShaderId{};
};

/*
 *** Define which stage of the filtering process will be shown in the viewer
 */
enum class ProcStage {
    RAW
};

class Viewer {
public:
    int frame = 0;
    int maxFrame = -1;
    bool framePlay = false;
    bool inMenu = false;
    bool record = false;
    int currentFPS = -1;

    Viewer();

    ~Viewer();

    void initGraphics();

    void draw();

    void drawUI();

    bool open();

    void addObject(Object3D&& obj, bool ephemeral);

    void clearEphemerals();

    void setCenter();

    void setCenter(vec3 center);

    void updatePointCloud(PointCloud& pc);

    bool doParameterInit = true;
    ProcStage procStage = ProcStage::RAW;
    std::unordered_map<std::string, long long> stageTimings;
    float epsilon{};
    int iterations{};
    float threshold{};
    float removalRadius{};
    bool removeGround{};
    float tolerance{};
    float minSize{};
    float refineHeight{};
    float refineDistance{};

    glm::vec3 getCenter() {
        return pcCenter;
    }

private:
    // Internals
    Camera camera;
    std::vector<Object3D> objects;
    std::vector<Object3D> ephemeralObjects;
    Shader objectShader;
    Shader pcShader;
    PointCloudGraphics pcGraphics;
    GLFWwindow* window{};

    glm::vec3 pcCenter{};
    float mouseSensitivity = 1.0f;

    // Callbacks
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    // states
    double prevMouseX{}, prevMouseY{};
    bool prevFocused = false;
};
