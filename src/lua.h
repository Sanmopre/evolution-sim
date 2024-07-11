// ======== lua.h ==========================================================================================
//
//                                     Application
//
// =========================================================================================================
#pragma once

#include "config.h"

#define LUA_ENTRY_POINT "lua/main.lua"

extern "C" 
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}


void testLuaCpp() 
{
	try 
    {
		spdlog::info("Testing Lua C++ integration");
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);

        int result = luaL_dofile(L, LUA_ENTRY_POINT);

        if (result != LUA_OK) 
        {
            spdlog::error("Error running script: {}", lua_tostring(L, -1));
        }
	}
	catch (std::runtime_error& e)
  	{
		spdlog::error("Exception caught in testLuaCpp: {}", e.what());
  	}
}