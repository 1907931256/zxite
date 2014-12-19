# makefile for Tentacle/N

.SUFFIXES: .cxx .cs .h .cs

CSC=csc
COPY=copy
DEL=del

all: TentacleN.exe SinkWorld.dll

clean:
	$(DEL) /q *.exe

# The quotes here makes it work.
TentacleN.exe: *.cs ../../csharp/*.cs
	$(CSC) /out:$@ /nowarn:0162,0168 /target:winexe /debug *.cs "..\..\csharp\*.cs"

SinkWorld.dll: *.cs ../../csharp/*.cs
	$(CSC) /out:$@ /nowarn:0162,0168 /target:library /debug *.cs "..\..\csharp\*.cs"
