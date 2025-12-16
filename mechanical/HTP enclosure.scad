// High-Throuput Station

module makerbeam(x=0,y=0,z=0,w=15){
    color("gray",0.7){
    if(x!=0){cube([x,w,w]);}
    else if(y!=0){cube([w,y,w]);}
    else if(z!=0){cube([w,w,z]);}}
}//makerbeam


//base size
translate([15,15])cube([300,350,30]);

translate([15,0])makerbeam(300);
translate([0,15])makerbeam(0,350);
translate([315,15])makerbeam(0,350);
translate([15,0,+15])makerbeam(300);
translate([0,15,15])makerbeam(0,350);
translate([315,15,15])makerbeam(0,350);

color("orange",0.7)translate([-5,-5,-5])cube([50,50,35]);