/*!
 * \brief
 *
 * \author ddubois 
 * \date 20-Sep-16.
 */

#ifndef RENDERER_SHADER_INTERFACE_H
#define RENDERER_SHADER_INTERFACE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <initializer_list>
#include <mutex>
#include <optional.hpp>

#include "gl_shader_program.h"
#include "geometry_filter.h"
#include "../../../data_loading/loaders/shader_source_structs.h"

namespace nova {
    /*!
     * \brief Represents a shaderpack in all of its glory
     */
    class shaderpack {
    public:
        /*!
         * \brief Loads the shaderpack with the given name
         *
         * This is kinda gross because the shaderpack loading logic is all
         * in the data_loading module... thing is, there's no longer any
         * reason to keep that running in a separate thread, so why not put
         * it here?
         *
         * \param shaderpack_name The name of the shaderpcack to load
         *
         */
        shaderpack(std::string name, nlohmann::json shaders_json, std::vector<shader_definition> &shaders);

        /*!
         * \brief Move constructor
         *
         * \param other The shaderpack to move from
         */
        shaderpack(shaderpack &&other);

        gl_shader_program &operator[](std::string key);

        std::unordered_map<std::string, gl_shader_program> &get_loaded_shaders();

        void operator=(const shaderpack& other);

        std::string& get_name();

    private:
        /*!
         * \brief Holds the functions used to modify a geometry_filter
         *
         * The idea here is that a list of filters will be provided in the shaders.json file. Each of those filters
         * corresponds to either a function in this map, or a request to add a specific geometry_type, name, or
         * name part to the filter.
         */
        static std::unordered_map<std::string, std::function<void(geometry_filter&)>> filter_modifying_functions;

        std::unordered_map<std::string, gl_shader_program> loaded_shaders;

        std::string name;
    };
}

#endif //RENDERER_SHADER_INTERFACE_H.