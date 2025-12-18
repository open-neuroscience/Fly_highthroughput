//HTP plates

use <fillets_and_rounds.scad>

module base_1d(n=20,spacing=3,thickness=1,cw=5,outer_w=15){
    d=5;
    l=65;
    $fn=30;
    plate_len=(spacing+d)*(n-1)+d+2*thickness;
    echo(plate_len);
    translate([d/2+thickness,0]){
    translate([0,5+d/2])for(i=[0:n-1]){
        translate([i*(spacing+d),0])add_rounds(axis="z",R=0.2,fn=10)difference(){
            cylinder(d=d+2*thickness,h=cw);
            //echo(i*(spacing+d),"=pos")
            translate([0,0,-1])cylinder(d=d,h=l);
            translate([0,d/2,0])cube([d*2/3,d,l],center=true);
        }//difference
    }//for
    translate([-d/2-thickness,0])difference(){
        cube([plate_len,5,outer_w]);
        screw_p=cw+(outer_w-cw)/2;
        translate([d,-1,screw_p])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        translate([plate_len-d,-1,screw_p])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        translate([plate_len/3,-1,screw_p])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        translate([plate_len*2/3,-1,screw_p])rotate([-90,0,0])cylinder(d=3.3,h=10,$fn=6);
        
        translate([d,2,screw_p])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
        translate([plate_len-d,2,screw_p])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
        translate([plate_len/3,2,screw_p])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
        translate([plate_len*2/3,2,screw_p])rotate([-90,0,0])cylinder(d=6.4,h=5,$fn=6);
    }//difference
}
}//base_1d

//base_1d();

symmetry_test=false;
if(symmetry_test==true){
//the below confirms the model is symmetrical
    //enter the echoed value into the X of below translate
base_1d();
translate([159,0])color("green",0.5)mirror([1,0,0])base_1d();
}

module tubes(n=20,spacing=5,thickness=1,cw=5){
    d=5;
    l=65;
    $fn=30;
    color("blue",0.4)translate([0,l])rotate([90,0,0])for(i=[0:n-1]){
        translate([i*(spacing+d),0])add_rounds(axis="z",R=0.2,fn=10)difference(){
            cylinder(d=d,h=l);
        }}
}//tubes



module base_assembly(tube=false,spacing=3,outer_w=15){
    d=5;
    n=20;
    thickness=1;
    plate_len=(spacing+d)*(n-1)+d+2*thickness;
    //200x85x12
    translate([0,outer_w,0]){rotate([90,0,0])base_1d(spacing=spacing,outer_w=outer_w);
    translate([plate_len,55,0])rotate([-90,180,0])base_1d(spacing=spacing,outer_w=outer_w);
    if(tube==true)translate([3.5,-5,7.5])tubes(spacing=spacing);}
}//base_assembly

//base_assembly(true);
color("green",0.5)translate([0,0,-5])cube([300,350,5]);
cols=3;
outer_w=15;
assembly_y=65+2*outer_w-2*5;
translate([cols*assembly_y,0])rotate(90)for(i=[0:cols-1])translate([0,assembly_y*i])base_assembly(true,outer_w=outer_w);
translate([cols*assembly_y,159])rotate(90)for(i=[0:cols-1])translate([0,assembly_y*i])base_assembly(true,outer_w=outer_w);
