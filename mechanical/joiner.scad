//makerbeam parallel joiner

use <fillets_and_rounds.scad>


module joiner(h=5){
	add_rounds(fn=20,R=0.5,axis="z")union(){translate([-1.5,0])cube([3,10,h]);
	translate([-5.7/2,1.4])cube([5.7,2.5,h]);
	translate([-5.7/2,10-3.9])cube([5.7,2.5,h]);}
}//joiner

joiner();