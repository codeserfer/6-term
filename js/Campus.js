// ==UserScript==
// @name         my
// @namespace    http://your.homepage/
// @version      0.1
// @description  my
// @author       You
// @match        http://study.mesi.ru/sites/*
// @grant        none
// ==/UserScript==

function my () {
	var s=document.getElementById ("ctl00_PlaceHolderSiteName_onetidProjectPropertyTitle").innerHTML;
	var start=0;
	var finish=0;
	for (var i=0; i<s.length; i++) 
	{ 
		if (s[i]=='(') start=i;
		if (s[i]==')') finish=i;
	}
	var group=s.substring (start+2, finish-1);
	if (group=="ДКО-301"){
		alert ("!!!");
	}
}
my ();
function myfoo() {
	//alert (document.location.href); 
	var current_id=Number(document.location.href.split ("/")[5]);
	current_id+=5;
    //alert (current_id);
	document.location.href="http://study.mesi.ru/sites/WorkPlaces_15/"+current_id+"/Lists/List3/AllItems.aspx";
}
var element = document.createElement('input');
element.type="submit";
element.name="mybutton";
element.value="GO!";
element.onclick=function() { myfoo(); return false;};
//document.body.appendChild(element);
if (document.getElementById ("s4-searcharea")!=null)
{
    document.getElementById ("s4-searcharea").appendChild(element);
}
else {
    document.body.appendChild(element);
}
