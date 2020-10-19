var x,y,p,degree,volt=100,busy=0;
var rota_sayi=4;
var positionsX = new Array();
var positionsY = new Array();
var i=0,j=0 ,a=1;
var mousePressed = false;
var lastX = 250, lastY = 250;
var canvas = document.getElementById("myCanvas");
var canvas2 = document.getElementById("myCanvas2");
var batarya = document.getElementById("battery");
var ping = document.getElementById("ping");
var kont = 1;
var ctx = canvas.getContext("2d");
var ctx2 = canvas2.getContext("2d");
var btx = batarya.getContext("2d");
base_image = new Image();
base_image.src = 'okc.png';
car  = new Image();
car.src = 'kartezyen.png';
car.onload = function(){
    ctx.beginPath();
    ctx.drawImage(car,0,0,500,500);
    ctx.closePath();
    ctx2.beginPath();
    ctx2.drawImage(car,0,0,500,500);
    ctx2.closePath();
}
base_image.onload = function(){
ctx.save();
ctx.translate(canvas.width/2,canvas.height/2); // change origin
ctx.rotate(degree*Math.PI/180);	  
ctx.drawImage(base_image,-18,-18,36,36); 
ctx.restore();
}
ctx.moveTo(canvas.width/2,canvas.height/2);


/*for(i=0;i<500;i=i+25)   //Koordinat çizdiren.
{
    ctx.moveTo(0,i);
    ctx.lineTo(500,i);
    if(i <= 250)
    ctx.strokeText(" "+(250-i).toString(),250,i);
    else
    ctx.strokeText(" "+(250-i).toString(),250,i);
    
}
for(i=0;i<500;i=i+25)
{
    ctx.moveTo(i,0);
    ctx.lineTo(i,500);
    if(i <= 250)
    ctx.strokeText(" "+(-(250-i)).toString(),i,250);
    else
    ctx.strokeText(" "+(i-250).toString(),i,250);
    
}
ctx.stroke();
ctx.beginPath();
ctx.strokeStyle = "#000000";
ctx.lineWidth = 3;
ctx.moveTo(250,0);
ctx.lineTo(250,500);
ctx.moveTo(0,250);
ctx.lineTo(500,250);
ctx.stroke();
*/
function startLiveUpdate(){

 setInterval(function(){
 fetch('viewcount.php').then(function(response){
     return response.json();

 }).then(function(data){
  x = data.viewX;
  y = data.viewY;
  ping.innerHTML = data.Ping;
  volt = data.volt;
  degree = parseInt(data.Angle);
  busy = parseInt(data.f);
 if(busy==1)
    document.getElementById('busycheck').src='images/busy.gif';
  else
    document.getElementById('busycheck').src='images/free.png';
  if(volt > 100)
   {
     volt = volt - 100;
     isCharging(1);
   }else{isCharging(0);}
  if(volt>=75)
	  btx.fillStyle = "#c8f86b";
  if(volt >=50 && volt < 75)
	  btx.fillStyle = "#6bf887";
  if(volt < 50 && volt >= 25)
	  btx.fillStyle = "#f1c631";
  if(volt <25)
	  btx.fillStyle = "#e43c39";
  if(volt < 10)
	  if(kont)
	 { alert("Batarya kritik seviyede! ,robotu durdurup şarj etmeyi düşünebilirsiniz."); kont=0;}
  btx.clearRect(1,1,48,13);
  btx.fillRect(1, 1, volt/100*batarya.width, 13);
  ctx.clearRect(0,0,canvas.width,canvas.height);
  ctx.save();
  ctx.drawImage(car,0,0,500,500);
  ctx.translate(x+250,250-y); // change origi
  ctx.rotate(degree*Math.PI/180);	  
  ctx.drawImage(base_image,-18,-18,36,36); 
  ctx.restore();
  ctx.lineTo(x+250,250-y);
  ctx.lineWidth = 3;
  ctx.strokeStyle = "#008000";
  ctx.stroke();
  
 }).catch(function (error){
   console.log(error);
 });
 


},100);
}
document.addEventListener('DOMContentLoaded',function(){
startLiveUpdate();
});

canvas2.addEventListener("click", function (evt) {
       if(remot==-1 && rota_sayi > 0){
        var mousePos = getMousePos(canvas2, evt);
        ctx2.beginPath();
        ctx2.strokeStyle = "blue";
        ctx2.lineWidth = 4;
        ctx2.lineJoin = "round";
        ctx2.moveTo(lastX, lastY);
        ctx2.lineTo(mousePos.x, mousePos.y);
        positionsX[a] = mousePos.x - 250;
        positionsY[a] = 250-mousePos.y;
        rota_sayi = rota_sayi - 1;
        rota_sinir.innerHTML = rota_sayi; 
        a = a + 1;
        ctx2.closePath();
        ctx2.stroke();
        lastY = mousePos.y; lastX = mousePos.x;  
}else if(remot!=-1){
    
    alert("Çizerek yönledirme için otomatik kontrol moduna geçiniz.");
}else if(!(rota_sayi > 0)){
    alert("Rota sayısı aşıldı.");
}else{}

}, false);

