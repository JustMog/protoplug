#include "LuaState.h"

namespace protolua
{

DynamicLibrary *	LuaState::dll = 0;
ptr_luaL_newstate	LuaState::luaL_newstate			= 0;
ptr_luaL_openlibs	LuaState::luaL_openlibs			= 0;
ptr_luaL_loadbuffer	LuaState::luaL_loadbuffer		= 0;
ptr_lua_tolstring	LuaState::lua_tolstring			= 0;
ptr_lua_tonumber	LuaState::lua_tonumber			= 0;
ptr_lua_toboolean	LuaState::lua_toboolean			= 0;
ptr_lua_pushcclosure	LuaState::lua_pushcclosure	= 0;
ptr_lua_close		LuaState::lua_close				= 0;
ptr_lua_gettop		LuaState::lua_gettop			= 0;
ptr_lua_settop		LuaState::lua_settop			= 0;
ptr_lua_pcall		LuaState::lua_pcall				= 0;
ptr_lua_getfield	LuaState::lua_getfield			= 0;
ptr_lua_pushlightuserdata	LuaState::lua_pushlightuserdata	= 0;
ptr_lua_pushstring	LuaState::lua_pushstring		= 0;
ptr_lua_pushnumber	LuaState::lua_pushnumber		= 0;
ptr_lua_pushboolean	LuaState::lua_pushboolean		= 0;
ptr_lua_type		LuaState::lua_type				= 0;
ptr_lua_setfield	LuaState::lua_setfield			= 0;
ptr_lua_isstring	LuaState::lua_isstring			= 0;
ptr_lua_isnumber	LuaState::lua_isnumber			= 0;
ptr_lua_typename	LuaState::lua_typename			= 0;
ptr_lua_newuserdata	LuaState::lua_newuserdata		= 0;

LuaState::LuaState()
{
	l = 0;
#if JUCE_WINDOWS
	String libName = "lua51.dll";
#else
	String libName = "libluajit-5.1.so";
#endif
	String defaultPath = getProtoplugDir().getChildFile("lib/"+libName).getFullPathName();
	if (!dll) {
		dll = new DynamicLibrary();
		if (!dll->open(defaultPath))
			dll->open(libName);
		// why
		luaL_newstate		= ptr_luaL_newstate		(dll->getFunction("luaL_newstate"));
		luaL_openlibs		= ptr_luaL_openlibs		(dll->getFunction("luaL_openlibs"));
		luaL_loadbuffer		= ptr_luaL_loadbuffer	(dll->getFunction("luaL_loadbuffer"));
		lua_tolstring		= ptr_lua_tolstring		(dll->getFunction("lua_tolstring"));
		lua_tonumber		= ptr_lua_tonumber		(dll->getFunction("lua_tonumber"));
		lua_toboolean		= ptr_lua_toboolean		(dll->getFunction("lua_toboolean"));
		lua_pushcclosure	= ptr_lua_pushcclosure	(dll->getFunction("lua_pushcclosure"));
		lua_close			= ptr_lua_close			(dll->getFunction("lua_close"));
		lua_gettop			= ptr_lua_gettop		(dll->getFunction("lua_gettop"));
		lua_settop			= ptr_lua_settop		(dll->getFunction("lua_settop"));
		lua_pcall			= ptr_lua_pcall			(dll->getFunction("lua_pcall"));
		lua_getfield		= ptr_lua_getfield		(dll->getFunction("lua_getfield"));
		lua_pushlightuserdata = ptr_lua_pushlightuserdata	(dll->getFunction("lua_pushlightuserdata"));
		lua_pushstring		= ptr_lua_pushstring	(dll->getFunction("lua_pushstring"));
		lua_pushnumber		= ptr_lua_pushnumber	(dll->getFunction("lua_pushnumber"));
		lua_pushboolean		= ptr_lua_pushboolean	(dll->getFunction("lua_pushboolean"));
		lua_type			= ptr_lua_type			(dll->getFunction("lua_type"));
		lua_setfield		= ptr_lua_setfield		(dll->getFunction("lua_setfield"));
		lua_isstring		= ptr_lua_isstring		(dll->getFunction("lua_isstring"));
		lua_isnumber		= ptr_lua_isnumber		(dll->getFunction("lua_isnumber"));
		lua_typename		= ptr_lua_typename		(dll->getFunction("lua_typename"));
		lua_newuserdata		= ptr_lua_newuserdata	(dll->getFunction("lua_newuserdata"));
	}
	void *kk[22] = {
		// cast required for some compilers
        (void*)luaL_newstate,
        (void*)luaL_openlibs,
        (void*)luaL_loadbuffer,
        (void*)lua_tolstring,
        (void*)lua_tonumber,
        (void*)lua_toboolean,
        (void*)lua_pushcclosure,
        (void*)lua_close,
        (void*)lua_gettop,
        (void*)lua_settop,
        (void*)lua_pcall,
        (void*)lua_getfield,
        (void*)lua_pushlightuserdata,
        (void*)lua_pushstring,
        (void*)lua_pushnumber,
        (void*)lua_pushboolean,
        (void*)lua_type,
        (void*)lua_setfield,
        (void*)lua_isstring,
        (void*)lua_isnumber,
        (void*)lua_typename,
        (void*)lua_newuserdata};
	for (int i=0; i<22; i++)
		if (kk[i]==0) {
			failed = 1;
			errmsg = 	"Could not load " + libName + 
						". Tried " + defaultPath + " and system path.";
			return;
		}
	failed = 0;
	l = (*luaL_newstate)();
}

LuaState::~LuaState() {
	if (l) // so, does your font suck ?
		close();
}

void LuaState::openlibs()
{ (*luaL_openlibs)(l);}

int LuaState::loadbuffer(const char *buff, size_t sz, const char *name)
{ return (*luaL_loadbuffer)(l, buff, sz, name);}

const char * LuaState::tolstring(int idx, size_t *len)
{ return (*lua_tolstring)(l, idx, len);}

lua_Number LuaState::tonumber(int idx)
{ return (*lua_tonumber)(l, idx);}

int LuaState::toboolean(int idx)
{ return (*lua_toboolean)(l, idx);}

void LuaState::pushcclosure(lua_CFunction fn, int n)
{ (*lua_pushcclosure)(l, fn, n);}

void LuaState::close()
{ (*lua_close)(l);}

int LuaState::gettop()
{ return (*lua_gettop)(l);}

void LuaState::settop(int idx)
{ (*lua_settop)(l, idx);}

int LuaState::pcall(int nargs, int nresults, int errfunc)
{ return (*lua_pcall)(l, nargs, nresults, errfunc);}

void LuaState::getfield(int idx, const char *k)
{ (*lua_getfield)(l, idx, k);}

void LuaState::pushlightuserdata(void *p)
{ (*lua_pushlightuserdata)(l, p);}

void LuaState::pushstring(const char *s)
{ (*lua_pushstring)(l, s);}

void LuaState::pushnumber(lua_Number n)
{ (*lua_pushnumber)(l, n);}

void LuaState::pushboolean(int b)
{ (*lua_pushboolean)(l, b);}

int LuaState::type(int idx)
{ return (*lua_type)(l, idx);}

void LuaState::setfield(int idx, const char *k)
{ (*lua_setfield)(l, idx, k);}

int LuaState::isstring(int idx)
{ return (*lua_isstring)(l, idx);}

int LuaState::isnumber(int idx)
{ return (*lua_isnumber)(l, idx);}

const char * LuaState::ltypename(int t)
{ return (*lua_typename)(l, t);}

void * LuaState::newuserdata(size_t sz)
{ return (*lua_newuserdata)(l, sz);}


} // namespace protolua