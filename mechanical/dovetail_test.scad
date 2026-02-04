use <dovetail.scad>

pos = [0,0,0];
dim = [20,20,20];
teeth = [5,2,0.3];

intersection(){
cube(20,center=true);
	rotate([0,90,0])cutter(position=pos,dimension=dim,teeths=teeth,male=true);
}//intersection

intersection(){
cube(20,center=true);
	rotate([0,90,0])cutter(position=pos,dimension=dim,teeths=teeth,male=false);
}//intersection