//Get Mouse Position
function getMousePos(canvas, evt) {
    var rect = canvas2.getBoundingClientRect();
    return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

var h;
var xmlhttp; var remot=1;
xmlhttp=new XMLHttpRequest();
document.addEventListener('keydown', keyDownHandler, false);
document.addEventListener('keyup', keyUpHandler, false);
document.getElementById("stop").onmousedown = function() {stop()};
document.getElementById("stop").onmouseup = function() {nstop()};
document.getElementById("stop2").onmousedown = function() {stop2()};
document.getElementById("stop2").onmouseup = function() {nstop2()};
document.getElementById("go").onmousedown = function() {go()};
document.getElementById("go").onmouseup = function() {ngo()};
var rota_sinir = document.getElementById("rota_sinir");
var remotetext = document.getElementById("kontrol");
var demoX = document.getElementById("posX");
var demoY = document.getElementById("posY");
var ispressed = 0;
var buttonCode = 0;
function isCharging(ch) {
    var img = document.getElementById("charge");
    if(ch==1)
    img.style.visibility = 'visible';
    else 
    img.style.visibility = 'hidden';
    
}

function remote(){
    remot = remot * -1;
  if(remot==1)
   {   remotetext.innerHTML = "Manuel Kontrol";
       xmlhttp.open("GET","sqlhandler.php?code=1",true);
	   xmlhttp.send();
       
   }
  if(remot==-1)
   {       
        positionsX[0]=x;
        positionsY[0]=y;
        lastX = x+250;lastY=250-y;
        remotetext.innerHTML = "Otomatik Kontrol"; 
        xmlhttp.open("GET","sqlhandler.php?code=7",true);
	    xmlhttp.send();
   }
    
}
function go(){
$.ajax({ 
       type: "POST", 
       url: "sqlhandler.php", 
       data: { xArray:JSON.stringify(positionsX) , yArray:JSON.stringify(positionsY)}, 
       success: function() { 
              //alert("Koordinatlar başarılı bir şekilde aktarıldı."); 
        } 
}); 

var sonX=positionsX[positionsX.length-1];
var sonY=positionsY[positionsY.length-1];
positionsX.length = 0;
positionsY.length = 0;
positionsX[0]=sonX;
positionsY[0]=sonY;
a=1;rota_sayi = 4;rota_sinir.innerHTML = 4;
xmlhttp.open("GET","fetch.php?x="+x+"&y="+y+"&p=0"+"&a="+degree+"&v="+volt+"&f=1",true);
xmlhttp.send();
}
function ngo(){
    
    
}
function keyDownHandler(event) {
   if(remot==1){
   
  
   
   
    if(event.keyCode == 68) {
      if(!ispressed)
      {
          xmlhttp.open("GET","sqlhandler.php?code=4",true);
	      xmlhttp.send();
	      buttonCode = 68;
	      ispressed = 1;
	    
	     document.getElementById("right").classList.remove("button1");
   document.getElementById("right").classList.add("button1basti");
      }
	
    }
    else if(event.keyCode == 65) {
      if(!ispressed)
      {
      xmlhttp.open("GET","sqlhandler.php?code=5",true);
	  xmlhttp.send();
	  buttonCode = 65;
	  ispressed = 1;
	  
	   document.getElementById("left").classList.remove("button1");
   document.getElementById("left").classList.add("button1basti");
      }
	}
    if(event.keyCode == 83) {
    if(!ispressed)
      {	
    	xmlhttp.open("GET","sqlhandler.php?code=3",true);
	    xmlhttp.send();
      buttonCode = 83;
      ispressed = 1;
     
      document.getElementById("reverse").classList.remove("button1");
   document.getElementById("reverse").classList.add("button1basti");
      } 
    }
    else if(event.keyCode == 87) {
      if(!ispressed)
      {
        xmlhttp.open("GET","sqlhandler.php?code=2",true);
	    xmlhttp.send();
	    buttonCode = 87;
	    ispressed = 1;
	  
	    document.getElementById("forw").classList.remove("button1");
        document.getElementById("forw").classList.add("button1basti");
      } 
          
    }

   }else{
       alert("Tuşları kullanmak için manuel kontrol moduna geçiniz.");
       
   }
  }

function keyUpHandler(event) {
    if(event.keyCode==buttonCode)
     {
       document.getElementById("forw").classList.remove("button1basti");
       document.getElementById("forw").classList.add("button1");
       document.getElementById("left").classList.remove("button1basti");
       document.getElementById("left").classList.add("button1");
       document.getElementById("right").classList.remove("button1basti");
       document.getElementById("right").classList.add("button1");
       document.getElementById("reverse").classList.remove("button1basti");
       document.getElementById("reverse").classList.add("button1");
         xmlhttp.open("GET","sqlhandler.php?code=1",true);
	     xmlhttp.send();
	     ispressed = 0;
	   
     }
}

function stop()
{
	 ctx.beginPath();
     ctx.clearRect(0, 0,500,500);
     ctx.beginPath();
     ctx.drawImage(car,0,0,500,500);
     ctx.closePath();
	 xmlhttp.open("GET","fetch.php?x=0&y=0&p=0&a=0&v=0&f=0",true);
	 xmlhttp.send();
	 document.getElementById('sto').src='images/reset2.png';
}
function nstop()
{
	 document.getElementById('sto').src='images/reset.png';
}
function stop2()
{
	  a=1; 
	  positionsX.length = 0;
	  positionsY.length = 0;
	  positionsX[0]=x;
      positionsY[0]=y;
	  ctx2.setTransform(1, 0, 0, 1, 0, 0);
	  ctx2.clearRect(0, 0,500,500);
	  ctx2.beginPath();
      ctx2.drawImage(car,0,0,500,500);
      ctx2.closePath();
	  lastX = x+250; lastY = 250-y;
	  document.getElementById('sto2').src='images/reset2.png';
	 rota_sayi = 4;rota_sinir.innerHTML = 4;
	  $.ajax({ 
       type: "POST", 
       url: "sqlhandler.php", 
       data: { xArray:JSON.stringify(null) , yArray:JSON.stringify(null)}, 
       success: function() { 
              //alert("Koordinatlar başarılı bir şekilde aktarıldı."); 
        } 
}); 
}
function nstop2()
{
	 document.getElementById('sto2').src='images/reset.png';
}