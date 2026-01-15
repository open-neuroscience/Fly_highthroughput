// High-Throuput Station

use <fillets_and_rounds.scad>
use <1D_holder.scad>
use <diffuser.scad>

module makerbeam(x=0,y=0,z=0,w=15){
    color("silver"){
    if(x!=0){cube([x,w,w]);}
    else if(y!=0){cube([w,y,w]);}
    else if(z!=0){cube([w,w,z]);}}
}//makerbeam

module screw(l=10,w=3.3,head=true,head_h=5,head_d=6.5,fn=6,head_fn=6){
    color("gray"){
    translate([0,0,-l])cylinder(h=l+0.1,d=w,$fn=fn);
    if(head==true)cylinder(d=head_d,h=head_h,$fn=head_fn);}
}//screw

//screw(l=50,w=6.3,head_d=11.7,fn=20); //m6 hex head useful

module corner(slide="",n=3,w=15){
h=n*w+10;
color("orange")difference(){
    add_rounds(axis="z",R=3,fn=20)translate([-5,-5,-5])cube([50,50,h]);
    translate([w,0])cube([50,20,n*w+0.1]);
    translate([0,w])cube([16,50,n*w+0.1]);
    translate([w,w])cube(h+10);
    translate([0,0,h-10])cube(w+0.25);
    if(slide=="x")translate([w,-5.1,h-10])cube(h);
    if(slide=="y")translate([-5.1,w,h-10])cube(h);
    
for(i=[0:n-1]){
	translate([30,-1.5,w/2+i*w])rotate([90,0,0])screw();
    translate([-1.5,30,w/2+i*w])rotate([0,-90,0])screw();
}//for
}//difference
}//corner

module tube(od,h,fn=28,id=6.3,v_shift=-1){
    $fn=fn;
    difference(){
        cylinder(d=od,h=h);
        translate([0,0,v_shift])cylinder(d=id,h=h+2);
    }//difference
}//tube

module x_brace(h=15,w=87.5,l=90,cutout=true,d=55,cut_h=6,stem_h=10,mid_w=15){
	//l is in x, w is in y
    render(){
    difference(){
    add_fillets(axis="z",R=6,fn=10)difference(){
    union(){hull(){cube([l,0.5,h]);
    translate([l/2-mid_w/2,w/2])cube([mid_w,0.5,h]);}
    hull(){translate([l/2-mid_w/2,w/2])cube([mid_w,0.5,h]);
    translate([0,w-0.5])cube([l,0.5,h]);}}
    
    translate([l/2,2,(h)/2])difference(){
        rotate(45)cube([w/2,w/2,h+1],center=true);
        translate([0,-w/2])cube([w,w,h+1],center=true);
    }//difference
    
    translate([l/2,w-2,(h)/2])difference(){
        rotate(45)cube([w/2,w/2,h+1],center=true);
        translate([0,w/2])cube([w,w,h+1],center=true);
    }//difference
}
translate([l/8,2,7.5])rotate([-90,0,0])screw(head_h=w-10,w=4);
translate([l*7/8,2,7.5])rotate([-90,0,0])screw(head_h=w-10,w=4);
translate([l/8,w-2,7.5])rotate([90,0,0])screw(head_h=w-10,w=4);
translate([l*7/8,w-2,7.5])rotate([90,0,0])screw(head_h=w-10,w=4);
if(cutout==true)translate([l/2,w/2])cylinder(d=d,h=cut_h,$fn=60);
if(cutout==true)translate([l/2,w/2])cylinder(d=6.3,h=h+1,$fn=28);
}}//render
if(cutout==true){
    translate([l/2,w/2,cut_h-1])tube(h=2,od=12.3);
    *%translate([l/2,w/2,25])screw(l=25,w=6.3,head_d=11.7,fn=20);
    *%translate([l/2,w/2])color("red")tube(h=5,od=50,id=11.7,v_shift=-3,fn=6);
    //translate([l/2,w/2,h-1])tube(h=stem_h,od=15);
}
translate([l/2,0,h/2])cube([l/2,2.5,2.5],center=true);
translate([l/2,w,h/2])cube([l/2,2.5,2.5],center=true);
}//x_brace


