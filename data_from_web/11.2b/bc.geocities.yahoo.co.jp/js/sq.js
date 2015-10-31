// JavaScript Document

var day = new Date();
var id = ''+day.getFullYear()+day.getDate()+'';
var ie4 = document.all;
var ie5 = document.all && document.getElementById;
var ns4 = document.layers;
var ns6 = document.getElementById && !document.all; 
var auWritten=false;
var mac = (navigator.userAgent.indexOf("Mac") >=0) ? true : false;
var url = 'http://' + document.domain+location.pathname;
var ref = document.referrer;


function noFrameAu()
{
    try { if (top.location.hostname){ throw "ok"; } else { throw "accesser"; } }
    catch(er) { if (er=="ok") {yfrau = (top.location.href.indexOf(id)>0) ? true : false;} else {yfrau=true;} }
    return yfrau;
}
function writeAu()
{
    cnt='<center><iframe width=160 height=785 border=0 marginwidth=0 marginheight=0 hspace=0 vspace=0 frameborder=0 scrolling=no src=\"http://bc.geocities.yahoo.co.jp/bc/sq/i=' + jps + '&u=' + encodeURIComponent(url) + '&ref=' + encodeURIComponent(ref) +'\"></iframe></center>';
    document.write("<div id=y_gc_div_adcntr class=y_gcss_ovrtr_cntr><div id=y_gc_div_mast class=y_gcss_ovrtr_msthd><img src=http://i.yimg.jp/images/clear.gif width=2 height=1><a href=\"http://rdsig.yahoo.co.jp/geo/geoguide/adsq/top/RV=1/RU=aHR0cDovL2dlb2NpdGllcy55YWhvby5jby5qcC8-\" target=\"new\"><img src=http://i.yimg.jp/images/geo/ygeosm.gif alt=\"Yahoo!ジオシティーズ\" width=122 height=16 hspace=0 vspace=2 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=3 height=1><a href=\"http://rdsig.yahoo.co.jp/geo/geoguide/adsq/help/RV=1/RU=aHR0cDovL2hlbHAueWFob28uY28uanAvaGVscC9qcC9nZW8v\" target=\"new\"><img src=http://i.yimg.jp/images/geo/adsq/geo_ad_help.gif width=14 height=13 hspace=0 vspace=6 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=1 height=1><a href=\"javascript:;\" onMouseDown=\"maximizeAu('y_gc_div_au1')\";><img src=http://i.yimg.jp/images/geo/adsq/geo_ad_dwn_widg2.gif width=14 height=13 hspace=0 vspace=6 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=1 height=1><a href=\"javascript:;\" onClick=\"closeAu('y_gc_div_adcntr'); return false;\";><img src=http://i.yimg.jp/images/geo/adsq/geo_ad_close_widg2.gif width=14 height=13 hspace=0 vspace=6 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=2 height=1></div><div id=y_gc_div_au1 class=y_gcss_ovrtr_au><img src=http://i.yimg.jp/images/clear.gif width=2 height=1><a href=\"http://rdsig.yahoo.co.jp/geo/geoguide/adsq/top/RV=1/RU=aHR0cDovL2dlb2NpdGllcy55YWhvby5jby5qcC8-\" target=\"new\"><img src=http://i.yimg.jp/images/geo/ygeosm.gif alt=\"Yahoo!ジオシティーズ\" width=122 height=16 hspace=0 vspace=2 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=3 height=1><a href=\"http://rdsig.yahoo.co.jp/geo/geoguide/adsq/help/RV=1/RU=aHR0cDovL2hlbHAueWFob28uY28uanAvaGVscC9qcC9nZW8v\" target=\"new\"><img src=http://i.yimg.jp/images/geo/adsq/geo_ad_help.gif width=14 height=13 hspace=0 vspace=6 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=1 height=1><a href=\"javascript:;\" onMouseDown=\"minimizeAu('y_gc_div_au1')\";><img src=http://i.yimg.jp/images/geo/adsq/geo_ad_up_widg2.gif width=14 height=13 hspace=0 vspace=6 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=1 height=1><a href=\"javascript:;\" onClick=\"closeAu('y_gc_div_adcntr'); return false;\";><img src=http://i.yimg.jp/images/geo/adsq/geo_ad_close_widg2.gif width=14 height=13 hspace=0 vspace=6 border=0></a><img src=http://i.yimg.jp/images/clear.gif width=2 height=1><div class=y_gcss_ovrtr_au_cbox>"+cnt+"</div></div></div>");
    auPos('y_gc_div_adcntr');
    auPos('y_gc_div_au1');
    auPos('y_gc_div_mast');
}
function minimizeAu(divId) {
    if (ns4) {
        document.layers[divId].visibility = "hide";
    } else if (ie5) {
        if (mac)
            document.getElementById(divId).style.visibility = "hidden";
        else
            document.getElementById(divId).style.display = "none";
    } else if (ns6) {
        document.getElementById(divId).style.display = "none";
    } else if (ie4) {
        document.all[divId].style.visibility = "hidden";
    }
}
function maximizeAu(divId) {
    if (ns4) {
        document.layers[divId].visibility = "show";
    } else if (ie5) {
        if (mac)
            document.getElementById(divId).style.visibility = "visible";
        else
            document.getElementById(divId).style.display = "inline";
    } else if (ns6) {
        document.getElementById(divId).style.display = "inline";
    } else if (ie4) {
        document.all[divId].style.visibility = "visible";
    }
}
function closeAu(divId) {
    minimizeAu('y_gc_div_au1');
    minimizeAu('y_gc_div_mast');
}
function mmAction(divId) {
    if (ie4) {
        auVis = (document.all[divId].style.visibility == "visible") ? true : false;
    } else if(ns4) {
        auVis = (document.layers[divId].visibility == "show") ? true : false;
    } else {
        auVis = (document.getElementById(divId).style.visibility == "visible") ? true : false;
    }
    if (auVis){minimizeAu(divId);}
    else {maximizeAu(divId);}
}
function auPos(divId)
{
    posL = findX()-177;
    if (ns4) {
    posL='1';
    document.layers[divId].visibility = "show";
    document.layers[divId].right = "0px";
    } else if (ie5 || ns6) {
    document.getElementById(divId).style.visibility = "visible";
    document.getElementById(divId).style.right = "0px";
    } else if (ie4) {
    document.all[divId].style.visibility = "visible";
    document.all[divId].style.right = "0px";
    }
}
function isFrameset()
{
    var numFr=0;
    numFr=frames.length;
    numFr=numFr-document.getElementsByTagName("iframe").length;
    isFr=(numFr>0) ? true : false;
    return isFr; 
}
function divAu()
{
//    if (noFrameAu()) {
        if (!isFrameset()) {
            if (inFrame()) {
                if (checkFrame()) {
                    writeAu();
                    auWritten=true;
                    rlPg(true);
                }
            } else {
                writeAu();
                auWritten=true;
                rlPg(true);
            }
        }
//    }
}
function inFrame() {
    inFr=(parent.frames.length != 0) ? true : false;
    return inFr;
}
function findX()
{
    var x=0;
    if (self.innerWidth){x = self.innerWidth;} // ns
    else if (document.documentElement && document.documentElement.clientHeight){x = document.documentElement.clientWidth;} // ie6 strict
    else if (document.body){x = document.body.clientWidth;} // other ie
    return x;
}
function findY()
{
    var y=0;
    if (self.innerHeight){y=self.innerHeight;} // ns
    else if (document.documentElement && document.documentElement.clientHeight){y = document.documentElement.clientHeight;} // ie6 strict
    else if (document.body){y = document.body.clientHeight;} // other ie
    return y;
}
function checkFrame() {
    chFr=(findX() > 400 && findY() > 300) ? true : false;
    return chFr;
}
divAu();
function rlPg(init) {
     if (init==true) {
        document.pgW = findX();
        onresize=rlPg;
    } else if (window.innerWidth!=document.pgW || document.body.offsetWidth!=document.pgW) {
      auPos('y_gc_div_adcntr');
    auPos('y_gc_div_au1');
    auPos('y_gc_div_mast');
  }
}
if (auWritten){ eval('setTimeout(\"minimizeAu(\'y_gc_div_au1\')\", 30*1000);')}

