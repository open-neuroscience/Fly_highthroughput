//HTP plates

use <fillets_and_rounds.scad>

//cube([300,350,5]);

module base_1d(n=20,spacing=5,thickness=1,cw=5){
    d=5;
    l=65;
    $fn=30;
    plate_len=(spacing+d)*n;
    translate([d,5+d/2])for(i=[0:n-1]){
        translate([i*(spacing+d),0])add_rounds(axis="z",R=0.2,fn=10)difference(){
            cylinder(d=d+2*thickness,h=cw);
            translate([0,0,-1])cylinder(d=d,h=l);
            translate([0,d/2,0])cube([d*2/3,d,l],center=true);
        }//difference
    }//for
    difference(){
        cube([plate_len,5,cw*3]);
        translate([d,-1,cw+d])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        translate([plate_len-d,-1,cw+d])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        translate([plate_len/3,-1,cw+d])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        translate([plate_len*2/3,-1,cw+d])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        
        translate([d,2,cw+d])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
        translate([plate_len-d,2,cw+d])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
        translate([plate_len/3,2,cw+d])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
        translate([plate_len*2/3,2,cw+d])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
    }//difference
}//base_1d

//the below confirms the model is symmetrical
//base_1d();
//translate([200,0])color("green",0.5)mirror([1,0,0])base_1d();

module tubes(n=20,spacing=5,thickness=1,cw=5){
    d=5;
    l=65;
    $fn=30;
    color("blue",0.4)translate([0,l])rotate([90,0,0])for(i=[0:n-1]){
        translate([i*(spacing+d),0])add_rounds(axis="z",R=0.2,fn=10)difference(){
            cylinder(d=d,h=l);
        }}
}//tubes



module base_assembly(){
    rotate([90,0,0])base_1d();
    translate([200,55,0])rotate([-90,180,0])base_1d();
}//base_assembly

base_assembly();
translate([5,-5,7.5])tubes();