module lip(l=150,w=15,lip_w=5,lip_h=5,n=3){
	color("gold")
	difference(){
		union(){
			cube([l,w,lip_h]);
			translate([0,0,lip_h])cube([l,w+lip_w,lip_h]);}
	for(i=[0.5:n]){
		translate([i*(l-2*lip_w)/n+lip_w,w/2,2*lip_h-4.9])screw();
	}//for
}//difference
}//lip

//base size
module assembly(n=3,lips=true){
    //color("green",0.1)translate([15,15])cube([300,350,30]);
    /*translate([300/4+15-45,15])x_brace(cutout=true);
    translate([300*3/4+15-45,15])x_brace(cutout=true);
    translate([300/4+15-45,350*3/4+15])x_brace(cutout=true);
    translate([300*3/4+15-45,350*3/4+15])x_brace(cutout=true);
    translate([15+75,350/4+30])x_brace(w=350*0.5-30,l=150,cutout=false,mid_w=25);
    */
    corner(slide="x",n=n);
    translate([330,0])rotate(90)corner(slide="y",n=n);
    translate([0,380])rotate(-90)corner(n=n);
    translate([330,380])rotate(180)corner(n=n);
	
	for(i=[0:n-1]){
    translate([15,0,i*15])makerbeam(300);
    translate([0,15,i*15])makerbeam(0,350);
    translate([315,15,i*15])makerbeam(0,350);
    translate([15,365,i*15])makerbeam(300);
    }
    translate([15,9.5+86.5*0.5])makerbeam(300);
    translate([15,9.5+86.5*1.5])makerbeam(300);
    translate([15,9.5+86.5*2.5])makerbeam(300);
    translate([15,9.5+86.5*3.5])makerbeam(300);
	
	if(lips==true){
		translate([0,15+75+350/2,n*15])rotate(-90)lip();
translate([330,15+350/2-75,n*15])rotate(90)lip();
translate([150+150/2+15,380,n*15])rotate(180)lip();
	}//if
	
	translate([15+98.6-45,9.5+86.5*0.5+15,0])x_brace(h=15,w=86.5-15,l=90,cutout=true,d=12,cut_h=6,stem_h=10,mid_w=15);
translate([15+98.6-45,9.5+86.5*1.5+15,0])x_brace(h=15,w=86.5-15,l=90,cutout=true,d=12,cut_h=6,stem_h=10,mid_w=15);
translate([15+98.6-45,9.5+86.5*2.5+15,0])x_brace(h=15,w=86.5-15,l=90,cutout=true,d=12,cut_h=6,stem_h=10,mid_w=15);
translate([15+98.6*2-45,9.5+86.5*0.5+15,0])x_brace(h=15,w=86.5-15,l=90,cutout=true,d=12,cut_h=6,stem_h=10,mid_w=15);
translate([15+98.6*2-45,9.5+86.5*1.5+15,0])x_brace(h=15,w=86.5-15,l=90,cutout=true,d=12,cut_h=6,stem_h=10,mid_w=15);
translate([15+98.6*2-45,9.5+86.5*2.5+15,0])x_brace(h=15,w=86.5-15,l=90,cutout=true,d=12,cut_h=6,stem_h=10,mid_w=15);
	
}//assembly

assembly(3);

*translate([15,15,15])color("pink")translate([300,0,30])rotate([0,180,0])diffuser(wall_th=4,R=35,arch_depth=5,z=20);
*translate([115.7,22])rotate([180,0,0])screw(l=30,w=6.6,head_d=10.3,head_fn=30);


//"screw_x:", 98.6667
//"screw_y:", 86.5

translate([-15,350/2+15])makerbeam(z=350);
translate([330,350/2+15])makerbeam(z=350);

translate([-15,350/4+15])makerbeam(z=350);
translate([330,350/4+15])makerbeam(z=350);

translate([-15,350*3/4+15])makerbeam(z=350);
translate([330,350*3/4+15])makerbeam(z=350);

translate([0,380,350])rotate([180,0,0])assembly(2,false);

*translate([15,15,35])dros_plate();