<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title></title>
        <script src="loadxmldoc.js"></script>
        <script src ="loadxmlstring.js"></script>
    </head>
    <body>
<!--  Example of Javascript for working with DOM  -->
        <script>
       	xmlDoc=loadXMLDoc("books.xml");

	document.write(xmlDoc.getElementsByTagName("title")[0].childNodes[0].nodeValue + "<br>");
	document.write(xmlDoc.getElementsByTagName("author")[0].childNodes[0].nodeValue + "<br>");
	document.write(xmlDoc.getElementsByTagName("year")[0].childNodes[0].nodeValue);

	text="<bookstore>"
	text=text+"<book>";
	text=text+"<title>Everyday Italian</title>";
	text=text+"<author>Giada De Laurentiis</author>";
	text=text+"<year>2005</year>";
	text=text+"</book>";
	text=text+"</bookstore>";

	xmlDoc=loadXMLString(text);
        document.write("<hr>");
	document.write(xmlDoc.getElementsByTagName("title")[0].childNodes[0].nodeValue + "<br>");
	document.write(xmlDoc.getElementsByTagName("author")[0].childNodes[0].nodeValue + "<br>");
	document.write(xmlDoc.getElementsByTagName("year")[0].childNodes[0].nodeValue);
        
       </script>
    </body>
</html>
