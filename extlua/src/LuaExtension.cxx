// SciTE - Scintilla based Text Editor
// LuaExtension.cxx - Lua scripting extension
// Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>

#include "Scintilla.h"
#include "Accessor.h"
#include "Extender.h"
#include "LuaExtension.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static ExtensionAPI *host = 0;
static int tagScintilla = 0;

LuaExtension::LuaExtension() {
	tagScintilla = 0;
}

LuaExtension::~LuaExtension() {}


bool LuaExtension::Initialise(ExtensionAPI *host_) {
	host = host_;
	lua_open();
	lua_iolibopen();
	lua_strlibopen();
	lua_beginblock();
	return true;
}

bool LuaExtension::Finalise() {
	lua_endblock();
	lua_close();
	return true;
}

bool LuaExtension::Clear() {
	lua_endblock();
	lua_beginblock();
	return true;
}

bool LuaExtension::Load(const char *filename) {
	//lua_dofile("C:\\os\\scite\\vcbuild\\Debug\\SciTEStart.lua");
	lua_dofile(const_cast<char *>(filename));
	return true;
}

static ExtensionAPI::Pane CheckForPane(int &arg) {
	lua_Object self = lua_getparam(arg);
	ExtensionAPI::Pane p = ExtensionAPI::paneEditor;
	if (lua_istable(self)) {
		arg++;
		lua_pushobject(self);
		lua_pushstring("self");
		lua_Object pself = lua_gettable();
		if (lua_isuserdata(pself)) {
			void *pdest = lua_getuserdata(pself);
			if (pdest != reinterpret_cast<void *>(ExtensionAPI::paneEditor))
				p = ExtensionAPI::paneOutput;
		}
	}
	return p;
}

static int UntypedArg(int arg) {
	lua_Object lw = lua_getparam(arg);
	if (lua_isnumber(lw))
		return static_cast<int>(lua_getnumber(lw));
	else if (lua_isstring(lw))
		return reinterpret_cast<int>(lua_getstring(lw));
	else
		return 0;
}

static void ScriptSendMessage() {
	int arg = 1;
	ExtensionAPI::Pane p = CheckForPane(arg);
	int m = static_cast<int>(luaL_check_number(arg));
	int w = UntypedArg(arg + 1);
	int l = UntypedArg(arg + 2);
	int result = host->Send(p, m, w, l);
	lua_pushnumber(result);
}

static void ScriptSelectionRange() {
	int arg = 1;
	ExtensionAPI::Pane p = CheckForPane(arg);
	lua_pushnumber(host->Send(p, SCI_GETSELECTIONSTART));
	lua_pushnumber(host->Send(p, SCI_GETSELECTIONEND));
}

static void ScriptGetRange() {
	int arg = 1;
	ExtensionAPI::Pane p = CheckForPane(arg);
	int cpMin = static_cast<int>(luaL_check_number(arg));
	int cpMax = static_cast<int>(luaL_check_number(arg + 1));
	char *range = host->Range(p, cpMin, cpMax);
	if (range) {
		lua_pushstring(range);
		delete []range;
	} else {
		lua_pushstring("");
	}
}

static void ScriptRemoveRange() {
	int arg = 1;
	ExtensionAPI::Pane p = CheckForPane(arg);
	int cpMin = static_cast<int>(luaL_check_number(arg));
	int cpMax = static_cast<int>(luaL_check_number(arg + 1));
	host->Remove(p, cpMin, cpMax);
}

static void ScriptInsert() {
	int arg = 1;
	ExtensionAPI::Pane p = CheckForPane(arg);
	int pos = static_cast<int>(luaL_check_number(arg));
	char *s = luaL_check_string(arg + 1);
	host->Insert(p, pos, s);
}

static void ScriptTrace() {
	host->Trace(luaL_check_string(1));
}

static void ScriptGetProperty() {
	char *value = host->Property(luaL_check_string(1));
	if (value) {
		lua_pushstring(value);
		delete []value;
	} else {
		lua_pushstring("");
	}
}

static Accessor *GetAccessor() {
	Accessor *a = 0;
	lua_Object self = lua_getparam(1);
	if (lua_istable(self)) {
		lua_pushobject(self);
		lua_pushstring("self");
		lua_Object pself = lua_gettable();
		if (lua_isuserdata(pself)) {
			a = reinterpret_cast<Accessor *>(lua_getuserdata(pself));
		}
	}
	return a;
}

static void ScriptStartAt() {
	Accessor *a = GetAccessor();
	if (a) {
		int start = static_cast<int>(luaL_check_number(2));
		int mask = static_cast<int>(luaL_check_number(3));
		a->StartAt(start, static_cast<char>(mask));
	}
}

static void ScriptStartSegment() {
	Accessor *a = GetAccessor();
	if (a) {
		int pos = static_cast<int>(luaL_check_number(2));
		a->StartSegment(pos);
	}
}

static void ScriptColourTo() {
	Accessor *a = GetAccessor();
	if (a) {
		int pos = static_cast<int>(luaL_check_number(2));
		int attr = static_cast<int>(luaL_check_number(3));
		a->ColourTo(pos, attr);
	}
}

