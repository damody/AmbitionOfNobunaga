
var expdate = new Date();
expdate.setTime (expdate.getTime() +  (60 * 1000 )); 
document.cookie = "HMS=3f417e953a2f65c8; expires=" + expdate.toGMTString() +  "; path=/";

var nav = navigator;
var doc = document;
var agt = nav.userAgent.toLowerCase();
var apv = nav.appVersion.toLowerCase();
var cpu = nav.cpuClass ? escape(nav.cpuClass) : escape(nav.oscpu);
var plf = escape(nav.platform);
var lan = nav.browserLanguage ? nav.browserLanguage : 
	nav.userLanguage ? nav.userLanguage : nav.language;
var apn = (nav.appName == 'Microsoft Internet Explorer') ? '_MSIE' :
	(nav.appName == 'Netscape') ? '_NS' : escape(nav.appName);

var tz = (new Date()).getTimezoneOffset() * (-1);
var sc = 'W' + screen.width + 'H' + screen.height;
var cd = screen.colorDepth;
var wv,fv,xv,cy,acv,xmv,qtv;
var dt = encodeURIComponent(doc.title);
var dl = encodeURIComponent(doc.location);
var rr = encodeURIComponent(doc.referrer);

var qs = 'uid=' + '65140';
qs += '&ts=1444426815';
qs += '&cs=3f417e953a2f65c8';
qs += '&apn=' + apn;
qs += '&plf=' + plf;
qs += '&cpu=' + cpu;
qs += '&lan=' + lan;
qs += '&tz=' + tz;
qs += '&sc=' + sc;
qs += '&cd=' + cd;
qs += '&dl=' + dl;
qs += '&rr=' + rr;

if ( nav.product ) {
	qs += '&npd=' + escape(nav.product);
	}
if ( nav.productSub ) {
	qs += '&nps=' + escape(nav.productSub);
	}
if ( nav.vendor ) {
	qs += '&nvd=' + escape(nav.vendor);
	}
if ( nav.vendorSub ) {
	qs += '&nvs=' + escape(nav.vendorSub);
	}
/**
 * SWFObject v1.4: Flash Player detection and embed - http://blog.deconcept.com/swfobject/
 *
 * SWFObject is (c) 2006 Geoff Stearns and is released under the MIT License:
 * http://www.opensource.org/licenses/mit-license.php
 *
 * **SWFObject is the SWF embed script formarly known as FlashObject. The name was changed for
 *   legal reasons.
 */
