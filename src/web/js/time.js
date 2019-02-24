function byHand(){
	var hour=eval(document.getElementById('in_hour')).value;
	var minute=eval(document.getElementById('in_minute')).value;
	var time_hand=parseInt(parseInt(hour)*60+parseInt(minute));
	// 调用CMD命令
	var cmd = new ActiveXObject("WScript.Shell");
	var setTime= "cd C:\\Users\\clm\\Desktop\\430-2017-19\\8-Group\\web\\py && python Main.py " +time_hand;
	cmd.run("cmd.exe /c "+setTime);
	alert("Set success!");
}
function auto(){
	var now=new Date();
	var hours=now.getHours();
	var minutes=now.getMinutes();
	var time_auto=parseInt((parseInt(hours)-12)*60+parseInt(minutes));
	// 调用CMD命令
	var cmd = new ActiveXObject("WScript.Shell");
	var setTime= "cd C:\\Users\\clm\\Desktop\\430-2017-19\\8-Group\\web\\py && python Main.py " +time_auto;
	cmd.run("cmd.exe /c "+setTime);
	alert("Set success!");
}
//获得当前时间,刻度为一千分一秒
var initializationTime=(new Date()).getTime();
function showLeftTime(){
	var now=new Date();
	var hours=now.getHours();
	var minutes=now.getMinutes();
	document.all.show.innerHTML=hours+":"+minutes;
	//一秒刷新一次显示时间
	var timeID=setTimeout(showLeftTime,1000);
}
function zero(){
	document.getElementById('in_hour').value = 0;
	document.getElementById('in_minute').value = 0;
}