static void ScriptRGB() {
	int r = static_cast<int>(luaL_check_number(1));
	int g = static_cast<int>(luaL_check_number(2));
	int b = static_cast<int>(luaL_check_number(3));
	lua_pushnumber(r + g * 256 + b * 65536);
}

static void AddTableFunction(lua_Object table, char *name, lua_CFunction fn) {
	lua_pushobject(table);
	lua_pushstring(name);
	lua_pushcfunction(fn);
	lua_settable();
}

static void AddPaneFunctions(lua_Object table) {
	AddTableFunction(table, "send", ScriptSendMessage);
	AddTableFunction(table, "selection", ScriptSelectionRange);
	AddTableFunction(table, "range", ScriptGetRange);
	AddTableFunction(table, "remove", ScriptRemoveRange);
	AddTableFunction(table, "insert", ScriptInsert);
}

static void ScriptStartBlock() {
	if (tagScintilla == 0)
		tagScintilla = lua_newtag();

	lua_beginblock();
	lua_register("trace", ScriptTrace);
	lua_register("property", ScriptGetProperty);
	lua_register("RGB", ScriptRGB);

	lua_Object editortable = lua_createtable();
	AddPaneFunctions(editortable);
	lua_pushobject(editortable);
	lua_pushstring("self");
	lua_pushusertag(reinterpret_cast<void *>(ExtensionAPI::paneEditor), tagScintilla);
	lua_settable();
	lua_pushobject(editortable);
	lua_setglobal("editor");

	lua_Object outputtable = lua_createtable();
	AddPaneFunctions(outputtable);
	lua_pushobject(outputtable);
	lua_pushstring("self");
	lua_pushusertag(reinterpret_cast<void *>(ExtensionAPI::paneOutput), tagScintilla);
	lua_settable();
	lua_pushobject(outputtable);
	lua_setglobal("output");

	lua_pushnumber(SCI_START);
	lua_setglobal("SCI_START");
}

static void ScriptEndBlock() {
	lua_endblock();
}

bool CallFn(lua_Object fn) {
	lua_callfunction(fn);
	lua_Object result = lua_getresult(1);
	return !!lua_getnumber(result);
}

static bool NoArgsFunction(char *name) {
	bool handled = false;
	ScriptStartBlock();
	lua_Object fn = lua_getglobal(name);
	if (fn && lua_isfunction(fn)) {
		handled = CallFn(fn);
	}
	ScriptEndBlock();
	return handled;
}

bool LuaExtension::OnOpen() {
	return NoArgsFunction("onOpen");
}

bool LuaExtension::OnChar(char ch) {
	bool handled = false;
	ScriptStartBlock();
	lua_Object fn = lua_getglobal("onChar");
	if (fn && lua_isfunction(fn)) {
		char chs[2] = {ch, '\0'};
		lua_pushstring(chs);
		handled = CallFn(fn);
	}
	ScriptEndBlock();
	return handled;
}

bool LuaExtension::OnExecute(const char *s) {
	bool handled = false;
	ScriptStartBlock();
	lua_Object fn = lua_getglobal("onExecute");
	if (fn && lua_isfunction(fn)) {
		lua_pushstring(const_cast<char *>(s));
		handled = CallFn(fn);
	}
	ScriptEndBlock();
	return handled;
}

bool LuaExtension::OnSavePointReached() {
	return NoArgsFunction("onSavePointReached");
}

bool LuaExtension::OnSavePointLeft() {
	return NoArgsFunction("onSavePointLeft");
}

bool LuaExtension::OnStyle(unsigned int startPos, int lengthDoc, int initStyle, Accessor *styler) {
	// Prepare styling in case script fails
	styler->StartAt(startPos, static_cast<char>(31));
	styler->StartSegment(startPos);

	bool handled = false;
	ScriptStartBlock();
	lua_Object fn = lua_getglobal("onStyle");
	if (fn && lua_isfunction(fn)) {
		lua_pushnumber(startPos);
		lua_pushnumber(lengthDoc);
		lua_pushnumber(initStyle);
		lua_Object stylertable = lua_createtable();
		AddTableFunction(stylertable, "startat", ScriptStartAt);
		AddTableFunction(stylertable, "startsegment", ScriptStartSegment);
		AddTableFunction(stylertable, "colourto", ScriptColourTo);
		lua_pushobject(stylertable);
		lua_pushstring("self");
		lua_pushusertag(reinterpret_cast<void *>(styler), tagScintilla);
		lua_settable();
		lua_pushobject(stylertable);
		handled = CallFn(fn);
	}
	if (!handled) {    // No function so style the whole buffer
		styler->ColourTo(startPos + lengthDoc - 1, 0);
	}
	ScriptEndBlock();
	return handled;
}

// These should probably have arguments

bool LuaExtension::OnDoubleClick() {
	return NoArgsFunction("onDoubleClick");
}

bool LuaExtension::OnUpdateUI() {
	return NoArgsFunction("onUpdateUI");
}

bool LuaExtension::OnMarginClick() {
	return NoArgsFunction("onMarginClick");
}

#ifdef _MSC_VER
// Unreferenced inline functions are OK
#pragma warning(disable: 4514)
#endif 
