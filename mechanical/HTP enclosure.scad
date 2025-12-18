// High-Throuput Station

use <fillets_and_rounds.scad>

module makerbeam(x=0,y=0,z=0,w=15){
    color("gray",0.7){
    if(x!=0){cube([x,w,w]);}
    else if(y!=0){cube([w,y,w]);}
    else if(z!=0){cube([w,w,z]);}}
}//makerbeam

module screw(l=10,w=3.3,head=true,head_h=5){
    translate([0,0,-l])cylinder(h=l+0.1,d=w,$fn=6);
    if(head==true)cylinder(d=5.5,h=head_h,$fn=28);
}//screw

module corner(){
color("orange")difference(){
    add_rounds(axis="z",R=3,fn=20)translate([-5,-5,-5])cube([50,50,40]);
    translate([15,0])cube([50,20,30]);
    translate([0,15])cube([16,50,30]);
    translate([15,15])cube(50);
    translate([30,-1.5,7.5])rotate([90,0,0])screw();
    translate([30,-1.5,22.5])rotate([90,0,0])screw();
    translate([-1.5,30,7.5])rotate([0,-90,0])screw();
    translate([-1.5,30,22.5])rotate([0,-90,0])screw();
}//difference
}//corner

module x_brace(h=15,w=87.5,l=90){
    mid_w = 15;
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
translate([l/8,2,7.5])rotate([-90,0,0])screw(head_h=15);
translate([l*7/8,2,7.5])rotate([-90,0,0])screw(head_h=15);
translate([l/8,w-2,7.5])rotate([90,0,0])screw(head_h=15);
translate([l*7/8,w-2,7.5])rotate([90,0,0])screw(head_h=15);
}}
    
}//x_brace

translate([60,15])x_brace();

//base size
module assembly(){
    //color("green",0.1)translate([15,15])cube([300,350,30]);
translate([15,0])makerbeam(300);
translate([15,0,15])makerbeam(300);
translate([0,15])makerbeam(0,350);
translate([0,15,15])makerbeam(0,350);
translate([315,15])makerbeam(0,350);
translate([315,15,15])makerbeam(0,350);
    translate([15,350/4+15])makerbeam(300);
    translate([15,350*2/4+15])makerbeam(300);
    translate([15,350*3/4+15])makerbeam(300);
    translate([15,365])makerbeam(300);
}//assembly

assembly();

corner();
translate([330,0])rotate(90)corner();