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

#include "gfw.h"
#ifdef GFW_ABORT_ON_BACKEND_ERROR
#include <stdlib.h>
#endif
#ifdef GFW_PRINT_BACKEND_ERROR
#include <stdio.h>
#endif

#ifndef GFW_SHADER_LOG_MAX_LENGTH
#define GFW_SHADER_LOG_MAX_LENGTH 1024
#endif

/* Texture */
void gfw_texture_unbind(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_texture_bind(struct gfw_texture *texture)
{
	glBindTexture(GL_TEXTURE_2D, texture->texture_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_texture_activate(uint32_t index)
{
	GLenum texture = GL_TEXTURE0;
	if (index > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS || index > 31) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to activate texture of index out of range.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
	texture = texture + index;
	glActiveTexture(texture);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to activate image.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_texture_put_subimage(struct gfw_texture *texture, int32_t x, int32_t y, uint8_t *data, uint32_t width, uint32_t height)
{
	glBindTexture(GL_TEXTURE_2D, texture->texture_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		x,
		y,
		width,
		height,
		texture->pixel_format,
		GL_UNSIGNED_BYTE,
		data);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set texture sub image.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	glBindTexture(GL_TEXTURE_2D, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_free_texture(struct gfw_texture *texture)
{
	if (glIsTexture(texture->texture_gl_id)) {
		glDeleteTextures(1, &texture->texture_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Error: failed to delete texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#endif
		}
#endif
	} else {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to delete invalid texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
	texture->texture_gl_id = 0;
	texture->pixel_format = 0;
	texture->width = 0;
	texture->height = 0;
}

bool gfw_init_texture(struct gfw_texture *texture, struct gfw_texture_descriptor descriptor)
{
	bool success = true;
	texture->width = descriptor.width;
	texture->height = descriptor.height;
	texture->pixel_format = descriptor.pixel_format;
	glGenTextures(1, &texture->texture_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to generate texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindTexture(GL_TEXTURE_2D, texture->texture_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, descriptor.horizontal_wrap);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set texture wrap-s parameter.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, descriptor.vertical_wrap);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set texture wrap-t parameter.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, descriptor.mag_filter);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set texture mag-filter parameter.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, descriptor.min_filter);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set texture min-filter parameter.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glTexImage2D(GL_TEXTURE_2D,
		0,
		descriptor.pixel_format,
		descriptor.width,
		descriptor.height,
		0,
		descriptor.pixel_format,
		GL_UNSIGNED_BYTE,
		descriptor.data);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set texture image.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
#ifdef GFW_CHECK_BACKEND_ERROR
	if (!glIsTexture(texture->texture_gl_id)) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to generate valid texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindTexture(GL_TEXTURE_2D, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Warning: failed to unbind texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
#if defined(GFW_CHECK_BACKEND_ERROR) && !defined(GFW_ABORT_ON_BACKEND_ERROR)
done:
#endif
	return success;
}

/* Framebuffer */
void gfw_framebuffer_clear_color(gfw_float_t r, gfw_float_t g, gfw_float_t b, gfw_float_t a)
{
	glClearColor(r, g, b, a);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set clear color value.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_framebuffer_clear_depth(gfw_float_t depth)
{
	glClearDepth(depth);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set clear depth value.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_framebuffer_clear(bool color, bool depth)
{
	GLbitfield mask = 0;
	if (color) {
		mask = mask | GL_COLOR_BUFFER_BIT;
	}
	if (depth) {
		mask = mask | GL_DEPTH_BUFFER_BIT;
	}
	glClear(mask);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to clear framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_framebuffer_unbind(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_framebuffer_bind(struct gfw_framebuffer *framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->framebuffer_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_free_framebuffer(struct gfw_framebuffer *framebuffer)
{
	if (glIsFramebuffer(framebuffer->framebuffer_gl_id)) {
		glDeleteFramebuffers(1, &framebuffer->framebuffer_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Error: failed to delete framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#endif
		}
#endif
	} else {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to delete invalid framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
	framebuffer->framebuffer_gl_id = 0;
	framebuffer->texture = NULL;
}

bool gfw_init_framebuffer(struct gfw_framebuffer *framebuffer, struct gfw_texture *texture)
{
	bool success = true;
	framebuffer->texture = texture;
	glGenFramebuffers(1, &framebuffer->framebuffer_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to generate framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->framebuffer_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->texture_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
			success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Error: failed to bind texture for framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#else
			goto done;
#endif
		}
#endif
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->texture_gl_id, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
			success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Warning: failed to set framebuffer texture.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#else
			goto done;
#endif
		}
#endif
	}
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to create complete framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind framebuffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
#if defined(GFW_CHECK_BACKEND_ERROR) && !defined(GFW_ABORT_ON_BACKEND_ERROR)
done:
#endif
	return success;
}

/* Vertex Data */
void gfw_vertex_data_clear(struct gfw_vertex_data *vertex_data)
{
	uint32_t i = 0;
	while (i < vertex_data->range) {
		vertex_data->buffer[i] = 0;
		i++;
	}
}

bool gfw_vertex_data_push(struct gfw_vertex_data *vertex_data, uint8_t *data, size_t size)
{
	bool success = true;
	uint32_t i = 0;
	if (vertex_data->count + size > vertex_data->range) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Warning: not enough space in vertex data for data.\n");
#endif
		goto done;
	}
	while (i < size) {
		vertex_data->buffer[vertex_data->count + i] = data[i];
		i++;
	}
	vertex_data->count = vertex_data->count + size;
done:
	return success;
}

void gfw_vertex_data_unmap(void)
{
	if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Warning: failed to unmap vertex data.\n");
#endif
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Error: failed to unmap vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#endif
		}
#endif
	}
}

void gfw_vertex_data_map_range(struct gfw_vertex_data *vertex_data, bool read, bool write, size_t offset, size_t range)
{
	GLint vbo_gl_id = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (vbo_gl_id == 0 || vbo_gl_id != (GLint)vertex_data->vbo_gl_id) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to map not bound vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
#ifdef GFW_CHECK_BACKEND_ERROR
	if (offset + range >= vertex_data->size) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to map vertex data range larger than vertex data size.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	if (!read && write) {
		vertex_data->buffer = glMapBufferRange(GL_ARRAY_BUFFER, offset, range, GL_MAP_WRITE_BIT);
	} else if (read && !write) {
		vertex_data->buffer = glMapBufferRange(GL_ARRAY_BUFFER, offset, range, GL_MAP_READ_BIT);
	} else if (read && write) {
		vertex_data->buffer = glMapBufferRange(GL_ARRAY_BUFFER, offset, range, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
	}
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to map vertex data with range.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	vertex_data->range = range;
	vertex_data->count = 0;
}

void gfw_vertex_data_map(struct gfw_vertex_data *vertex_data, bool read, bool write)
{
	GLint vbo_gl_id = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (vbo_gl_id == 0 || vbo_gl_id != (GLint)vertex_data->vbo_gl_id) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to map not bound vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	if (!read && write) {
		vertex_data->buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	} else if (read && !write) {
		vertex_data->buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	} else if (read && write) {
		vertex_data->buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	}
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to map vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	vertex_data->range = vertex_data->size;
	vertex_data->count = 0;
}

void gfw_vertex_data_unbind(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_vertex_data_bind(struct gfw_vertex_data *vertex_data)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_data->vbo_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_free_vertex_data(struct gfw_vertex_data *vertex_data)
{
	glDeleteBuffers(1, &vertex_data->vbo_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to delete buffer from vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	vertex_data->vbo_gl_id = 0;
	vertex_data->count = 0;
	vertex_data->range = 0;
	vertex_data->size = 0;
	vertex_data->buffer = NULL;
}

bool gfw_init_vertex_data(struct gfw_vertex_data *vertex_data, size_t size, enum gfw_vertex_data_usage usage)
{
	bool success = false;
	vertex_data->count = 0;
	vertex_data->range = 0;
	vertex_data->size = size;
	glGenBuffers(1, &vertex_data->vbo_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed generate vertex buffer object for vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindBuffer(GL_ARRAY_BUFFER, vertex_data->vbo_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	/* Buffer allocation */
	glBufferData(GL_ARRAY_BUFFER,
		vertex_data->size,
		NULL,
		usage);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to create buffer for vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
#ifdef GFW_CHECK_BACKEND_ERROR
	if (!glIsBuffer(vertex_data->vbo_gl_id)) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to initialize vertex data.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind vertex data buffer.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
#if defined(GFW_CHECK_BACKEND_ERROR) && !defined(GFW_ABORT_ON_BACKEND_ERROR)
done:
#endif
	return success;
}

/* Vertex State */
void gfw_vertex_state_unbind(void)
{
	glBindVertexArray(0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_vertex_state_bind(struct gfw_vertex_state *vertex_state)
{
	glBindVertexArray(vertex_state->vao_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_free_vertex_state(struct gfw_vertex_state *vertex_state)
{
	glDeleteVertexArrays(1, &vertex_state->vao_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to delete vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

bool gfw_init_vertex_state(struct gfw_vertex_state *vertex_state)
{
	bool success = false;
	glGenVertexArrays(1, &vertex_state->vao_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to generate vertex array object for vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindVertexArray(vertex_state->vao_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to bind vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
#ifdef GFW_CHECK_BACKEND_ERROR
	if (!glIsVertexArray(vertex_state->vao_gl_id)) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to initialize vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glBindVertexArray(0);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to unbind vertex state.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
#if defined(GFW_CHECK_BACKEND_ERROR) && !defined(GFW_ABORT_ON_BACKEND_ERROR)
done:
#endif
	return success;
}

/* Shader */
void gfw_shader_set_uniform_mat4(gfw_int_t location, bool normalize, gfw_float_t *matrix)
{
	glUniformMatrix4fv(location, 1, normalize, matrix);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set 4x4 matrix uniform variable.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_mat3(gfw_int_t location, bool normalize, gfw_float_t *matrix)
{
	glUniformMatrix3fv(location, 1, normalize, matrix);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set 3x3 matrix uniform variable.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_float(gfw_int_t location, gfw_float_t f)
{
	glUniform1f(location, f);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set floating-point uniform variable.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_floatv(gfw_int_t location, gfw_float_t *f, uint32_t count)
{
	glUniform1fv(location, count, f);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set floating-point uniform array.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_int(gfw_int_t location, gfw_int_t i)
{
	glUniform1i(location, i);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set integer uniform variable.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_intv(gfw_int_t location, gfw_int_t *i, uint32_t count)
{
	glUniform1iv(location, count, i);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set integer uniform array.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_uint(gfw_int_t location, gfw_uint_t i)
{
	glUniform1ui(location, i);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set unsigned integer uniform variable.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_shader_set_uniform_uintv(gfw_int_t location, gfw_uint_t *i, uint32_t count)
{
	glUniform1uiv(location, count, i);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set unsigned integer uniform array.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

gfw_int_t gfw_shader_get_uniform_location(struct gfw_shader *shader, char *name)
{
	gfw_int_t location = glGetUniformLocation(shader->program_gl_id, name);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to get uniform location.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	return location;
}

gfw_int_t gfw_shader_get_attribute_location(struct gfw_shader *shader, char *name)
{
	gfw_int_t location = glGetAttribLocation(shader->program_gl_id, name);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to get shader attribute location.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	return location;
}

void gfw_shader_draw_range(struct gfw_attribute *attributes,
	size_t attributes_count,
	enum gfw_primitive primitive,
	size_t first,
	size_t count)
{
	uint32_t i = 0;
	while (i < attributes_count) {
		glEnableVertexAttribArray(attributes[i].location);
		glVertexAttribPointer(attributes[i].location,
			attributes[i].count,
			attributes[i].type,
			attributes[i].normalize,
			attributes[i].stride,
			(GLvoid *)attributes[i].offset);
		i++;
	}
	glDrawArrays(primitive, first, count);
	i = 0;
	while (i < attributes_count) {
		glDisableVertexAttribArray(attributes[i].location);
		i++;
	}
}

void gfw_shader_use(struct gfw_shader *shader)
{
	glUseProgram(shader->program_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to use shader.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_free_shader(struct gfw_shader *shader)
{
	glDeleteProgram(shader->program_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to delete shader program.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
	shader->program_gl_id = 0;
}

static void gfw_detach_shader(GLuint program_gl_id, GLuint shader_gl_id)
{
	glDetachShader(program_gl_id, shader_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to detach shader.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

static void gfw_delete_shader(GLuint shader_gl_id)
{
	glDeleteShader(shader_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to delete shader.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

static bool gfw_compile_shader(GLuint *shader_gl_id, GLenum shader_type, char *source)
{
	bool success = true;
	GLint param = 0;
	char log_string[GFW_SHADER_LOG_MAX_LENGTH] = {0};
	*shader_gl_id = glCreateShader(shader_type);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		if (shader_type == GL_VERTEX_SHADER) {
			printf("Error: failed to create vertex shader.\n");
		} else if (shader_type == GL_FRAGMENT_SHADER) {
			printf("Error: failed to create fragment shader.\n");
		} else if (shader_type == GL_GEOMETRY_SHADER) {
			printf("Error: failed to create geometry shader.\n");
		}
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glShaderSource(*shader_gl_id, 1, (const char **)&source, NULL);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		if (shader_type == GL_VERTEX_SHADER) {
			printf("Error: failed to set vertex shader source.\n");
		} else if (shader_type == GL_FRAGMENT_SHADER) {
			printf("Error: failed to set fragment shader source.\n");
		} else if (shader_type == GL_GEOMETRY_SHADER) {
			printf("Error: failed to set geometry shader source.\n");
		}
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glCompileShader(*shader_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		if (shader_type == GL_VERTEX_SHADER) {
			printf("Error: failed to compile vertex shader.\n");
		} else if (shader_type == GL_FRAGMENT_SHADER) {
			printf("Error: failed to compile fragment shader.\n");
		} else if (shader_type == GL_GEOMETRY_SHADER) {
			printf("Error: failed to compile geometry shader.\n");
		}
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glGetShaderiv(*shader_gl_id, GL_COMPILE_STATUS, &param);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		if (shader_type == GL_VERTEX_SHADER) {
			printf("Error: failed to get vertex shader status.\n");
		} else if (shader_type == GL_FRAGMENT_SHADER) {
			printf("Error: failed to get fragment shader status.\n");
		} else if (shader_type == GL_GEOMETRY_SHADER) {
			printf("Error: failed to get geometry shader status.\n");
		}
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	if (!param) {
		GLsizei length;
		glGetShaderInfoLog(*shader_gl_id,
			GFW_SHADER_LOG_MAX_LENGTH,
			&length,
			log_string);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
			success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
			if (shader_type == GL_VERTEX_SHADER) {
				printf("Error: failed to get vertex shader info log.\n");
			} else if (shader_type == GL_FRAGMENT_SHADER) {
				printf("Error: failed to get fragment shader info log.\n");
			} else if (shader_type == GL_GEOMETRY_SHADER) {
				printf("Error: failed to get geometry shader info log.\n");
			}
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#else
			goto done;
#endif
		}
#endif
#ifdef GFW_PRINT_BACKEND_ERROR
		if (shader_type == GL_VERTEX_SHADER) {
			printf("Vertex shader error:\n %s", log_string);
		} else if (shader_type == GL_FRAGMENT_SHADER) {
			printf("Fragment shader error:\n %s", log_string);
		} else if (shader_type == GL_GEOMETRY_SHADER) {
			printf("Geometry shader error:\n %s", log_string);
		}
#endif
		success = false;
		goto done;
	}
done:
	return success;
}

bool gfw_init_shader(struct gfw_shader *shader, char *vertex_source, char *geometry_source, char *fragment_source)
{
	bool success = true;
	GLuint vertex_shader_gl_id = 0;
	GLuint geometry_shader_gl_id = 0;
	GLuint fragment_shader_gl_id = 0;
	GLint param = 0;
	char log_string[GFW_SHADER_LOG_MAX_LENGTH] = {0};
	/* Vertex shader */
	if (!gfw_compile_shader(&vertex_shader_gl_id, GL_VERTEX_SHADER, vertex_source)) {
		success = false;
		goto done;
	}
	if (geometry_source) {
		if (!gfw_compile_shader(&geometry_shader_gl_id, GL_GEOMETRY_SHADER, geometry_source)) {
			success = false;
			goto done;
		}
	}
	if (!gfw_compile_shader(&fragment_shader_gl_id, GL_FRAGMENT_SHADER, fragment_source)) {
		success = false;
		goto done;
	}
	/* Create program and attach shaders */
	shader->program_gl_id = glCreateProgram();
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to create shader program.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glAttachShader(shader->program_gl_id, vertex_shader_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to attach vertex shader.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	if (geometry_source) {
		glAttachShader(shader->program_gl_id, geometry_shader_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
			success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Error: failed to attach geometry shader.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#else
			goto done;
#endif
		}
#endif
	}
	glAttachShader(shader->program_gl_id, fragment_shader_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to attach fragment shader.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	glLinkProgram(shader->program_gl_id);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to link shader program.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	/* Check status */
	glGetProgramiv(shader->program_gl_id, GL_LINK_STATUS, &param);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
		success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to get shader program status.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#else
		goto done;
#endif
	}
#endif
	if (!param) {
		GLsizei length;
		glGetProgramInfoLog(shader->program_gl_id,
			GFW_SHADER_LOG_MAX_LENGTH,
			&length,
			log_string);
#ifdef GFW_CHECK_BACKEND_ERROR
		if (glGetError() != GL_NO_ERROR) {
			success = false;
#ifdef GFW_PRINT_BACKEND_ERROR
			printf("Error: failed to get program info log.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
			abort();
#else
			goto done;
#endif
		}
#endif
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Program shader error:\n %s", log_string);
#endif
		gfw_delete_shader(vertex_shader_gl_id);
		if (geometry_source) {
			gfw_delete_shader(geometry_shader_gl_id);
		}
		gfw_delete_shader(fragment_shader_gl_id);
		success = false;
		goto done;
	}
	/* Detach and delete unneeded shaders */
	gfw_detach_shader(shader->program_gl_id, vertex_shader_gl_id);
	gfw_delete_shader(vertex_shader_gl_id);
	if (geometry_source) {
		gfw_detach_shader(shader->program_gl_id, geometry_shader_gl_id);
		gfw_delete_shader(geometry_shader_gl_id);
	}
	gfw_detach_shader(shader->program_gl_id, fragment_shader_gl_id);
	gfw_delete_shader(fragment_shader_gl_id);
done:
	return success;
}

/* Graphic state */
void gfw_set_viewport(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set viewport.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_blend(void)
{
	glEnable(GL_BLEND);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable blending.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_blend(void)
{
	glDisable(GL_BLEND);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable blending.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_clip_distance(uint32_t i)
{
	glEnable(GL_CLIP_DISTANCE0 + i);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable clip distance in space %u.\n", i);
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_clip_distance(uint32_t i)
{
	glDisable(GL_CLIP_DISTANCE0 + i);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable clip distance in space %u.\n", i);
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_color_logic_operation(void)
{
	glEnable(GL_COLOR_LOGIC_OP);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable color logic operation.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_color_logic_operation(void)
{
	glDisable(GL_COLOR_LOGIC_OP);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable color logic operation.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_cull_face(void)
{
	glEnable(GL_CULL_FACE);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable cull face.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_cull_face(void)
{
	glDisable(GL_CULL_FACE);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable cull face.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_depth_clamp(void)
{
	glEnable(GL_DEPTH_CLAMP);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable depth clamp.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_depth_clamp(void)
{
	glDisable(GL_DEPTH_CLAMP);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable depth clamp.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_depth_test(void)
{
	glEnable(GL_DEPTH_TEST);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable depth test.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_depth_test(void)
{
	glDisable(GL_DEPTH_TEST);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable depth test.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_dither(void)
{
	glEnable(GL_DITHER);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable dither.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_dither(void)
{
	glDisable(GL_DITHER);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable dither.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_enable_line_smooth(void)
{
	glEnable(GL_LINE_SMOOTH);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to enable line smooth.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_disable_line_smooth(void)
{
	glDisable(GL_LINE_SMOOTH);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to disable line smooth.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_blend_equation(enum gfw_blend_equation blend_equation_rgb, enum gfw_blend_equation blend_equation_alpha)
{
	glBlendEquationSeparate(blend_equation_rgb, blend_equation_alpha);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set blend equation.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_blend_function(enum gfw_blend_factor blend_factor_source, enum gfw_blend_factor blend_factor_destination)
{
	glBlendFunc(blend_factor_source, blend_factor_destination);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set blend function.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_blend_function_separate(enum gfw_blend_factor blend_factor_source_rgb, enum gfw_blend_factor blend_factor_destination_rgb, enum gfw_blend_factor blend_factor_source_alpha, enum gfw_blend_factor blend_factor_destination_alpha)
{
	glBlendFuncSeparate(blend_factor_source_rgb, blend_factor_destination_rgb, blend_factor_source_alpha, blend_factor_destination_alpha);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set blend functions.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_blend_color(gfw_float_t r, gfw_float_t g, gfw_float_t b, gfw_float_t a)
{
	glBlendColor(r, g, b, a);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set blending color.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_color_logic_operation(enum gfw_color_logic_operation color_logic_operation)
{
	glLogicOp(color_logic_operation);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set color logic operation.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_cull_face_mode(enum gfw_cull_face_mode cull_face_mode)
{
	glCullFace(cull_face_mode);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set cull face mode.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_front_face(enum gfw_cull_face_front front_face)
{
	glFrontFace(front_face);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set face culling front face.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_depth_test_function(enum gfw_depth_test_function depth_test_function)
{
	glDepthFunc(depth_test_function);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set depth function.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_depth_range(gfw_float_t near, gfw_float_t far)
{
	glDepthRange(near, far);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set depth range.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_point_size(gfw_float_t size)
{
	glPointSize(size);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set point size.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}

void gfw_set_line_width(gfw_float_t width)
{
	glLineWidth(width);
#ifdef GFW_CHECK_BACKEND_ERROR
	if (glGetError() != GL_NO_ERROR) {
#ifdef GFW_PRINT_BACKEND_ERROR
		printf("Error: failed to set line width.\n");
#endif
#ifdef GFW_ABORT_ON_BACKEND_ERROR
		abort();
#endif
	}
#endif
}
