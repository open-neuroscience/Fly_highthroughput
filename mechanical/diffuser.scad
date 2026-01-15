//diffuser module for HTP system

use <fillets_and_rounds.scad>

module diffuser(x=300,y=350,z=20,xn=3,yn=4,face_th=0.4,wall_th=10,m=6,arch_depth=10,use_f_values=false,R=30,nut_depth=10){
    sqx = (x-(xn+1)*wall_th)/xn;
    sqy = (y-(yn+1)*wall_th)/yn;
    echo(sqx,sqy);
    color("pink")difference(){
        add_rounds(axis="x",R=5,fn=6){
		cube([x,y,z]);
		translate([0,0,-z/2])cube([x,y,z]);
		}
        //cutouts + board screw holes
        board_x = 88;
        board_y = 78;
        translate([wall_th,wall_th])for(i=[0:xn-1])for(j=[0:yn-1])
            translate([i*(sqx+wall_th),j*(sqy+wall_th),+face_th]){
                add_rounds(axis="z",R=R,fn=30)cube([sqx,sqy,z]);
                translate([(sqx-board_x)/2,(sqy-board_y)/2,z-2])
                led_motor_board(5,x=board_x,y=board_y);
            }
        
        //screw_holes
        f_th = 15;
        ffn=6;
        fx = (x-(xn+1)*f_th)/xn;
        fy = (y-(yn+1)*f_th)/yn;
        
        smx = 5; //screw margin x
        smy = 5; //screw margin y
        echo("screw_x:",sqx+wall_th);
        echo("screw_y:",sqy+wall_th);
        translate([wall_th/2,wall_th/2])for(i=[0:xn])for(j=[0:yn]){
            temp_x = i*(sqx+wall_th);
            temp_y = j*(sqy+wall_th);
            
            shift_x = 
            (i==0) ? temp_x+smx : 
            (i==xn) ? temp_x-smx : temp_x;
            
            shift_y = 
            (j==0) ? temp_y+smy : 
            (j==yn) ? temp_y-smy : temp_y;
            //echo(shift_x)
            translate([shift_x,shift_y,face_th])cylinder(d=m,h=z,$fn=ffn);
			translate([shift_x,shift_y,z-nut_depth])rotate(45)nut_slot();
			}//end translate
        
            //arch cuts
        outer_wall = wall_th;
        for(i=[0:xn-1])translate([i*(sqx+wall_th)+wall_th,sqy/2,z-arch_depth])
            add_rounds(axis="y",R=40,fn=30)cube([sqx,y-sqy,sqx]);
        for(i=[0:yn-1])translate([sqx/2,i*(sqy+wall_th)+wall_th,z-arch_depth])
        add_rounds(axis="x",R=38,fn=30)cube([x-sqx,sqy,sqy]);
    }//difference
}//diffuser

module led_motor_board(border=5,hole=10,x=90,y=80){
    color("green",0.8)cube([x,y,2]);
    for(dx=[border:x-2*border:x-border])for(dy=[border:y-2*border:y-border]){translate([dx,dy,-hole/2])cylinder(d=3.3,h=hole+2,center=true);}
} // led_motor_board

module nut_slot(){
	difference(){union(){
		cylinder(d=11.8,h=6.4,$fn=6);
		translate([0,-15,6.4/2])cube([11.8,30,6.4],center=true);}//union
		translate([-6,-33,6.2])cube([12,30,5]);
		translate([-6,3,6.2])cube([12,30,5]);}//difference	
}//nut_slot

diffuser(wall_th=4,R=35,arch_depth=5,z=30);
//spacing of boards in x = wall_th + sqx -board_x = 10.6667
//spacing in y = wall_th+sqy-board_y = 8.5
*led_motor_board(5,x=89,y=79);