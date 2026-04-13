//assembly for 3d printing diffuser without dovetails
use <diffuser.scad>

module lshape(){
    color("thistle",0.7)union(){cube([200,350/4,h]);
    translate([0,350/4])cube([100,350/4,h]);}
}//lshape

module domino(axis="x"){
    color("indianred",0.7){if(axis=="x")cube([200,350/4,h]);
    if(axis=="y")cube([100,350/2,h]);}
}//domino

module dif(){
render()diffuser(m6_nuts=false);}

h=10;
tol=0;
dist=10;


//layer 1
translate([0,dist,dist])intersection(){dif();
translate([0,350/4,h])lshape();}

translate([0,0,dist])intersection(){dif();
translate([0,0,h])domino();}

translate([dist,dist*2,dist])intersection(){dif();
translate([300,350*3/4,h])rotate(180)lshape();}

translate([dist,dist*3,dist])intersection(){dif();
translate([100,350*3/4,h])domino();}

translate([dist,0])intersection(){dif();
translate([200,0,h])cube([100,350/4,h]);}

translate([0,dist])intersection(){dif();
translate([0,350*3/4,h])cube([100,350/4,h]);}


//layer 2

intersection(){dif();
domino("y");}

translate([dist,0])intersection(){dif();
translate([100,0])cube([200-tol,-tol+350/2,h]);}

translate([dist,dist])intersection(){dif();
translate([200,350/2])domino("y");}

translate([0,dist])intersection(){dif();
translate([0,350/2])cube([200-tol,-tol+350/2,h]);}

