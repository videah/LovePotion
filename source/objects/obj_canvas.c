// This code is licensed under the MIT Open Source License.

// Copyright (c) 2015 Ruairidh Carmichael - ruairidhcarmichael@live.co.uk

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <shared.h>

#define CLASS_TYPE  LUAOBJ_TYPE_CANVAS
#define CLASS_NAME  "Canvas"

char * canvasInit(love_canvas * self) {
	self->width = luaL_checkinteger(L, 1);

	self->height = luaL_checkinteger(L, 2);

	self->renderTarget = sf2d_create_rendertarget(self->width, self->height);
}

int canvasNew(lua_State *L) { // love.graphics.newCanvas()

	love_canvas *self = luaobj_newudata(L, sizeof(*self));

	luaobj_setclass(L, CLASS_TYPE, CLASS_NAME);

	const char *error = imageInit(self);

	if (error) luaError(L, error);

	return 1;

}

int canvasGetDimensions(lua_State * L) {
	love_canvas * self = luaobj_checkudata(L, 1, CLASS_TYPE);

	lua_pushinteger(L, self->width);

	lua_pushinteger(L, self->height);

	return 2;
}

int canvasGetWidth(lua_State * L) {
	love_canvas * self = luaobj_checkudata(L, 1, CLASS_TYPE);

	lua_pushinteger(L, self->width);

	return 1;
}

int canvasGetHeight(lua_State * L) {
	love_canvas * self = luaobj_checkudata(L, 1, CLASS_TYPE);

	lua_pushinteger(L, self->height);

	return 1;
}

int canvasRenderTo(lua_State * L) {
	love_canvas * self = luaobj_checkudata(L, 1, CLASS_TYPE);

	self->renderFunction = lua_tocfunction(L, 2);

	return 0;
}

int canvasDraw(lua_State * L, u32 color) {
	love_canvas * self = luaobj_checkudata(L, 1, CLASS_TYPE);

	sf2d_start_frame_target(self->renderTarget);

	sf2d_clear_target(self->renderTarget, color);

	sf2d_end_frame();
}

int canvasGC(lua_State * L) {
	love_canvas *self = luaobj_checkudata(L, 1, CLASS_TYPE);

	if (self->renderTarget == NULL) return 0;

	sf2d_free_target(self->renderTarget);

	return 0;
}

int initCanvasClass(lua_State *L) {

	luaL_Reg reg[] = {
		{ "new",            canvasNew           },
		{ "__gc",           canvasGC            },
		{ "getDimensions", 	canvasGetDimensions },
		{ "getWidth",       canvasGetWidth      },
		{ "getHeight",      canvasGetHeight     },
		{ "renderTo",		canvasRenderTo		},
		{ 0, 0 },
	};

  luaobj_newclass(L, CLASS_NAME, NULL, canvasNew, reg);

  return 1;

}
