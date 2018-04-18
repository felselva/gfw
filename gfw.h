/*
Copyright © 2018 Felipe Ferreira da Silva

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

  1. The origin of this software must not be misrepresented; you must not claim
     that you wrote the original software. If you use this software in a
     product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef GFW_H
#define GFW_H

#include <stdint.h>
#include <stdbool.h>
#include <glad.h>

/* Types */
typedef GLbyte gfw_byte_t;
typedef GLubyte gfw_ubyte_t;
typedef GLshort gfw_short_t;
typedef GLushort gfw_ushort_t;
typedef GLint gfw_int_t;
typedef GLuint gfw_uint_t;
typedef GLhalf gfw_half_float_t;
typedef GLfloat gfw_float_t;
typedef GLdouble gfw_double_t;

/* Texture */
enum gfw_texture_wrap {
	GFW_TEXTURE_WRAP_REPEAT = GL_REPEAT,
	GFW_TEXTURE_WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	GFW_TEXTURE_WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	GFW_TEXTURE_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

enum gfw_texture_filter {
	GFW_TEXTURE_FILTER_NEAREST = GL_NEAREST,
	GFW_TEXTURE_FILTER_LINEAR = GL_LINEAR
};

enum gfw_texture_pixel_format {
	GFW_TEXTURE_PIXEL_FORMAT_RGB = GL_RGB,
	GFW_TEXTURE_PIXEL_FORMAT_RGBA = GL_RGBA,
	GFW_TEXTURE_PIXEL_FORMAT_PALETTE = GL_RED
};

struct gfw_texture_descriptor {
	enum gfw_texture_pixel_format pixel_format;
	enum gfw_texture_wrap horizontal_wrap;
	enum gfw_texture_wrap vertical_wrap;
	enum gfw_texture_filter mag_filter;
	enum gfw_texture_filter min_filter;
	uint32_t width;
	uint32_t height;
	void *data;
};

struct gfw_texture {
	gfw_uint_t texture_gl_id;
	enum gfw_texture_pixel_format pixel_format;
	uint32_t width;
	uint32_t height;
};

/* Framebuffer */
struct gfw_framebuffer {
	gfw_uint_t framebuffer_gl_id;
	struct gfw_texture *texture;
};

/* Vertex Data */
enum gfw_primitive {
	GFW_PRIMITIVE_TRIANGLES = GL_TRIANGLES,
	GFW_PRIMITIVE_LINES = GL_LINES,
	GFW_PRIMITIVE_LINE_STRIPS = GL_LINE_STRIP,
	GFW_PRIMITIVE_POINTS = GL_POINTS
};

enum gfw_vertex_data_usage {
	GFW_VERTEX_DATA_USAGE_STATIC = GL_STATIC_DRAW,
	GFW_VERTEX_DATA_USAGE_DYNAMIC = GL_DYNAMIC_DRAW
};

enum gfw_attribute_type {
	GFW_ATTRIBUTE_BYTE = GL_BYTE,
	GFW_ATTRIBUTE_UBYTE = GL_UNSIGNED_BYTE,
	GFW_ATTRIBUTE_SHORT = GL_SHORT,
	GFW_ATTRIBUTE_USHORT = GL_UNSIGNED_SHORT,
	GFW_ATTRIBUTE_INT = GL_INT,
	GFW_ATTRIBUTE_UINT = GL_UNSIGNED_INT,
	GFW_ATTRIBUTE_HALF_FLOAT = GL_HALF_FLOAT,
	GFW_ATTRIBUTE_FLOAT = GL_FLOAT,
	GFW_ATTRIBUTE_DOUBLE = GL_DOUBLE
};

struct gfw_attribute {
	gfw_int_t location;
	enum gfw_attribute_type type;
	size_t count;
	bool normalize;
	size_t stride;
	size_t offset;
};

struct gfw_vertex_data {
	gfw_uint_t vbo_gl_id;
	size_t range;
	size_t count;
	size_t size;
	uint8_t *buffer;
};

/* Vertex State */
struct gfw_vertex_state {
	gfw_uint_t vao_gl_id;
};

/* Shader */
struct gfw_shader {
	gfw_uint_t program_gl_id;
};

/* Graphic states */
enum gfw_blend_factor {
	GFW_BLEND_FACTOR_ZERO = GL_ZERO,
	GFW_BLEND_FACTOR_ONE = GL_ONE,
	GFW_BLEND_FACTOR_SOURCE_COLOR = GL_SRC_COLOR,
	GFW_BLEND_FACTOR_ONE_MINUS_SOURCE_COLOR = GL_ONE_MINUS_SRC_ALPHA,
	GFW_BLEND_FACTOR_DESTINATION_COLOR = GL_DST_COLOR,
	GFW_BLEND_FACTOR_ONE_MINUS_DESTINATION_COLOR = GL_ONE_MINUS_DST_COLOR,
	GFW_BLEND_FACTOR_SOURCE_ALPHA = GL_SRC_ALPHA,
	GFW_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	GFW_BLEND_FACTOR_DESTINATION_ALPHA = GL_DST_ALPHA,
	GFW_BLEND_FACTOR_ONE_MINUS_DESTINATION_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	GFW_BLEND_FACTOR_CONSTANT_COLOR = GL_CONSTANT_COLOR,
	GFW_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
	GFW_BLEND_FACTOR_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
	GFW_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
	GFW_BLEND_FACTOR_SOURCE_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
	GFW_BLEND_FACTOR_SOURCE1_COLOR = GL_SRC1_COLOR,
	GFW_BLEND_FACTOR_ONE_MINUS_SOURCE1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
	GFW_BLEND_FACTOR_SOURCE1_ALPHA = GL_SRC1_ALPHA,
	GFW_BLEND_FACTOR_ONE_MINUS_SOURCE1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA
};

enum gfw_blend_equation {
	GFW_BLEND_EQUATION_ADD = GL_FUNC_ADD,
	GFW_BLEND_EQUATION_SUBTRACT = GL_FUNC_SUBTRACT,
	GFW_BLEND_EQUATION_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
	GFW_BLEND_EQUATION_MIN = GL_MIN,
	GFW_BLEND_EQUATION_MAX = GL_MAX
};

enum gfw_color_logic_operation {
	GFW_COLOR_LOGIC_OPERATION_CLEAR = GL_CLEAR,
	GFW_COLOR_LOGIC_OPERATION_SET = GL_SET,
	GFW_COLOR_LOGIC_OPERATION_COPY = GL_COPY,
	GFW_COLOR_LOGIC_OPERATION_COPY_INVERTED = GL_COPY_INVERTED,
	GFW_COLOR_LOGIC_OPERATION_NO_OPERATION = GL_NOOP,
	GFW_COLOR_LOGIC_OPERATION_INVERT = GL_INVERT,
	GFW_COLOR_LOGIC_OPERATION_AND = GL_AND,
	GFW_COLOR_LOGIC_OPERATION_NOT_AND = GL_NAND,
	GFW_COLOR_LOGIC_OPERATION_OR = GL_OR,
	GFW_COLOR_LOGIC_OPERATION_NOT_OR = GL_NOR,
	GFW_COLOR_LOGIC_OPERATION_EXCLUSIVE_OR = GL_XOR,
	GFW_COLOR_LOGIC_OPERATION_EQUIVALENT = GL_EQUIV,
	GFW_COLOR_LOGIC_OPERATION_AND_REVERSE = GL_AND_REVERSE,
	GFW_COLOR_LOGIC_OPERATION_AND_INVERTED = GL_AND_INVERTED,
	GFW_COLOR_LOGIC_OPERATION_OR_REVERSE = GL_OR_REVERSE,
	GFW_COLOR_LOGIC_OPERATION_OR_INVERTED = GL_OR_INVERTED
};

enum gfw_cull_face_mode {
	GFW_CULL_FACE_FRONT = GL_FRONT,
	GFW_CULL_FACE_BACK = GL_BACK,
	GFW_CULL_FACE_FRONT_AND_BACK = GL_FRONT_AND_BACK
};

enum gfw_cull_face_front {
	GFW_CULL_FACE_FRONT_CW = GL_CW,
	GFW_CULL_FACE_FRONT_CCW = GL_CCW
};

enum gfw_depth_test_function {
	GFW_DEPTH_TEST_FUNCTION_NEVER = GL_NEVER,
	GFW_DEPTH_TEST_FUNCTION_LESS = GL_LESS,
	GFW_DEPTH_TEST_FUNCTION_EQUAL = GL_EQUAL,
	GFW_DEPTH_TEST_FUNCTION_LESS_OR_EQUAL = GL_LEQUAL,
	GFW_DEPTH_TEST_FUNCTION_GREATER = GL_GREATER,
	GFW_DEPTH_TEST_FUNCTION_NOT_EQUAL = GL_NOTEQUAL,
	GFW_DEPTH_TEST_FUNCTION_GREATER_OR_EQUAL = GL_GEQUAL,
	GFW_DEPTH_TEST_FUNCTION_ALWAYS = GL_ALWAYS
};

/* Texture */
void gfw_texture_unbind(void);
void gfw_texture_bind(struct gfw_texture *texture);
void gfw_texture_activate(uint32_t index);
void gfw_texture_put_subimage(struct gfw_texture *texture, int32_t x, int32_t y, uint8_t *data, uint32_t width, uint32_t height);
void gfw_free_texture(struct gfw_texture *texture);
bool gfw_init_texture(struct gfw_texture *texture, struct gfw_texture_descriptor descriptor);

/* Framebuffer */
void gfw_framebuffer_clear_color(float r, float g, float b, float a);
void gfw_framebuffer_clear_depth(float depth);
void gfw_framebuffer_clear(bool color, bool depth);
void gfw_framebuffer_unbind(void);
void gfw_framebuffer_bind(struct gfw_framebuffer *framebuffer);
void gfw_free_framebuffer(struct gfw_framebuffer *framebuffer);
bool gfw_init_framebuffer(struct gfw_framebuffer *framebuffer, struct gfw_texture *texture);

/* Vertex data */
void gfw_vertex_data_clear(struct gfw_vertex_data *vertex_data);
bool gfw_vertex_data_push(struct gfw_vertex_data *vertex_data, uint8_t *data, size_t size);
void gfw_vertex_data_unmap(void);
void gfw_vertex_data_map_range(struct gfw_vertex_data *vertex_data, bool read, bool write, size_t offset, size_t range);
void gfw_vertex_data_map(struct gfw_vertex_data *vertex_data, bool read, bool write);
void gfw_vertex_data_unbind(void);
void gfw_vertex_data_bind(struct gfw_vertex_data *vertex_data);
void gfw_free_vertex_data(struct gfw_vertex_data *vertex_data);
bool gfw_init_vertex_data(struct gfw_vertex_data *vertex_data, size_t size, enum gfw_vertex_data_usage usage);

/* Vertex state */
void gfw_vertex_state_unbind(void);
void gfw_vertex_state_bind(struct gfw_vertex_state *vertex_state);
void gfw_free_vertex_state(struct gfw_vertex_state *vertex_state);
bool gfw_init_vertex_state(struct gfw_vertex_state *vertex_state);

/* Shader */
void gfw_shader_set_uniform_mat4(gfw_int_t location, bool normalize, gfw_float_t *matrix);
void gfw_shader_set_uniform_mat3(gfw_int_t location, bool normalize, gfw_float_t *matrix);
void gfw_shader_set_uniform_float(gfw_int_t location, gfw_float_t f);
void gfw_shader_set_uniform_floatv(gfw_int_t location, gfw_float_t *f, uint32_t count);
void gfw_shader_set_uniform_int(gfw_int_t location, gfw_int_t i);
void gfw_shader_set_uniform_intv(gfw_int_t location, gfw_int_t *i, uint32_t count);
void gfw_shader_set_uniform_uint(gfw_int_t location, gfw_uint_t i);
void gfw_shader_set_uniform_uintv(gfw_int_t location, gfw_uint_t *i, uint32_t count);
gfw_int_t gfw_shader_get_uniform_location(struct gfw_shader *shader, char *name);
gfw_int_t gfw_shader_get_attribute_location(struct gfw_shader *shader, char *name);
void gfw_shader_draw_range(struct gfw_attribute *attributes,
	size_t attributes_count,
	enum gfw_primitive primitive,
	size_t first,
	size_t count);
void gfw_shader_use(struct gfw_shader *shader);
void gfw_free_shader(struct gfw_shader *shader);
bool gfw_init_shader(struct gfw_shader *shader, char *vertex_source, char *geometry_source, char *fragment_source);

/* Graphic state */
void gfw_set_viewport(int32_t x, int32_t y, uint32_t width, uint32_t height);
void gfw_enable_blend(void);
void gfw_disable_blend(void);
void gfw_enable_clip_distance(uint32_t i);
void gfw_disable_clip_distance(uint32_t i);
void gfw_enable_color_logic_operation(void);
void gfw_disable_color_logic_operation(void);
void gfw_enable_cull_face(void);
void gfw_disable_cull_face(void);
void gfw_enable_depth_clamp(void);
void gfw_disable_depth_clamp(void);
void gfw_enable_depth_test(void);
void gfw_disable_depth_test(void);
void gfw_enable_dither(void);
void gfw_disable_dither(void);
void gfw_enable_line_smooth(void);
void gfw_disable_line_smooth(void);
void gfw_set_blend_equation(enum gfw_blend_equation blend_equation_rgb, enum gfw_blend_equation blend_equation_alpha);
void gfw_set_blend_function(enum gfw_blend_factor blend_factor_source, enum gfw_blend_factor blend_factor_destination);
void gfw_set_blend_function_separate(enum gfw_blend_factor blend_factor_source_rgb, enum gfw_blend_factor blend_factor_destination_rgb, enum gfw_blend_factor blend_factor_source_alpha, enum gfw_blend_factor blend_factor_destination_alpha);
void gfw_set_blend_color(float r, float g, float b, float a);
void gfw_set_color_logic_operation(enum gfw_color_logic_operation color_logic_operation);
void gfw_set_cull_face_mode(enum gfw_cull_face_mode cull_face_mode);
void gfw_set_front_face(enum gfw_cull_face_front cull_front_face);
void gfw_set_depth_test_function(enum gfw_depth_test_function depth_test_function);
void gfw_set_depth_range(float near, float far);
void gfw_set_point_size(float size);
void gfw_set_line_width(float width);

#endif
