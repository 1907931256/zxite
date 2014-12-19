# makefile for Tentacle/J

.SUFFIXES: .java

JAVAC=c:\jdk\bin\javac 
JAR=c:\jdk\bin\jar
COPY=copy
DEL=del

all: tentacle.jar

tentacle.jar: *.java ../../java/*.java
	$(JAVAC) -d . -source 1.4 -Xlint:deprecation -g *.java ../../java/*.java
	$(JAR) cfm tentacle.jar tentacle.MF SinkWorld/*.class

clean:
	$(DEL) /q *.class *.jar
