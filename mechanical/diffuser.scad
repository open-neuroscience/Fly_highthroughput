xn = 3;
yn = 3;

difference(){
    translate([-1,-1,-1])cube([45,45,10]);
for(i=[0:1:xn]){
    for (j=[0:1:yn]){
        translate([11*i,11*j])cube(10);
        echo(i,j);  
    }
        
}
}