if(typeof deconcept=="undefined"){var deconcept=new Object();}
if(typeof deconcept.util=="undefined"){deconcept.util=new Object();}
if(typeof deconcept.SWFObjectUtil=="undefined"){deconcept.SWFObjectUtil=new Object();}
deconcept.SWFObject=function(_1,id,w,h,_5,c,_7,_8,_9,_a,_b){
if(!document.createElement||!document.getElementById){return;}
this.DETECT_KEY=_b?_b:"detectflash";
this.skipDetect=deconcept.util.getRequestParameter(this.DETECT_KEY);
this.params=new Object();
this.variables=new Object();
this.attributes=new Array();
if(_1){this.setAttribute("swf",_1);}
if(id){this.setAttribute("id",id);}
if(w){this.setAttribute("width",w);}
if(h){this.setAttribute("height",h);}
if(_5){this.setAttribute("version",new deconcept.PlayerVersion(_5.toString().split(".")));}
this.installedVer=deconcept.SWFObjectUtil.getPlayerVersion(this.getAttribute("version"),_7);
if(c){this.addParam("bgcolor",c);}
var q=_8?_8:"high";
this.addParam("quality",q);
this.setAttribute("useExpressInstall",_7);
this.setAttribute("doExpressInstall",false);
var _d=(_9)?_9:window.location;
this.setAttribute("xiRedirectUrl",_d);
this.setAttribute("redirectUrl","");
if(_a){this.setAttribute("redirectUrl",_a);}};
deconcept.SWFObject.prototype={setAttribute:function(_e,_f){
this.attributes[_e]=_f;
},getAttribute:function(_10){
return this.attributes[_10];
},addParam:function(_11,_12){
this.params[_11]=_12;
},getParams:function(){
return this.params;
},addVariable:function(_13,_14){
this.variables[_13]=_14;
},getVariable:function(_15){
return this.variables[_15];
},getVariables:function(){
return this.variables;
},getVariablePairs:function(){
var _16=new Array();
var key;
var _18=this.getVariables();
for(key in _18){
_16.push(key+"="+_18[key]);}
return _16;
},getSWFHTML:function(){
var _19="";
if(navigator.plugins&&navigator.mimeTypes&&navigator.mimeTypes.length){
if(this.getAttribute("doExpressInstall")){this.addVariable("MMplayerType","PlugIn");}
_19="<embed type=\"application/x-shockwave-flash\" src=\""+this.getAttribute("swf")+"\" width=\""+this.getAttribute("width")+"\" height=\""+this.getAttribute("height")+"\"";
_19+=" id=\""+this.getAttribute("id")+"\" name=\""+this.getAttribute("id")+"\" ";
var _1a=this.getParams();
for(var key in _1a){_19+=[key]+"=\""+_1a[key]+"\" ";}
var _1c=this.getVariablePairs().join("&");
if(_1c.length>0){_19+="flashvars=\""+_1c+"\"";}
_19+="/>";
}else{
if(this.getAttribute("doExpressInstall")){this.addVariable("MMplayerType","ActiveX");}
_19="<object id=\""+this.getAttribute("id")+"\" classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\" width=\""+this.getAttribute("width")+"\" height=\""+this.getAttribute("height")+"\">";
_19+="<param name=\"movie\" value=\""+this.getAttribute("swf")+"\" />";
var _1d=this.getParams();
for(var key in _1d){_19+="<param name=\""+key+"\" value=\""+_1d[key]+"\" />";}
var _1f=this.getVariablePairs().join("&");
if(_1f.length>0){_19+="<param name=\"flashvars\" value=\""+_1f+"\" />";}
_19+="</object>";}
return _19;
},write:function(_20){
if(this.getAttribute("useExpressInstall")){
var _21=new deconcept.PlayerVersion([6,0,65]);
if(this.installedVer.versionIsValid(_21)&&!this.installedVer.versionIsValid(this.getAttribute("version"))){
this.setAttribute("doExpressInstall",true);
this.addVariable("MMredirectURL",escape(this.getAttribute("xiRedirectUrl")));
document.title=document.title.slice(0,47)+" - Flash Player Installation";
this.addVariable("MMdoctitle",document.title);}}
if(this.skipDetect||this.getAttribute("doExpressInstall")||this.installedVer.versionIsValid(this.getAttribute("version"))){
var n=(typeof _20=="string")?document.getElementById(_20):_20;
n.innerHTML=this.getSWFHTML();
return true;
}else{
if(this.getAttribute("redirectUrl")!=""){document.location.replace(this.getAttribute("redirectUrl"));}}
return false;}};
deconcept.SWFObjectUtil.getPlayerVersion=function(_23,_24){
var _25=new deconcept.PlayerVersion([0,0,0]);
if(navigator.plugins&&navigator.mimeTypes.length){
var x=navigator.plugins["Shockwave Flash"];
if(x&&x.description){_25=new deconcept.PlayerVersion(x.description.replace(/([a-z]|[A-Z]|\s)+/,"").replace(/(\s+r|\s+b[0-9]+)/,".").split("."));}
}else{try{
var axo=new ActiveXObject("ShockwaveFlash.ShockwaveFlash");
for(var i=3;axo!=null;i++){
axo=new ActiveXObject("ShockwaveFlash.ShockwaveFlash."+i);
_25=new deconcept.PlayerVersion([i,0,0]);}}
catch(e){}
if(_23&&_25.major>_23.major){return _25;}
if(!_23||((_23.minor!=0||_23.rev!=0)&&_25.major==_23.major)||_25.major!=6||_24){
try{_25=new deconcept.PlayerVersion(axo.GetVariable("$version").split(" ")[1].split(","));}
catch(e){}}}
return _25;};
deconcept.PlayerVersion=function(_29){
this.major=parseInt(_29[0])!=null?parseInt(_29[0]):0;
this.minor=parseInt(_29[1])||0;
this.rev=parseInt(_29[2])||0;};
deconcept.PlayerVersion.prototype.versionIsValid=function(fv){
if(this.major<fv.major){return false;}
if(this.major>fv.major){return true;}
if(this.minor<fv.minor){return false;}
if(this.minor>fv.minor){return true;}
if(this.rev<fv.rev){return false;}return true;};
deconcept.util={getRequestParameter:function(_2b){
var q=document.location.search||document.location.hash;
if(q){
var _2d=q.indexOf(_2b+"=");
var _2e=(q.indexOf("&",_2d)>-1)?q.indexOf("&",_2d):q.length;
if(q.length>1&&_2d>-1){
return q.substring(q.indexOf("=",_2d)+1,_2e);
}}return "";}};
if(Array.prototype.push==null){
Array.prototype.push=function(_2f){
this[this.length]=_2f;
return this.length;};}
var getQueryParamValue=deconcept.util.getRequestParameter;
var FlashObject=deconcept.SWFObject; // for backwards compatibility
var SWFObject=deconcept.SWFObject;

 
var swfv = deconcept.SWFObjectUtil.getPlayerVersion();
var nfv = swfv['major'] +"."+ swfv['minor'] +"."+ swfv['rev'];
// document.write("<h3>Detect...</h3>");
var is_minor = parseFloat(apv);
var is_major = parseInt(is_minor);
var is_opera = (agt.indexOf("opera") != -1);
var is_opera5 = (agt.indexOf("opera 5") != -1 || agt.indexOf("opera/5") != -1);
var is_opera6 = (agt.indexOf("opera 6") != -1 || agt.indexOf("opera/6") != -1);
var is_opera7 = (agt.indexOf("opera 7") != -1 || agt.indexOf("opera/7") != -1);

