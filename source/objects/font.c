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

#include "../shared.h"

#include "Vera_ttf.h"

// Initializes two arrays filled with null pointers.
sftd_font *fontPntrArray[10] = { 0 };
char *fontPathArray[10] = { 0 };
int fontUseArray[10] = { 0 };

#define CLASS_TYPE  LUAOBJ_TYPE_FONT
#define CLASS_NAME  "Font"

const char *fontInit(love_font *self, const char *filename, int size) {

	if (!fileExists(filename)) return "Could not open font. Does not exist.";

	int i;
	bool fontExists = false;

	for (i = 1; i < 10; i++) { // Has the font already been loaded before?

		if (strcmp(fontPathArray[i], filename) == 0) {
			fontExists = true;
			break;
		}

	}

	if (fontExists) { // The font's already been loaded.

		self->font = fontPntrArray[i];
		self->id = i;

		fontUseArray[i]++;

	} else { // The font hasn't been loaded, load it in and store it in the cache.

		int k;
		for (k = 1; k < 10; k++) {

			if (fontPathArray[k] == 0) {

				strcpy(fontPathArray[k], filename);
				fontPntrArray[k] = sftd_load_font_file(filename);

				self->font = fontPntrArray[k];
				self->id = k;

				fontUseArray[k]++;
				break;

			}
		}

	}

	self->size = size;

	return NULL;

}

const char *fontDefaultInit(love_font *self, int size) {

	if (fontPntrArray[0] == 0) {

		fontPntrArray[0] = sftd_load_font_mem(Vera_ttf, Vera_ttf_size);
		strcpy(fontPathArray[0], "Memory");

	}

	self->font = fontPntrArray[0];
	self->size = size;
	self->id = 0;

	fontUseArray[0]++;

	return NULL;

}

int fontNew(lua_State *L) { // love.graphics.newFont()

	char *error;
	char *filename;
	int size;

	// const char *filename = lua_isnoneornil(L, 1) ? NULL : luaL_checkstring(L, 1);
	// int size = lua_isnoneornil(L, 2) ? NULL : luaL_checkinteger(L, 2);

	love_font *self = luaobj_newudata(L, sizeof(*self));

	luaobj_setclass(L, CLASS_TYPE, CLASS_NAME);

	// if (!size) size = 14;

	// if (filename) {

	// 	const char *error = fontInit(self, filename, size);

	// 	if (error) luaError(L, error);

	// } else {

	// 	fontDefaultInit(self, size);
	// }

	// return 1;

	if (lua_type(L, 1) == LUA_TNUMBER) {

		size = luaL_checkinteger(L, 1);
		error = fontDefaultInit(self, size);

	} else if (lua_type(L, 1) == LUA_TSTRING) {

		filename = luaL_checkstring(L, 1);
		size = luaL_optnumber(L, 2, 14);
		error = fontInit(self, filename, size);

	} else {

		size = 14;
		error = fontDefaultInit(self, size);

	}

	if (error) luaError(L, error);

	return 1;

}

int fontGC(lua_State *L) { // Garbage Collection

	love_font *self = luaobj_checkudata(L, 1, CLASS_TYPE);

	if (fontUseArray[self->id] == 0) {

		sftd_free_font(self->font);
		fontPntrArray[self->id] = 0;
		fontPathArray[self->id] = 0;

	} else {

		fontUseArray[self->id]--;

	}

	return 0;

}

int fontGetWidth(lua_State *L) { // font:getWidth()

	love_font *self = luaobj_checkudata(L, 1, CLASS_TYPE);
	const char *text = luaL_checkstring(L, 2);

	lua_pushinteger(L, sftd_get_text_width(self->font, self->size, text));

	return 1;

}

int fontGetHeight(lua_State *L) { // font:getHeight()

	love_font *self = luaobj_checkudata(L, 1, CLASS_TYPE);
	lua_pushinteger(L, self->size);

	return 1;

}

int initFontClass(lua_State *L) {

	luaL_Reg reg[] = {
		{ "new",       fontNew       },
		{ "__gc",      fontGC        },
		{ "getWidth",  fontGetWidth  },
		{ "getHeight", fontGetHeight },
		{ 0, 0 },
	};

  luaobj_newclass(L, CLASS_NAME, NULL, fontNew, reg);

  return 1;

}