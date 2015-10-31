var ycsdone;
var url = 'http://' + document.domain+location.pathname;

function geovisit()
{
  if (ycsdone)
    return;
  if (checkUad()) {
    addUad('y_gc_div_uadcntr');
    uadPos('y_gc_div_uadcntr');
  }
  ycsdone=1;
}
function checkUad()
{
    var adElement = document.getElementById("y_gc_div_adcntr");
    if (adElement == null) {
        return false;
    }
    var adFrame = adElement.getElementsByTagName("iFrame");
    if (adFrame.length == 0) {
        return false;
    }
    if (adFrame.item(0).width == 0 && adFrame.item(0).height == 0) {
        return false;
    }
    var adsElement = document.getElementById("y_gc_div_mast");
    if (adsElement != null) {
        return false;
    }
    return true;
}
function addUad(divId)
{
    if (!document.createElement) {
        return false;
    }

    var element = document.createElement('div');
    element.id = divId;
    element.innerHTML = "<center><iframe src=\"http://bc.geocities.yahoo.co.jp/bc/uad/i=" + jps + "&u=" + encodeURIComponent(url) + "\" width=615 height=265 border=0 marginwidth=0 marginheight=0 hspace=0 vspace=0 frameborder=0 scrolling=no></iframe></center>";

    document.body.appendChild(element);
}
function uadPos(divId)
{
    document.getElementById(divId).style.visibility = "visible";
    document.getElementById(divId).style.position = "relative";
    document.getElementById(divId).style.zIndex = 2147483646;
    document.getElementById(divId).style.height = 265;
    document.getElementById(divId).style.clear = "both";
}
