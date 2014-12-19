BEGIN{
	FS = "[=,()]"
	count = 0;
	evtcount = 0;
	propcount = 0;
	class_name = "CScintillaCtrl";
	# file names
	file_base = "scintilla_iface"
	format_file = file_base ".format";
	impl_file = file_base ".impl";

	odlmeth_file = file_base ".odlmeth";
	odlprop_file = file_base ".odlprop";
	odlevt_file = file_base ".odlevt";
	dispmsg_file = file_base ".dispmsg";
	dispid_file = file_base ".dispid";
	dispevt_file = file_base ".dispevt";
	dispmap_file = file_base ".dispmap";
	evtmap_file = file_base ".evtmap";
	methcpp_file = file_base ".methcpp";
	evtcpp_file = file_base ".evtcpp";
}

/^val[ \t]+/{
	split($1,vals,"[ \t]+");
	props[propcount++] = vals[2];
}

/^evt[ \t]+/{
	split($1,proto,"[ \t]+");

	evttype[evtcount,0] = proto[2];
	evtname[evtcount,0] = proto[3];

	for(i = 3; i <= NF; i++)
	{
		match($i,"[^ \t ].*");
		if(RSTART)
		{
			split(substr($i,RSTART,RLENGTH),param,"[ \t]+");
			evttype[evtcount,i - 2] = param[1]; 
			evtname[evtcount,i - 2] = param[2]; 
		}
	}
	evtcount++
}

/^(fun|get|set)[ \t]+[a-zA-Z0-9_]+[ \t]+[a-zA-Z0-9_]+(=[0-9]+)?\(([a-zA-Z0-9_]+[ \t]+[a-zA-Z0-9_]+(=[a-zA-Z0-9_])?)?,[ \t]*([a-zA-Z0-9_]+[ \t]+[a-zA-Z0-9_]+(=[a-zA-Z0-9_])?)?\)/	{
	split($1,proto,"[ \t]+");


	match($3,"[^ \t ].*");
	if(RSTART)
	{
		split(substr($3,RSTART,RLENGTH),param1,"[ \t]+");
	}
	match($4,"[^ \t ].*");
	if(RSTART)
	{
		split(substr($4,RSTART,RLENGTH),param2,"[ \t]+");
	}
	

	type[count,0] = proto[2];	# return type
	name[count,0] = proto[3];	# function name
	type[count,1] = param1[1];	# type of parameter 1
	name[count,1] = param1[2];	# name of parameter 1
	type[count,2] = param2[1];	# type of parameter 2
	name[count,2] = param2[2];	# name of parameter 2

	count++;
	delete param1[1];
	delete param1[2];
	delete param2[1];
	delete param2[2];

#	printf("%-3s  %-9s  %-23s ( %-9s  %-20s , %-15s  %-18s)\n",proto[1],proto[2],proto[3],param1[1],param1[2],param2[1],param2[2]);
}

