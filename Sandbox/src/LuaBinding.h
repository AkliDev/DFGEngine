#pragma once
#include <rttr/registration>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace DFGEngine
{
	static bool CheckLua(lua_State* L, uint32_t r)
	{
		if (r != LUA_OK)
		{
			std::string error = lua_tostring(L, -1);
			DFG_ERROR(error);
			return false;
		}
		return true;
	}

	//static int InvokeMehtod(lua_State* L, rttr::method& methodToInvoke, rttr::instance& object)
	//{
	//	rttr::array_range<rttr::parameter_info> nativeParams = methodToInvoke.get_parameter_infos();
	//	uint32_t luaParamsStackOffset = 0;
	//	uint32_t numNativeArgs = nativeParams.size();
	//	uint32_t numLuaArgs = lua_gettop(L);
	//	if (numLuaArgs > numNativeArgs)
	//	{
	//		luaParamsStackOffset = numLuaArgs - numNativeArgs;
	//		numLuaArgs = numNativeArgs;
	//	}
	//
	//	if (numLuaArgs != numNativeArgs)
	//	{
	//		DFG_ERROR("Unable to invoke {0}, Incorrect argument count expected {1} got {2}",
	//			methodToInvoke.get_name().to_string().c_str(), numNativeArgs, numLuaArgs);
	//	}
	//
	//	union PassByValue
	//	{
	//		int intVal;
	//		short shortVal;
	//	};
	//
	//
	//	std::vector<PassByValue> pbv(numNativeArgs);
	//	std::vector<rttr::argument> nativeArgs(numNativeArgs);
	//	auto nativeParamsIt = nativeParams.begin();
	//
	//	for (uint32_t i = 0; i < numNativeArgs; i++, nativeParamsIt++)
	//	{
	//		const rttr::type nativeParamType = nativeParamsIt->get_type();
	//		uint32_t luaArgIndex = i + 1 + luaParamsStackOffset;
	//		uint32_t luaType = lua_type(L, luaArgIndex);
	//
	//		switch (luaType)
	//		{
	//		case LUA_TNIL:				break;
	//		case LUA_TBOOLEAN:			break;
	//		case LUA_TLIGHTUSERDATA:	break;
	//		case LUA_TNUMBER:
	//			if (nativeParamType == rttr::type::get<int>())
	//			{
	//				pbv[i].intVal = (int)lua_tonumber(L, luaArgIndex);
	//				nativeArgs[i] = pbv[i].intVal;
	//			}
	//			else if (nativeParamType == rttr::type::get<short>())
	//			{
	//				pbv[i].intVal = (short)lua_tonumber(L, luaArgIndex);
	//				nativeArgs[i] = pbv[i].shortVal;
	//			}
	//			else
	//			{
	//				DFG_ERROR("Incorrect type passed {0}", nativeParamType.get_name().to_string().c_str());
	//			}
	//			break;
	//		case LUA_TSTRING:			break;
	//		case LUA_TTABLE:			break;
	//		case LUA_TFUNCTION:			break;
	//		case LUA_TUSERDATA:			break;
	//		case LUA_TTHREAD:			break;
	//
	//		default: DFG_ERROR("Unknown lua type passed {0}, Parameter {1}, when calling", lua_typename(L, luaType), i, methodToInvoke.get_name().to_string().c_str()); break;
	//		}
	//	}
	//
	//	rttr::variant result = methodToInvoke.invoke_variadic(object, nativeArgs);
	//	uint32_t numberOfReturnValues = 0;
	//
	//	if (result.is_valid() == false)
	//	{
	//		DFG_ERROR("Unable to invoke {0}", methodToInvoke.get_name().to_string().c_str());
	//	}
	//	else if (result.is_type<void>() == false)
	//	{
	//		if (result.is_type<int32_t >())
	//		{
	//			lua_pushnumber(L, result.get_value<int32_t>());
	//			numberOfReturnValues++;
	//		}
	//		else
	//		{
	//			DFG_ERROR("Unhandeld return type from native method {0}", methodToInvoke.get_name().to_string().c_str());
	//		}
	//	}
	//
	//	return numberOfReturnValues;
	//}
	//
	//static int CallGlobalFromLua(lua_State* L)
	//{
	//	rttr::method* method = (rttr::method*)lua_touserdata(L, lua_upvalueindex(1));
	//	rttr::method& methodToInvoke(*method);
	//	rttr::instance object = {};
	//	return	InvokeMehtod(L, methodToInvoke, object);
	//}
	//
	//static std::string MetaTableName(const rttr::type& type)
	//{
	//	std::string metaTableName = type.get_name().to_string();
	//	metaTableName.append("_MT_");
	//
	//	return metaTableName;
	//}
	//
	//static int CreateUserData(lua_State* L)
	//{
	//	const char* typeName = lua_tostring(L, lua_upvalueindex(1));
	//	rttr::type type = rttr::type::get_by_name(typeName);
	//
	//	void* ud = lua_newuserdata(L, sizeof(rttr::variant));
	//	new (ud)rttr::variant(type.create());
	//
	//	luaL_getmetatable(L, MetaTableName(type).c_str());
	//	lua_setmetatable(L, 1);
	//
	//	lua_newtable(L);
	//	lua_setuservalue(L, 1);
	//
	//	return 1;
	//}
	//
	//static int DestroyUserData(lua_State* L)
	//{
	//	rttr::variant* ud  = (rttr::variant*)lua_touserdata(L, -1);
	//	ud->~variant();
	//	return 0;
	//}
	//
	//static int InvokeFuncOnUserData(lua_State* L)
	//{
	//	rttr::method& method = *(rttr::method*)lua_touserdata(L, lua_upvalueindex(1));
	//	if (lua_isuserdata(L, 1) == false)
	//	{
	//		DFG_ERROR("Expected a userdata on the lua stack when invoking native method {0}", method.get_name().to_string());
	//	}
	//
	//	rttr::variant& ud = *(rttr::variant*)lua_touserdata(L, 1);
	//	rttr::instance object(ud);
	//	return InvokeMehtod(L, method, object);
	//}
	//
	//static int IndexUserData(lua_State* L)
	//{
	//	const char* typeName = (const char*)lua_tostring(L, lua_upvalueindex(1));
	//	rttr::type typeInfo = rttr::type::get_by_name(typeName);
	//	
	//	if (lua_isuserdata(L, 1) == false)
	//	{
	//		DFG_ERROR("Expected a userdata on the lua stack when indexing native type: {0}", typeName);
	//	}
	//	if (lua_isstring(L, -1) == false)
	//	{
	//		DFG_ERROR("Expected a name of a native propertty or method when indexing native type: {0}", typeName);
	//	}
	//	
	//	//rttr::variant& ud = *(rttr::variant*)lua_touserdata(L, -1);
	//	const char* fieldName = lua_tostring(L, -1);
	//	rttr::method method = typeInfo.get_method(fieldName);
	//	if (method.is_valid())
	//	{
	//		void* methodUD = lua_newuserdata(L, sizeof(rttr::method));
	//		new (methodUD) rttr::method(method);
	//		lua_pushcclosure(L, InvokeFuncOnUserData,1);
	//		return 1;
	//	}
	//
	//	return 0;
	//}

	struct Sprite
	{
		int32_t m_X, m_Y;
		std::string m_SpriteName;

		Sprite(std::string& spriteName) : m_X(0), m_Y(0) , m_SpriteName(spriteName){};
		~Sprite(){ DFG_TRACE("Sprite is destroyed"); }

		void SetPosition(int32_t x, int32_t y)
		{
			m_X = x;
			m_Y = y;
		}

		void Draw()
		{
			Renderer2D::DrawQuad({ m_X,m_Y }, { 1,1 }, Renderer2D::s_TextureLibrary.Get(m_SpriteName));
		}

		static int CreateSprite(lua_State* L)
		{
			assert(lua_isstring(L, -1));
			const char* name = lua_tostring(L, -1);
			std::string s = name;

			void* pointer = lua_newuserdata(L, sizeof(Sprite));		
			new (pointer) Sprite(s);

			luaL_getmetatable(L, "SpriteMetaTable");
			lua_setmetatable(L, -2);

			lua_newtable(L);
			lua_setuservalue(L,-2);
			return 1;
		}

		static int DestroySprite(lua_State* L)
		{
			Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
			sprite->~Sprite();
			return 0;
		}

		static int SetSpritePosition(lua_State* L)
		{
			Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
			lua_Number x = lua_tonumber(L, -2);
			lua_Number y = lua_tonumber(L, -1);
			sprite->SetPosition(x, y);
			return 0;
		}

		static int DrawSprite(lua_State* L)
		{
			Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
			sprite->Draw();
			return 0;
		}

		static int SpriteIndex(lua_State* L)
		{
			Sprite* sprite = (Sprite*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "x") == 0)
			{
				lua_pushnumber(L, sprite->m_X);
				return 1;
			}
			else if (strcmp(index, "y") == 0)
			{
				lua_pushnumber(L, sprite->m_Y);
				return 1;
			}
			else
			{
				lua_getuservalue(L, -2);
				lua_getfield(L, -1, index);
				if (lua_isnil(L, -1) == true)
				{
					lua_getglobal(L, "Sprite");
					lua_getfield(L, -1, index);
					if (lua_isnil(L, -1) == true)
					{
						return 0;
					}
				}
				return 1;
			}
		}

		static int SpriteNewIndex(lua_State* L)
		{
			Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "x") == 0)
			{
				sprite->m_X = lua_tonumber(L, -1);
				return 1;
			}
			else if (strcmp(index, "y") == 0)
			{
				sprite->m_Y = lua_tonumber(L, -1);
				return 1;
			}
			else
			{
				lua_getuservalue(L, -3);
				lua_pushvalue(L, -2);
				lua_setfield(L, -2, index);
				return 1;
			}
		}
	};

	static void LuaBinding()
	{
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		//lua_newtable(L);
		//lua_pushvalue(L, -1);
		//lua_setglobal(L, "Global");
		//
		//lua_pushvalue(L, -1);
		//for (auto& method : rttr::type::get_global_methods())
		//{
		//	lua_pushstring(L, method.get_name().to_string().c_str());
		//	lua_pushlightuserdata(L, (void*)&method);
		//	lua_pushcclosure(L, CallGlobalFromLua, 1);
		//	lua_settable(L, -3);
		//}
		//
		//for (auto& type : rttr::type::get_types())
		//{
		//	if (type.is_class())
		//	{
		//		const std::string s = type.get_name().to_string();
		//		const char* typeName = s.c_str();
		//		lua_newtable(L);
		//		lua_pushvalue(L, -1);
		//		lua_setglobal(L, typeName);
		//
		//		lua_pushvalue(L, -1);
		//		lua_pushstring(L, typeName);
		//		lua_pushcclosure(L, CreateUserData, 1);
		//		lua_setfield(L, -2, "new");
		//
		//		luaL_newmetatable(L, MetaTableName(type).c_str());
		//
		//		lua_pushstring(L, "__gc");
		//		lua_pushcfunction(L, DestroyUserData);
		//		lua_settable(L, -3);
		//		
		//		lua_pushstring(L, "__index");
		//		lua_pushstring(L, typeName);
		//		lua_pushcclosure(L, IndexUserData, 1);
		//
		//		lua_settable(L, -3);
		//	}
		//}

		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, "Sprite");

		lua_pushcfunction(L, Sprite::CreateSprite);
		lua_setfield(L, -2, "new");
		lua_pushcfunction(L, Sprite::SetSpritePosition);
		lua_setfield(L, -2, "SetPosition");
		lua_pushcfunction(L, Sprite::DrawSprite);
		lua_setfield(L, -2, "Draw");

		luaL_newmetatable(L, "SpriteMetaTable");
		lua_pushcfunction(L, Sprite::DestroySprite);
		lua_setfield(L, -2, "__gc");
		lua_pushcfunction(L, Sprite::SpriteIndex);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, Sprite::SpriteNewIndex);
		lua_setfield(L, -2, "__newindex");

		
		if (CheckLua(L, luaL_dofile(L, "assets/scripts/test.lua")))
		{

		}

		lua_close(L);
	}
}