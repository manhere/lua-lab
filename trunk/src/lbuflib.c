/* Buffer library for mutable string (for lua-lab) */

#define LUA_LIB

#include <stdlib.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lmem.h"

#define INIT_BUFFER_SIZE 16

typedef struct
{
	unsigned char* data;
	lua_Integer len;
	lua_Integer cap;
}lua_Buffer;

static void convert_pos(lua_Integer n, lua_Integer* pi, lua_Integer* pj)
{
	lua_Integer i = *pi - 1;
	lua_Integer j = *pj;
	if(i < 0) { i = n + i + 1;      if(i < 0) i = 0; }
	if(j < 0)   j = n + j + 1; else if(j > n) j = n;
	if((j -= i) < 0) j = 0;
	*pi = i;
	*pj = j;
}

/* buffer = buffer.new([string|buffer, [i = 1, [j = -1]]]) */
static int buf_new(lua_State* L)
{
	lua_Buffer* buffer;
	lua_Buffer srcbuf;
	lua_Integer i, j, n;
	if(lua_isnil(L, 1))
	{
new_:	buffer = (lua_Buffer*)lua_newuserdata(L, sizeof(lua_Buffer));
		buffer->data = luaM_malloc(L, INIT_BUFFER_SIZE);
		buffer->len = 0;
		buffer->cap = INIT_BUFFER_SIZE;
		luaL_getmetatable(L, LUA_BUFFERPOINTER);
		lua_setmetatable(L, -2);
		return 1;
	}
	buffer = luaL_testudata(L, 1, LUA_BUFFERPOINTER);
	if(buffer)
	{
		srcbuf.data = buffer->data;
		srcbuf.len = buffer->len;
	}
	else
	{
		size_t srclen;
		srcbuf.data = (unsigned char*)lua_tolstring(L, 1, &srclen);
		if(!srcbuf.data) goto new_;
		srcbuf.len = (lua_Integer)srclen;
	}
	buffer = (lua_Buffer*)lua_newuserdata(L, sizeof(lua_Buffer));
	i = luaL_optinteger(L, 2, 1);
	j = luaL_optinteger(L, 3, -1);
	convert_pos(srcbuf.len, &i, &j);
	for(n = INIT_BUFFER_SIZE; n < j;) n *= 2;
	buffer->data = luaM_malloc(L, n);
	buffer->len = j;
	buffer->cap = n;
	memcpy(buffer->data, srcbuf.data + i, j);
	luaL_getmetatable(L, LUA_BUFFERPOINTER);
	lua_setmetatable(L, -2);
	return 1;
}

/* buffer:lower() */
static int buf_lower(lua_State* L)
{
	lua_Integer i;
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	for(i = 0; i < buffer->len; ++i)
		buffer->data[i] = (unsigned char)tolower(buffer->data[i]);
	return 0;
}

/* buffer:upper() */
static int buf_upper(lua_State* L)
{
	lua_Integer i;
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	for(i = 0; i < buffer->len; ++i)
		buffer->data[i] = (unsigned char)toupper(buffer->data[i]);
	return 0;
}

/* buffer:reserve(n) */
static int buf_reserve(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Integer cap = luaL_checkinteger(L, 2);
	if(cap >= (lua_Integer)buffer->len && cap != (lua_Integer)buffer->cap)
	{
		buffer->data = luaM_realloc_(L, buffer->data, buffer->cap, cap);
		buffer->cap = cap;
	}
	return 0;
}

/* buffer:resize(n) */
static int buf_resize(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Integer len = luaL_checkinteger(L, 2);
	if(len < 0) len = 0;
	if(len > buffer->cap)
	{
		lua_Integer cap = buffer->cap;
		do cap *= 2; while(cap < len);
		buffer->data = luaM_realloc_(L, buffer->data, buffer->cap, cap);
		buffer->cap = cap;
	}
	buffer->len = len;
	return 0;
}

/* string = buffer:sub(i, [j = -1]) */
static int buf_sub(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Integer i = luaL_checkinteger(L, 2);
	lua_Integer j = luaL_optinteger(L, 3, -1);
	convert_pos(buffer->len, &i, &j);
	lua_pushlstring(L, (const char*)buffer->data + i, j);
	return 1;
}

/* buffer:fill([i_dst = 1, [j_dst = -1]], buffer|string) */
static int buf_fill(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Integer dsti, dstj;
	int i;
	lua_Buffer* srcbuf;
	const void* src;
	lua_Integer srclen;
	if(lua_isnumber(L, 2))
	{
		dsti = lua_tointeger(L, 2);
		if(lua_isnumber(L, 3)) { dstj = lua_tointeger(L, 3); i = 4; }
		else				   { dstj = -1;					 i = 3;	}
	}	else		 { dsti = 1; dstj = -1;					 i = 2; }
	convert_pos(buffer->len, &dsti, &dstj);
	srcbuf = luaL_testudata(L, i, LUA_BUFFERPOINTER);
	if(srcbuf)
	{
		src = srcbuf->data;
		srclen = srcbuf->len;
	}
	else
	{
		size_t len;
		src = luaL_checklstring(L, i, &len);
		srclen = (lua_Integer)len;
	}
	while(dstj > 0)
	{
		size_t len = (dstj < srclen ? dstj : srclen);
		memcpy(buffer->data + dsti, src, len);
		dsti += len;
		dstj -= len;
	}
	return 0;
}