END{
	#
	# Load parameter formats
	#
	file_exist = 0;
	while ((getline line < format_file) > 0)
	{
		file_exist = 1;
		split(line,parts,"[ \t]+=[ \t]+");
		split(parts[1],subparts,"[][ ]");
		if(subparts[1] == "return")
			rettypes[subparts[2],subparts[3]] = parts[2];
		if(subparts[1] == "param")
			paramtypes[subparts[2],subparts[3]] = parts[2];
	}
	if(file_exist)
		close(format_file);

	#
	# Load function implementations
	#
	file_exist = 0;
	fname = "";
	implbody = 0;
	while ((getline line < impl_file) > 0)
	{
		file_exist = 1;
		if(implbody)
		{
			if(line == "}")
				implbody = 0;
			else
				impl[fname] = impl[fname] line "\n" 
		}else{
			split(line,parts,"[ \t]");
			if(parts[1] == "fun")
				fname = parts[2];
			if(line == "{")
				implbody = 1;
		}
	}
	if(impl_file)
		close(format_file);


	#
	#		OLE interface properties
	#
	printf("\t\t\t//{{AFX_ODL_PROP(" class_name ")\n") > odlprop_file;
	for(i = 0; i < propcount; i++)
	{
		printf("\t\t\t[id(%d)] %s %s;\n",i + 1,rettypes["int","odl"],props[i]) > odlprop_file;
	}
	printf("\t\t\t//}}AFX_ODL_PROP") > odlprop_file;

	#
	#		OLE interface methods
	#
	printf("\t\t\t//{{AFX_ODL_METHOD(" class_name ")\n") > odlmeth_file;
	for(i = 0; i < count; i++)
	{
		printf("\t\t\t[id(%d)] %s %s(",i + 1 + propcount,rettypes[type[i,0],"odl"],name[i,0]) > odlmeth_file;
		write_params(i,"odl",",",odlmeth_file);
		printf(");\n") > odlmeth_file;
	}
	printf("\t\t\t//}}AFX_ODL_METHOD") > odlmeth_file;

	#
	#		OLE interface events
	#
	printf("\t\t\t//{{AFX_ODL_EVENT(" class_name ")\n") > odlevt_file;
	for(i = 0; i < evtcount; i++)
	{
		printf("\t\t\t[id(%d)] %s %s(",i + 1,rettypes[evttype[i,0],"odl"],evtname[i,0]) > odlevt_file;
		write_evtparams(i,"odl",",",odlevt_file);
		printf(");\n") > odlevt_file;
	}
	printf("\t\t\t//}}AFX_ODL_EVENT") > odlevt_file;

	#
	#		OLE dispatch messages
	#
	printf("\t//{{AFX_DISPATCH(" class_name ")\n") > dispmsg_file;
	for(i = 0; i < propcount; i++)
	{
		printf("\tafx_msg %s Get%s();\n",rettypes["int","cpp"],props[i]) > dispmsg_file;
	}
	for(i = 0; i < count; i++)
	{
		printf("\tafx_msg %s %s(",rettypes[type[i,0],"cpp"],name[i,0]) > dispmsg_file;
		write_params(i,"cpp",",",dispmsg_file);
		printf(");\n") > dispmsg_file;
	}
	printf("\t//}}AFX_DISPATCH") > dispmsg_file;

	#
	#		OLE dispatch events
	#
	printf("\t//{{AFX_EVENT(" class_name ")\n") > dispevt_file;
	for(i = 0; i < evtcount; i++)
	{
		printf("\t%s Fire%s(",
				rettypes[evttype[i,0],"cpp"], evtname[i,0]) > dispevt_file;
		write_evtparams(i,"cpp",",",dispevt_file);
		printf(")\n\t\t{FireEvent(eventid%s,EVENT_PARAM(",
				evtname[i,0]) > dispevt_file;
		write_evtparams(i,"ole"," ",dispevt_file);
		printf(")") > dispevt_file;
		write_evtparamnames(i,",","%s",",",dispevt_file);
		printf(");}\n") > dispevt_file;
	}
	printf("\t//}}AFX_EVENT") > dispevt_file;

	#
	#		OLE dispatch ID's
	#
	printf("\t//{{AFX_DISP_ID(" class_name ")\n") > dispid_file;
	for(i = 0; i < propcount; i++)
	{
		printf("\tdispid%s = %dL,\n",props[i],i + 1) > dispid_file;
	}
	for(i = 0; i < count; i++)
	{
		printf("\tdispid%s = %dL,\n",name[i,0],i + 1 + propcount) > dispid_file;
	}
	for(i = 0; i < evtcount; i++)
	{
		printf("\teventid%s = %dL,\n",evtname[i,0],i + 1) > dispid_file;
	}
	printf("\t//}}AFX_DISP_ID") > dispid_file;

	#
	#		OLE dispatch map
	#
	printf("\t//{{AFX_DISPATCH_MAP(" class_name ")\n") > dispmap_file;
	for(i = 0; i < propcount; i++)
	{
		printf("\tDISP_PROPERTY_EX(" class_name ", \"%s\", Get%s, SetNotSupported, %s)\n",props[i],props[i],rettypes["int","ole"]) > dispmap_file;
	}
	for(i = 0; i < count; i++)
	{
		printf("\tDISP_FUNCTION(" class_name ", \"%s\", %s, %s, ",name[i,0],name[i,0],rettypes[type[i,0],"ole"]) > dispmap_file;
		write_params(i,"ole"," ",dispmap_file);
		printf(")\n") > dispmap_file;
	}
	printf("\t//}}AFX_DISPATCH_MAP") > dispmap_file;

	#
	#		OLE event map
	#
	printf("\t//{{AFX_EVENT_MAP(" class_name ")\n") > evtmap_file;
	for(i = 0; i < evtcount; i++)
	{
		printf("\tEVENT_CUSTOM(\"%s\", Fire%s, ",evtname[i,0],evtname[i,0]) > evtmap_file;
		write_evtparams(i,"ole"," ",evtmap_file);
		printf(")\n") > evtmap_file;
	}
	printf("\t//}}AFX_EVENT_MAP") > evtmap_file;

	#
	#		C++ method & properties implementations
	#
	for(i = 0; i < propcount; i++)
	{
		printf("%s " class_name "::Get%s()\n{\n\treturn %s;\n}\n\n",rettypes["int","cpp"],props[i],props[i]) > methcpp_file;
	}
	for(i = 0; i < count; i++)
	{
		printf("%s " class_name "::%s(",rettypes[type[i,0],"cpp"],name[i,0]) > methcpp_file;
		write_params(i,"cpp",",",methcpp_file);
		printf(")\n{\n") > methcpp_file;
				
		if(name[i,0] in impl)
			printf("%s",impl[name[i,0]]) > methcpp_file;
		else
			printf("\t%sSendMessage(%s,(WPARAM)%s,(LPARAM)%s);\n",
				((type[i,0] == "void") ? "" : "return "),
				(index(name[i,0],"_") ? toupper(name[i,0]) : ("SCI_" toupper(name[i,0]))),
				((name[i,1] == "") ? "0" : name[i,1]),
				((name[i,2] == "") ? "0" : name[i,2])) > methcpp_file;
		printf("}\n\n") > methcpp_file;
	}


	#
	#		C++ event implementations
	#
	for(i = 0; i < evtcount; i++)
	{
		printf("\t\tcase %s:\n\t\t\tFire%s(",
			(index(evtname[i,0],"_") ? toupper(evtname[i,0]) : ("SCN_" toupper(evtname[i,0]))),
			evtname[i,0]) > evtcpp_file;
		write_evtparamnames(i,"","scn->%s",",",evtcpp_file);
		printf(");\n\t\t\tbreak;\n\n") > evtcpp_file;
	}
}

