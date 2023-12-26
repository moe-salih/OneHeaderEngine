

#ifndef OGE_HH
#define OGE_HH

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <type_traits>
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <format>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace oge {

    namespace utils {
        template<typename T> concept arithmetic = std::is_arithmetic_v<T>;

        template<arithmetic T, size_t N> struct vec { T x[N]; };

        template<arithmetic T> struct vec<T,2> { T x, y; }; 
        template<arithmetic T> struct vec<T,3> { T x, y, z; };
        template<arithmetic T> struct vec<T,4> { T x, y, z, w; };

        template<arithmetic T> using vec2 = vec<T,2>;
        template<arithmetic T> using vec3 = vec<T,3>;
        template<arithmetic T> using vec4 = vec<T,4>; 

        using vec2i = vec2<int>;
        using vec2u = vec2<unsigned int>;
        using vec2f = vec2<float>;
        using vec2d = vec2<double>;

        using vec3i = vec3<int>;
        using vec3u = vec3<unsigned int>;
        using vec3f = vec3<float>;
        using vec3d = vec3<double>;

        using vec4i = vec4<int>;
        using vec4u = vec4<unsigned int>;
        using vec4f = vec4<float>;
        using vec4d = vec4<double>;

        struct log {
            public:
                static void init();
                inline static std::shared_ptr<spdlog::logger>& get_core_logger() { return _core_logger; }
            private:
                static std::shared_ptr<spdlog::logger> _core_logger;
        };

        #define LOG_TRACE(...) ::oge::utils::log::get_core_logger()->trace(__VA_ARGS__)
        #define LOG_INFO(...) ::oge::utils::log::get_core_logger()->info(__VA_ARGS__)
        #define LOG_WARN(...) ::oge::utils::log::get_core_logger()->warn(__VA_ARGS__)
        #define LOG_ERROR(...) ::oge::utils::log::get_core_logger()->error(__VA_ARGS__)
        #define LOG_CRITICAL(...) ::oge::utils::log::get_core_logger()->critical(__VA_ARGS__)

        #ifndef OGE_ASSERT
        #define OGE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
        #endif

        struct ogldbg {
            public:
                enum level {
                    none = 0, highassert, high, medium , low, notification
                };

                static void init();
                static void set_level(const level& );
            private:
                static void message(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);
                static level _level;
        };
        // shader
        struct shader {
            private:
                unsigned int _id;
                std::unordered_map<std::string, int> _uniform_location_cache;

            public:
                // shader() {}
                shader(const char* vertex_path, const char* fragment_path);
                ~shader() { glDeleteProgram(_id); }

                void bind() const;
                void unbind() const;

                // void load(const char* vertex_path, const char* fragment_path);
                
                template<typename T> void set_uniform(const char* name, const T& value);

                void set_uniform(const char* name, const int& value);
                void set_uniform(const char* name, const unsigned int& value);
                void set_uniform(const char* name, const float& value);
                void set_uniform(const char* name, const double& value);

                void set_uniform(const char* name, const vec2i value);
                void set_uniform(const char* name, const vec2u& value);
                void set_uniform(const char* name, const vec2f& value);
                void set_uniform(const char* name, const vec2d& value);

                void set_uniform(const char* name, const vec3i& value);
                void set_uniform(const char* name, const vec3u& value);
                void set_uniform(const char* name, const vec3f& value);
                void set_uniform(const char* name, const vec3d& value);

                void set_uniform(const char* name, const vec4i& value);
                void set_uniform(const char* name, const vec4u& value);
                void set_uniform(const char* name, const vec4f& value);
                void set_uniform(const char* name, const vec4d& value);

                void set_uniform(const char* name, const glm::vec2& value);
                void set_uniform(const char* name, const glm::vec3& value);
                void set_uniform(const char* name, const glm::vec4& value);

                void set_uniform(const char* name, const glm::mat2& value);
                void set_uniform(const char* name, const glm::mat3& value);
                void set_uniform(const char* name, const glm::mat4& value);

            private:
                int get_uniform_location(const char* name);
                // 
                unsigned int compile_shader(unsigned int type, const char* source);
                unsigned int compile_program(const char* vertex_source, const char* fragment_source);

        };


        // camera

        class ortho_camera {
            public:

                ortho_camera( const glm::vec4& bounds = { -1.0f, 1.0f, -1.0f, 1.0f } );
                
                void set_projection(const glm::vec4& bounds);

                const glm::vec3& position() const { return _position; }
                void set_position(const glm::vec3& position) { _position = position; update_view(); }

                float rotation() const { return _rotation; }
                void set_rotation(float rotation) { _rotation = rotation; update_view(); }

                const glm::mat4& projection() const { return _projection; }
                const glm::mat4& view() const { return _view; }
                const glm::mat4& view_projection() const { return _view_projection; }

            private:
                void update_view();

            private:    
                glm::mat4 _projection, _view, _view_projection;

                glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
                float _rotation = 0.0f;

        };

        struct presepctive_camera {
            public:
                presepctive_camera(float fov, float aspect_ratio, float near_clip, float far_clip);

                void set_projection(float fov, float aspect_ratio, float near_clip, float far_clip);

                const glm::vec3& position() const { return _position; }
                void set_position(const glm::vec3& position) { _position = position; update_view(); }

                float rotation() const { return _rotation; }
                void set_rotation(float rotation) { _rotation = rotation; update_view(); }

                const glm::mat4& projection() const { return _projection; }
                const glm::mat4& view() const { return _view; }
                const glm::mat4& view_projection() const { return _view_projection; }

            private:
                void update_view();

            private:
                glm::mat4 _projection, _view, _view_projection;

                glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
                float _rotation = 0.0f;
        };

    }

    namespace events {

        enum class type {
            none = 0,
            window_close, window_resize, window_focus, window_move,
            key_press, key_release, key_type,
            mouse_button_press, mouse_button_release, mouse_move, mouse_scroll
        };

        enum category {
            none = 0,
            application = 1 << 0,
            input = 1 << 1,
            keyboard = 1 << 2,
            mouse = 1 << 3,
            mouse_button = 1 << 4,
        };

        #define ECT(etype) \
            static type get_static_type() { return type::##etype; } \
            virtual type get_type() const override { return get_static_type(); } \
            virtual const char* get_name() const override { return #etype; }

        #define ECC(ecat) \
            virtual int get_category_flags() const override { return ecat; } 

        
        struct event {
            public:
                bool handled = false;

                virtual type get_type() const = 0;
                virtual const char* get_name() const = 0;
                virtual int get_category_flags() const = 0;
                virtual std::string to_string() const { return get_name(); }

                inline bool is_in_category(category cat) const { return get_category_flags() & cat; }
        
                inline friend std::ostream& operator<<(std::ostream& os, const event& e) {
                    return os << e.to_string();
                }
        };

        struct event_dispatcher {
            public:
                event_dispatcher(event& event) : _event(event) {}

                template<typename T, typename F>
                bool dispatch(const F& func) {
                    if (_event.get_type() == T::get_static_type()) {
                        _event.handled |= func(static_cast<T&>(_event));
                        return true;
                    }
                    return false;
                }

            private:
                event& _event;
        };


        struct key_event : public event {
            public:
                inline int code() const { return _code; }
                ECC(keyboard | input)
            protected:
                key_event(int key_code) : _code(key_code) {}
                int _code;
        };

        struct key_press_event : public key_event {
            public:
                key_press_event(int key_code, int repeat_count) : key_event(key_code), _repeat_count(repeat_count) {}
                inline int repeat_count() const { return _repeat_count; }

                std::string to_string() const override {
                    return std::format("key_press_event: {} ({} repeats)", _code, _repeat_count);
                }

                ECT(key_press)
            private:
                int _repeat_count;
        };

        struct key_release_event : public key_event {
            public:
                key_release_event(int key_code) : key_event(key_code) {}

                std::string to_string() const override {
                    return std::format("key_release_event: {}", _code);
                }

                ECT(key_release)
        };

        struct key_type_event : public key_event {
            public:
                key_type_event(int key_code) : key_event(key_code) {}

                std::string to_string() const override {
                    return std::format("key_type_event: {}", _code);
                }

                ECT(key_type)
        };

        struct mouse_button_event : public event {
            public:
                inline int button() const { return _button; }
                ECC(mouse_button | input)
            protected:
                mouse_button_event(int button) : _button(button) {}
                int _button;
        };

        struct mouse_button_press_event : public mouse_button_event {
            public:
                mouse_button_press_event(int button) : mouse_button_event(button) {}

                std::string to_string() const override {
                    return std::format("mouse_button_press_event: {}", _button);
                }

                ECT(mouse_button_press)
        };

        struct mouse_button_release_event : public mouse_button_event {
            public:
                mouse_button_release_event(int button) : mouse_button_event(button) {}

                std::string to_string() const override {
                    return std::format("mouse_button_release_event: {}", _button);
                }

                ECT(mouse_button_release)
        };

        struct mouse_move_event : public event {
            public:
                mouse_move_event(const utils::vec2f& pos) : _pos(pos) {}

                inline float x() const { return _pos.x; }
                inline float y() const { return _pos.y; }

                std::string to_string() const override {
                    return std::format("mouse_move_event: {}, {}", _pos.x, _pos.y);
                }

                ECT(mouse_move)
                ECC(mouse | input)
            private:
                utils::vec2f _pos;
        };

        struct mouse_scroll_event : public event {
            public:
                mouse_scroll_event(const utils::vec2f& offset) : _offset(offset) {}

                inline float x() const { return _offset.x; }
                inline float y() const { return _offset.y; }

                std::string to_string() const override {
                    return std::format("mouse_scroll_event: {}, {}", _offset.x, _offset.y);
                }

                ECT(mouse_scroll)
                ECC(mouse | input)
            private:
                utils::vec2f _offset;
        };

        struct window_resize_event : public event {
            public:
                window_resize_event(const utils::vec2u& size) : _size(size) {}

                inline unsigned int width() const { return _size.x; }
                inline unsigned int height() const { return _size.y; }

                std::string to_string() const override {
                    return std::format("window_resize_event: {}, {}", _size.x, _size.y);
                }

                ECT(window_resize)
                ECC(application)
            private:
                utils::vec2u _size;
        };

        struct window_close_event : public event {
            public:
                window_close_event() {}

                std::string to_string() const override {
                    return std::format("window_close_event");
                }

                ECT(window_close)
                ECC(application)
        };

        struct window_focus_event : public event {
            public:
                window_focus_event(bool focused) : _focused(focused) {}

                inline bool focused() const { return _focused; }

                std::string to_string() const override {
                    return std::format("window_focus_event: {}", _focused);
                }

                ECT(window_focus)
                ECC(application)
            private:
                bool _focused;
        };

        struct window_move_event : public event {
            public:
                window_move_event(const utils::vec2i& pos) : _pos(pos) {}

                inline int x() const { return _pos.x; }
                inline int y() const { return _pos.y; }

                std::string to_string() const override {
                    return std::format("window_move_event: {}, {}", _pos.x, _pos.y);
                }

                ECT(window_move)
                ECC(application)
            private:
                utils::vec2i _pos;
        };


    }


    namespace core {

        struct glfw {
            public:
                static void init();
                static void terminate();
            private:
                static bool initialized;
        };

        struct ogl {
            public:
                static void init();
                static void terminate();
            private:
                static bool initialized;
        };


        class input {
            protected:
                input() = default;
            public:
                input(const input&) = delete;
                input& operator=(const input&) = delete;

                static bool is_key_pressed(int key_code);
                static bool is_mouse_button_pressed(int button);
                static utils::vec2f mouse_position();
        };

        struct callbacks {
            static void error_callback(int error, const char* description);

            static void window_size_callback(GLFWwindow* window, int width, int height);
            static void window_close_callback(GLFWwindow* window);
            // static void window_refresh_callback(GLFWwindow* window);
            static void window_focus_callback(GLFWwindow* window, int focused);
            // static void window_iconify_callback(GLFWwindow* window, int iconified);
            // static void window_maximize_callback(GLFWwindow* window, int maximized);
            // static void window_content_scale_callback(GLFWwindow* window, float xscale, float yscale);
            static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);

            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void key_type_callback(GLFWwindow* window, unsigned int codepoint);

            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
            static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
            
        };

        using window_event_callback_fn = std::function<void(events::event&)>;

        struct window_state {

            const char* title = "OGE Window";

            utils::vec2i position;
            utils::vec2u size = { 1024, 512 };
            utils::vec2u framebuffer_size;
            utils::vec2f cursor_position;
            utils::vec2f scroll_offset;

            bool vsync = true;
            bool fullscreen = false;

            window_event_callback_fn callback;

            GLFWwindow* window;
            GLFWmonitor* monitor;

            window_state(
                const char* title = "OGE Window",
                const utils::vec2u& size = { 1024, 512 }
            ) : title(title), size(size) {}

        };

        struct window {
            public:
                window( const window_state& state = window_state());
                ~window();

                void on_update();

                inline const utils::vec2u& size() const { return state.size; }

                void set_event_callback(const window_event_callback_fn& callback);
                void set_vsync(bool enabled);
                void set_fullscreen(bool enabled);
                void set_icon(const char* path);

                bool is_vsync() const;
                bool is_fullscreen() const;

                inline bool is_open() const { return !glfwWindowShouldClose(state.window); }

                inline void* native_window() const { return state.window; }

            private:
                void init();
                void shutdown();

            private:
                window_state state;
        };

        struct layer {
            public:
                layer(const char* name = "Layer") : _name(name) {}

                virtual void on_attach() {}
                virtual void on_detach() {}
                virtual void on_update( const float&) {}
                virtual void on_event(events::event&) {}

                virtual void pre_update() {}
                virtual void post_update() {}
                
                inline const char* name() const { return _name; }

            protected:
                const char* _name;
        };

        struct imgui_layer : public layer {
            public:
                imgui_layer();
                ~imgui_layer() = default;

                virtual void on_attach() override;
                virtual void on_detach() override;
                virtual void on_update( const float&) override {}
                virtual void on_event(events::event&) override;

                void pre_update() override;
                void post_update() override;

                bool on_mouse_button_press(events::mouse_button_event& e);

        };

        struct layer_stack {
            public:
                layer_stack();
                ~layer_stack();

                void push_layer(layer* layer);
                void pop_layer(layer* layer);
                void push_overlay(layer* overlay);
                void pop_overlay(layer* overlay);

                std::vector<layer*>::iterator begin() { return _layers.begin(); }
                std::vector<layer*>::iterator end() { return _layers.end(); }
            private:
                std::vector<layer*> _layers;
                size_t _layer_insert_index = 0;
        };

        struct application {
            public:
                application( const char* title = "OGE Application", const utils::vec2u& size = { 1024, 512 } );
                virtual ~application() = default;

                void run();

                void on_event(events::event& event);

                void push_layer(layer* layer);
                void push_overlay(layer* overlay);

                inline window& get_window() { return *_window; }
                inline static application& get() { return *_instance; }

            private:
                bool on_window_close(events::window_close_event& event);

            private:
                static application* _instance;
                std::unique_ptr<window> _window;

                bool _running = true;
                float _lastframe_time = 0.0f;

                layer_stack _layer_stack;
        };


    }

    namespace utils {

        struct ortho_camera_controller {
            public:
                ortho_camera_controller(float aspect_ratio, bool rotation = false);

                void on_update(const float& dt);
                void on_event(events::event& e);

                ortho_camera& camera() { return _camera; }
                const ortho_camera& camera() const { return _camera; }

                float zoom_level() const { return _zoom_level; }
                void set_zoom_level(float level) { _zoom_level = level; }

            private:
                bool on_mouse_scroll(events::mouse_scroll_event& e);
                bool on_window_resize(events::window_resize_event& e);

            private:
                float _aspect_ratio;
                float _zoom_level = 1.0f;

                ortho_camera _camera;

                bool _rotation;

                glm::vec3 _camera_position = { 0.0f, 0.0f, 0.0f };

                float _camera_rotation = 0.0f;
                float _camera_translation_speed = 5.0f, _camera_rotation_speed = 180.0f;
        };

        struct presepctive_camera_controller {
            public:
                presepctive_camera_controller(float fov, float aspect_ratio, float near_clip, float far_clip);

                void on_update(const float& dt);
                void on_event(events::event& e);

                presepctive_camera& camera() { return _camera; }
                const presepctive_camera& camera() const { return _camera; }

                float zoom_level() const { return _zoom_level; }
                void set_zoom_level(float level) { _zoom_level = level; }

            private:
                bool on_mouse_scroll(events::mouse_scroll_event& e);
                bool on_window_resize(events::window_resize_event& e);

            private:
                float _aspect_ratio;
                float _zoom_level = 1.0f;

                presepctive_camera _camera;

                glm::vec3 _camera_position = { 0.0f, 0.0f, 0.0f };

                float _camera_rotation = 0.0f;
                float _camera_translation_speed = 5.0f, _camera_rotation_speed = 180.0f;
        };
        
    }

}

#endif//OGE_HH
// #define OGE_IMPL
#ifdef OGE_IMPL

#include <fstream>
#include <sstream>

namespace oge {

    namespace utils {
            
        std::shared_ptr<spdlog::logger> log::_core_logger;
    
        void log::init() {
            spdlog::set_pattern("%^[%T] %n: %v%$");
            _core_logger = spdlog::stdout_color_mt("OGE");
            _core_logger->set_level(spdlog::level::trace);
        }  
   
        // ogldbg

        ogldbg::level ogldbg::_level = ogldbg::level::highassert;

        void ogldbg::init() {
            glDebugMessageCallback(message, nullptr);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }

        void ogldbg::set_level( const ogldbg::level& lvl) {
            _level = lvl;
        }

        void ogldbg::message(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
            switch(severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                    if (_level > level::high) {
                        LOG_ERROR("OpenGL Error: {}", message);
                        if(_level == level::highassert) {
                            OGE_ASSERT(false, "OpenGL Error: {}", message);
                        }
                    }
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    if (_level > level::medium) {
                        LOG_WARN("OpenGL Error: {}", message);
                    }
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    if (_level > level::low) {
                        LOG_INFO("OpenGL Error: {}", message);
                    }
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    if (_level > level::notification) {
                        LOG_TRACE("OpenGL Error: {}", message);
                    }
                    break;
            }
        }
   
        // shader

        std::string read_file(const char* path) {
            std::ifstream file(path, std::ios::in | std::ios::binary);
            if (!file) {
                LOG_ERROR("Failed to open file: {}", path);
                return "";
            }
            std::stringstream ss;
            ss << file.rdbuf();
            file.close();
            return ss.str();
        }

        shader::shader(const char* vertex_path, const char* fragment_path) {

            const char* vertex_source = read_file(vertex_path).c_str();
            const char* fragment_source = read_file(fragment_path).c_str();

            _id = compile_program(vertex_source, fragment_source);

        }

        void shader::bind() const { glUseProgram(_id); }
        void shader::unbind() const { glUseProgram(0); }

        void shader::set_uniform(const char* name, const int& value) {
            glUniform1i(get_uniform_location(name), value);
        }

        void shader::set_uniform(const char* name, const unsigned int& value) {
            glUniform1ui(get_uniform_location(name), value);
        }

        void shader::set_uniform(const char* name, const float& value) {
            glUniform1f(get_uniform_location(name), value);
        }

        void shader::set_uniform(const char* name, const double& value) {
            glUniform1d(get_uniform_location(name), value);
        }

        void shader::set_uniform(const char* name, const vec2i value) {
            glUniform2i(get_uniform_location(name), value.x, value.y);
        }

        void shader::set_uniform(const char* name, const vec2u& value) {
            glUniform2ui(get_uniform_location(name), value.x, value.y);
        }

        void shader::set_uniform(const char* name, const vec2f& value) {
            glUniform2f(get_uniform_location(name), value.x, value.y);
        }

        void shader::set_uniform(const char* name, const vec2d& value) {
            glUniform2d(get_uniform_location(name), value.x, value.y);
        }

        void shader::set_uniform(const char* name, const vec3i& value) {
            glUniform3i(get_uniform_location(name), value.x, value.y, value.z);
        }

        void shader::set_uniform(const char* name, const vec3u& value) {
            glUniform3ui(get_uniform_location(name), value.x, value.y, value.z);
        }

        void shader::set_uniform(const char* name, const vec3f& value) {
            glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
        }

        void shader::set_uniform(const char* name, const vec3d& value) {
            glUniform3d(get_uniform_location(name), value.x, value.y, value.z);
        }

        void shader::set_uniform(const char* name, const vec4i& value) {
            glUniform4i(get_uniform_location(name), value.x, value.y, value.z, value.w);
        }

        void shader::set_uniform(const char* name, const vec4u& value) {
            glUniform4ui(get_uniform_location(name), value.x, value.y, value.z, value.w);
        }

        void shader::set_uniform(const char* name, const vec4f& value) {
            glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
        }

        void shader::set_uniform(const char* name, const vec4d& value) {
            glUniform4d(get_uniform_location(name), value.x, value.y, value.z, value.w);
        }

        void shader::set_uniform(const char* name, const glm::vec2& value) {
            glUniform2f(get_uniform_location(name), value.x, value.y);
        }

        void shader::set_uniform(const char* name, const glm::vec3& value) {
            glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
        }

        void shader::set_uniform(const char* name, const glm::vec4& value) {
            glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
        }

        void shader::set_uniform(const char* name, const glm::mat2& value) {
            glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
        }

        void shader::set_uniform(const char* name, const glm::mat3& value) {
            glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
        }

        void shader::set_uniform(const char* name, const glm::mat4& value) {
            glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
        }

        int shader::get_uniform_location(const char* name) {
            if (_uniform_location_cache.find(name) != _uniform_location_cache.end()) {
                return _uniform_location_cache[name];
            }
            int location = glGetUniformLocation(_id, name);
            if (location == -1) {
                LOG_WARN("Uniform '{}' doesn't exist!", name);
            }
            _uniform_location_cache[name] = location;
            return location;
        }

        unsigned int shader::compile_shader(unsigned int type, const char* source) {
            unsigned int id = glCreateShader(type);
            glShaderSource(id, 1, &source, nullptr);
            glCompileShader(id);

            int result;
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE) {
                int length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetShaderInfoLog(id, length, &length, message);
                LOG_ERROR("Failed to compile {} shader:\n{}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
                glDeleteShader(id);
                return 0;
            }
            return id;
        }

        unsigned int shader::compile_program(const char* vertex_source, const char* fragment_source) {
            unsigned int program = glCreateProgram();
            unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_source);
            unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_source);

            glAttachShader(program, vs);
            glAttachShader(program, fs);
            glLinkProgram(program);
            glValidateProgram(program);

            int result;
            glGetProgramiv(program, GL_LINK_STATUS, &result);
            if (result == GL_FALSE) {
                int length;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetProgramInfoLog(program, length, &length, message);
                LOG_ERROR("Failed to link shader program:\n{}", message);

                glDeleteProgram(program);
                glDeleteShader(vs);
                glDeleteShader(fs);
                
                return 0;
            }

            glDeleteShader(vs);
            glDeleteShader(fs);

            return program;
        }


        // camera code

        ortho_camera::ortho_camera( const glm::vec4& bounds ) 
            : _projection(glm::ortho(bounds.x, bounds.y, bounds.z, bounds.w, -1.0f, 1.0f)),
              _view(1.0f)
        {
            _view_projection = _projection * _view;
        }

        void ortho_camera::set_projection(const glm::vec4& bounds) {
            _projection = glm::ortho(bounds.x, bounds.y, bounds.z, bounds.w, -1.0f, 1.0f);
            update_view();
        }

        void ortho_camera::update_view() {
            glm::mat4 transform = glm::translate(
                glm::mat4(1.0f),
                _position
            ) * glm::rotate(
                glm::mat4(1.0f),
                glm::radians(_rotation),
                glm::vec3(0, 0, 1)
            );
            _view = glm::inverse(transform);
            _view_projection = _projection * _view;
        }

        presepctive_camera::presepctive_camera(float fov, float aspect_ratio, float near_clip, float far_clip) {
            set_projection(fov, aspect_ratio, near_clip, far_clip);
        }

        void presepctive_camera::set_projection(float fov, float aspect_ratio, float near_clip, float far_clip) {
            _projection = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
            update_view();
        }

        void presepctive_camera::update_view() {
            glm::mat4 transform = glm::translate(
                glm::mat4(1.0f),
                _position
            ) * glm::rotate(
                glm::mat4(1.0f),
                glm::radians(_rotation),
                glm::vec3(0, 0, 1)
            );
            _view = glm::inverse(transform);
            _view_projection = _projection * _view;
        }


    }

    namespace core {

        bool glfw::initialized = false;
        
        void glfw::init() {
            if (!initialized) {
                OGE_ASSERT(glfwInit(), "Failed to initialize GLFW");
                initialized = true;
            }
        }

        void glfw::terminate() {
            if (initialized) {
                glfwTerminate();
                initialized = false;
            }
        }

        bool ogl::initialized = false;

        void ogl::init() {
            if (!initialized) {
                OGE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");
                initialized = true;
            }
        }

        void ogl::terminate() {
            if (initialized) {
                initialized = false;
            }
        }

        bool input::is_key_pressed(int key_code) {
            auto window = static_cast<GLFWwindow*>(application::get().get_window().native_window());
            auto state = glfwGetKey(window, key_code);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool input::is_mouse_button_pressed(int button) {
            auto window = static_cast<GLFWwindow*>(application::get().get_window().native_window());
            auto state = glfwGetMouseButton(window, button);
            return state == GLFW_PRESS;
        }

        utils::vec2f input::mouse_position() {
            auto window = static_cast<GLFWwindow*>(application::get().get_window().native_window());
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            return { (float)xpos, (float)ypos };
        }

        void callbacks::error_callback(int error, const char* description) {
            LOG_ERROR("GLFW Error ({0}): {1}", error, description);
        }

        void callbacks::window_size_callback(GLFWwindow* window, int width, int height) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            
            state->size = { (unsigned int)width, (unsigned int)height };
            state->framebuffer_size = { (unsigned int)width, (unsigned int)height };

            glViewport(0, 0, width, height);

            events::window_resize_event evt(state->size);
            state->callback(evt);
        }

        void callbacks::window_close_callback(GLFWwindow* window) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            events::window_close_event evt;
            state->callback(evt);
        }

        void callbacks::window_focus_callback(GLFWwindow* window, int focused) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            events::window_focus_event evt(focused);
            state->callback(evt);
        }

        void callbacks::window_pos_callback(GLFWwindow* window, int xpos, int ypos) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            state->position = { xpos, ypos };
            events::window_move_event evt(state->position);
            state->callback(evt);
        }

        void callbacks::key_callback(GLFWwindow* window, int key, int, int action, int) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    events::key_press_event evt(key, 0);
                    state->callback(evt);
                    break;
                }
                case GLFW_RELEASE: {
                    events::key_release_event evt(key);
                    state->callback(evt);
                    break;
                }
                case GLFW_REPEAT: {
                    events::key_press_event evt(key, 1);
                    state->callback(evt);
                    break;
                }
            }
        }

        void callbacks::key_type_callback(GLFWwindow* window, unsigned int codepoint) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            events::key_type_event evt(codepoint);
            state->callback(evt);
        }

        void callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    events::mouse_button_press_event evt(button);
                    state->callback(evt);
                    break;
                }
                case GLFW_RELEASE: {
                    events::mouse_button_release_event evt(button);
                    state->callback(evt);
                    break;
                }
            }
        }

        void callbacks::mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            state->cursor_position = { (float)xpos, (float)ypos };
            events::mouse_move_event evt(state->cursor_position);
            state->callback(evt);
        }

        void callbacks::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
            window_state* state = reinterpret_cast<window_state*>(glfwGetWindowUserPointer(window));
            state->scroll_offset = { (float)xoffset, (float)yoffset };
            events::mouse_scroll_event evt(state->scroll_offset);
            state->callback(evt);
        }


        window::window( const window_state& state ) : state(state) {
            init();
        }

        window::~window() {
            shutdown();
        }

        void window::on_update() {
            glfwPollEvents();
            glfwSwapBuffers(state.window);
        }

        void window::set_event_callback(const window_event_callback_fn& callback) {
            state.callback = callback;
        }

        void window::set_vsync(bool enabled) {
            glfwSwapInterval(state.vsync = enabled ? 1 : 0);
        }

        void window::set_fullscreen(bool) {
            // TODO: Implement
        }

        void window::set_icon(const char*) {
            // TODO: Implement
        }

        bool window::is_vsync() const {
            return state.vsync;
        }

        bool window::is_fullscreen() const {
            return state.fullscreen;
        }

        void window::init() {
            glfw::init();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            state.window = glfwCreateWindow(state.size.x, state.size.y, state.title, nullptr, nullptr);

            glfwMakeContextCurrent(state.window);
            glfwSetWindowUserPointer(state.window, &state);

            glfwSetWindowSizeCallback(state.window, callbacks::window_size_callback);
            glfwSetWindowCloseCallback(state.window, callbacks::window_close_callback);
            // glfwSetWindowRefreshCallback(state.window, callbacks::window_refresh_callback);
            glfwSetWindowFocusCallback(state.window, callbacks::window_focus_callback);
            // glfwSetWindowIconifyCallback(state.window, callbacks::window_iconify_callback);
            // glfwSetWindowMaximizeCallback(state.window, callbacks::window_maximize_callback);
            // glfwSetWindowContentScaleCallback(state.window, callbacks::window_content_scale_callback);
            glfwSetWindowPosCallback(state.window, callbacks::window_pos_callback);
            glfwSetKeyCallback(state.window, callbacks::key_callback);
            glfwSetCharCallback(state.window, callbacks::key_type_callback);
            glfwSetMouseButtonCallback(state.window, callbacks::mouse_button_callback);
            glfwSetCursorPosCallback(state.window, callbacks::mouse_position_callback);
            glfwSetScrollCallback(state.window, callbacks::mouse_scroll_callback);

            glfwSwapInterval(state.vsync);

            ogl::init();
        }

        void window::shutdown() {
            glfwDestroyWindow(state.window);
            glfw::terminate();
        }

        imgui_layer::imgui_layer() : layer("ImGui Layer") {}

        void imgui_layer::on_attach() {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();

            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

            ImGui::StyleColorsDark();

            ImGuiStyle& style = ImGui::GetStyle();
            if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                style.WindowRounding = 1.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            application& app = application::get();
            GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().native_window());

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 460");
            LOG_INFO("ImGuiLayer::Attached");
        }

        void imgui_layer::on_detach() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            LOG_INFO("ImGuiLayer::Detached");
        }

        void imgui_layer::pre_update() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void imgui_layer::post_update() {
            application& app = application::get();
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)app.get_window().size().x, (float)app.get_window().size().y);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }

        void imgui_layer::on_event(events::event& e) {
            events::event_dispatcher dispatcher(e);
            dispatcher.dispatch<events::mouse_button_press_event>(std::bind(&imgui_layer::on_mouse_button_press, this, std::placeholders::_1));
        }

        bool imgui_layer::on_mouse_button_press(events::mouse_button_event& e) {
            ImGuiIO& io = ImGui::GetIO();
            return io.WantCaptureMouse;
        }


        layer_stack::layer_stack() {}

        layer_stack::~layer_stack() {
            for (layer* layer : _layers) {
                layer->on_detach();
                delete layer;
            }
        }

        void layer_stack::push_layer(layer* lyr) {
            _layers.emplace(_layers.begin() + _layer_insert_index, lyr);
            _layer_insert_index++;
            lyr->on_attach();
        }

        void layer_stack::push_overlay(layer* lyr) {
            _layers.emplace_back(lyr);
            lyr->on_attach();
        }

        void layer_stack::pop_layer(layer* lyr) {
            auto it = std::find(_layers.begin(), _layers.begin() + _layer_insert_index, lyr);
            if (it != _layers.begin() + _layer_insert_index) {
                lyr->on_detach();
                _layers.erase(it);
                _layer_insert_index--;
            }
        }

        void layer_stack::pop_overlay(layer* lyr) {
            auto it = std::find(_layers.begin() + _layer_insert_index, _layers.end(), lyr);
            if (it != _layers.end()) {
                lyr->on_detach();
                _layers.erase(it);
            }
        }

        application* application::_instance = nullptr;

        application::application( const char* title, const utils::vec2u& size ) {
            
            if(!_instance) {
                utils::log::init();
            }
            
            OGE_ASSERT(!_instance, "Application already exists!");
            _instance = this;
            
            _window = std::make_unique<window>( window_state(title, size) );
            _window->set_event_callback(std::bind(&application::on_event, this, std::placeholders::_1));
        }


        void application::run() {
            float time, delta_time;
            while (_running) {
                time = (float)glfwGetTime();
                delta_time = time - _lastframe_time;
                _lastframe_time = time;

                for (layer* layer : _layer_stack) {
                    layer->pre_update();
                    layer->on_update(delta_time);
                    layer->post_update();
                }

                _window->on_update();
            }
        }

        void application::push_layer(layer* layer) {
            _layer_stack.push_layer(layer);
        }

        void application::push_overlay(layer* overlay) {
            _layer_stack.push_overlay(overlay);
        }

        bool application::on_window_close(events::window_close_event&) {
            _running = false;
            return true;
        }

        void application::on_event(events::event& event) {
            events::event_dispatcher dispatcher(event);
            dispatcher.dispatch<events::window_close_event>(std::bind(&application::on_window_close, this, std::placeholders::_1));

            for (auto it = _layer_stack.end(); it != _layer_stack.begin(); ) {
                (*--it)->on_event(event);
                if (event.handled) {
                    break;
                }
            }
        }

    }

    namespace utils {
        ortho_camera_controller::ortho_camera_controller(float aspect_ratio, bool rotation) 
            : _aspect_ratio(aspect_ratio),
              _rotation(rotation),
              _camera({-_aspect_ratio * _zoom_level, _aspect_ratio * _zoom_level, -_zoom_level, _zoom_level})
        {}

        void ortho_camera_controller::on_update(const float& dt) {
            if (core::input::is_key_pressed(GLFW_KEY_A)) {
                _camera_position.x -= cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y -= sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            } else if (core::input::is_key_pressed(GLFW_KEY_D)) {
                _camera_position.x += cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y += sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            }

            if (core::input::is_key_pressed(GLFW_KEY_W)) {
                _camera_position.x += -sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y += cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            } else if (core::input::is_key_pressed(GLFW_KEY_S)) {
                _camera_position.x -= -sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y -= cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            }

            if (_rotation) {
                if (core::input::is_key_pressed(GLFW_KEY_Q)) {
                    _camera_rotation += _camera_rotation_speed * dt;
                } else if (core::input::is_key_pressed(GLFW_KEY_E)) {
                    _camera_rotation -= _camera_rotation_speed * dt;
                }

                if (_camera_rotation > 180.0f) {
                    _camera_rotation -= 360.0f;
                } else if (_camera_rotation <= -180.0f) {
                    _camera_rotation += 360.0f;
                }

                _camera.set_rotation(_camera_rotation);
            }

            _camera.set_position(_camera_position);

            _camera_translation_speed = _zoom_level;
        }

        void ortho_camera_controller::on_event(events::event& e) {
            events::event_dispatcher dispatcher(e);
            dispatcher.dispatch<events::mouse_scroll_event>(std::bind(&ortho_camera_controller::on_mouse_scroll, this, std::placeholders::_1));
            dispatcher.dispatch<events::window_resize_event>(std::bind(&ortho_camera_controller::on_window_resize, this, std::placeholders::_1));
        }

        bool ortho_camera_controller::on_mouse_scroll(events::mouse_scroll_event& e) {
            _zoom_level -= e.y() * 0.25f;
            _zoom_level = std::max(_zoom_level, 0.25f);
            _camera.set_projection({-_aspect_ratio * _zoom_level, _aspect_ratio * _zoom_level, -_zoom_level, _zoom_level});
            return false;
        }

        bool ortho_camera_controller::on_window_resize(events::window_resize_event& e) {
            _aspect_ratio = (float)e.width() / (float)e.height();
            _camera.set_projection({-_aspect_ratio * _zoom_level, _aspect_ratio * _zoom_level, -_zoom_level, _zoom_level});
            return false;
        }

        presepctive_camera_controller::presepctive_camera_controller(float fov, float aspect_ratio, float near_clip, float far_clip) 
            : _aspect_ratio(aspect_ratio),
              _camera(fov, aspect_ratio, near_clip, far_clip)
        {}

        void presepctive_camera_controller::on_update(const float& dt) {
            if (core::input::is_key_pressed(GLFW_KEY_A)) {
                _camera_position.x -= cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y -= sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            } else if (core::input::is_key_pressed(GLFW_KEY_D)) {
                _camera_position.x += cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y += sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            }

            if (core::input::is_key_pressed(GLFW_KEY_W)) {
                _camera_position.x += -sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y += cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            } else if (core::input::is_key_pressed(GLFW_KEY_S)) {
                _camera_position.x -= -sin(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
                _camera_position.y -= cos(glm::radians(_camera_rotation)) * _camera_translation_speed * dt;
            }

            if (core::input::is_key_pressed(GLFW_KEY_Q)) {
                _camera_position.z += _camera_translation_speed * dt;
            } else if (core::input::is_key_pressed(GLFW_KEY_E)) {
                _camera_position.z -= _camera_translation_speed * dt;
            }

            _camera.set_position(_camera_position);
        }

        void presepctive_camera_controller::on_event(events::event& e) {
            events::event_dispatcher dispatcher(e);
            dispatcher.dispatch<events::mouse_scroll_event>(std::bind(&presepctive_camera_controller::on_mouse_scroll, this, std::placeholders::_1));
            dispatcher.dispatch<events::window_resize_event>(std::bind(&presepctive_camera_controller::on_window_resize, this, std::placeholders::_1));
        }

        bool presepctive_camera_controller::on_mouse_scroll(events::mouse_scroll_event& e) {
            _zoom_level -= e.y() * 0.25f;
            _zoom_level = std::max(_zoom_level, 0.25f);
            // set prespective camera projection
            _camera.set_projection(_zoom_level, _aspect_ratio, 0.1f, 100.0f);
            return false;
        }

        bool presepctive_camera_controller::on_window_resize(events::window_resize_event& e) {
            _aspect_ratio = (float)e.width() / (float)e.height();
            _camera.set_projection(_zoom_level, _aspect_ratio, 0.1f, 100.0f);
            return false;
        }


    }

}

#endif//OGE_IMPL
