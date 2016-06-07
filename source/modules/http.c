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

int httpRequest(lua_State * L) { //http.request()
	char * url = luaL_checkstring(L, 1);

	httpcContext httpContext;

	u8 * buffer;

	int statusCode;

	Result returnCode = httpcOpenContext(&httpContext, HTTPC_METHOD_GET, url, 0);

	if (returnCode == 0) {
		u32 statusCode = 0;

		u32 size = 0;

		u32 contentSize = 0;

		returnCode = httpcBeginRequest(&httpContext);

		if (returnCode != 0) {
			lua_pushnil(L);

			lua_pushnumber(L, returnCode);

			return 2;
		}

		returnCode = httpcGetResponseStatusCode(&httpContext, &statusCode, 0);

		if (returnCode != 0 || statusCode != 200) {
			lua_pushnil(L);

			lua_pushnumber(L, returnCode);

			return 2;
		}

		returnCode = httpcGetDownloadSizeState(&httpContext, NULL, &contentSize);

		if (returnCode != 0) {
			lua_pushnil(L);

			lua_pushnumber(L, returnCode);

			return 2;
		}

		printf("Download size is %dB\n", contentSize);

		buffer = (u8 *)malloc(contentSize);

		if (!buffer) {
			lua_pushnil(L);

			lua_pushnumber(L, returnCode);

			return 2;
		}

		memset(buffer, 0, contentSize);

		printf("Downloading..\n");

		returnCode = httpcDownloadData(&httpContext, buffer, contentSize, &size);
		
		if (returnCode != 0) {
			free(buffer);

			lua_pushnil(L);

			lua_pushnumber(L, returnCode);

			return 2;
		}

		printf("Downloaded: %dB/%dB!\n", size, contentSize);

		lua_pushlstring(L, buffer, contentSize);

		lua_pushnumber(L, statusCode);

		httpcCloseContext(&httpContext);

		return 2;
	}
		
	httpcCloseContext(&httpContext);

	lua_pushnil(L);

	lua_pushstring(L, "Failed to get a response");
		
	return 2;
}