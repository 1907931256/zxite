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
<script language="vbscript">
function changeMagnification(n) {
	alert(":" + document.b.getMagnification());
	document.a.setMagnification(document.a.getMagnification() + n);
}
</script>
