.SILENT:
cp = cp.exe
rm = rm.exe -rf

generate_ocx:
	echo Generating ocx bindings
	awk -f scintilla_iface.awk ../scintilla/include/scintilla.iface

	echo Merging Ole interface
	awk -f skelmerge.awk ScintillaCtrl.odl \
			-ODLPROP_SECTION="scintilla_iface.odlprop" \
			-ODLMETH_SECTION="scintilla_iface.odlmeth" \
			-ODLEVT_SECTION="scintilla_iface.odlevt" \
			> ScintillaCtrl.odl.tmp 
	$(cp) ScintillaCtrl.odl.tmp ScintillaCtrl.odl
	$(rm) ScintillaCtrl.odl.tmp

	echo Merging Ole dispatch ID's and function prototypes
	awk -f skelmerge.awk ScintillaCtrl.h \
			-DISPMSG_SECTION="scintilla_iface.dispmsg" \
			-DISPEVT_SECTION="scintilla_iface.dispevt" \
			-DISPID_SECTION="scintilla_iface.dispid" \
			> ScintillaCtrl.h.tmp 
	$(cp) ScintillaCtrl.h.tmp ScintillaCtrl.h
	$(rm) ScintillaCtrl.h.tmp

	echo Merging Ole dispatch map and function implementations
	awk -f skelmerge.awk ScintillaCtrl.cpp \
			-EVTCPP_SECTION="scintilla_iface.evtcpp" \
			-METHCPP_SECTION="scintilla_iface.methcpp" \
			-DISPMAP_SECTION="scintilla_iface.dispmap" \
			-EVTMAP_SECTION="scintilla_iface.evtmap" \
			> ScintillaCtrl.cpp.tmp 
	$(cp) ScintillaCtrl.cpp.tmp ScintillaCtrl.cpp
	$(rm) ScintillaCtrl.cpp.tmp

	$(rm) scintilla_iface.odlprop	
	$(rm) scintilla_iface.odlmeth	
	$(rm) scintilla_iface.odlevt	
	$(rm) scintilla_iface.dispmsg	
	$(rm) scintilla_iface.dispevt
	$(rm) scintilla_iface.dispid
	$(rm) scintilla_iface.dispmap
	$(rm) scintilla_iface.evtmap
	$(rm) scintilla_iface.methcpp
	$(rm) scintilla_iface.evtcpp