function write_params(i,context,separator, file)
{
	if(type[i,1] == "" && type[i,2] == "")
	{
		printf(paramtypes["void",context]) > file;
	}else{
		if(type[i,1] != "")
		{
			printf(paramtypes[type[i,1],context],name[i,1],name[i,1],name[i,1],name[i,1],name[i,1]) > file;
			if(type[i,2] != "")
				printf(separator) > file;
		}
		if(type[i,2] != "")
		{
			printf(paramtypes[type[i,2],context],name[i,2],name[i,2],name[i,2],name[i,2],name[i,2]) > file;
		}
	}
}

function write_evtparams(i,context,separator, file)
{
	j = 1;
	while(evttype[i,j] != "")
	{
		printf(paramtypes[evttype[i,j],context],evtname[i,j],evtname[i,j],evtname[i,j],evtname[i,j],evtname[i,j]) > file;
		if(evttype[i,j + 1] != "")
			printf(separator) > file;
		j++;
	}
	if(j == 1)
		printf(paramtypes["void",context]) > file;
}

function write_evtparamnames(i,pre,format,separator,file)
{
	j = 1;
	if(evtname[i,j] != "")
		printf("%s",pre) > file;

	while(evtname[i,j] != "")
	{
		printf(format,evtname[i,j]) > file;
		j++;
		if(evtname[i,j] != "")
			printf("%s",separator) > file;
	}
}
