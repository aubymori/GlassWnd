GlassWnd.exe: main.obj resource.res
	link /nologo /subsystem:windows /machine:x86 /entry:main /out:build\GlassWnd.exe \
	build\main.obj build\resource.res \
	kernel32.lib user32.lib gdi32.lib uxtheme.lib dwmapi.lib

main.obj: build
	cl /c /Fo"build\main.obj" /DUNICODE main.c

resource.res: build
	rc /fo build\resource.res resource.rc

build:
	md build

clean:
	rd /s /q build