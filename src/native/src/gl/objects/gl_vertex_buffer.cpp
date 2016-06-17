/*!
 * \author David
 * \date 13-May-16.
 */

#include <c++/4.8.3/stdexcept>
#include "gl_vertex_buffer.h"

gl_vertex_buffer::gl_vertex_buffer() {
    gl_name = 0xFFFFFFFF;
    element_array_name = 0xFFFFFFFF;
    create();
}

gl_vertex_buffer::~gl_vertex_buffer() {
    destroy();
}

void gl_vertex_buffer::create() {
    glGenBuffers(1, &gl_name);
    glGenBuffers(1, &element_array_name);
}

void gl_vertex_buffer::destroy() {
    if(gl_name != 0xFFFFFFFF) {
        glDeleteBuffers(1, &gl_name);
    }

    if(element_array_name != 0xFFFFFFFF) {
        glDeleteBuffers(1, &element_array_name);
    }
}

void gl_vertex_buffer::set_data(std::vector<float> data, format data_format, usage data_usage) {
    this->data_format = data_format;

    glBindBuffer(GL_ARRAY_BUFFER, gl_name);
    GLenum buffer_usage = translate_usage(data_usage);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), buffer_usage);

    enable_vertex_attributes(data_format);
}

GLenum gl_vertex_buffer::translate_usage(const usage data_usage) const {
    switch(data_usage) {
        case usage::dynamic_draw:
            return GL_DYNAMIC_DRAW;
        case usage::static_draw:
            return GL_STATIC_DRAW;
        default:
            // In case something bad happens
            throw std::invalid_argument("data_usage value unsupported");
    }
}

void gl_vertex_buffer::set_active() {
    glBindBuffer(GL_ARRAY_BUFFER, gl_name);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_name);
}

void gl_vertex_buffer::set_index_array(std::vector<unsigned short> data, usage data_usage) {
    GLuint last_buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint *) &last_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_name);
    GLenum buffer_usage = translate_usage(data_usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned short), data.data(), buffer_usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_buffer);
}

void gl_vertex_buffer::draw() {

}

void gl_vertex_buffer::enable_vertex_attributes(format data_format) {
    switch(data_format) {
        case format::POS:
            // We only need to set up positional data
            // Positions are always at vertex attribute 0
            glEnableVertexAttribArray(0);   // Position

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            break;

        case format::POS_UV:
            glEnableVertexAttribArray(0);   // Position
            glEnableVertexAttribArray(1);   // Texture UV

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);

            break;

        case format::POS_UV_LIGHTMAPUV_NORMAL_TANGENT:
            glEnableVertexAttribArray(0);   // Position
            glEnableVertexAttribArray(1);   // Texture UV
            glEnableVertexAttribArray(2);   // Lightmap UV
            glEnableVertexAttribArray(3);   // Normal
            glEnableVertexAttribArray(4);   // Tangent

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GL_FLOAT), (void*)0);

            break;
    }
}