/* buffer:copy([i_src = 1], buffer|string, [i_src = 1, [j_src = -1]]) */
static int buf_copy(lua_State* L)
{
	return 0;
}

/* v|func = buffer:__index(i|funcname) */
static int buf_index(lua_State* L)
{
	return 0;
}

/* buffer:__index(i, v) */
static int buf_newindex(lua_State* L)
{
	return 0;
}

/* buffer = buffer:__concat(buffer|string) */
static int buf_concat(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Buffer* srcbuf = luaL_testudata(L, 2, LUA_BUFFERPOINTER);
	const void* src;
	lua_Integer srclen, alllen;
	if(srcbuf)
	{
		src = srcbuf->data;
		srclen = srcbuf->len;
	}
	else
	{
		size_t len;
		src = luaL_checklstring(L, 2, &len);
		srclen = (lua_Integer)len;
	}
	alllen = buffer->len + srclen;
	if(alllen > buffer->cap)
	{
		lua_Integer cap = buffer->cap;
		do cap *= 2; while(cap < alllen);
		buffer->data = luaM_realloc_(L, buffer->data, buffer->cap, cap);
		buffer->cap = cap;
	}
	memcpy(buffer->data + buffer->len, src, srclen);
	buffer->len = alllen;
	lua_pushvalue(L, 1);
	return 1;
}

/* n = buffer:__len() */
static int buf_len(lua_State* L)
{
	lua_pushinteger(L, ((lua_Buffer*)luaL_checkudata(L, 1, LUA_BUFFERPOINTER))->len);
	return 1;
}

/* b = buffer:__eq(buffer|string) */
static int buf_eq(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Buffer* srcbuf = luaL_testudata(L, 2, LUA_BUFFERPOINTER);
	const void* src;
	lua_Integer srclen;
	if(srcbuf)
	{
		src = srcbuf->data;
		srclen = srcbuf->len;
	}
	else
	{
		size_t len;
		src = luaL_checklstring(L, 2, &len);
		srclen = (lua_Integer)len;
	}
	lua_pushboolean(L, buffer->len == srclen && !memcmp(buffer->data, src, srclen));
	return 1;
}

/* b = buffer:__lt(buffer|string) */
static int buf_lt(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Buffer* srcbuf = luaL_testudata(L, 2, LUA_BUFFERPOINTER);
	const void* src;
	lua_Integer srclen;
	if(srcbuf)
	{
		src = srcbuf->data;
		srclen = srcbuf->len;
	}
	else
	{
		size_t len;
		src = luaL_checklstring(L, 2, &len);
		srclen = (lua_Integer)len;
	}
	lua_pushboolean(L, memcmp(buffer->data, src, (buffer->len < srclen ? buffer->len : srclen)) < 0 || buffer->len < srclen);
	return 1;
}

/* b = buffer:__le(buffer|string) */
static int buf_le(lua_State* L)
{
	lua_Buffer* buffer = luaL_checkudata(L, 1, LUA_BUFFERPOINTER);
	lua_Buffer* srcbuf = luaL_testudata(L, 2, LUA_BUFFERPOINTER);
	const void* src;
	lua_Integer srclen;
	if(srcbuf)
	{
		src = srcbuf->data;
		srclen = srcbuf->len;
	}
	else
	{
		size_t len;
		src = luaL_checklstring(L, 2, &len);
		srclen = (lua_Integer)len;
	}
	lua_pushboolean(L, memcmp(buffer->data, src, (buffer->len < srclen ? buffer->len : srclen)) < 0 || buffer->len <= srclen);
	return 1;
}

/* buffer:__gc() */
static int buf_gc(lua_State* L)
{
	luaM_free(L, ((lua_Buffer*)luaL_checkudata(L, 1, LUA_BUFFERPOINTER))->data);
	return 0;
}

static const luaL_Reg buflib[] =
{
	{"new", buf_new},
	{"lower", buf_lower},
	{"upper", buf_upper},
	{"reserve", buf_reserve},
	{"resize", buf_resize},
	{"sub", buf_sub},
	{"fill", buf_fill},
	{"copy", buf_copy},
	{NULL, NULL}
};

static const luaL_Reg bufmeta[] =
{
	{"lower", buf_lower},
	{"upper", buf_upper},
	{"reserve", buf_reserve},
	{"resize", buf_resize},
	{"sub", buf_sub},
	{"fill", buf_fill},
	{"copy", buf_copy},
	{"__index", buf_index},
	{"__newindex", buf_newindex},
	{"__concat", buf_concat},
	{"__len", buf_len},
	{"__eq", buf_eq},
	{"__lt", buf_lt},
	{"__le", buf_le},
	{"__gc", buf_gc},
	{NULL, NULL}
};

LUAMOD_API int luaopen_buffer(lua_State* L)
{
	luaL_register(L, LUA_BUFLIBNAME, buflib);
	luaL_newmetatable(L, LUA_BUFFERPOINTER);
	luaL_register(L, NULL, bufmeta);
	lua_pop(L, 1);
	return 1;
}
