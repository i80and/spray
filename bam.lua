function setup()
	settings = NewSettings()
	settings.cc.includes:Add("libs/SDL/local/include/SDL")
	settings.link.libpath:Add("libs/SDL/local/lib")
	settings.link.libs:Add("SDL", "GL", "openal", "png")
	settings.link.flags:Add("-Wl,-rpath,libs/SDL/local/lib")
	
	-- For now, force all debugging stuff on
	settings.optimize = 0
	settings.debug = 1
	settings.cc.flags:Add("-Wall -Wextra")
	settings.cc.flags:Add("-ftrapv")
	
	if ScriptArgs["CC"] then
		settings.cc.exe_c = ScriptArgs["CC"]
		-- Causes problems with tcc
		settings.link.exe = ScriptArgs["CC"]
	end

	-- Compiler-specific features and support	
	if settings.cc.exe_c == 'gcc' then
		settings.cc.flags:Add("-std=c99")
	elseif settings.cc.exe_c == 'clang' then
		settings.cc.flags:Add("-fcatch-undefined-behavior")
	end

	objs = Compile(settings, Collect("src/*.c"))
	exe = Link(settings, "spray", objs)
end

setup()