var iePos = apv.indexOf('msie');
if (iePos !=-1) {
	is_minor = parseFloat(apv.substring(iePos+5,apv.indexOf(';',iePos)))
	is_major = parseInt(is_minor);
	}

var is_konq = false;
var kqPos = agt.indexOf('konqueror');
if ( kqPos != -1 ) { 
	is_konq  = true;
	is_minor = parseFloat(agt.substring(kqPos+10,agt.indexOf(';',kqPos)));
	is_major = parseInt(is_minor);
	} 

var is_safari = ((agt.indexOf('safari')!=-1)&&(agt.indexOf('mac')!=-1))? 1 : 0;
var is_khtml  = (is_safari || is_konq);
var is_gecko = ((!is_khtml)&&(nav.product)&&(nav.product.toLowerCase()=="gecko"))? 1 : 0;
var is_fb = ((agt.indexOf('mozilla/5')!=-1) && (nav.vendor=="Firebird"));
var is_fx = ((agt.indexOf('mozilla/5')!=-1) && (nav.vendor=="Firefox"));

var is_moz   = ((agt.indexOf('mozilla/5')!=-1) && (agt.indexOf('spoofer')==-1) &&
	(agt.indexOf('compatible')==-1) && (agt.indexOf('opera')==-1)  &&
	(agt.indexOf('webtv')==-1) && (agt.indexOf('hotjava')==-1) &&
	(is_gecko) && ((nav.vendor=="")||(nav.vendor=="Mozilla")));

var is_nav  = ((agt.indexOf('mozilla')!=-1) && (agt.indexOf('spoofer')==-1)
	&& (agt.indexOf('compatible') == -1) && (agt.indexOf('opera')==-1)
	&& (agt.indexOf('webtv')==-1) && (agt.indexOf('hotjava')==-1)
	&& (!is_khtml) && (!is_moz) && (!is_fb) && (!is_fx));

if ( (nav.vendor) && 
	((nav.vendor=="Netscape6")||(nav.vendor=="Netscape")) && (is_nav)) {
	is_major = parseInt(nav.vendorSub);
	is_minor = parseFloat(nav.vendorSub);
	}

var is_nav2 = (is_nav && (is_major == 2));
var is_nav3 = (is_nav && (is_major == 3));
var is_nav4 = (is_nav && (is_major == 4));
var is_nav6 = (is_nav && is_major == 6);
var is_nav6up = (is_nav && is_minor >= 6);
var is_nav5up = (is_nav && is_major >= 5 && !is_nav6);

