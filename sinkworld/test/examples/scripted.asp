<%@language=javas%>
<% 
#include 
function x() {
}
%>
<%@language=vbscript%>
<% 
sub x 'comment 
%>
<head>
    <script language="javascript">
	if window.location == "top" {
		return loc.indexOf('http:') != -1;
   	}
   </script>
    <script language="vbscript">
	if x then
	    ' Do it
	    x = x + 1
	end if
   </script>
</head>
