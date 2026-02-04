//dovetials for diffuser

use <diffuser.scad>
use <dovetail.scad>

teeth =[5,2,0.3];

module div1(male=true){
	intersection(){
		cube([30,350,300],center=true);
		cutter(position=[0,0,0],dimension=[30,350,300],teeths=teeth,male=male);
}}

module block1(male=true,filler=false){
	intersection(){
		translate([300/2,350/2,30/2])rotate([0,90,0])div1(male);
		if(filler==false)diffuser(wall_th=4,R=35,arch_depth=-1,z=30,m6_nuts=false);
		if(filler==true)translate([0,350/2-5])cube([300,10,30]);
}}



module side1(front=true){
	if(front==true)render(){
	block1();
	block1(false,true);}
	if(front==false)translate([0,5-350/2])render(){
	block1(false);
	block1(true,true);
}}
*side1(false);

module div2_cutout(x1=100,x2=200,x3=300,y=185,z=30,seg=0){
	if(seg==0||seg==1){
	intersection(){
		cube([x2,y,z]);
		translate([x1,y/2,z/2])rotate([90,90,0])
			cutter(position=[0,0,0],teeths=teeth,dimension=[z,x1,y]);
	}}
	if(seg==0||seg==2){
	intersection(){
			cube([x3,y,z]);
			
		translate([x2,y/2,z/2])rotate([90,90,0])
			cutter(position=[0,0,0],teeths=teeth,dimension=[z,x2,y]);
		translate([x1,y/2,z/2])rotate([90,90,0])
			cutter(position=[0,0,0],teeths=teeth,dimension=[z,x1,y],male=false);
	}}
	if(seg==0||seg==3){
	intersection(){
		cube([x3,y,z]);
		translate([x2,y/2,z/2])rotate([90,90,0])
			cutter(position=[0,0,0],teeths=teeth,dimension=[z,x2,y],male=false);
	}}
}

module div2(seg,front=true){
	intersection(){
		div2_cutout(seg=seg);
		side1(front);
	}
	if(seg==1){
		intersection(){
			div2_cutout(seg=2);
			cube([105,180,30]);
		}
	}
	if(seg==3){
		intersection(){
			div2_cutout(seg=2);
			translate([195,0])cube([100,180,30]);
		}
	}
	if(seg==2){
		intersection(){
			union(){
			div2_cutout(seg=1);
				div2_cutout(seg=3);
			}
			translate([95,0])cube([110,180,30]);
		}
	}
}

//div2(2,false);

module plate(spacing=11){
	for(i=[1:3]){
		translate([(i-1)*spacing,0])div2(i,true);
		translate([(i-1)*spacing,350/2+spacing])div2(i,false);
	}
}

plate();