var is_ie = ((iePos!=-1) && (!is_opera) && (!is_khtml));
var is_ie3 = (is_ie && (is_major < 4));
var is_ie4 = (is_ie && is_major == 4);
var is_ie5 = (is_ie && is_major == 5);
var is_ie5up = (is_ie && is_minor >= 5);

var jsv;
if (is_nav2 || is_ie3) jsv = 'JS10';
else if (is_nav6up || is_opera7 || is_khtml || is_moz || is_fb || is_fx) jsv = 'JS15';
else if ((is_opera5)||(is_opera6)) jsv = 'JS13';
else if (is_nav3 || is_opera) jsv = 'JS11';
else if ((is_nav4 && (is_minor <= 4.05)) || is_ie4) jsv = 'JS12';
else if ((is_nav4 && (is_minor > 4.05)) || is_ie5up) jsv = 'JS13';
else if (is_nav5up ) jsv = 'JS14';
else jsv = 'JS00';

if ( is_ie5up ) {
	doc.write('<HTML xmlns:HH><STYLE>');
	doc.write('@media all{HH\\:clientCaps{behavior:url(#default#clientcaps);}}');
	doc.write('</STYLE><HH:CLIENTCAPS ID="Caps">');
	cy = Caps.connectionType;
	fv = Caps.getComponentVersion("{D27CDB6E-AE6D-11CF-96B8-444553540000}",'ComponentID'); 
	wv = Caps.getComponentVersion("{22D6F312-B0F6-11D0-94AB-0080C74C7E95}",'ComponentID'); 
	xv = Caps.getComponentVersion("{89820200-ECBD-11CF-8B85-00AA005B4383}",'ComponentID'); 
	acv = get_acrobat_ver();
	xmv = get_xml_ver();
	qtv = get_quicktime_ver();
	}
fv = nfv;
qs += '&cy=' + cy;
qs += '&fv=' + fv;
qs += '&wv=' + wv;
qs += '&xv=' + xv;
qs += '&acv=' + acv;
qs += '&xmv=' + xmv;
qs += '&qtv=' + qtv;
qs += '&jsv=' + jsv;

qs += '&cx=1';

// ***** Begin Function

function get_xml_ver() {
var xml_ver = '0.0';
XM = new Array('Microsoft.XMLDOM', 'Msxml2.DOMDocument', 'Msxml2.DOMDocument.2.6',
	'Msxml2.DOMDocument.3.0', 'Msxml2.DOMDocument.4.0', 'Msxml2.DOMDocument.5.0',
	'Msxml2.DOMDocument.6.0', 'Msxml2.DOMDocument.7.0');
XV = new Array('1.0', '2.0', '2.6', '3.0', '4.0', '5.0', '6.0', '7.0');

if ( window.ActiveXObject ) {
	for (i = 0; i < XM.length; i++) {
		try	{
			Xmldom = eval("new ActiveXObject(XM[i]);");
			if ( Xmldom ) { xml_ver = XV[i]; }
			}
		catch (e) { }
		}
	}
return xml_ver;
}

function get_acrobat_ver() {
var acrobat_ver = '0.0';
if ( window.ActiveXObject ) {
	for ( x = 2; x < 10; x++ ) {
		try	{
			acro = eval("new ActiveXObject('PDF.PdfCtrl."+x+"');");
			if ( acro ) { acrobat_ver = x + '.0'; }
			}
		catch(e) {}
		}
	try	{
		acro = new ActiveXObject('PDF.PdfCtrl.1');
		if ( acro ) { acrobat_ver = '4.0'; }
		}
	catch(e) {}
	}
return acrobat_ver;
}

function get_quicktime_ver() {
var qtv_ver = '0.0';
if ( window.ActiveXObject ) {
	try {
		qtv = new ActiveXObject('QuickTimeCheckObject.QuickTimeCheck.1');
		if ( qtv ) {
			qtv_ver = parseInt(qtv.QuickTimeVersion.toString(16).substring(0,3))/100;
			}
		}
	catch(e) {}
	}
return qtv_ver;
}

qs += '&dt=' + dt;
document.write("<script src=http://h01.hotrank.com.tw/hotrank88?" + qs + "></script>");
