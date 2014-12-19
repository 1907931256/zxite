python Translate.py clean
python Translate.py
cd ..
call scons.bat .
cd test
cl /Zi -EHsc -MD /I..\base /I..\lexers /IC:\cppunit\include SimpleTest.cxx /link /LIBPATH:..\base /LIBPATH:..\lexers /LIBPATH:C:\cppunit\lib base.lib lexers.lib cppunit.lib
SimpleTest
cd ..\tentacle\csharp
mingw32-make
cd ..\java
mingw32-make
cd ..\cxx
vcexpress Tentacle.sln /rebuild Release
cd ..\python
bjam -sBUILD=release test
copy bin\boost\libs\python\build\boost_python.dll\vc-7_1\release\threading-multi\*.dll .
copy bin\python\SinkWorld.pyd\vc-7_1\release\threading-multi\*.pyd .
