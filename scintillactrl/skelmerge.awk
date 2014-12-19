BEGIN	{ 
	make_vars(); 
	skipflag = "";
}

{
	# test if we are skipping lines between a $[...) and $(...]-block.
	if(skipflag != "") 
	{
		# clear skipping if we reach the wanted $(...]-block
		if($0 ~ sprintf("\\$\\(%s\\]",skipflag))
		{
			print;
			skipflag = "";
		}
	}else{
		no_print = 0;
		for (j in env_vars)
		{
			if(match($0,sprintf("\\$\\(%s\\)",j)))
			{
				var_contents = ""
				if(env_types[j] == "f") # interprete variable as file name
				{
					# The lines in the file will be concatenated into one line
					# Linebreaks are replaced by one space
					file_exist = 0;
					while ((getline line < env_vars[j]) > 0)
					{
						file_exist = 1;
						var_contents = var_contents " " line;
					}
					if(file_exist)
						close(env_vars[j])
				}else{ # interprete variable as literal
					var_contents = env_vars[j];
				}
				# substitute regular $(...) variables for the value
				gsub(sprintf("\\$\\(%s\\)",j),var_contents);
			}
			# Do we have a one-line file substitution block $[...]?
			if($0 ~ sprintf("\\$\\[%s\\]",j))
			{
				# if so, don't print the current line, but replace it by the 
				# contents of the requested file or the literal contents of
				# the variable, whichever is appropriate
				no_print = 1;
				if(env_types[j] == "f") # interprete variable as file name
				{
					file_exist = 0;
					while ((getline line < env_vars[j]) > 0)
					{
						file_exist = 1;
						print line
					}
					if(file_exist)
						close(env_vars[j])
				}else{ # interprete variable as literal
					print env_vars[j];
				}
				break;
			}

			# Do we have a multi-line file substitution block $[...) $(...]?
			if($0 ~ sprintf("\\$\\[%s\\)",j))
			{
				# if so, do print the current line, and additionally insert 
				# the contents of the requested file or the literal contents of
				# the variable, whichever is appropriate
				print;
				if(env_types[j] == "f") # interprete variable as file name
				{
					file_exist = 0;
					while ((getline line < env_vars[j]) > 0)
					{
						file_exist = 1;
						print line
					}
					if(file_exist)
						close(env_vars[j])
				}else{ # interprete variable as literal
					print env_vars[j];
				}
				# make sure the current line doesn't get print again, and
				# start our skipping mode
				no_print = 1;
				skipflag = j;
				break;
			}
		}			
		if(!no_print)
			print;
	}
}


function make_vars()
{
	for (i = 2; i < ARGC; i++)
	{
		if(ARGV[i] ~ /[a-zA-Z0-9_]+=\"?.*\"?/)
		{
			eqsign = match(ARGV[i],/\=/);
			name = substr(ARGV[i],1,eqsign - 1);
			value = substr(ARGV[i],eqsign + 1);
			# If the variable name starts with a hyphen (-), it's value will
			# be interpreted as the path of a file whose contents will be
			# used to substitute the designated variable
			if(substr(name,1,1) == "-")
			{
				name = substr(name,2);
				env_vars[name] = value;
				env_types[name] = "f"; # value is file path
			}else{
				env_vars[name] = value;
				env_types[name] = "l"; # value is literal
			}
		}
	}

	ARGC = 2;
}
